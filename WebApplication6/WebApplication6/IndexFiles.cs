using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.IO;
using Lucene.Net.Analysis.Standard;
using Lucene.Net.Index;
using Lucene.Net.Documents;
using Lucene.Net.Search;
using Lucene.Net.Analysis.DChinese;

using Version = Lucene.Net.Util.Version;
using FSDirectory = Lucene.Net.Store.FSDirectory;
using NativeFSLockFactory = Lucene.Net.Store.NativeFSLockFactory;

namespace WebApplication6
{
    public class IndexFiles
    {
        public static bool CreateIndexFromFile(DirectoryInfo docDir, DirectoryInfo IndexDir)
        {
            bool bExist = File.Exists(docDir.FullName) || Directory.Exists(docDir.FullName);
            if (!bExist)
            {
                return false;
            }

            //using (IndexWriter writer = new IndexWriter(FSDirectory.Open(IndexDir), new DChineseAnalyzer(Version.LUCENE_30), true, IndexWriter.MaxFieldLength.LIMITED) )
            FSDirectory fsDirrctory = FSDirectory.Open(IndexDir,new NativeFSLockFactory());
            //IndexWriter writer = new IndexWriter(fsDirrctory, new StandardAnalyzer(Version.LUCENE_30), true, IndexWriter.MaxFieldLength.LIMITED);
            IndexWriter writer = new IndexWriter(fsDirrctory, new DChineseAnalyzer(Version.LUCENE_30), true, IndexWriter.MaxFieldLength.LIMITED);
            try
            {
                IndexDirectory(writer, docDir);
                writer.Optimize();
                writer.Commit();
            }
            finally
            {
                writer.Dispose();
                fsDirrctory.Dispose();
            }
            
            return true;
        }

        internal static void IndexDirectory(IndexWriter writer, DirectoryInfo directory)
        {
            foreach (var subDirectory in directory.GetDirectories())
                IndexDirectory(writer, subDirectory);

            foreach (var file in directory.GetFiles())
                IndexDocs(writer, file);
        }

        internal static void IndexDocs(IndexWriter writer, FileInfo file)
        {
            Console.Out.WriteLine("adding " + file);

            try
            {
                writer.AddDocument(Document(file));
            }
            catch (FileNotFoundException)
            {
                // At least on Windows, some temporary files raise this exception with an
                // "access denied" message checking if the file can be read doesn't help.
            }
            catch (UnauthorizedAccessException)
            {
                // Handle any access-denied errors that occur while reading the file.    
            }
            catch (IOException)
            {
                // Generic handler for any io-related exceptions that occur.
            }
        }

        public static Document Document(FileInfo f)
        {

            // make a new, empty document
            Document doc = new Document();

            // Add the path of the file as a field named "path".  Use a field that is 
            // indexed (i.e. searchable), but don't tokenize the field into words.
            doc.Add(new Field("path", f.FullName, Field.Store.YES, Field.Index.NOT_ANALYZED));

            // Add the last modified date of the file a field named "modified".  Use 
            // a field that is indexed (i.e. searchable), but don't tokenize the field
            // into words.
            doc.Add(new Field("modified", DateTools.TimeToString(f.LastWriteTime.Millisecond, DateTools.Resolution.MINUTE), Field.Store.YES, Field.Index.NOT_ANALYZED));

            // Add the contents of the file to a field named "contents".  Specify a Reader,
            // so that the text of the file is tokenized and indexed, but not stored.
            // Note that FileReader expects the file to be in the system's default encoding.
            // If that's not the case searching for special characters will fail.

            string str = File.ReadAllText(f.FullName);
            //doc.Add(new Field("contents", new StreamReader(f.FullName, System.Text.Encoding.UTF8)));
            doc.Add(new Field("contents", str, Field.Store.YES, Field.Index.ANALYZED, Field.TermVector.WITH_POSITIONS_OFFSETS));

            // return the document
            return doc;
        }
    }
}