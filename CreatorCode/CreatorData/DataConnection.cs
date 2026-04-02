using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Configuration;

namespace CreatorData
{
    public partial class CreatorDataContext
    {
        public CreatorDataContext()
            : base(ConfigurationManager.ConnectionStrings["CreatorDB"].ConnectionString, mappingSource)
        {
        }
    }
}
