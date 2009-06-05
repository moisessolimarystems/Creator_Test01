using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using Client.Creator.CreatorService;

namespace Client.Creator
{
    [TypeConverter(typeof(PropertySorterTypeConverter))]
    public class PacketProperty
    {
        private string _outputPath;        
        private PacketTable _packetData;

        enum Status
        {
            Verified,
            UnVerfied
        }

        public PacketProperty(string newName, string newDescription, DateTime newDate, string newOutputPath, string newUser)
        {
            _packetData = new PacketTable();
            _packetData.PacketName = newName;
            _packetData.PacketComments = newDescription;
            _packetData.DateCreated = newDate;
            _packetData.UserName = newUser;

            _outputPath = newOutputPath;
        }

        [Category("Packet"), PropertyOrder(1)]
        [DisplayName("Name")]
        [Description("Name")]
        [Browsable(true)]
        [ReadOnly(true)]
        public string Name
        {
            get { return _packetData.PacketName; }
            set { _packetData.PacketName = value; }
        }

        [Category("Packet"), PropertyOrder(2)]
        [DisplayName("Date")]
        [Description("Date")]
        [ReadOnly(true)]
        public DateTime Date
        {
            get { return _packetData.DateCreated; }
            set { _packetData.DateCreated = value; }
        }

        [EditorAttribute(typeof(System.Windows.Forms.Design.FolderNameEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("Packet"), PropertyOrder(3)]
        [DisplayName("Output Path")]
        [Description("Output Path")]
        public string OutputPath
        {
            get { return _outputPath; }
            set { _outputPath = value; }
        }

        [Editor(typeof(System.ComponentModel.Design.MultilineStringEditor), typeof(System.Drawing.Design.UITypeEditor))]
        [Category("Packet"), PropertyOrder(4)]
        [DisplayName("Description")]
        [Description("Description")]
        public string Description
        {
            get { return _packetData.PacketComments; }
            set { _packetData.PacketComments = value; }
        }

        [Category("Packet"), PropertyOrder(5)]
        [DisplayName("User")]
        [Description("User")]
        [Browsable(true)]
        [ReadOnly(true)]
        public string User
        {
            get { return _packetData.UserName; }
            set { _packetData.UserName = value; }
        }

    }
}
