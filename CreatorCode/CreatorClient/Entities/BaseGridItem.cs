using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client.Creator
{
	public class BaseGridItem
	{
		//private string name;														// CR.34456.V01; commented out, not needed.

		/// <summary>
		/// This property is expected to be over-ridden by derived class.
		/// </summary>
		public virtual string Name
		{
			get { return "NoNameSet"; }												// CR.34456.V01; return a non-null value.
		}
	}
}
