using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;
using Lucene.Net.Analysis;
using Lucene.Net.Analysis.Standard;
using Lucene.Net.Documents;
using Lucene.Net.Index;
using Lucene.Net.Store;
using Lucene.Net.Util;
using Lucene.Net.QueryParsers;
using Lucene.Net.Search;
using Lucene.Net.Analysis.DChinese;


namespace WebApplication6
{
    public partial class Search : System.Web.UI.Page
    {
        
        protected void Page_Load(object sender, EventArgs e)
        {
            createIndexData();
        }

        private void createIndexData()
        {
            string        indexPath   = Context.Server.MapPath("~/IndexData");
            DirectoryInfo Dir         = new DirectoryInfo(indexPath);
            FSDirectory   directory   = FSDirectory.Open(Dir, new NativeFSLockFactory());
            bool isExist = IndexReader.IndexExists(directory); //是否存在索引库文件夹以及索引库特征文件
            if (isExist)
            {
                if (IndexWriter.IsLocked(directory))
                {
                    IndexWriter.Unlock(directory);
                }
            }

            Analyzer analyzer = new DChineseAnalyzer(Lucene.Net.Util.Version.LUCENE_30);
            //Analyzer analyzer = new StandardAnalyzer(Lucene.Net.Util.Version.LUCENE_30);
            IndexWriter writer  = new IndexWriter(directory, analyzer, !isExist, IndexWriter.MaxFieldLength.UNLIMITED);
            
            addDocument(writer,1, "ASP.Net MVC框架配置与分析", "也就是Microsoft ASP.NET 3.5 Extensions，中国，加油！");
            addDocument(writer, 0, "论爱国", "我是一个中国人，我热爱自己的国家");
            
            //writer.Close();//会自动解锁
            //directory.Close(); //不要忘了Close，否则索引结果搜不到
            writer.Dispose();
            directory.Dispose();
        }

        private void addDocument(IndexWriter writer, int id,string title, string content)
        {
            Document doc = new Document();
            doc.Add(new Field("id", id.ToString(), Field.Store.YES, Field.Index.NOT_ANALYZED));
            doc.Add(new Field("title", title, Field.Store.YES, Field.Index.ANALYZED, Field.TermVector.WITH_POSITIONS_OFFSETS));
            doc.Add(new Field("content", content, Field.Store.YES, Field.Index.ANALYZED, Field.TermVector.WITH_POSITIONS_OFFSETS));
            writer.AddDocument(doc);
        }

        private void searchIndex()
        {
            string          indexPath   = Context.Server.MapPath("~/IndexData");
            FSDirectory     directory   = FSDirectory.Open(new DirectoryInfo(indexPath), new NoLockFactory());
            IndexReader     reader      = IndexReader.Open(directory, true);
            IndexSearcher   searcher    = new IndexSearcher(reader);
            PhraseQuery     query       = new PhraseQuery();

            //query.Add(new Term("content", "就"));
            string strSearch = tbxSearch.Text;
            query.Add(new Term("content", strSearch));
            TopScoreDocCollector collector = TopScoreDocCollector.Create(1000, true);
            searcher.Search(query, null, collector);
            ScoreDoc[] docs = collector.TopDocs(0, collector.TotalHits).ScoreDocs;

            tbxResult.Text = "";
            for (int i = 0; i < docs.Length; i++)
            {
                int      docId  = docs[i].Doc;
                Document doc    = searcher.Doc(docId);
                tbxResult.Text += doc.Get("title");
                tbxResult.Text += doc.Get("content");
                tbxResult.Text += "\n";
            }
            
        }

        protected void btnSearch_Click(object sender, EventArgs e)
        {
            searchIndex();
        }


        
    }
}