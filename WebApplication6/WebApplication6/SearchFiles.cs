using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.IO;
using Lucene.Net.Analysis;
using Lucene.Net.Analysis.DChinese;
using Lucene.Net.Documents;
using Lucene.Net.QueryParsers;
using Lucene.Net.Index;
using Lucene.Net.Search;

using FSDirectory = Lucene.Net.Store.FSDirectory;
using NoLockFactory = Lucene.Net.Store.NoLockFactory;
using Version = Lucene.Net.Util.Version;

namespace WebApplication6
{
    public static class SearchFiles
    {
        public static List<ItemList> SearchIndex(DirectoryInfo dirIndex, List<string> termList)
        {
            FSDirectory     dirFS    = FSDirectory.Open(dirIndex, new NoLockFactory());
            IndexReader     reader   = IndexReader.Open(dirFS,true);
            IndexSearcher   searcher = new IndexSearcher(reader);
            Analyzer        analyzer = new DChineseAnalyzer(Version.LUCENE_30);
            PhraseQuery     query    = new PhraseQuery();

            foreach (string word in termList)
            {
                query.Add( new Term("contents",word) );
            }
            query.Slop = 100;

            TopScoreDocCollector collector = TopScoreDocCollector.Create(1000, true);
            searcher.Search(query,collector);
            ScoreDoc[] hits = collector.TopDocs().ScoreDocs;
            List<ItemList> lstResult = new List<ItemList>();
            for (int i = 0; i < hits.Length; i++)
            {
                Document doc = new Document();
                doc = searcher.Doc(hits[i].Doc);
                ItemList item = new ItemList();
                item.ItemContent = doc.Get("contents");
                item.ItemPath = doc.Get("path");
                lstResult.Add(item);
            }
            return lstResult;
        }
    }
}