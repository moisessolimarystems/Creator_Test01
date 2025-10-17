#include "stdafx.h"
#include "ModListViewItemComparer.h"

using namespace ModListViewItemComparer;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
ModListViewComparer::ModListViewComparer()
{
	// Initialize the column to '0'.
	ColumnToSort = 0;

	// Initialize the sort order to 'none'.
	OrderOfSort = SortOrder::None;

	// Initialize the CaseInsensitiveComparer object.
	ObjectCompare = new CaseInsensitiveComparer();
}

// This method is inherited from the IComparer interface.  It compares the two objects
// that are passed by using a case insensitive comparison.
// Returns the result of the comparison. 
// "0" if equal, 
// negative if 'x' is less than 'y' 
// positive if 'x' is greater than 'y'
int ModListViewComparer::Compare(Object* x, Object* y) 
{
	int compareResult;
	ListViewItem *listviewX, *listviewY;

	// Cast the objects to be compared to ListViewItem objects.
	listviewX = static_cast <ListViewItem *> (x);
	listviewY = static_cast <ListViewItem *> (y);

	if(ColumnToSort == LICENSE_IN_USE_COLUMN)
	{
		//int firstint = System::Convert::ToInt16(listviewX->SubItems->Item[ColumnToSort]->Text);
		//int secondint = System::Convert::ToInt16(listviewY->SubItems->Item[ColumnToSort]->Text);

		long firstint = System::Convert::ToInt32(listviewX->SubItems->Item[ColumnToSort]->Text);
		long secondint = System::Convert::ToInt32(listviewY->SubItems->Item[ColumnToSort]->Text);
		
		compareResult = (firstint < secondint ? -1 : 
						(firstint > secondint ? 1 : 0));
	}

	else if(ColumnToSort == TOTAL_LICENSES_COLUMN)
	{
		if(listviewX->SubItems->Item[ColumnToSort]->Text->Equals("Unlimited"))
		{
			if(listviewY->SubItems->Item[ColumnToSort]->Text->Equals("Unlimited"))
				compareResult = 0;

			compareResult = 1;
		}

		else if(listviewY->SubItems->Item[ColumnToSort]->Text->Equals("Unlimited"))
			compareResult = -1;

		else
		{
			//int firstint = System::Convert::ToInt16(listviewX->SubItems->Item[ColumnToSort]->Text);
			//int secondint = System::Convert::ToInt16(listviewY->SubItems->Item[ColumnToSort]->Text);

			long firstint = System::Convert::ToInt32(listviewX->SubItems->Item[ColumnToSort]->Text);
			long secondint = System::Convert::ToInt32(listviewY->SubItems->Item[ColumnToSort]->Text);

		
			compareResult = (firstint < secondint ? -1 : 
							(firstint > secondint ? 1 : 0));
		}
	}
	else
	{
		compareResult = ObjectCompare->Compare(listviewX->SubItems->Item[ColumnToSort]->Text,listviewY->SubItems->Item[ColumnToSort]->Text);
	}
		
	// Calculate correct return value based on object comparison.
	if (OrderOfSort == SortOrder::Ascending)
	{
		// If ascending sort is selected, return the normal result of the compare operation.
		return compareResult;
	}
	else if (OrderOfSort == SortOrder::Descending)
	{
		// If descending sort is selected, return the negative result of the compare operation.
		return (-compareResult);
	}
	else
	{
		// Return '0' to indicate that they are equal.
		return 0;
	}
}