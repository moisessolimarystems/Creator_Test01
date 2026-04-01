#ifndef __L_SysInAAttribs_h__
#define __L_SysInAAttribs_h__

#include "LicAttribsBase.h"
class Lic_SystemInfoAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_SysInA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"System Info Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_SystemInfoAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
		
	SpdAttribs::WStringAttrib licenseServerVersion;
	SpdAttribs::VectorStringAttrib computerNameList;
	SpdAttribs::VectorStringAttrib macAddressList;
	SpdAttribs::VectorStringAttrib biosSerialNumberList;
	SpdAttribs::VectorStringAttrib domainNameList;
	SpdAttribs::VectorStringAttrib operatingSystemList;
	SpdAttribs::BoolAttrib bPartOFDomain;
	SpdAttribs::VectorStringAttrib systemManufacturerList;
	SpdAttribs::VectorStringAttrib systemModelList;
	SpdAttribs::VectorStringAttrib systemTypeList;
	SpdAttribs::VectorStringAttrib systemUuidList;

	

	Lic_SystemInfoAttribs& operator =(const Lic_SystemInfoAttribs &c)
	{
		licenseServerVersion = c.licenseServerVersion;
		computerNameList = c.computerNameList;
		macAddressList = c.macAddressList;
		biosSerialNumberList = c.biosSerialNumberList;
		domainNameList = c.domainNameList;
		operatingSystemList = c.operatingSystemList;
		bPartOFDomain = c.bPartOFDomain;
		systemManufacturerList = c.systemManufacturerList;
		systemModelList = c.systemModelList;
		systemTypeList = c.systemTypeList;
		systemUuidList = c.systemUuidList;
		return *this ;
	}

	Lic_SystemInfoAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		licenseServerVersion(m_mapAttribObjs, L"lV", SpdAttribs::WStringObj(L"")),
		computerNameList(m_mapAttribObjs, L"cLt", SpdAttribs::VectorStringObj()),
		macAddressList(m_mapAttribObjs, L"mLt", SpdAttribs::VectorStringObj()),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", SpdAttribs::VectorStringObj()),
		domainNameList(m_mapAttribObjs, L"dLt", SpdAttribs::VectorStringObj()),
		operatingSystemList(m_mapAttribObjs, L"osLt", SpdAttribs::VectorStringObj()),
		bPartOFDomain(m_mapAttribObjs, L"pD", true),
		systemManufacturerList(m_mapAttribObjs, L"sMFLt", SpdAttribs::VectorStringObj()),
		systemModelList(m_mapAttribObjs, L"sMLt", SpdAttribs::VectorStringObj()),
		systemTypeList(m_mapAttribObjs, L"sTLt", SpdAttribs::VectorStringObj()),
		systemUuidList(m_mapAttribObjs, L"sULt", SpdAttribs::VectorStringObj()){;}
	
	Lic_SystemInfoAttribs(const Lic_SystemInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		licenseServerVersion(m_mapAttribObjs, L"lV", c.licenseServerVersion),
		computerNameList(m_mapAttribObjs, L"cLt", c.computerNameList),
		macAddressList(m_mapAttribObjs, L"mLt", c.macAddressList),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", c.biosSerialNumberList),
		domainNameList(m_mapAttribObjs, L"dLt", c.domainNameList),
		operatingSystemList(m_mapAttribObjs, L"osLt", c.operatingSystemList),
		bPartOFDomain(m_mapAttribObjs, L"pD", c.bPartOFDomain),
		systemManufacturerList(m_mapAttribObjs, L"sMFLt", c.systemManufacturerList),
		systemModelList(m_mapAttribObjs, L"sMLt", c.systemModelList),
		systemTypeList(m_mapAttribObjs, L"sTLt", c.systemTypeList),
		systemUuidList(m_mapAttribObjs, L"sULt", c.systemUuidList){;}
	
	Lic_SystemInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		licenseServerVersion(m_mapAttribObjs, L"lV", SpdAttribs::WStringObj(L"")),
		computerNameList(m_mapAttribObjs, L"cLt", SpdAttribs::VectorStringObj()),
		macAddressList(m_mapAttribObjs, L"mLt", SpdAttribs::VectorStringObj()),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", SpdAttribs::VectorStringObj()),
		domainNameList(m_mapAttribObjs, L"dLt", SpdAttribs::VectorStringObj()),
		operatingSystemList(m_mapAttribObjs, L"osLt", SpdAttribs::VectorStringObj()),
		bPartOFDomain(m_mapAttribObjs, L"pD", true),
		systemManufacturerList(m_mapAttribObjs, L"sMFLt", SpdAttribs::VectorStringObj()),
		systemModelList(m_mapAttribObjs, L"sMLt", SpdAttribs::VectorStringObj()),
		systemTypeList(m_mapAttribObjs, L"sTLt", SpdAttribs::VectorStringObj()),
		systemUuidList(m_mapAttribObjs, L"sULt", SpdAttribs::VectorStringObj()){;}
	
	Lic_SystemInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_SystemInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		licenseServerVersion(m_mapAttribObjs, L"lV", c.licenseServerVersion),
		computerNameList(m_mapAttribObjs, L"cLt", c.computerNameList),
		macAddressList(m_mapAttribObjs, L"mLt", c.macAddressList),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", c.biosSerialNumberList),
		domainNameList(m_mapAttribObjs, L"dLt", c.domainNameList),
		operatingSystemList(m_mapAttribObjs, L"osLt", c.operatingSystemList),
		bPartOFDomain(m_mapAttribObjs, L"pD", c.bPartOFDomain),
		systemManufacturerList(m_mapAttribObjs, L"sMFLt", c.systemManufacturerList),
		systemModelList(m_mapAttribObjs, L"sMLt", c.systemModelList),
		systemTypeList(m_mapAttribObjs, L"sTLt", c.systemTypeList),
		systemUuidList(m_mapAttribObjs, L"sULt", c.systemUuidList){;}
	
	Lic_SystemInfoAttribs(const CWStringStream &wsAttribsClassName,
		const SpdAttribs::WStringAttrib &param_licenseServerVersion,
		const SpdAttribs::VectorStringAttrib &param_computerNameList,
		const SpdAttribs::VectorStringAttrib &param_macAddressList,
		const SpdAttribs::VectorStringAttrib &param_biosSerialNumberList,
		const SpdAttribs::VectorStringAttrib &param_domainNameList,
		const SpdAttribs::VectorStringAttrib &param_operatingSystemList,
		const SpdAttribs::BoolAttrib &param_bPartOFDomain,
		const SpdAttribs::VectorStringAttrib &param_systemManufacturerList,
		const SpdAttribs::VectorStringAttrib &param_systemModelList,
		const SpdAttribs::VectorStringAttrib &param_systemTypeList,
		const SpdAttribs::VectorStringAttrib &param_systemUuidList) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			licenseServerVersion(m_mapAttribObjs, L"lV", param_licenseServerVersion),
			computerNameList(m_mapAttribObjs, L"cLt", param_computerNameList),
			macAddressList(m_mapAttribObjs, L"mLt", param_macAddressList),
			biosSerialNumberList(m_mapAttribObjs, L"bLt", param_biosSerialNumberList),
			domainNameList(m_mapAttribObjs, L"dLt", param_domainNameList),
			operatingSystemList(m_mapAttribObjs, L"osLt", param_operatingSystemList),
			bPartOFDomain(m_mapAttribObjs, L"pD", param_bPartOFDomain),
			systemManufacturerList(m_mapAttribObjs, L"sMFLt", param_systemManufacturerList),
			systemModelList(m_mapAttribObjs, L"sMLt", param_systemModelList),
			systemTypeList(m_mapAttribObjs, L"sTLt", param_systemTypeList),
			systemUuidList(m_mapAttribObjs, L"sULt", param_systemUuidList){;}
	
	Lic_SystemInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const SpdAttribs::WStringAttrib &param_licenseServerVersion,
		const SpdAttribs::VectorStringAttrib &param_computerNameList,
		const SpdAttribs::VectorStringAttrib &param_macAddressList,
		const SpdAttribs::VectorStringAttrib &param_biosSerialNumberList,
		const SpdAttribs::VectorStringAttrib &param_domainNameList,
		const SpdAttribs::VectorStringAttrib &param_operatingSystemList,
		const SpdAttribs::BoolAttrib &param_bPartOFDomain,
		const SpdAttribs::VectorStringAttrib &param_systemManufacturerList,
		const SpdAttribs::VectorStringAttrib &param_systemModelList,
		const SpdAttribs::VectorStringAttrib &param_systemTypeList,
		const SpdAttribs::VectorStringAttrib &param_systemUuidList) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			licenseServerVersion(m_mapAttribObjs, L"lV", param_licenseServerVersion),
			computerNameList(m_mapAttribObjs, L"cLt", param_computerNameList),
			macAddressList(m_mapAttribObjs, L"mLt", param_macAddressList),
			biosSerialNumberList(m_mapAttribObjs, L"bLt", param_biosSerialNumberList),
			domainNameList(m_mapAttribObjs, L"dLt", param_domainNameList),
			operatingSystemList(m_mapAttribObjs, L"osLt", param_operatingSystemList),
			bPartOFDomain(m_mapAttribObjs, L"pD", param_bPartOFDomain),
			systemManufacturerList(m_mapAttribObjs, L"sMFLt", param_systemManufacturerList),
			systemModelList(m_mapAttribObjs, L"sMLt", param_systemModelList),
			systemTypeList(m_mapAttribObjs, L"sTLt", param_systemTypeList),
			systemUuidList(m_mapAttribObjs, L"sULt", param_systemUuidList){;}
	

};

#endif //__L_SysInAAttribs_h__
