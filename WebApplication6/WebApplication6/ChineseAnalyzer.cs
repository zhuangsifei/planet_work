using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using Lucene.Net.Analysis;
using Lucene.Net.Analysis.Standard;
using Lucene.Net.Util;
using Lucene.Net.Documents;
using Lucene.Net.Analysis.Tokenattributes;
using Version = Lucene.Net.Util.Version;

namespace Lucene.Net.Analysis.DChinese
{
    public class DChineseAnalyzer : Analyzer
    {
        private ISet<string> stopSet;
        public static readonly ISet<string> STOP_WORDS_SET;
        private Version matchVersion;
        private bool replaceInvalidAcronym;
        private bool enableStopPositionIncrements;

        public DChineseAnalyzer(Version version, ISet<string> stopWords)
        {
            stopSet = stopWords;
            replaceInvalidAcronym = false;
            enableStopPositionIncrements = StopFilter.GetEnablePositionIncrementsVersionDefault(version);
            replaceInvalidAcronym = matchVersion.OnOrAfter(Version.LUCENE_24);
            this.matchVersion = version;
        }

        public DChineseAnalyzer(Version version)
            : this(version, STOP_WORDS_SET)
        {
        }

        public DChineseAnalyzer(Version version, System.IO.FileInfo stopWords)
            : this(version, WordlistLoader.GetWordSet(stopWords))
        {
        }

        static DChineseAnalyzer()
        {
            STOP_WORDS_SET = StopAnalyzer.ENGLISH_STOP_WORDS_SET;
        }
        public override TokenStream TokenStream(System.String fieldName, System.IO.TextReader reader)
        {
            //string indexPath = System.Environment.CurrentDirectory;
            //string dirParent = Directory.GetParent(indexPath).Parent.FullName;

            //bool bInit = SplitWord.NLPIR_Init(dirParent, 0,null);
            //if (!bInit)
            //{   
            //    return null;
            //}
            //String inputString  = reader.ReadToEnd();
            //IntPtr intPtr       = SplitWord.NLPIR_ParagraphProcess(inputString,0);
            //string strResult    = Marshal.PtrToStringAnsi(intPtr);

            //TokenStream result = new DChineseTokenizer( matchVersion, new StringReader(strResult) );
            TokenStream result = new DChineseTokenizer(matchVersion, reader);
            result = new LowerCaseFilter(result);
            result = new StopFilter(enableStopPositionIncrements, result, stopSet);
            result = new PorterStemFilter(result);
            return result;
        }

        private class SavedStreams
        {
            protected internal DChineseTokenizer source;
            protected internal TokenStream result;
        };
        public override TokenStream ReusableTokenStream(System.String fieldName, System.IO.TextReader reader)
        {
            SavedStreams streams = (SavedStreams)PreviousTokenStream;
            if (streams == null)
            {
                streams = new SavedStreams();
                streams.source = new DChineseTokenizer(matchVersion, reader);
                streams.result = new LowerCaseFilter(streams.source);
                streams.result = new StopFilter(enableStopPositionIncrements, streams.source, stopSet);
                streams.result = new PorterStemFilter(streams.source);
                PreviousTokenStream = streams;
            }
            else
            {
                streams.source.Reset(reader);
            }

            streams.source.SetReplaceInvalidAcronym(replaceInvalidAcronym);
            return streams.result;
        }

    }

    public sealed class DChineseTokenizer : Tokenizer
    {

        private bool m_replaceInvalidAcronym;
        private int offset = 0;
        private int bufferIndex = 0;
        private int dataLen = 0;
        private const int MAX_WORD_LEN = 255;
        private const int IO_BUFFER_SIZE = 4096;
        private readonly char[] ioBuffer = new char[IO_BUFFER_SIZE];

        private ITermAttribute termAtt;
        private IOffsetAttribute offsetAtt;
        private IPositionIncrementAttribute posIncrAtt;



        private void Init(System.IO.TextReader input, Version matchVersion)
        {
            if (matchVersion.OnOrAfter(Version.LUCENE_24))
            {
                m_replaceInvalidAcronym = true;
            }
            else
            {
                m_replaceInvalidAcronym = false;
            }
            //this.input = input;
            this.input = ChangeInput(input);
            termAtt = AddAttribute<ITermAttribute>();
            offsetAtt = AddAttribute<IOffsetAttribute>();
            posIncrAtt = AddAttribute<IPositionIncrementAttribute>();
        }

        public DChineseTokenizer(Version matchVersion, System.IO.TextReader input)
            : base()
        {
            Init(input, matchVersion);
        }

        public DChineseTokenizer(Version matchVersion, System.IO.TextReader input, AttributeSource source)
            : base(source)
        {
            Init(input, matchVersion);
        }

        public DChineseTokenizer(Version matchVersion, System.IO.TextReader input, AttributeFactory factory)
            : base(factory)
        {
            Init(input, matchVersion);
        }

        public override bool IncrementToken()
        {
            ClearAttributes();
            int length = 0;
            int start = bufferIndex;
            char[] buffer = termAtt.TermBuffer();
            while (true)
            {

                if (bufferIndex >= dataLen)
                {
                    offset += dataLen;
                    dataLen = input.Read(ioBuffer, 0, ioBuffer.Length);
                    if (dataLen <= 0)
                    {
                        dataLen = 0;
                        if (length > 0)
                            break;
                        return false;
                    }
                    bufferIndex = 0;
                }

                char c = ioBuffer[bufferIndex++];

                if (!System.Char.IsWhiteSpace(c))
                {
                    if (length == 0)
                    {
                        start = offset + bufferIndex - 1;
                    }
                    else if (length == buffer.Length)
                    {
                        buffer = termAtt.ResizeTermBuffer(1 + length);
                    }

                    buffer[length++] = c;
                    if (length == MAX_WORD_LEN)
                        break;
                }
                else if (length > 0)
                    break;
            }

            termAtt.SetTermLength(length);
            offsetAtt.SetOffset(CorrectOffset(start), CorrectOffset(start + length));
            posIncrAtt.PositionIncrement = 1;
            return true;
        }

        public override void Reset()
        {
            base.Reset(input);
            bufferIndex = 0;
            offset = 0;
            dataLen = 0;
        }

        public override void Reset(TextReader input)
        {
            String inputString = input.ReadToEnd();
            IntPtr intPtr = SplitWord.NLPIR_ParagraphProcess(inputString, 0);
            string strResult = Marshal.PtrToStringAnsi(intPtr);
            this.input = new StringReader(strResult);
            bufferIndex = 0;
            offset = 0;
            dataLen = 0;
        }

        public override void End()
        {
            int finalOffset = CorrectOffset(offset);
            offsetAtt.SetOffset(finalOffset, finalOffset);
        }

        public void SetReplaceInvalidAcronym(bool replaceInvalidAcronym)
        {
            this.m_replaceInvalidAcronym = replaceInvalidAcronym;
        }

        private TextReader ChangeInput(TextReader input)
        {
            //string indexPath = System.Environment.CurrentDirectory;
            //string indexPath = GetType().Assembly.Location;
            //string indexPath = System.IO.Path.GetDirectoryName(Page.Request.PhysicalPath);
            //string dirParent = Directory.GetParent(indexPath).Parent.FullName;
            string dirParent = System.AppDomain.CurrentDomain.BaseDirectory;
            

            bool bInit = SplitWord.NLPIR_Init(dirParent, 0, null);
            if (!bInit)
            {
                return null;
            }
            String inputString = input.ReadToEnd();
            IntPtr intPtr = SplitWord.NLPIR_ParagraphProcess(inputString, 0);
            string strResult = Marshal.PtrToStringAnsi(intPtr);
            return new StringReader(strResult);
        }
    }
}
