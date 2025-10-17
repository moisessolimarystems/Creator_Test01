#ifndef __Lic_GenericAttribsAttribs_h__
#define __Lic_GenericAttribsAttribs_h__

#include "LicAttribsBase.h"
#include "Lic_PackageAttribsHelper.h"
#include "Lic_UsageInfoAttribs.h"
//
// This is a file of Generic classes to be used.


//	Would like to have just used a SpdAttribs::VectorStringAttrib, but the C# code
// will only see SpdAttribs::VectorStringObj if it is the first member in a class.
// If you use SpdAttribs::VectorStringAttrib, it will generate empty xml tags <>, 
// must be in a class.
class Lic_StringListAttribs : public SpdAttribs::CAttribsBase
{
	public:
		Lic_StringListAttribs() : 
			SpdAttribs::CAttribsBase(L"Lic_StringListAttribs"),
			strList(m_mapAttribObjs, L"strList", SpdAttribs::VectorStringObj()){;}
		SpdAttribs::VectorStringAttrib strList;
};

//	Would like to have just used a SpdAttribs::VectorStringAttrib, but the C# code
// will only see SpdAttribs::VectorStringObj if it is the first member in a class.
// If you use SpdAttribs::VectorStringAttrib, it will generate empty xml tags <>, 
// must be in a class.

//	Would like to have just used a Lic_UsAppInstanceInfoAttribsList, but the C# code
// will only see Vector_Lic_UsAppInstanceInfoAttribsList_Obj if it is the first member in a class.
// If you use Lic_UsAppInstanceInfoAttribsList, it will generate empty xml tags <>, 
// must be in a class.
class Lic_UsAppInstanceInfoAttribsListAttribs : public SpdAttribs::CAttribsBase
{
	public:
		Lic_UsAppInstanceInfoAttribsListAttribs() : 
			SpdAttribs::CAttribsBase(L"Lic_UsAppInstanceInfoAttribsListAttribs"),
			attrList(m_mapAttribObjs, L"aLt", Lic_UsageInfoAttribs::Lic_UsProductInfoAttribs::Vector_Lic_UsAppInstanceInfoAttribsList_Obj()){;}
		Lic_UsageInfoAttribs::Lic_UsProductInfoAttribs::Lic_UsAppInstanceInfoAttribsList attrList;
};

//	Would like to have just used a SpdAttribs::MapStringDwordAttrib, but the C# code
// will only see SpdAttribs::MapStringDwordObj if it is the first member in a class.
// If you use SpdAttribs::MapStringDwordAttrib, it will generate empty xml tags <>, 
// must be in a class.
class Lic_StringDwordMapAttribs : public SpdAttribs::CAttribsBase
{
	public:
		Lic_StringDwordMapAttribs() : 
			SpdAttribs::CAttribsBase(L"Lic_StringDwordMapAttribs"),
			strDwordMap(m_mapAttribObjs, L"strDwordMap", SpdAttribs::MapStringDwordObj()){;}
		SpdAttribs::MapStringDwordAttrib strDwordMap;
};

//	Would like to have just used a SpdAttribs::Lic_StringStringMapAttribs, but the C# code
// will only see SpdAttribs::MapStringStringObj if it is the first member in a class.
// If you use SpdAttribs::Lic_StringStringMapAttribs, it will generate empty xml tags <>, 
// must be in a class.
class Lic_StringStringMapAttribs : public SpdAttribs::CAttribsBase
{
	public:
		Lic_StringStringMapAttribs() : 
			SpdAttribs::CAttribsBase(L"Lic_StringStringMapAttribs"),
			strStringMap(m_mapAttribObjs, L"strStringMap", SpdAttribs::MapStringStringObj()){;}
		SpdAttribs::MapStringStringAttrib strStringMap;
};

#endif //__Lic_GenericAttribsAttribs_h__
