using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;

namespace Client.Creator
{
    [ToolboxItem(false)]
    public partial class StaticImageList : Component
    {
        public StaticImageList()
        {
            InitializeComponent();
        }

        public StaticImageList(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
        }

        private ImageList globalImageList;
        public ImageList GlobalImageList
        {
            get
            {
                return globalImageList;
            }
            set
            {
                globalImageList = value;
            }
        }

        private static StaticImageList _instance;
        public static StaticImageList Instance
        {
            get
            {
                if (_instance == null) _instance = new StaticImageList();
                return _instance;
            }
        }
    }
}
