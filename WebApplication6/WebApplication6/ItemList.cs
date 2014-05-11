using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace WebApplication6
{
    [Serializable]
    public class ItemList
    {
        private int     nItemId;
        private string  strItemName;
        private string  strItemContent;
        private string  strItemPath;

        public int ItemID
        {
            get { return this.nItemId; }
            set { this.nItemId = value; }
        }

        public string ItemName
        {
            get { return this.strItemName; }
            set { this.strItemName = value; }
        }

        public string ItemContent
        {
            get { return this.strItemContent; }
            set { this.strItemContent = value; }
        }

        public string ItemPath
        {
            get { return this.strItemPath; }
            set { this.strItemPath = value; }
        }
    }
}