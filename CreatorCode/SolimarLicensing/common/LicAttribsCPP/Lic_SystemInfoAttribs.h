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

	

	Lic_SystemInfoAttribs& operator =(const Lic_SystemInfoAttribs &c)
	{
		licenseServerVersion = c.licenseServerVersion;
		computerNameList = c.computerNameList;
		macAddressList = c.macAddressList;
		biosSerialNumberList = c.biosSerialNumberList;
		return *this ;
	}

	Lic_SystemInfoAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		licenseServerVersion(m_mapAttribObjs, L"lV", SpdAttribs::WStringObj(L"")),
		computerNameList(m_mapAttribObjs, L"cLt", SpdAttribs::VectorStringObj()),
		macAddressList(m_mapAttribObjs, L"mLt", SpdAttribs::VectorStringObj()),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", SpdAttribs::VectorStringObj()){;}
	
	Lic_SystemInfoAttribs(const Lic_SystemInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		licenseServerVersion(m_mapAttribObjs, L"lV", c.licenseServerVersion),
		computerNameList(m_mapAttribObjs, L"cLt", c.computerNameList),
		macAddressList(m_mapAttribObjs, L"mLt", c.macAddressList),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", c.biosSerialNumberList){;}
	
	Lic_SystemInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		licenseServerVersion(m_mapAttribObjs, L"lV", SpdAttribs::WStringObj(L"")),
		computerNameList(m_mapAttribObjs, L"cLt", SpdAttribs::VectorStringObj()),
		macAddressList(m_mapAttribObjs, L"mLt", SpdAttribs::VectorStringObj()),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", SpdAttribs::VectorStringObj()){;}
	
	Lic_SystemInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_SystemInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		licenseServerVersion(m_mapAttribObjs, L"lV", c.licenseServerVersion),
		computerNameList(m_mapAttribObjs, L"cLt", c.computerNameList),
		macAddressList(m_mapAttribObjs, L"mLt", c.macAddressList),
		biosSerialNumberList(m_mapAttribObjs, L"bLt", c.biosSerialNumberList){;}
	
	Lic_SystemInfoAttribs(const CWStringStream &wsAttribsClassName,
		const SpdAttribs::WStringAttrib &param_licenseServerVersion,
		const SpdAttribs::VectorStringAttrib &param_computerNameList,
		const SpdAttribs::VectorStringAttrib &param_macAddressList,
		const SpdAttribs::VectorStringAttrib &param_biosSerialNumberList) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			licenseServerVersion(m_mapAttribObjs, L"lV", param_licenseServerVersion),
			computerNameList(m_mapAttribObjs, L"cLt", param_computerNameList),
			macAddressList(m_mapAttribObjs, L"mLt", param_macAddressList),
			biosSerialNumberList(m_mapAttribObjs, L"bLt", param_biosSerialNumberList){;}
	
	Lic_SystemInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const SpdAttribs::WStringAttrib &param_licenseServerVersion,
		const SpdAttribs::VectorStringAttrib &param_computerNameList,
		const SpdAttribs::VectorStringAttrib &param_macAddressList,
		const SpdAttribs::VectorStringAttrib &param_biosSerialNumberList) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			licenseServerVersion(m_mapAttribObjs, L"lV", param_licenseServerVersion),
			computerNameList(m_mapAttribObjs, L"cLt", param_computerNameList),
			macAddressList(m_mapAttribObjs, L"mLt", param_macAddressList),
			biosSerialNumberList(m_mapAttribObjs, L"bLt", param_biosSerialNumberList){;}
	

};

#endif //__L_SysInAAttribs_h__
