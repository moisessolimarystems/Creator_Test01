#pragma once

#include <deque>
#include <objbase.h>

class MultidimensionalSafeArray
{
public:
	
	/** Specifies dimensions of a multidimensional safe array */
	typedef std::deque<unsigned int> DimensionsType;
	
	/** Creates a multidimensional array of arrays
	  * where each array is a variant safearray, 
	  * and the non-array elements are variants
	  * Arguments:
	  *		array		provide pointer to a variant 
	  *					object that will receive the array
	  *		dimensions	listing of the size of each dimension
	  */
	static HRESULT CreateMultidimensionalSafearray(VARIANT* array, DimensionsType &dimensions);
	
	/** Provides access to an element or array in the 
	  * Arguments:
	  *		array		Pointer to an object created
	  *					using a call to CreateMultidimensionalSafearray
	  *		indices		Indices in to the multidimensional array.
	  *					If fewer indices are provided than dimensions
	  *					exist in the array, the array specified by
	  *					the indices will be returned. 
	  *					Eg.: for a 2 dimensional array, specifiying one index, n,
	  *					will cause the function to return a single dimensional
	  *					array, that array at the nth position of the 1st dimension.
	  *		element		Element recieves a pointer to the element
	  *					at the specified indices
	  */
	static HRESULT AccessMultidimensionalSafearray(VARIANT* array, DimensionsType &indices, VARIANT** element);

	/** Provides releases the element obtained with a call to Access...
	  * Decrements reference counts, etc.
	  */
	static HRESULT UnaccessMultidimensionalSafearray(VARIANT* array, DimensionsType &indices);
	
	/** Calls the SafeArrayDelete on all the internal arrays recursively */
	static HRESULT DeleteMultidimensionalSafearray(VARIANT* array);

	/** Gets the dimensions of a multidimensional safearray. *** indices must be empty/clear() ed *** */
	static HRESULT SizeofMultidimensionalSafearray(VARIANT* array, DimensionsType &indices);
	
	/** Reads raw data from a file in to a safe array */
	static HRESULT CreateSafeArrayFromFile(VARIANT* pvArray, BSTR path);

	/** Reads data from a buffer in to a safe array */
	static HRESULT CreateSafeArrayFromBuffer(VARIANT* pvArray, void* buffer, unsigned int buffersize);
};