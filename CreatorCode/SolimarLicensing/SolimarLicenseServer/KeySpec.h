
#pragma once

#include <comdef.h>
#pragma warning(disable:4786)
#include <vector>
#include <string>
#include <map>

// encapsulates a key layout specification which describes the 
// physical layout of data on the protection key or other protection storage
class KeySpec
{
public:
	KeySpec();	
	~KeySpec();
	
	template<class T>
	class DualDataLookup
	{
	public:	
	
		typedef std::map<unsigned int,unsigned int> LookupInt;
		typedef std::map<std::wstring,unsigned int> LookupString;
		
		// inheriting classes need to populate lookup_int and lookup_string!!!
		virtual void Initialize()=0;
		
		// lookup operators
		T& operator[](const unsigned int index)
		{
			LookupInt::iterator i = lookup_int.find(index);
			if (i==lookup_int.end())
				throw _com_error(E_INVALIDARG);
			return data[i->second];
		}
		
		// lookup operators
		T& operator[](const wchar_t* index)
		{
			LookupString::iterator i = lookup_string.find(index);
			if (i==lookup_string.end())
			{
				throw _com_error(E_INVALIDARG);
			}
			return data[i->second];
		}
		
		typedef std::vector<T> data_list_t;
		data_list_t data;
		
		LookupInt lookup_int;
		LookupString lookup_string;
	};
	
	class Module
	{
	public:
		// module read/write function prototype
		typedef unsigned int (*ReadWriteFunctionPtr)(const unsigned int);
		
		unsigned int id;
		wchar_t* name;
		unsigned short offset;
		unsigned short bits;
		unsigned int default_license;
		unsigned int unlimited;
		unsigned int pool;
		unsigned int counter;
		bool isPool;
		bool isLicense;
		bool isCounter;
		ReadWriteFunctionPtr fn_read;
		ReadWriteFunctionPtr fn_write;
		
		bool isUnlimited(unsigned int license_count) {return (isLicense && unlimited>0 && license_count>=unlimited);}
        
		// module read and write functions
		static unsigned int ReadWrite_IdentityFunction(const unsigned int value) { return value; }
#include "resources\KeySpecFunctions.h"
	};
	
	class Product : public DualDataLookup<Module>
	{
	public:
		unsigned int id;
		wchar_t* name;
		
		// populates the lookup lists with the data in DualDataLookup::data[]
		void Initialize();
	};
	
	class ProductList : public DualDataLookup<Product>
	{
	public:
		// populates the lookup lists with the data in DualDataLookup::data[]
		void Initialize();
	};
	
	class Header
	{
	public:
		unsigned int id;
		wchar_t* name;
		unsigned short offset;
		unsigned short bits;
		unsigned int default_value;
	};

	class HeaderList : public DualDataLookup<Header>
	{
	public:
		// populates the lookup lists with the data in DualDataLookup::data[]
		void Initialize();
	};
	
	class KeyRegion
	{
	public:
		unsigned short offset;
		unsigned short bits;
		wchar_t* type;
	};
	
	typedef std::vector<KeyRegion> KeyRegionList;
	
	KeyRegionList regions;
	HeaderList headers;
	ProductList products;
};
