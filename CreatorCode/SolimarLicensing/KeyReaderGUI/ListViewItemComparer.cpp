#include "stdafx.h"
#include "ListViewItemComparer.h"

using namespace ListViewComparer;

//Constructor. 
//Initializes the member variables and creates a table of appropriate size.
ListViewItemComparer::ListViewItemComparer()
{
	// Initialize the column to '0'.
	ColumnToSort = 0;

	// Initialize the sort order to 'none'.
	OrderOfSort = SortOrder::None;

	// Initialize the CaseInsensitiveComparer object.
	ObjectCompare = new CaseInsensitiveComparer();
}

/// <summary>
/// This method is inherited from the IComparer interface.  It compares the two objects
/// that are passed by using a case insensitive comparison.
/// </summary>
/// <param name="x">First object to be compared</param>
/// <param name="y">Second object to be compared</param>
/// <returns>The result of the comparison. "0" if equal, negative if 'x' is less than 'y' and positive if 'x' is greater than 'y'</returns>
int ListViewItemComparer::Compare(Object* x, Object* y) 
{
	int compareResult;
	ListViewItem *listviewX, *listviewY;

	// Cast the objects to be compared to ListViewItem objects.
	listviewX = static_cast <ListViewItem *> (x);
	listviewY = static_cast <ListViewItem *> (y);

	if(ColumnToSort == EXPIRATION_DATE_COLUMN)
	{
		System::DateTime firstDate =
			DateTime::Parse(listviewX->SubItems->Item[ColumnToSort]->Text);
		System::DateTime secondDate =
			DateTime::Parse(listviewY->SubItems->Item[ColumnToSort]->Text);

        // Compare the two dates.
		compareResult = DateTime::Compare(firstDate, secondDate);
	}

	else if(ColumnToSort == HOURS_LEFT_COLUMN)
	{
		int firstint = System::Convert::ToInt16(listviewX->SubItems->Item[ColumnToSort]->Text);
		int secondint = System::Convert::ToInt16(listviewY->SubItems->Item[ColumnToSort]->Text);
		
		compareResult = (firstint < secondint ? -1 : 
						(firstint > secondint ? 1 : 0));
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