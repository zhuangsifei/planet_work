using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;

namespace WebApplication6
{
    public partial class Search2 : System.Web.UI.Page
    {
        private const string strIndexPath = "~/DataIndex";
        private const string strSourcePath = "~/Document";
        private DirectoryInfo dirIndex = null;
        private DirectoryInfo dirSource = null;

        protected void Page_Load(object sender, EventArgs e)
        {
            string strIndexPathFull = Context.Server.MapPath(strIndexPath);
            string strSourcePathFull = Context.Server.MapPath(strSourcePath);
            dirIndex = new DirectoryInfo(strIndexPathFull);
            dirSource = new DirectoryInfo(strSourcePathFull);
            
            if (!dirIndex.Exists || dirIndex.GetFiles().Length + dirIndex.GetDirectories().Length == 0)
            {
                if (!dirSource.Exists || dirSource.GetFiles().Length + dirSource.GetDirectories().Length == 0)
                {
                    return;
                }
                IndexFiles.CreateIndexFromFile(dirSource, dirIndex);
            }
           
        }

        protected void Button1_Click(object sender, EventArgs e)
        {
            string strSearchWord = TextBox1.Text;
            List<string> lstTerm = new List<string>();
            lstTerm.Add(strSearchWord);

            List<ItemList> lstResult;
            lstResult = SearchFiles.SearchIndex(dirIndex, lstTerm);
            Repeater1.DataSource = lstResult;
            Repeater1.DataBind();
        }

    }
}