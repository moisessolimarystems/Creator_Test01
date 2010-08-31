#ifndef __L_LcSysAAttribs_h__
#define __L_LcSysAAttribs_h__

#include "LicAttribsBase.h"
class Lic_LicenseSystemAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_LcSysA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"License System Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_LicenseSystemAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
		
	SpdAttribs::VectorStringAttrib ListOfStreamed_KeyAttribs;
	SpdAttribs::WStringAttrib Streamed_ServerDataAttribs;
	SpdAttribs::WStringAttrib Streamed_SystemInfoAttribs;
	SpdAttribs::WStringAttrib Streamed_SoftwareSpecAttribs;
	SpdAttribs::WStringAttrib Streamed_UsageInfoAttribs;
	SpdAttribs::WStringAttrib Streamed_SystemEventLogInfoAttribs;
	SpdAttribs::VectorStringAttrib ListOfStreamed_InfoAttribs;
	SpdAttribs::WStringAttrib createdDate;

	

	Lic_LicenseSystemAttribs& operator =(const Lic_LicenseSystemAttribs &c)
	{
		ListOfStreamed_KeyAttribs = c.ListOfStreamed_KeyAttribs;
		Streamed_ServerDataAttribs = c.Streamed_ServerDataAttribs;
		Streamed_SystemInfoAttribs = c.Streamed_SystemInfoAttribs;
		Streamed_SoftwareSpecAttribs = c.Streamed_SoftwareSpecAttribs;
		Streamed_UsageInfoAttribs = c.Streamed_UsageInfoAttribs;
		Streamed_SystemEventLogInfoAttribs = c.Streamed_SystemEventLogInfoAttribs;
		ListOfStreamed_InfoAttribs = c.ListOfStreamed_InfoAttribs;
		createdDate = c.createdDate;
		return *this ;
	}

	Lic_LicenseSystemAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		ListOfStreamed_KeyAttribs(m_mapAttribObjs, L"kLt", SpdAttribs::VectorStringObj()),
		Streamed_ServerDataAttribs(m_mapAttribObjs, L"sD", SpdAttribs::WStringObj(L"")),
		Streamed_SystemInfoAttribs(m_mapAttribObjs, L"sI", SpdAttribs::WStringObj(L"")),
		Streamed_SoftwareSpecAttribs(m_mapAttribObjs, L"sS", SpdAttribs::WStringObj(L"")),
		Streamed_UsageInfoAttribs(m_mapAttribObjs, L"sU", SpdAttribs::WStringObj(L"")),
		Streamed_SystemEventLogInfoAttribs(m_mapAttribObjs, L"sE", SpdAttribs::WStringObj(L"")),
		ListOfStreamed_InfoAttribs(m_mapAttribObjs, L"iLt", SpdAttribs::VectorStringObj()),
		createdDate(m_mapAttribObjs, L"cDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")){;}
	
	Lic_LicenseSystemAttribs(const Lic_LicenseSystemAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		ListOfStreamed_KeyAttribs(m_mapAttribObjs, L"kLt", c.ListOfStreamed_KeyAttribs),
		Streamed_ServerDataAttribs(m_mapAttribObjs, L"sD", c.Streamed_ServerDataAttribs),
		Streamed_SystemInfoAttribs(m_mapAttribObjs, L"sI", c.Streamed_SystemInfoAttribs),
		Streamed_SoftwareSpecAttribs(m_mapAttribObjs, L"sS", c.Streamed_SoftwareSpecAttribs),
		Streamed_UsageInfoAttribs(m_mapAttribObjs, L"sU", c.Streamed_UsageInfoAttribs),
		Streamed_SystemEventLogInfoAttribs(m_mapAttribObjs, L"sE", c.Streamed_SystemEventLogInfoAttribs),
		ListOfStreamed_InfoAttribs(m_mapAttribObjs, L"iLt", c.ListOfStreamed_InfoAttribs),
		createdDate(m_mapAttribObjs, L"cDt", c.createdDate){;}
	
	Lic_LicenseSystemAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		ListOfStreamed_KeyAttribs(m_mapAttribObjs, L"kLt", SpdAttribs::VectorStringObj()),
		Streamed_ServerDataAttribs(m_mapAttribObjs, L"sD", SpdAttribs::WStringObj(L"")),
		Streamed_SystemInfoAttribs(m_mapAttribObjs, L"sI", SpdAttribs::WStringObj(L"")),
		Streamed_SoftwareSpecAttribs(m_mapAttribObjs, L"sS", SpdAttribs::WStringObj(L"")),
		Streamed_UsageInfoAttribs(m_mapAttribObjs, L"sU", SpdAttribs::WStringObj(L"")),
		Streamed_SystemEventLogInfoAttribs(m_mapAttribObjs, L"sE", SpdAttribs::WStringObj(L"")),
		ListOfStreamed_InfoAttribs(m_mapAttribObjs, L"iLt", SpdAttribs::VectorStringObj()),
		createdDate(m_mapAttribObjs, L"cDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")){;}
	
	Lic_LicenseSystemAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_LicenseSystemAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		ListOfStreamed_KeyAttribs(m_mapAttribObjs, L"kLt", c.ListOfStreamed_KeyAttribs),
		Streamed_ServerDataAttribs(m_mapAttribObjs, L"sD", c.Streamed_ServerDataAttribs),
		Streamed_SystemInfoAttribs(m_mapAttribObjs, L"sI", c.Streamed_SystemInfoAttribs),
		Streamed_SoftwareSpecAttribs(m_mapAttribObjs, L"sS", c.Streamed_SoftwareSpecAttribs),
		Streamed_UsageInfoAttribs(m_mapAttribObjs, L"sU", c.Streamed_UsageInfoAttribs),
		Streamed_SystemEventLogInfoAttribs(m_mapAttribObjs, L"sE", c.Streamed_SystemEventLogInfoAttribs),
		ListOfStreamed_InfoAttribs(m_mapAttribObjs, L"iLt", c.ListOfStreamed_InfoAttribs),
		createdDate(m_mapAttribObjs, L"cDt", c.createdDate){;}
	
	Lic_LicenseSystemAttribs(const CWStringStream &wsAttribsClassName,
		const SpdAttribs::VectorStringAttrib &param_ListOfStreamed_KeyAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_ServerDataAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_SystemInfoAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_SoftwareSpecAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_UsageInfoAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_SystemEventLogInfoAttribs,
		const SpdAttribs::VectorStringAttrib &param_ListOfStreamed_InfoAttribs,
		const SpdAttribs::WStringAttrib &param_createdDate) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			ListOfStreamed_KeyAttribs(m_mapAttribObjs, L"kLt", param_ListOfStreamed_KeyAttribs),
			Streamed_ServerDataAttribs(m_mapAttribObjs, L"sD", param_Streamed_ServerDataAttribs),
			Streamed_SystemInfoAttribs(m_mapAttribObjs, L"sI", param_Streamed_SystemInfoAttribs),
			Streamed_SoftwareSpecAttribs(m_mapAttribObjs, L"sS", param_Streamed_SoftwareSpecAttribs),
			Streamed_UsageInfoAttribs(m_mapAttribObjs, L"sU", param_Streamed_UsageInfoAttribs),
			Streamed_SystemEventLogInfoAttribs(m_mapAttribObjs, L"sE", param_Streamed_SystemEventLogInfoAttribs),
			ListOfStreamed_InfoAttribs(m_mapAttribObjs, L"iLt", param_ListOfStreamed_InfoAttribs),
			createdDate(m_mapAttribObjs, L"cDt", param_createdDate){;}
	
	Lic_LicenseSystemAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const SpdAttribs::VectorStringAttrib &param_ListOfStreamed_KeyAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_ServerDataAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_SystemInfoAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_SoftwareSpecAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_UsageInfoAttribs,
		const SpdAttribs::WStringAttrib &param_Streamed_SystemEventLogInfoAttribs,
		const SpdAttribs::VectorStringAttrib &param_ListOfStreamed_InfoAttribs,
		const SpdAttribs::WStringAttrib &param_createdDate) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			ListOfStreamed_KeyAttribs(m_mapAttribObjs, L"kLt", param_ListOfStreamed_KeyAttribs),
			Streamed_ServerDataAttribs(m_mapAttribObjs, L"sD", param_Streamed_ServerDataAttribs),
			Streamed_SystemInfoAttribs(m_mapAttribObjs, L"sI", param_Streamed_SystemInfoAttribs),
			Streamed_SoftwareSpecAttribs(m_mapAttribObjs, L"sS", param_Streamed_SoftwareSpecAttribs),
			Streamed_UsageInfoAttribs(m_mapAttribObjs, L"sU", param_Streamed_UsageInfoAttribs),
			Streamed_SystemEventLogInfoAttribs(m_mapAttribObjs, L"sE", param_Streamed_SystemEventLogInfoAttribs),
			ListOfStreamed_InfoAttribs(m_mapAttribObjs, L"iLt", param_ListOfStreamed_InfoAttribs),
			createdDate(m_mapAttribObjs, L"cDt", param_createdDate){;}
	

};

#endif //__L_LcSysAAttribs_h__
