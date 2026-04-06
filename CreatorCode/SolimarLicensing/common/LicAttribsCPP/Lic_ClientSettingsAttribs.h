#ifndef __L_csAAttribs_h__
#define __L_csAAttribs_h__

#include "LicAttribsBase.h"
class Lic_ClientSettingsAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_csA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"Client Settings Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_ClientSettingsAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
	class Lic_ProductConnectionAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_pcA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"Product Connection Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ProductConnectionAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
			
		SpdAttribs::DwordAttrib productID;
		SpdAttribs::WStringAttrib serverName;
		SpdAttribs::WStringAttrib backupServerName;
		SpdAttribs::BoolAttrib bTestDev;
		SpdAttribs::BoolAttrib bInstalled;
	
		
	
		Lic_ProductConnectionAttribs& operator =(const Lic_ProductConnectionAttribs &c)
		{
			productID = c.productID;
			serverName = c.serverName;
			backupServerName = c.backupServerName;
			bTestDev = c.bTestDev;
			bInstalled = c.bInstalled;
			return *this ;
		}
	
		Lic_ProductConnectionAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productID(m_mapAttribObjs, L"pI", 0),
			serverName(m_mapAttribObjs, L"sn", SpdAttribs::WStringObj(L"")),
			backupServerName(m_mapAttribObjs, L"bsn", SpdAttribs::WStringObj(L"")),
			bTestDev(m_mapAttribObjs, L"bTd", false),
			bInstalled(m_mapAttribObjs, L"bI", false){;}
		
		Lic_ProductConnectionAttribs(const Lic_ProductConnectionAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productID(m_mapAttribObjs, L"pI", c.productID),
			serverName(m_mapAttribObjs, L"sn", c.serverName),
			backupServerName(m_mapAttribObjs, L"bsn", c.backupServerName),
			bTestDev(m_mapAttribObjs, L"bTd", c.bTestDev),
			bInstalled(m_mapAttribObjs, L"bI", c.bInstalled){;}
		
		Lic_ProductConnectionAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productID(m_mapAttribObjs, L"pI", 0),
			serverName(m_mapAttribObjs, L"sn", SpdAttribs::WStringObj(L"")),
			backupServerName(m_mapAttribObjs, L"bsn", SpdAttribs::WStringObj(L"")),
			bTestDev(m_mapAttribObjs, L"bTd", false),
			bInstalled(m_mapAttribObjs, L"bI", false){;}
		
		Lic_ProductConnectionAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ProductConnectionAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productID(m_mapAttribObjs, L"pI", c.productID),
			serverName(m_mapAttribObjs, L"sn", c.serverName),
			backupServerName(m_mapAttribObjs, L"bsn", c.backupServerName),
			bTestDev(m_mapAttribObjs, L"bTd", c.bTestDev),
			bInstalled(m_mapAttribObjs, L"bI", c.bInstalled){;}
		
		Lic_ProductConnectionAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::DwordAttrib &param_productID,
			const SpdAttribs::WStringAttrib &param_serverName,
			const SpdAttribs::WStringAttrib &param_backupServerName,
			const SpdAttribs::BoolAttrib &param_bTestDev,
			const SpdAttribs::BoolAttrib &param_bInstalled) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				productID(m_mapAttribObjs, L"pI", param_productID),
				serverName(m_mapAttribObjs, L"sn", param_serverName),
				backupServerName(m_mapAttribObjs, L"bsn", param_backupServerName),
				bTestDev(m_mapAttribObjs, L"bTd", param_bTestDev),
				bInstalled(m_mapAttribObjs, L"bI", param_bInstalled){;}
		
		Lic_ProductConnectionAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::DwordAttrib &param_productID,
			const SpdAttribs::WStringAttrib &param_serverName,
			const SpdAttribs::WStringAttrib &param_backupServerName,
			const SpdAttribs::BoolAttrib &param_bTestDev,
			const SpdAttribs::BoolAttrib &param_bInstalled) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				productID(m_mapAttribObjs, L"pI", param_productID),
				serverName(m_mapAttribObjs, L"sn", param_serverName),
				backupServerName(m_mapAttribObjs, L"bsn", param_backupServerName),
				bTestDev(m_mapAttribObjs, L"bTd", param_bTestDev),
				bInstalled(m_mapAttribObjs, L"bI", param_bInstalled){;}
		
	
	};
	
	typedef std::vector< Lic_ProductConnectionAttribs > TVector_Lic_ProductConnectionAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_ProductConnectionAttribsList  > Vector_Lic_ProductConnectionAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ProductConnectionAttribsList_Obj, TVector_Lic_ProductConnectionAttribsList > Lic_ProductConnectionAttribsList;
		
	Lic_ProductConnectionAttribsList prodConnList;

	

	Lic_ClientSettingsAttribs& operator =(const Lic_ClientSettingsAttribs &c)
	{
		prodConnList = c.prodConnList;
		return *this ;
	}

	Lic_ClientSettingsAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		prodConnList(m_mapAttribObjs, L"pcLt", Vector_Lic_ProductConnectionAttribsList_Obj()){;}
	
	Lic_ClientSettingsAttribs(const Lic_ClientSettingsAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		prodConnList(m_mapAttribObjs, L"pcLt", c.prodConnList){;}
	
	Lic_ClientSettingsAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		prodConnList(m_mapAttribObjs, L"pcLt", Vector_Lic_ProductConnectionAttribsList_Obj()){;}
	
	Lic_ClientSettingsAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ClientSettingsAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		prodConnList(m_mapAttribObjs, L"pcLt", c.prodConnList){;}
	
	Lic_ClientSettingsAttribs(const CWStringStream &wsAttribsClassName,
		const Lic_ProductConnectionAttribsList &param_prodConnList) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			prodConnList(m_mapAttribObjs, L"pcLt", param_prodConnList){;}
	
	Lic_ClientSettingsAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const Lic_ProductConnectionAttribsList &param_prodConnList) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			prodConnList(m_mapAttribObjs, L"pcLt", param_prodConnList){;}
	

};

#endif //__L_csAAttribs_h__
