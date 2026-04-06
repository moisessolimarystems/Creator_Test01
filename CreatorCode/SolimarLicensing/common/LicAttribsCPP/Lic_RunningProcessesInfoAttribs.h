#ifndef __L_RPInAAttribs_h__
#define __L_RPInAAttribs_h__

#include "LicAttribsBase.h"
class Lic_RunningProcessesInfoAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_RPInA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"Running Processes Info Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_RunningProcessesInfoAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
	class Lic_ServicesInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_SIA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"Services Info Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ServicesInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
			
		SpdAttribs::WStringAttrib name;
		SpdAttribs::WStringAttrib path;
		SpdAttribs::BoolAttrib bStarted;
		SpdAttribs::WStringAttrib display;
		SpdAttribs::WStringAttrib description;
		SpdAttribs::WStringAttrib serviceType;
	
		
	
		Lic_ServicesInfoAttribs& operator =(const Lic_ServicesInfoAttribs &c)
		{
			name = c.name;
			path = c.path;
			bStarted = c.bStarted;
			display = c.display;
			description = c.description;
			serviceType = c.serviceType;
			return *this ;
		}
	
		Lic_ServicesInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			name(m_mapAttribObjs, L"nm", SpdAttribs::WStringObj(L"")),
			path(m_mapAttribObjs, L"pt", SpdAttribs::WStringObj(L"")),
			bStarted(m_mapAttribObjs, L"bS", false),
			display(m_mapAttribObjs, L"dn", SpdAttribs::WStringObj(L"")),
			description(m_mapAttribObjs, L"dc", SpdAttribs::WStringObj(L"")),
			serviceType(m_mapAttribObjs, L"st", SpdAttribs::WStringObj(L"")){;}
		
		Lic_ServicesInfoAttribs(const Lic_ServicesInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			name(m_mapAttribObjs, L"nm", c.name),
			path(m_mapAttribObjs, L"pt", c.path),
			bStarted(m_mapAttribObjs, L"bS", c.bStarted),
			display(m_mapAttribObjs, L"dn", c.display),
			description(m_mapAttribObjs, L"dc", c.description),
			serviceType(m_mapAttribObjs, L"st", c.serviceType){;}
		
		Lic_ServicesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			name(m_mapAttribObjs, L"nm", SpdAttribs::WStringObj(L"")),
			path(m_mapAttribObjs, L"pt", SpdAttribs::WStringObj(L"")),
			bStarted(m_mapAttribObjs, L"bS", false),
			display(m_mapAttribObjs, L"dn", SpdAttribs::WStringObj(L"")),
			description(m_mapAttribObjs, L"dc", SpdAttribs::WStringObj(L"")),
			serviceType(m_mapAttribObjs, L"st", SpdAttribs::WStringObj(L"")){;}
		
		Lic_ServicesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ServicesInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			name(m_mapAttribObjs, L"nm", c.name),
			path(m_mapAttribObjs, L"pt", c.path),
			bStarted(m_mapAttribObjs, L"bS", c.bStarted),
			display(m_mapAttribObjs, L"dn", c.display),
			description(m_mapAttribObjs, L"dc", c.description),
			serviceType(m_mapAttribObjs, L"st", c.serviceType){;}
		
		Lic_ServicesInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::WStringAttrib &param_name,
			const SpdAttribs::WStringAttrib &param_path,
			const SpdAttribs::BoolAttrib &param_bStarted,
			const SpdAttribs::WStringAttrib &param_display,
			const SpdAttribs::WStringAttrib &param_description,
			const SpdAttribs::WStringAttrib &param_serviceType) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				name(m_mapAttribObjs, L"nm", param_name),
				path(m_mapAttribObjs, L"pt", param_path),
				bStarted(m_mapAttribObjs, L"bS", param_bStarted),
				display(m_mapAttribObjs, L"dn", param_display),
				description(m_mapAttribObjs, L"dc", param_description),
				serviceType(m_mapAttribObjs, L"st", param_serviceType){;}
		
		Lic_ServicesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::WStringAttrib &param_name,
			const SpdAttribs::WStringAttrib &param_path,
			const SpdAttribs::BoolAttrib &param_bStarted,
			const SpdAttribs::WStringAttrib &param_display,
			const SpdAttribs::WStringAttrib &param_description,
			const SpdAttribs::WStringAttrib &param_serviceType) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				name(m_mapAttribObjs, L"nm", param_name),
				path(m_mapAttribObjs, L"pt", param_path),
				bStarted(m_mapAttribObjs, L"bS", param_bStarted),
				display(m_mapAttribObjs, L"dn", param_display),
				description(m_mapAttribObjs, L"dc", param_description),
				serviceType(m_mapAttribObjs, L"st", param_serviceType){;}
		
	
	};
	
	typedef std::vector< Lic_ServicesInfoAttribs > TVector_Lic_ServicesInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_ServicesInfoAttribsList  > Vector_Lic_ServicesInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ServicesInfoAttribsList_Obj, TVector_Lic_ServicesInfoAttribsList > Lic_ServicesInfoAttribsList;
		
	Lic_ServicesInfoAttribsList servicesList;

	

	Lic_RunningProcessesInfoAttribs& operator =(const Lic_RunningProcessesInfoAttribs &c)
	{
		servicesList = c.servicesList;
		return *this ;
	}

	Lic_RunningProcessesInfoAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		servicesList(m_mapAttribObjs, L"sLt", Vector_Lic_ServicesInfoAttribsList_Obj()){;}
	
	Lic_RunningProcessesInfoAttribs(const Lic_RunningProcessesInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		servicesList(m_mapAttribObjs, L"sLt", c.servicesList){;}
	
	Lic_RunningProcessesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		servicesList(m_mapAttribObjs, L"sLt", Vector_Lic_ServicesInfoAttribsList_Obj()){;}
	
	Lic_RunningProcessesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_RunningProcessesInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		servicesList(m_mapAttribObjs, L"sLt", c.servicesList){;}
	
	Lic_RunningProcessesInfoAttribs(const CWStringStream &wsAttribsClassName,
		const Lic_ServicesInfoAttribsList &param_servicesList) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			servicesList(m_mapAttribObjs, L"sLt", param_servicesList){;}
	
	Lic_RunningProcessesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const Lic_ServicesInfoAttribsList &param_servicesList) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			servicesList(m_mapAttribObjs, L"sLt", param_servicesList){;}
	

};

#endif //__L_RPInAAttribs_h__
