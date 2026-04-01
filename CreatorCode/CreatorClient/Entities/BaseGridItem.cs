using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
    public class BaseGridItem
    {
        private string name;
        public virtual string Name 
        {
            get { return name; }
        }
    }
}
