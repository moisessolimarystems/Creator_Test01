#ifndef __L_UsInAAttribs_h__
#define __L_UsInAAttribs_h__

#include "LicAttribsBase.h"
class Lic_UsageInfoAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_UsInA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"License Usage Info Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_UsageInfoAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
	class Lic_UsModuleInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_UMA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Information - Usage Module Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_UsModuleInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
			
		SpdAttribs::DwordAttrib moduleID;
		SpdAttribs::DwordAttrib moduleUsage;
		SpdAttribs::DwordAttrib moduleTotal;
	
		
	
		Lic_UsModuleInfoAttribs& operator =(const Lic_UsModuleInfoAttribs &c)
		{
			moduleID = c.moduleID;
			moduleUsage = c.moduleUsage;
			moduleTotal = c.moduleTotal;
			return *this ;
		}
	
		Lic_UsModuleInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			moduleID(m_mapAttribObjs, L"mI", 0),
			moduleUsage(m_mapAttribObjs, L"mU", 0),
			moduleTotal(m_mapAttribObjs, L"mT", 0){;}
		
		Lic_UsModuleInfoAttribs(const Lic_UsModuleInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			moduleID(m_mapAttribObjs, L"mI", c.moduleID),
			moduleUsage(m_mapAttribObjs, L"mU", c.moduleUsage),
			moduleTotal(m_mapAttribObjs, L"mT", c.moduleTotal){;}
		
		Lic_UsModuleInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			moduleID(m_mapAttribObjs, L"mI", 0),
			moduleUsage(m_mapAttribObjs, L"mU", 0),
			moduleTotal(m_mapAttribObjs, L"mT", 0){;}
		
		Lic_UsModuleInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_UsModuleInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			moduleID(m_mapAttribObjs, L"mI", c.moduleID),
			moduleUsage(m_mapAttribObjs, L"mU", c.moduleUsage),
			moduleTotal(m_mapAttribObjs, L"mT", c.moduleTotal){;}
		
		Lic_UsModuleInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::DwordAttrib &param_moduleID,
			const SpdAttribs::DwordAttrib &param_moduleUsage,
			const SpdAttribs::DwordAttrib &param_moduleTotal) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				moduleID(m_mapAttribObjs, L"mI", param_moduleID),
				moduleUsage(m_mapAttribObjs, L"mU", param_moduleUsage),
				moduleTotal(m_mapAttribObjs, L"mT", param_moduleTotal){;}
		
		Lic_UsModuleInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::DwordAttrib &param_moduleID,
			const SpdAttribs::DwordAttrib &param_moduleUsage,
			const SpdAttribs::DwordAttrib &param_moduleTotal) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				moduleID(m_mapAttribObjs, L"mI", param_moduleID),
				moduleUsage(m_mapAttribObjs, L"mU", param_moduleUsage),
				moduleTotal(m_mapAttribObjs, L"mT", param_moduleTotal){;}
		
	
	};
	
	class Lic_UsAppInstanceInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_UAA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Information - Usage App Instance Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_UsAppInstanceInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		typedef std::vector< Lic_UsModuleInfoAttribs > TVector_Lic_UsModuleInfoAttribsList;
		typedef SpdAttribs::CStreamableVectors< TVector_Lic_UsModuleInfoAttribsList  > Vector_Lic_UsModuleInfoAttribsList_Obj;
		typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_UsModuleInfoAttribsList_Obj, TVector_Lic_UsModuleInfoAttribsList > Lic_UsModuleInfoAttribsList;
			
		SpdAttribs::WStringAttrib applicationInstance;
		Lic_UsModuleInfoAttribsList moduleList;
	
		
	
		Lic_UsAppInstanceInfoAttribs& operator =(const Lic_UsAppInstanceInfoAttribs &c)
		{
			applicationInstance = c.applicationInstance;
			moduleList = c.moduleList;
			return *this ;
		}
	
		Lic_UsAppInstanceInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			applicationInstance(m_mapAttribObjs, L"aI", SpdAttribs::WStringObj(L"")),
			moduleList(m_mapAttribObjs, L"mLt", Vector_Lic_UsModuleInfoAttribsList_Obj()){;}
		
		Lic_UsAppInstanceInfoAttribs(const Lic_UsAppInstanceInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			applicationInstance(m_mapAttribObjs, L"aI", c.applicationInstance),
			moduleList(m_mapAttribObjs, L"mLt", c.moduleList){;}
		
		Lic_UsAppInstanceInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			applicationInstance(m_mapAttribObjs, L"aI", SpdAttribs::WStringObj(L"")),
			moduleList(m_mapAttribObjs, L"mLt", Vector_Lic_UsModuleInfoAttribsList_Obj()){;}
		
		Lic_UsAppInstanceInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_UsAppInstanceInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			applicationInstance(m_mapAttribObjs, L"aI", c.applicationInstance),
			moduleList(m_mapAttribObjs, L"mLt", c.moduleList){;}
		
		Lic_UsAppInstanceInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::WStringAttrib &param_applicationInstance,
			const Lic_UsModuleInfoAttribsList &param_moduleList) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				applicationInstance(m_mapAttribObjs, L"aI", param_applicationInstance),
				moduleList(m_mapAttribObjs, L"mLt", param_moduleList){;}
		
		Lic_UsAppInstanceInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::WStringAttrib &param_applicationInstance,
			const Lic_UsModuleInfoAttribsList &param_moduleList) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				applicationInstance(m_mapAttribObjs, L"aI", param_applicationInstance),
				moduleList(m_mapAttribObjs, L"mLt", param_moduleList){;}
		
	
	};
	
	class Lic_UsProductInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_UPA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Information - Usage Product Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_UsProductInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		typedef std::vector< Lic_UsAppInstanceInfoAttribs > TVector_Lic_UsAppInstanceInfoAttribsList;
		typedef SpdAttribs::CStreamableVectors< TVector_Lic_UsAppInstanceInfoAttribsList  > Vector_Lic_UsAppInstanceInfoAttribsList_Obj;
		typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_UsAppInstanceInfoAttribsList_Obj, TVector_Lic_UsAppInstanceInfoAttribsList > Lic_UsAppInstanceInfoAttribsList;
			
		SpdAttribs::DwordAttrib productID;
		Lic_UsAppInstanceInfoAttribsList appInstanceList;
	
		
	
		Lic_UsProductInfoAttribs& operator =(const Lic_UsProductInfoAttribs &c)
		{
			productID = c.productID;
			appInstanceList = c.appInstanceList;
			return *this ;
		}
	
		Lic_UsProductInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productID(m_mapAttribObjs, L"pI", 0),
			appInstanceList(m_mapAttribObjs, L"aLt", Vector_Lic_UsAppInstanceInfoAttribsList_Obj()){;}
		
		Lic_UsProductInfoAttribs(const Lic_UsProductInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productID(m_mapAttribObjs, L"pI", c.productID),
			appInstanceList(m_mapAttribObjs, L"aLt", c.appInstanceList){;}
		
		Lic_UsProductInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productID(m_mapAttribObjs, L"pI", 0),
			appInstanceList(m_mapAttribObjs, L"aLt", Vector_Lic_UsAppInstanceInfoAttribsList_Obj()){;}
		
		Lic_UsProductInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_UsProductInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productID(m_mapAttribObjs, L"pI", c.productID),
			appInstanceList(m_mapAttribObjs, L"aLt", c.appInstanceList){;}
		
		Lic_UsProductInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::DwordAttrib &param_productID,
			const Lic_UsAppInstanceInfoAttribsList &param_appInstanceList) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				productID(m_mapAttribObjs, L"pI", param_productID),
				appInstanceList(m_mapAttribObjs, L"aLt", param_appInstanceList){;}
		
		Lic_UsProductInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::DwordAttrib &param_productID,
			const Lic_UsAppInstanceInfoAttribsList &param_appInstanceList) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				productID(m_mapAttribObjs, L"pI", param_productID),
				appInstanceList(m_mapAttribObjs, L"aLt", param_appInstanceList){;}
		
	
	};
	
	typedef std::vector< Lic_UsProductInfoAttribs > TVector_Lic_UsProductInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_UsProductInfoAttribsList  > Vector_Lic_UsProductInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_UsProductInfoAttribsList_Obj, TVector_Lic_UsProductInfoAttribsList > Lic_UsProductInfoAttribsList;
		
	Lic_UsProductInfoAttribsList productList;

	

	Lic_UsageInfoAttribs& operator =(const Lic_UsageInfoAttribs &c)
	{
		productList = c.productList;
		return *this ;
	}

	Lic_UsageInfoAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		productList(m_mapAttribObjs, L"pLt", Vector_Lic_UsProductInfoAttribsList_Obj()){;}
	
	Lic_UsageInfoAttribs(const Lic_UsageInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		productList(m_mapAttribObjs, L"pLt", c.productList){;}
	
	Lic_UsageInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		productList(m_mapAttribObjs, L"pLt", Vector_Lic_UsProductInfoAttribsList_Obj()){;}
	
	Lic_UsageInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_UsageInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		productList(m_mapAttribObjs, L"pLt", c.productList){;}
	
	Lic_UsageInfoAttribs(const CWStringStream &wsAttribsClassName,
		const Lic_UsProductInfoAttribsList &param_productList) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			productList(m_mapAttribObjs, L"pLt", param_productList){;}
	
	Lic_UsageInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const Lic_UsProductInfoAttribsList &param_productList) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productList(m_mapAttribObjs, L"pLt", param_productList){;}
	

};

#endif //__L_UsInAAttribs_h__
