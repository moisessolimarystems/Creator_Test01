#ifndef __L_PkgAAttribs_h__
#define __L_PkgAAttribs_h__

#include "LicAttribsBase.h"
class Lic_PackageAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_PkgA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"License Validation Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_PackageAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
	typedef enum {
		pid_SPD = 0,
		pid_XImage = 1,
		pid_XImageNT = 2,
		pid_SolPCL = 3,
		pid_NotAvailable1 = 4,
		pid_IndexPlugin = 5,
		pid_Connect = 6,
		pid_Iconvert = 7,
		pid_SolsearcherEnt = 8,
		pid_SolScript = 9,
		pid_SolIndexer = 10,
		pid_Quantum = 11,
		pid_Rubika = 12,
		pid_SolFusion = 13,
		pid_Spde = 14,
		pid_SolsearcherSp = 15,
		pid_RubikaProcessBuilder = 16,
		pid_SdxDesigner = 17,
		pid_SpdeQueueManager = 18,
		pid_TestDevIconvert = 19,
		pid_TestDevSolsearcherEp = 20,
		pid_TestDevSolIndexer = 21,
		pid_TestDevRubika = 22,
		pid_TestDevSpde = 23,
		pid_TestDevSseSp = 24,
		pid_TestDevSolfusionSp = 25,
		pid_TestDevSdxDesigner = 26,
		pid_TestDevSpdeQueueManager = 27,
		pid_TestDevRubikaProcessBuilder = 28,
		pid_TestDevSolScript = 29,
		pid_UnknownProduct = 0xFFFF,
	} TLic_ProductID;
	
	/*
	Software Spec, Module Attributes
	*/
	class Lic_ModuleSoftwareSpecAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_MSwSpA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"Software Spec - Module Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ModuleSoftwareSpecAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
			
		SpdAttribs::DwordAttrib moduleID;
		SpdAttribs::WStringAttrib moduleName;
		SpdAttribs::DwordAttrib modUnlimitedValue;
		SpdAttribs::DwordAttrib modulePoolModuleID;
		SpdAttribs::DwordAttrib moduleDefaultLicense;
		SpdAttribs::DwordAttrib moduleTrialLicense;
		SpdAttribs::BoolAttrib bModuleIsCounter;
		SpdAttribs::BoolAttrib bModuleIsPool;
		SpdAttribs::DwordAttrib moduleVersionIntroduced_Major;
		SpdAttribs::DwordAttrib moduleVersionIntroduced_Minor;
		SpdAttribs::DwordAttrib moduleVersionIntroduced_SubMajor;
		SpdAttribs::DwordAttrib moduleVersionIntroduced_SubMinor;
		SpdAttribs::DwordAttrib moduleVersionDeprecated_Major;
		SpdAttribs::DwordAttrib moduleVersionDeprecated_Minor;
		SpdAttribs::DwordAttrib moduleVersionDeprecated_SubMajor;
		SpdAttribs::DwordAttrib moduleVersionDeprecated_SubMinor;
	
		
	
		Lic_ModuleSoftwareSpecAttribs& operator =(const Lic_ModuleSoftwareSpecAttribs &c)
		{
			moduleID = c.moduleID;
			moduleName = c.moduleName;
			modUnlimitedValue = c.modUnlimitedValue;
			modulePoolModuleID = c.modulePoolModuleID;
			moduleDefaultLicense = c.moduleDefaultLicense;
			moduleTrialLicense = c.moduleTrialLicense;
			bModuleIsCounter = c.bModuleIsCounter;
			bModuleIsPool = c.bModuleIsPool;
			moduleVersionIntroduced_Major = c.moduleVersionIntroduced_Major;
			moduleVersionIntroduced_Minor = c.moduleVersionIntroduced_Minor;
			moduleVersionIntroduced_SubMajor = c.moduleVersionIntroduced_SubMajor;
			moduleVersionIntroduced_SubMinor = c.moduleVersionIntroduced_SubMinor;
			moduleVersionDeprecated_Major = c.moduleVersionDeprecated_Major;
			moduleVersionDeprecated_Minor = c.moduleVersionDeprecated_Minor;
			moduleVersionDeprecated_SubMajor = c.moduleVersionDeprecated_SubMajor;
			moduleVersionDeprecated_SubMinor = c.moduleVersionDeprecated_SubMinor;
			return *this ;
		}
	
		Lic_ModuleSoftwareSpecAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			moduleID(m_mapAttribObjs, L"mI", 0),
			moduleName(m_mapAttribObjs, L"mN", SpdAttribs::WStringObj(L"")),
			modUnlimitedValue(m_mapAttribObjs, L"mUV", 0),
			modulePoolModuleID(m_mapAttribObjs, L"mPMI", 0),
			moduleDefaultLicense(m_mapAttribObjs, L"mDL", 0),
			moduleTrialLicense(m_mapAttribObjs, L"mTV", 0),
			bModuleIsCounter(m_mapAttribObjs, L"bMC", false),
			bModuleIsPool(m_mapAttribObjs, L"bMP", false),
			moduleVersionIntroduced_Major(m_mapAttribObjs, L"iVMj", 0),
			moduleVersionIntroduced_Minor(m_mapAttribObjs, L"iVMn", 0),
			moduleVersionIntroduced_SubMajor(m_mapAttribObjs, L"iVSMj", 0),
			moduleVersionIntroduced_SubMinor(m_mapAttribObjs, L"iVSMn", 0),
			moduleVersionDeprecated_Major(m_mapAttribObjs, L"dVMj", 0),
			moduleVersionDeprecated_Minor(m_mapAttribObjs, L"dVMn", 0),
			moduleVersionDeprecated_SubMajor(m_mapAttribObjs, L"dVSMj", 0),
			moduleVersionDeprecated_SubMinor(m_mapAttribObjs, L"dVSMn", 0){;}
		
		Lic_ModuleSoftwareSpecAttribs(const Lic_ModuleSoftwareSpecAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			moduleID(m_mapAttribObjs, L"mI", c.moduleID),
			moduleName(m_mapAttribObjs, L"mN", c.moduleName),
			modUnlimitedValue(m_mapAttribObjs, L"mUV", c.modUnlimitedValue),
			modulePoolModuleID(m_mapAttribObjs, L"mPMI", c.modulePoolModuleID),
			moduleDefaultLicense(m_mapAttribObjs, L"mDL", c.moduleDefaultLicense),
			moduleTrialLicense(m_mapAttribObjs, L"mTV", c.moduleTrialLicense),
			bModuleIsCounter(m_mapAttribObjs, L"bMC", c.bModuleIsCounter),
			bModuleIsPool(m_mapAttribObjs, L"bMP", c.bModuleIsPool),
			moduleVersionIntroduced_Major(m_mapAttribObjs, L"iVMj", c.moduleVersionIntroduced_Major),
			moduleVersionIntroduced_Minor(m_mapAttribObjs, L"iVMn", c.moduleVersionIntroduced_Minor),
			moduleVersionIntroduced_SubMajor(m_mapAttribObjs, L"iVSMj", c.moduleVersionIntroduced_SubMajor),
			moduleVersionIntroduced_SubMinor(m_mapAttribObjs, L"iVSMn", c.moduleVersionIntroduced_SubMinor),
			moduleVersionDeprecated_Major(m_mapAttribObjs, L"dVMj", c.moduleVersionDeprecated_Major),
			moduleVersionDeprecated_Minor(m_mapAttribObjs, L"dVMn", c.moduleVersionDeprecated_Minor),
			moduleVersionDeprecated_SubMajor(m_mapAttribObjs, L"dVSMj", c.moduleVersionDeprecated_SubMajor),
			moduleVersionDeprecated_SubMinor(m_mapAttribObjs, L"dVSMn", c.moduleVersionDeprecated_SubMinor){;}
		
		Lic_ModuleSoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			moduleID(m_mapAttribObjs, L"mI", 0),
			moduleName(m_mapAttribObjs, L"mN", SpdAttribs::WStringObj(L"")),
			modUnlimitedValue(m_mapAttribObjs, L"mUV", 0),
			modulePoolModuleID(m_mapAttribObjs, L"mPMI", 0),
			moduleDefaultLicense(m_mapAttribObjs, L"mDL", 0),
			moduleTrialLicense(m_mapAttribObjs, L"mTV", 0),
			bModuleIsCounter(m_mapAttribObjs, L"bMC", false),
			bModuleIsPool(m_mapAttribObjs, L"bMP", false),
			moduleVersionIntroduced_Major(m_mapAttribObjs, L"iVMj", 0),
			moduleVersionIntroduced_Minor(m_mapAttribObjs, L"iVMn", 0),
			moduleVersionIntroduced_SubMajor(m_mapAttribObjs, L"iVSMj", 0),
			moduleVersionIntroduced_SubMinor(m_mapAttribObjs, L"iVSMn", 0),
			moduleVersionDeprecated_Major(m_mapAttribObjs, L"dVMj", 0),
			moduleVersionDeprecated_Minor(m_mapAttribObjs, L"dVMn", 0),
			moduleVersionDeprecated_SubMajor(m_mapAttribObjs, L"dVSMj", 0),
			moduleVersionDeprecated_SubMinor(m_mapAttribObjs, L"dVSMn", 0){;}
		
		Lic_ModuleSoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ModuleSoftwareSpecAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			moduleID(m_mapAttribObjs, L"mI", c.moduleID),
			moduleName(m_mapAttribObjs, L"mN", c.moduleName),
			modUnlimitedValue(m_mapAttribObjs, L"mUV", c.modUnlimitedValue),
			modulePoolModuleID(m_mapAttribObjs, L"mPMI", c.modulePoolModuleID),
			moduleDefaultLicense(m_mapAttribObjs, L"mDL", c.moduleDefaultLicense),
			moduleTrialLicense(m_mapAttribObjs, L"mTV", c.moduleTrialLicense),
			bModuleIsCounter(m_mapAttribObjs, L"bMC", c.bModuleIsCounter),
			bModuleIsPool(m_mapAttribObjs, L"bMP", c.bModuleIsPool),
			moduleVersionIntroduced_Major(m_mapAttribObjs, L"iVMj", c.moduleVersionIntroduced_Major),
			moduleVersionIntroduced_Minor(m_mapAttribObjs, L"iVMn", c.moduleVersionIntroduced_Minor),
			moduleVersionIntroduced_SubMajor(m_mapAttribObjs, L"iVSMj", c.moduleVersionIntroduced_SubMajor),
			moduleVersionIntroduced_SubMinor(m_mapAttribObjs, L"iVSMn", c.moduleVersionIntroduced_SubMinor),
			moduleVersionDeprecated_Major(m_mapAttribObjs, L"dVMj", c.moduleVersionDeprecated_Major),
			moduleVersionDeprecated_Minor(m_mapAttribObjs, L"dVMn", c.moduleVersionDeprecated_Minor),
			moduleVersionDeprecated_SubMajor(m_mapAttribObjs, L"dVSMj", c.moduleVersionDeprecated_SubMajor),
			moduleVersionDeprecated_SubMinor(m_mapAttribObjs, L"dVSMn", c.moduleVersionDeprecated_SubMinor){;}
		
		Lic_ModuleSoftwareSpecAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::DwordAttrib &param_moduleID,
			const SpdAttribs::WStringAttrib &param_moduleName,
			const SpdAttribs::DwordAttrib &param_modUnlimitedValue,
			const SpdAttribs::DwordAttrib &param_modulePoolModuleID,
			const SpdAttribs::DwordAttrib &param_moduleDefaultLicense,
			const SpdAttribs::DwordAttrib &param_moduleTrialLicense,
			const SpdAttribs::BoolAttrib &param_bModuleIsCounter,
			const SpdAttribs::BoolAttrib &param_bModuleIsPool,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_Major,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_Minor,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_SubMajor,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_SubMinor,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_Major,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_Minor,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_SubMajor,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_SubMinor) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				moduleID(m_mapAttribObjs, L"mI", param_moduleID),
				moduleName(m_mapAttribObjs, L"mN", param_moduleName),
				modUnlimitedValue(m_mapAttribObjs, L"mUV", param_modUnlimitedValue),
				modulePoolModuleID(m_mapAttribObjs, L"mPMI", param_modulePoolModuleID),
				moduleDefaultLicense(m_mapAttribObjs, L"mDL", param_moduleDefaultLicense),
				moduleTrialLicense(m_mapAttribObjs, L"mTV", param_moduleTrialLicense),
				bModuleIsCounter(m_mapAttribObjs, L"bMC", param_bModuleIsCounter),
				bModuleIsPool(m_mapAttribObjs, L"bMP", param_bModuleIsPool),
				moduleVersionIntroduced_Major(m_mapAttribObjs, L"iVMj", param_moduleVersionIntroduced_Major),
				moduleVersionIntroduced_Minor(m_mapAttribObjs, L"iVMn", param_moduleVersionIntroduced_Minor),
				moduleVersionIntroduced_SubMajor(m_mapAttribObjs, L"iVSMj", param_moduleVersionIntroduced_SubMajor),
				moduleVersionIntroduced_SubMinor(m_mapAttribObjs, L"iVSMn", param_moduleVersionIntroduced_SubMinor),
				moduleVersionDeprecated_Major(m_mapAttribObjs, L"dVMj", param_moduleVersionDeprecated_Major),
				moduleVersionDeprecated_Minor(m_mapAttribObjs, L"dVMn", param_moduleVersionDeprecated_Minor),
				moduleVersionDeprecated_SubMajor(m_mapAttribObjs, L"dVSMj", param_moduleVersionDeprecated_SubMajor),
				moduleVersionDeprecated_SubMinor(m_mapAttribObjs, L"dVSMn", param_moduleVersionDeprecated_SubMinor){;}
		
		Lic_ModuleSoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::DwordAttrib &param_moduleID,
			const SpdAttribs::WStringAttrib &param_moduleName,
			const SpdAttribs::DwordAttrib &param_modUnlimitedValue,
			const SpdAttribs::DwordAttrib &param_modulePoolModuleID,
			const SpdAttribs::DwordAttrib &param_moduleDefaultLicense,
			const SpdAttribs::DwordAttrib &param_moduleTrialLicense,
			const SpdAttribs::BoolAttrib &param_bModuleIsCounter,
			const SpdAttribs::BoolAttrib &param_bModuleIsPool,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_Major,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_Minor,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_SubMajor,
			const SpdAttribs::DwordAttrib &param_moduleVersionIntroduced_SubMinor,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_Major,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_Minor,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_SubMajor,
			const SpdAttribs::DwordAttrib &param_moduleVersionDeprecated_SubMinor) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				moduleID(m_mapAttribObjs, L"mI", param_moduleID),
				moduleName(m_mapAttribObjs, L"mN", param_moduleName),
				modUnlimitedValue(m_mapAttribObjs, L"mUV", param_modUnlimitedValue),
				modulePoolModuleID(m_mapAttribObjs, L"mPMI", param_modulePoolModuleID),
				moduleDefaultLicense(m_mapAttribObjs, L"mDL", param_moduleDefaultLicense),
				moduleTrialLicense(m_mapAttribObjs, L"mTV", param_moduleTrialLicense),
				bModuleIsCounter(m_mapAttribObjs, L"bMC", param_bModuleIsCounter),
				bModuleIsPool(m_mapAttribObjs, L"bMP", param_bModuleIsPool),
				moduleVersionIntroduced_Major(m_mapAttribObjs, L"iVMj", param_moduleVersionIntroduced_Major),
				moduleVersionIntroduced_Minor(m_mapAttribObjs, L"iVMn", param_moduleVersionIntroduced_Minor),
				moduleVersionIntroduced_SubMajor(m_mapAttribObjs, L"iVSMj", param_moduleVersionIntroduced_SubMajor),
				moduleVersionIntroduced_SubMinor(m_mapAttribObjs, L"iVSMn", param_moduleVersionIntroduced_SubMinor),
				moduleVersionDeprecated_Major(m_mapAttribObjs, L"dVMj", param_moduleVersionDeprecated_Major),
				moduleVersionDeprecated_Minor(m_mapAttribObjs, L"dVMn", param_moduleVersionDeprecated_Minor),
				moduleVersionDeprecated_SubMajor(m_mapAttribObjs, L"dVSMj", param_moduleVersionDeprecated_SubMajor),
				moduleVersionDeprecated_SubMinor(m_mapAttribObjs, L"dVSMn", param_moduleVersionDeprecated_SubMinor){;}
		
	
	};
	
	/*
	Software Spec, Product Attributes
	*/
	class Lic_ProductSoftwareSpecAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_PSwSpA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"Software Spec - Product Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ProductSoftwareSpecAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		typedef enum {
			pltServer = 0,
			pltClient = 1,
		} TProductLicenseType;
		
		typedef std::map< SpdAttribs::DwordObj , Lic_ModuleSoftwareSpecAttribs > TMap_Lic_ModuleSoftwareSpecAttribsMap;
		typedef SpdAttribs::CStreamableMaps< TMap_Lic_ModuleSoftwareSpecAttribsMap  > Map_Lic_ModuleSoftwareSpecAttribsMap_Obj;
		typedef SpdAttribs::CAttribMemberBaseU< Map_Lic_ModuleSoftwareSpecAttribsMap_Obj , TMap_Lic_ModuleSoftwareSpecAttribsMap >  Lic_ModuleSoftwareSpecAttribsMap;
			
		SpdAttribs::AttribEnumTypeMember<TProductLicenseType> productLicType;
		SpdAttribs::DwordAttrib productID;
		SpdAttribs::DwordAttrib sameModSpecProductID;
		SpdAttribs::DwordAttrib prevSharedProductID;
		SpdAttribs::WStringAttrib productName;
		Lic_ModuleSoftwareSpecAttribsMap moduleSpecMap;
	
		
	
		Lic_ProductSoftwareSpecAttribs& operator =(const Lic_ProductSoftwareSpecAttribs &c)
		{
			productLicType = c.productLicType;
			productID = c.productID;
			sameModSpecProductID = c.sameModSpecProductID;
			prevSharedProductID = c.prevSharedProductID;
			productName = c.productName;
			moduleSpecMap = c.moduleSpecMap;
			return *this ;
		}
	
		Lic_ProductSoftwareSpecAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productLicType(m_mapAttribObjs, L"pLT", pltServer),
			productID(m_mapAttribObjs, L"pI", 0),
			sameModSpecProductID(m_mapAttribObjs, L"sMSpPI", 0),
			prevSharedProductID(m_mapAttribObjs, L"pSPI", 0),
			productName(m_mapAttribObjs, L"pN", SpdAttribs::WStringObj(L"")),
			moduleSpecMap(m_mapAttribObjs, L"mSpMp", Map_Lic_ModuleSoftwareSpecAttribsMap_Obj()){;}
		
		Lic_ProductSoftwareSpecAttribs(const Lic_ProductSoftwareSpecAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productLicType(m_mapAttribObjs, L"pLT", c.productLicType),
			productID(m_mapAttribObjs, L"pI", c.productID),
			sameModSpecProductID(m_mapAttribObjs, L"sMSpPI", c.sameModSpecProductID),
			prevSharedProductID(m_mapAttribObjs, L"pSPI", c.prevSharedProductID),
			productName(m_mapAttribObjs, L"pN", c.productName),
			moduleSpecMap(m_mapAttribObjs, L"mSpMp", c.moduleSpecMap){;}
		
		Lic_ProductSoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productLicType(m_mapAttribObjs, L"pLT", pltServer),
			productID(m_mapAttribObjs, L"pI", 0),
			sameModSpecProductID(m_mapAttribObjs, L"sMSpPI", 0),
			prevSharedProductID(m_mapAttribObjs, L"pSPI", 0),
			productName(m_mapAttribObjs, L"pN", SpdAttribs::WStringObj(L"")),
			moduleSpecMap(m_mapAttribObjs, L"mSpMp", Map_Lic_ModuleSoftwareSpecAttribsMap_Obj()){;}
		
		Lic_ProductSoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ProductSoftwareSpecAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productLicType(m_mapAttribObjs, L"pLT", c.productLicType),
			productID(m_mapAttribObjs, L"pI", c.productID),
			sameModSpecProductID(m_mapAttribObjs, L"sMSpPI", c.sameModSpecProductID),
			prevSharedProductID(m_mapAttribObjs, L"pSPI", c.prevSharedProductID),
			productName(m_mapAttribObjs, L"pN", c.productName),
			moduleSpecMap(m_mapAttribObjs, L"mSpMp", c.moduleSpecMap){;}
		
		Lic_ProductSoftwareSpecAttribs(const CWStringStream &wsAttribsClassName,
			const TProductLicenseType &param_productLicType,
			const SpdAttribs::DwordAttrib &param_productID,
			const SpdAttribs::DwordAttrib &param_sameModSpecProductID,
			const SpdAttribs::DwordAttrib &param_prevSharedProductID,
			const SpdAttribs::WStringAttrib &param_productName,
			const Lic_ModuleSoftwareSpecAttribsMap &param_moduleSpecMap) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				productLicType(m_mapAttribObjs, L"pLT", param_productLicType),
				productID(m_mapAttribObjs, L"pI", param_productID),
				sameModSpecProductID(m_mapAttribObjs, L"sMSpPI", param_sameModSpecProductID),
				prevSharedProductID(m_mapAttribObjs, L"pSPI", param_prevSharedProductID),
				productName(m_mapAttribObjs, L"pN", param_productName),
				moduleSpecMap(m_mapAttribObjs, L"mSpMp", param_moduleSpecMap){;}
		
		Lic_ProductSoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const TProductLicenseType &param_productLicType,
			const SpdAttribs::DwordAttrib &param_productID,
			const SpdAttribs::DwordAttrib &param_sameModSpecProductID,
			const SpdAttribs::DwordAttrib &param_prevSharedProductID,
			const SpdAttribs::WStringAttrib &param_productName,
			const Lic_ModuleSoftwareSpecAttribsMap &param_moduleSpecMap) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				productLicType(m_mapAttribObjs, L"pLT", param_productLicType),
				productID(m_mapAttribObjs, L"pI", param_productID),
				sameModSpecProductID(m_mapAttribObjs, L"sMSpPI", param_sameModSpecProductID),
				prevSharedProductID(m_mapAttribObjs, L"pSPI", param_prevSharedProductID),
				productName(m_mapAttribObjs, L"pN", param_productName),
				moduleSpecMap(m_mapAttribObjs, L"mSpMp", param_moduleSpecMap){;}
		
	
	};
	
	/*
	Software Spec Attributes
	*/
	class Lic_SoftwareSpecAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_SwSpA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"Software Spec Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_SoftwareSpecAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		typedef std::map< SpdAttribs::DwordObj , Lic_ProductSoftwareSpecAttribs > TMap_Lic_ProductSoftwareSpecAttribsMap;
		typedef SpdAttribs::CStreamableMaps< TMap_Lic_ProductSoftwareSpecAttribsMap  > Map_Lic_ProductSoftwareSpecAttribsMap_Obj;
		typedef SpdAttribs::CAttribMemberBaseU< Map_Lic_ProductSoftwareSpecAttribsMap_Obj , TMap_Lic_ProductSoftwareSpecAttribsMap >  Lic_ProductSoftwareSpecAttribsMap;
			
		Lic_ProductSoftwareSpecAttribsMap productSpecMap;
		SpdAttribs::DwordAttrib softwareSpec_Major;
		SpdAttribs::DwordAttrib softwareSpec_Minor;
		SpdAttribs::DwordAttrib softwareSpec_SubMajor;
		SpdAttribs::DwordAttrib softwareSpec_SubMinor;
	
		
	
		Lic_SoftwareSpecAttribs& operator =(const Lic_SoftwareSpecAttribs &c)
		{
			productSpecMap = c.productSpecMap;
			softwareSpec_Major = c.softwareSpec_Major;
			softwareSpec_Minor = c.softwareSpec_Minor;
			softwareSpec_SubMajor = c.softwareSpec_SubMajor;
			softwareSpec_SubMinor = c.softwareSpec_SubMinor;
			return *this ;
		}
	
		Lic_SoftwareSpecAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productSpecMap(m_mapAttribObjs, L"pMp", Map_Lic_ProductSoftwareSpecAttribsMap_Obj()),
			softwareSpec_Major(m_mapAttribObjs, L"vMj", 0),
			softwareSpec_Minor(m_mapAttribObjs, L"vMn", 0),
			softwareSpec_SubMajor(m_mapAttribObjs, L"vSMj", 0),
			softwareSpec_SubMinor(m_mapAttribObjs, L"vSMn", 0){;}
		
		Lic_SoftwareSpecAttribs(const Lic_SoftwareSpecAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productSpecMap(m_mapAttribObjs, L"pMp", c.productSpecMap),
			softwareSpec_Major(m_mapAttribObjs, L"vMj", c.softwareSpec_Major),
			softwareSpec_Minor(m_mapAttribObjs, L"vMn", c.softwareSpec_Minor),
			softwareSpec_SubMajor(m_mapAttribObjs, L"vSMj", c.softwareSpec_SubMajor),
			softwareSpec_SubMinor(m_mapAttribObjs, L"vSMn", c.softwareSpec_SubMinor){;}
		
		Lic_SoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productSpecMap(m_mapAttribObjs, L"pMp", Map_Lic_ProductSoftwareSpecAttribsMap_Obj()),
			softwareSpec_Major(m_mapAttribObjs, L"vMj", 0),
			softwareSpec_Minor(m_mapAttribObjs, L"vMn", 0),
			softwareSpec_SubMajor(m_mapAttribObjs, L"vSMj", 0),
			softwareSpec_SubMinor(m_mapAttribObjs, L"vSMn", 0){;}
		
		Lic_SoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_SoftwareSpecAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productSpecMap(m_mapAttribObjs, L"pMp", c.productSpecMap),
			softwareSpec_Major(m_mapAttribObjs, L"vMj", c.softwareSpec_Major),
			softwareSpec_Minor(m_mapAttribObjs, L"vMn", c.softwareSpec_Minor),
			softwareSpec_SubMajor(m_mapAttribObjs, L"vSMj", c.softwareSpec_SubMajor),
			softwareSpec_SubMinor(m_mapAttribObjs, L"vSMn", c.softwareSpec_SubMinor){;}
		
		Lic_SoftwareSpecAttribs(const CWStringStream &wsAttribsClassName,
			const Lic_ProductSoftwareSpecAttribsMap &param_productSpecMap,
			const SpdAttribs::DwordAttrib &param_softwareSpec_Major,
			const SpdAttribs::DwordAttrib &param_softwareSpec_Minor,
			const SpdAttribs::DwordAttrib &param_softwareSpec_SubMajor,
			const SpdAttribs::DwordAttrib &param_softwareSpec_SubMinor) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				productSpecMap(m_mapAttribObjs, L"pMp", param_productSpecMap),
				softwareSpec_Major(m_mapAttribObjs, L"vMj", param_softwareSpec_Major),
				softwareSpec_Minor(m_mapAttribObjs, L"vMn", param_softwareSpec_Minor),
				softwareSpec_SubMajor(m_mapAttribObjs, L"vSMj", param_softwareSpec_SubMajor),
				softwareSpec_SubMinor(m_mapAttribObjs, L"vSMn", param_softwareSpec_SubMinor){;}
		
		Lic_SoftwareSpecAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const Lic_ProductSoftwareSpecAttribsMap &param_productSpecMap,
			const SpdAttribs::DwordAttrib &param_softwareSpec_Major,
			const SpdAttribs::DwordAttrib &param_softwareSpec_Minor,
			const SpdAttribs::DwordAttrib &param_softwareSpec_SubMajor,
			const SpdAttribs::DwordAttrib &param_softwareSpec_SubMinor) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				productSpecMap(m_mapAttribObjs, L"pMp", param_productSpecMap),
				softwareSpec_Major(m_mapAttribObjs, L"vMj", param_softwareSpec_Major),
				softwareSpec_Minor(m_mapAttribObjs, L"vMn", param_softwareSpec_Minor),
				softwareSpec_SubMajor(m_mapAttribObjs, L"vSMj", param_softwareSpec_SubMajor),
				softwareSpec_SubMinor(m_mapAttribObjs, L"vSMn", param_softwareSpec_SubMinor){;}
		
	
	};
	
	/*
	License Information - Module Attributes
	*/
	class Lic_ModuleInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_MIA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Information - Module Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ModuleInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		typedef enum {
			msPerm = 0,
			msTrial = 1,
			msAddOn = 2,
			msSubscription = 3,
		} TModuleState;
		
			
		SpdAttribs::DwordAttrib moduleID;
		SpdAttribs::DwordAttrib moduleValue;
		SpdAttribs::DwordAttrib moduleAppInstance;
		SpdAttribs::WStringAttrib moduleExpirationDate;
		SpdAttribs::WStringAttrib contractNumber;
		SpdAttribs::AttribEnumTypeMember<TModuleState> moduleState;
	
		
	
		Lic_ModuleInfoAttribs& operator =(const Lic_ModuleInfoAttribs &c)
		{
			moduleID = c.moduleID;
			moduleValue = c.moduleValue;
			moduleAppInstance = c.moduleAppInstance;
			moduleExpirationDate = c.moduleExpirationDate;
			contractNumber = c.contractNumber;
			moduleState = c.moduleState;
			return *this ;
		}
	
		Lic_ModuleInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			moduleID(m_mapAttribObjs, L"mI", 0),
			moduleValue(m_mapAttribObjs, L"mV", 0),
			moduleAppInstance(m_mapAttribObjs, L"mAI", 0),
			moduleExpirationDate(m_mapAttribObjs, L"exDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			contractNumber(m_mapAttribObjs, L"cN", SpdAttribs::WStringObj(L"")),
			moduleState(m_mapAttribObjs, L"mSt", msPerm){;}
		
		Lic_ModuleInfoAttribs(const Lic_ModuleInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			moduleID(m_mapAttribObjs, L"mI", c.moduleID),
			moduleValue(m_mapAttribObjs, L"mV", c.moduleValue),
			moduleAppInstance(m_mapAttribObjs, L"mAI", c.moduleAppInstance),
			moduleExpirationDate(m_mapAttribObjs, L"exDt", c.moduleExpirationDate),
			contractNumber(m_mapAttribObjs, L"cN", c.contractNumber),
			moduleState(m_mapAttribObjs, L"mSt", c.moduleState){;}
		
		Lic_ModuleInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			moduleID(m_mapAttribObjs, L"mI", 0),
			moduleValue(m_mapAttribObjs, L"mV", 0),
			moduleAppInstance(m_mapAttribObjs, L"mAI", 0),
			moduleExpirationDate(m_mapAttribObjs, L"exDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			contractNumber(m_mapAttribObjs, L"cN", SpdAttribs::WStringObj(L"")),
			moduleState(m_mapAttribObjs, L"mSt", msPerm){;}
		
		Lic_ModuleInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ModuleInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			moduleID(m_mapAttribObjs, L"mI", c.moduleID),
			moduleValue(m_mapAttribObjs, L"mV", c.moduleValue),
			moduleAppInstance(m_mapAttribObjs, L"mAI", c.moduleAppInstance),
			moduleExpirationDate(m_mapAttribObjs, L"exDt", c.moduleExpirationDate),
			contractNumber(m_mapAttribObjs, L"cN", c.contractNumber),
			moduleState(m_mapAttribObjs, L"mSt", c.moduleState){;}
		
		Lic_ModuleInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::DwordAttrib &param_moduleID,
			const SpdAttribs::DwordAttrib &param_moduleValue,
			const SpdAttribs::DwordAttrib &param_moduleAppInstance,
			const SpdAttribs::WStringAttrib &param_moduleExpirationDate,
			const SpdAttribs::WStringAttrib &param_contractNumber,
			const TModuleState &param_moduleState) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				moduleID(m_mapAttribObjs, L"mI", param_moduleID),
				moduleValue(m_mapAttribObjs, L"mV", param_moduleValue),
				moduleAppInstance(m_mapAttribObjs, L"mAI", param_moduleAppInstance),
				moduleExpirationDate(m_mapAttribObjs, L"exDt", param_moduleExpirationDate),
				contractNumber(m_mapAttribObjs, L"cN", param_contractNumber),
				moduleState(m_mapAttribObjs, L"mSt", param_moduleState){;}
		
		Lic_ModuleInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::DwordAttrib &param_moduleID,
			const SpdAttribs::DwordAttrib &param_moduleValue,
			const SpdAttribs::DwordAttrib &param_moduleAppInstance,
			const SpdAttribs::WStringAttrib &param_moduleExpirationDate,
			const SpdAttribs::WStringAttrib &param_contractNumber,
			const TModuleState &param_moduleState) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				moduleID(m_mapAttribObjs, L"mI", param_moduleID),
				moduleValue(m_mapAttribObjs, L"mV", param_moduleValue),
				moduleAppInstance(m_mapAttribObjs, L"mAI", param_moduleAppInstance),
				moduleExpirationDate(m_mapAttribObjs, L"exDt", param_moduleExpirationDate),
				contractNumber(m_mapAttribObjs, L"cN", param_contractNumber),
				moduleState(m_mapAttribObjs, L"mSt", param_moduleState){;}
		
	
	};
	
	/*
	License Information - Product Attributes
	*/
	class Lic_ProductInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_PIA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Information - Product Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ProductInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		typedef std::vector< Lic_ModuleInfoAttribs > TVector_Lic_ModuleInfoAttribsList;
		typedef SpdAttribs::CStreamableVectors< TVector_Lic_ModuleInfoAttribsList  > Vector_Lic_ModuleInfoAttribsList_Obj;
		typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ModuleInfoAttribsList_Obj, TVector_Lic_ModuleInfoAttribsList > Lic_ModuleInfoAttribsList;
			
		SpdAttribs::DwordAttrib productID;
		SpdAttribs::DwordAttrib productAppInstance;
		SpdAttribs::DwordAttrib product_Major;
		SpdAttribs::DwordAttrib product_Minor;
		SpdAttribs::DwordAttrib product_SubMajor;
		SpdAttribs::DwordAttrib product_SubMinor;
		Lic_ModuleInfoAttribsList moduleList;
	
		
	
		Lic_ProductInfoAttribs& operator =(const Lic_ProductInfoAttribs &c)
		{
			productID = c.productID;
			productAppInstance = c.productAppInstance;
			product_Major = c.product_Major;
			product_Minor = c.product_Minor;
			product_SubMajor = c.product_SubMajor;
			product_SubMinor = c.product_SubMinor;
			moduleList = c.moduleList;
			return *this ;
		}
	
		Lic_ProductInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productID(m_mapAttribObjs, L"pI", 0),
			productAppInstance(m_mapAttribObjs, L"pAI", 0),
			product_Major(m_mapAttribObjs, L"pMj", 0),
			product_Minor(m_mapAttribObjs, L"pMn", 0),
			product_SubMajor(m_mapAttribObjs, L"pSMj", 0),
			product_SubMinor(m_mapAttribObjs, L"pSMn", 0),
			moduleList(m_mapAttribObjs, L"mLt", Vector_Lic_ModuleInfoAttribsList_Obj()){;}
		
		Lic_ProductInfoAttribs(const Lic_ProductInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			productID(m_mapAttribObjs, L"pI", c.productID),
			productAppInstance(m_mapAttribObjs, L"pAI", c.productAppInstance),
			product_Major(m_mapAttribObjs, L"pMj", c.product_Major),
			product_Minor(m_mapAttribObjs, L"pMn", c.product_Minor),
			product_SubMajor(m_mapAttribObjs, L"pSMj", c.product_SubMajor),
			product_SubMinor(m_mapAttribObjs, L"pSMn", c.product_SubMinor),
			moduleList(m_mapAttribObjs, L"mLt", c.moduleList){;}
		
		Lic_ProductInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productID(m_mapAttribObjs, L"pI", 0),
			productAppInstance(m_mapAttribObjs, L"pAI", 0),
			product_Major(m_mapAttribObjs, L"pMj", 0),
			product_Minor(m_mapAttribObjs, L"pMn", 0),
			product_SubMajor(m_mapAttribObjs, L"pSMj", 0),
			product_SubMinor(m_mapAttribObjs, L"pSMn", 0),
			moduleList(m_mapAttribObjs, L"mLt", Vector_Lic_ModuleInfoAttribsList_Obj()){;}
		
		Lic_ProductInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ProductInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			productID(m_mapAttribObjs, L"pI", c.productID),
			productAppInstance(m_mapAttribObjs, L"pAI", c.productAppInstance),
			product_Major(m_mapAttribObjs, L"pMj", c.product_Major),
			product_Minor(m_mapAttribObjs, L"pMn", c.product_Minor),
			product_SubMajor(m_mapAttribObjs, L"pSMj", c.product_SubMajor),
			product_SubMinor(m_mapAttribObjs, L"pSMn", c.product_SubMinor),
			moduleList(m_mapAttribObjs, L"mLt", c.moduleList){;}
		
		Lic_ProductInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::DwordAttrib &param_productID,
			const SpdAttribs::DwordAttrib &param_productAppInstance,
			const SpdAttribs::DwordAttrib &param_product_Major,
			const SpdAttribs::DwordAttrib &param_product_Minor,
			const SpdAttribs::DwordAttrib &param_product_SubMajor,
			const SpdAttribs::DwordAttrib &param_product_SubMinor,
			const Lic_ModuleInfoAttribsList &param_moduleList) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				productID(m_mapAttribObjs, L"pI", param_productID),
				productAppInstance(m_mapAttribObjs, L"pAI", param_productAppInstance),
				product_Major(m_mapAttribObjs, L"pMj", param_product_Major),
				product_Minor(m_mapAttribObjs, L"pMn", param_product_Minor),
				product_SubMajor(m_mapAttribObjs, L"pSMj", param_product_SubMajor),
				product_SubMinor(m_mapAttribObjs, L"pSMn", param_product_SubMinor),
				moduleList(m_mapAttribObjs, L"mLt", param_moduleList){;}
		
		Lic_ProductInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::DwordAttrib &param_productID,
			const SpdAttribs::DwordAttrib &param_productAppInstance,
			const SpdAttribs::DwordAttrib &param_product_Major,
			const SpdAttribs::DwordAttrib &param_product_Minor,
			const SpdAttribs::DwordAttrib &param_product_SubMajor,
			const SpdAttribs::DwordAttrib &param_product_SubMinor,
			const Lic_ModuleInfoAttribsList &param_moduleList) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				productID(m_mapAttribObjs, L"pI", param_productID),
				productAppInstance(m_mapAttribObjs, L"pAI", param_productAppInstance),
				product_Major(m_mapAttribObjs, L"pMj", param_product_Major),
				product_Minor(m_mapAttribObjs, L"pMn", param_product_Minor),
				product_SubMajor(m_mapAttribObjs, L"pSMj", param_product_SubMajor),
				product_SubMinor(m_mapAttribObjs, L"pSMn", param_product_SubMinor),
				moduleList(m_mapAttribObjs, L"mLt", param_moduleList){;}
		
	
	};
	
	class Lic_LicenseInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"L_LIA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Information Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_LicenseInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		typedef enum {
			sltStandard = 0,
			sltFailover = 1,
			sltDisasterRecovery = 2,
			sltTestDev = 3,
			sltStandardSubscription = 4,
		} TSoftwareLicenseType;
		
		typedef std::vector< Lic_ProductInfoAttribs > TVector_Lic_ProductInfoAttribsList;
		typedef SpdAttribs::CStreamableVectors< TVector_Lic_ProductInfoAttribsList  > Vector_Lic_ProductInfoAttribsList_Obj;
		typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ProductInfoAttribsList_Obj, TVector_Lic_ProductInfoAttribsList > Lic_ProductInfoAttribsList;
		class Lic_ValidationTokenAttribs : public SpdAttribs::CAttribsBase
		{
		public:
			static wchar_t* GetAttribsClassName()
			{
				 return L"L_vTA";
			}
			
		
			static const wchar_t* DescriptiveName()
			{
				return L"License Validation Token Attributes";
			}
		
			virtual const wchar_t* GetDescriptiveName() const
			{
				return DescriptiveName();
			}
		
			Lic_ValidationTokenAttribs& operator =(const CWStringStream &wsData)
			{
				InitFromString(wsData);
				return *this ;
			}
			typedef enum {
				ttNone = 0,
				ttHardwareKeyID = 1,
				ttLicenseCode = 2,
				ttBiosSerialNumber = 3,
				ttMacAddress = 4,
				ttCompuerName = 5,
				ttTypeCopyFromCustomerOnly = 6,
				ttTypeArchiveOnly = 7,
				ttTypePackageOnly = 8,
			} TTokenType;
			
				
			SpdAttribs::WStringAttrib tokenValue;
			SpdAttribs::AttribEnumTypeMember<TTokenType> tokenType;
		
			
		
			Lic_ValidationTokenAttribs& operator =(const Lic_ValidationTokenAttribs &c)
			{
				tokenValue = c.tokenValue;
				tokenType = c.tokenType;
				return *this ;
			}
		
			Lic_ValidationTokenAttribs() : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				tokenValue(m_mapAttribObjs, L"tV", SpdAttribs::WStringObj(L"")),
				tokenType(m_mapAttribObjs, L"tT", ttNone){;}
			
			Lic_ValidationTokenAttribs(const Lic_ValidationTokenAttribs &c) : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				tokenValue(m_mapAttribObjs, L"tV", c.tokenValue),
				tokenType(m_mapAttribObjs, L"tT", c.tokenType){;}
			
			Lic_ValidationTokenAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				tokenValue(m_mapAttribObjs, L"tV", SpdAttribs::WStringObj(L"")),
				tokenType(m_mapAttribObjs, L"tT", ttNone){;}
			
			Lic_ValidationTokenAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ValidationTokenAttribs &c) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				tokenValue(m_mapAttribObjs, L"tV", c.tokenValue),
				tokenType(m_mapAttribObjs, L"tT", c.tokenType){;}
			
			Lic_ValidationTokenAttribs(const CWStringStream &wsAttribsClassName,
				const SpdAttribs::WStringAttrib &param_tokenValue,
				const TTokenType &param_tokenType) : 
					SpdAttribs::CAttribsBase(wsAttribsClassName),
					tokenValue(m_mapAttribObjs, L"tV", param_tokenValue),
					tokenType(m_mapAttribObjs, L"tT", param_tokenType){;}
			
			Lic_ValidationTokenAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
				const SpdAttribs::WStringAttrib &param_tokenValue,
				const TTokenType &param_tokenType) : 
					SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
					tokenValue(m_mapAttribObjs, L"tV", param_tokenValue),
					tokenType(m_mapAttribObjs, L"tT", param_tokenType){;}
			
		
		};
		
		class Lic_VerificationCodeAttribs : public SpdAttribs::CAttribsBase
		{
		public:
			static wchar_t* GetAttribsClassName()
			{
				 return L"L_vCA";
			}
			
		
			static const wchar_t* DescriptiveName()
			{
				return L"License Verification Code Attributes";
			}
		
			virtual const wchar_t* GetDescriptiveName() const
			{
				return DescriptiveName();
			}
		
			Lic_VerificationCodeAttribs& operator =(const CWStringStream &wsData)
			{
				InitFromString(wsData);
				return *this ;
			}
				
			SpdAttribs::WStringAttrib verificationDate;
			SpdAttribs::WStringAttrib verificationValue;
		
			
		
			Lic_VerificationCodeAttribs& operator =(const Lic_VerificationCodeAttribs &c)
			{
				verificationDate = c.verificationDate;
				verificationValue = c.verificationValue;
				return *this ;
			}
		
			Lic_VerificationCodeAttribs() : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				verificationDate(m_mapAttribObjs, L"vDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
				verificationValue(m_mapAttribObjs, L"vVl", SpdAttribs::WStringObj(L"")){;}
			
			Lic_VerificationCodeAttribs(const Lic_VerificationCodeAttribs &c) : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				verificationDate(m_mapAttribObjs, L"vDt", c.verificationDate),
				verificationValue(m_mapAttribObjs, L"vVl", c.verificationValue){;}
			
			Lic_VerificationCodeAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				verificationDate(m_mapAttribObjs, L"vDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
				verificationValue(m_mapAttribObjs, L"vVl", SpdAttribs::WStringObj(L"")){;}
			
			Lic_VerificationCodeAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_VerificationCodeAttribs &c) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				verificationDate(m_mapAttribObjs, L"vDt", c.verificationDate),
				verificationValue(m_mapAttribObjs, L"vVl", c.verificationValue){;}
			
			Lic_VerificationCodeAttribs(const CWStringStream &wsAttribsClassName,
				const SpdAttribs::WStringAttrib &param_verificationDate,
				const SpdAttribs::WStringAttrib &param_verificationValue) : 
					SpdAttribs::CAttribsBase(wsAttribsClassName),
					verificationDate(m_mapAttribObjs, L"vDt", param_verificationDate),
					verificationValue(m_mapAttribObjs, L"vVl", param_verificationValue){;}
			
			Lic_VerificationCodeAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
				const SpdAttribs::WStringAttrib &param_verificationDate,
				const SpdAttribs::WStringAttrib &param_verificationValue) : 
					SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
					verificationDate(m_mapAttribObjs, L"vDt", param_verificationDate),
					verificationValue(m_mapAttribObjs, L"vVl", param_verificationValue){;}
			
		
		};
		
		class Lic_VerificationAttribs : public SpdAttribs::CAttribsBase
		{
		public:
			static wchar_t* GetAttribsClassName()
			{
				 return L"L_VrfA";
			}
			
		
			static const wchar_t* DescriptiveName()
			{
				return L"License Verification Attributes";
			}
		
			virtual const wchar_t* GetDescriptiveName() const
			{
				return DescriptiveName();
			}
		
			Lic_VerificationAttribs& operator =(const CWStringStream &wsData)
			{
				InitFromString(wsData);
				return *this ;
			}
			typedef std::vector< Lic_ValidationTokenAttribs > TVector_Lic_ValidationTokenAttribsList;
			typedef SpdAttribs::CStreamableVectors< TVector_Lic_ValidationTokenAttribsList  > Vector_Lic_ValidationTokenAttribsList_Obj;
			typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ValidationTokenAttribsList_Obj, TVector_Lic_ValidationTokenAttribsList > Lic_ValidationTokenAttribsList;
			typedef std::vector< Lic_VerificationCodeAttribs > TVector_Lic_VerificationCodeAttribsList;
			typedef SpdAttribs::CStreamableVectors< TVector_Lic_VerificationCodeAttribsList  > Vector_Lic_VerificationCodeAttribsList_Obj;
			typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_VerificationCodeAttribsList_Obj, TVector_Lic_VerificationCodeAttribsList > Lic_VerificationCodeAttribsList;
				
			Lic_ValidationTokenAttribsList validationTokenList;
			Lic_VerificationCodeAttribsList verificationCodeHistoryList;
		
			
		
			Lic_VerificationAttribs& operator =(const Lic_VerificationAttribs &c)
			{
				validationTokenList = c.validationTokenList;
				verificationCodeHistoryList = c.verificationCodeHistoryList;
				return *this ;
			}
		
			Lic_VerificationAttribs() : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				validationTokenList(m_mapAttribObjs, L"tLt", Vector_Lic_ValidationTokenAttribsList_Obj()),
				verificationCodeHistoryList(m_mapAttribObjs, L"hLt", Vector_Lic_VerificationCodeAttribsList_Obj()){;}
			
			Lic_VerificationAttribs(const Lic_VerificationAttribs &c) : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				validationTokenList(m_mapAttribObjs, L"tLt", c.validationTokenList),
				verificationCodeHistoryList(m_mapAttribObjs, L"hLt", c.verificationCodeHistoryList){;}
			
			Lic_VerificationAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				validationTokenList(m_mapAttribObjs, L"tLt", Vector_Lic_ValidationTokenAttribsList_Obj()),
				verificationCodeHistoryList(m_mapAttribObjs, L"hLt", Vector_Lic_VerificationCodeAttribsList_Obj()){;}
			
			Lic_VerificationAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_VerificationAttribs &c) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				validationTokenList(m_mapAttribObjs, L"tLt", c.validationTokenList),
				verificationCodeHistoryList(m_mapAttribObjs, L"hLt", c.verificationCodeHistoryList){;}
			
			Lic_VerificationAttribs(const CWStringStream &wsAttribsClassName,
				const Lic_ValidationTokenAttribsList &param_validationTokenList,
				const Lic_VerificationCodeAttribsList &param_verificationCodeHistoryList) : 
					SpdAttribs::CAttribsBase(wsAttribsClassName),
					validationTokenList(m_mapAttribObjs, L"tLt", param_validationTokenList),
					verificationCodeHistoryList(m_mapAttribObjs, L"hLt", param_verificationCodeHistoryList){;}
			
			Lic_VerificationAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
				const Lic_ValidationTokenAttribsList &param_validationTokenList,
				const Lic_VerificationCodeAttribsList &param_verificationCodeHistoryList) : 
					SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
					validationTokenList(m_mapAttribObjs, L"tLt", param_validationTokenList),
					verificationCodeHistoryList(m_mapAttribObjs, L"hLt", param_verificationCodeHistoryList){;}
			
		
		};
		
			
		SpdAttribs::DwordAttrib customerID;
		SpdAttribs::DwordAttrib destinationID;
		SpdAttribs::DwordAttrib softwareGroupLicenseID;
		SpdAttribs::AttribEnumTypeMember<TSoftwareLicenseType> softwareLicType;
		SpdAttribs::DwordAttrib softwareLicTypeIndex;
		SpdAttribs::WStringAttrib activationExpirationDate;
		SpdAttribs::DwordAttrib activationTotal;
		SpdAttribs::DwordAttrib activationCurrent;
		SpdAttribs::DwordAttrib activationAmountInDays;
		SpdAttribs::WStringAttrib modifiedDate;
		Lic_ProductInfoAttribsList productList;
		Lic_VerificationAttribs licVerificationAttribs;
	
		
	
		Lic_LicenseInfoAttribs& operator =(const Lic_LicenseInfoAttribs &c)
		{
			customerID = c.customerID;
			destinationID = c.destinationID;
			softwareGroupLicenseID = c.softwareGroupLicenseID;
			softwareLicType = c.softwareLicType;
			softwareLicTypeIndex = c.softwareLicTypeIndex;
			activationExpirationDate = c.activationExpirationDate;
			activationTotal = c.activationTotal;
			activationCurrent = c.activationCurrent;
			activationAmountInDays = c.activationAmountInDays;
			modifiedDate = c.modifiedDate;
			productList = c.productList;
			licVerificationAttribs = c.licVerificationAttribs;
			return *this ;
		}
	
		Lic_LicenseInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			customerID(m_mapAttribObjs, L"cI", 0),
			destinationID(m_mapAttribObjs, L"dI", 0),
			softwareGroupLicenseID(m_mapAttribObjs, L"swI", 0),
			softwareLicType(m_mapAttribObjs, L"lT", sltStandard),
			softwareLicTypeIndex(m_mapAttribObjs, L"lTI", 1),
			activationExpirationDate(m_mapAttribObjs, L"aEDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			activationTotal(m_mapAttribObjs, L"aT", 0),
			activationCurrent(m_mapAttribObjs, L"aC", 0),
			activationAmountInDays(m_mapAttribObjs, L"aA", 0),
			modifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			productList(m_mapAttribObjs, L"pLt", Vector_Lic_ProductInfoAttribsList_Obj()),
			licVerificationAttribs(m_mapAttribObjs, L"L_VrfA", L"lVA", Lic_VerificationAttribs()){;}
		
		Lic_LicenseInfoAttribs(const Lic_LicenseInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			customerID(m_mapAttribObjs, L"cI", c.customerID),
			destinationID(m_mapAttribObjs, L"dI", c.destinationID),
			softwareGroupLicenseID(m_mapAttribObjs, L"swI", c.softwareGroupLicenseID),
			softwareLicType(m_mapAttribObjs, L"lT", c.softwareLicType),
			softwareLicTypeIndex(m_mapAttribObjs, L"lTI", c.softwareLicTypeIndex),
			activationExpirationDate(m_mapAttribObjs, L"aEDt", c.activationExpirationDate),
			activationTotal(m_mapAttribObjs, L"aT", c.activationTotal),
			activationCurrent(m_mapAttribObjs, L"aC", c.activationCurrent),
			activationAmountInDays(m_mapAttribObjs, L"aA", c.activationAmountInDays),
			modifiedDate(m_mapAttribObjs, L"mDt", c.modifiedDate),
			productList(m_mapAttribObjs, L"pLt", c.productList),
			licVerificationAttribs(m_mapAttribObjs, L"L_VrfA", L"lVA", c.licVerificationAttribs){;}
		
		Lic_LicenseInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			customerID(m_mapAttribObjs, L"cI", 0),
			destinationID(m_mapAttribObjs, L"dI", 0),
			softwareGroupLicenseID(m_mapAttribObjs, L"swI", 0),
			softwareLicType(m_mapAttribObjs, L"lT", sltStandard),
			softwareLicTypeIndex(m_mapAttribObjs, L"lTI", 1),
			activationExpirationDate(m_mapAttribObjs, L"aEDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			activationTotal(m_mapAttribObjs, L"aT", 0),
			activationCurrent(m_mapAttribObjs, L"aC", 0),
			activationAmountInDays(m_mapAttribObjs, L"aA", 0),
			modifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			productList(m_mapAttribObjs, L"pLt", Vector_Lic_ProductInfoAttribsList_Obj()),
			licVerificationAttribs(m_mapAttribObjs, L"L_VrfA", L"lVA", Lic_VerificationAttribs()){;}
		
		Lic_LicenseInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_LicenseInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			customerID(m_mapAttribObjs, L"cI", c.customerID),
			destinationID(m_mapAttribObjs, L"dI", c.destinationID),
			softwareGroupLicenseID(m_mapAttribObjs, L"swI", c.softwareGroupLicenseID),
			softwareLicType(m_mapAttribObjs, L"lT", c.softwareLicType),
			softwareLicTypeIndex(m_mapAttribObjs, L"lTI", c.softwareLicTypeIndex),
			activationExpirationDate(m_mapAttribObjs, L"aEDt", c.activationExpirationDate),
			activationTotal(m_mapAttribObjs, L"aT", c.activationTotal),
			activationCurrent(m_mapAttribObjs, L"aC", c.activationCurrent),
			activationAmountInDays(m_mapAttribObjs, L"aA", c.activationAmountInDays),
			modifiedDate(m_mapAttribObjs, L"mDt", c.modifiedDate),
			productList(m_mapAttribObjs, L"pLt", c.productList),
			licVerificationAttribs(m_mapAttribObjs, L"L_VrfA", L"lVA", c.licVerificationAttribs){;}
		
		Lic_LicenseInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::DwordAttrib &param_customerID,
			const SpdAttribs::DwordAttrib &param_destinationID,
			const SpdAttribs::DwordAttrib &param_softwareGroupLicenseID,
			const TSoftwareLicenseType &param_softwareLicType,
			const SpdAttribs::DwordAttrib &param_softwareLicTypeIndex,
			const SpdAttribs::WStringAttrib &param_activationExpirationDate,
			const SpdAttribs::DwordAttrib &param_activationTotal,
			const SpdAttribs::DwordAttrib &param_activationCurrent,
			const SpdAttribs::DwordAttrib &param_activationAmountInDays,
			const SpdAttribs::WStringAttrib &param_modifiedDate,
			const Lic_ProductInfoAttribsList &param_productList,
			const Lic_VerificationAttribs &param_licVerificationAttribs) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				customerID(m_mapAttribObjs, L"cI", param_customerID),
				destinationID(m_mapAttribObjs, L"dI", param_destinationID),
				softwareGroupLicenseID(m_mapAttribObjs, L"swI", param_softwareGroupLicenseID),
				softwareLicType(m_mapAttribObjs, L"lT", param_softwareLicType),
				softwareLicTypeIndex(m_mapAttribObjs, L"lTI", param_softwareLicTypeIndex),
				activationExpirationDate(m_mapAttribObjs, L"aEDt", param_activationExpirationDate),
				activationTotal(m_mapAttribObjs, L"aT", param_activationTotal),
				activationCurrent(m_mapAttribObjs, L"aC", param_activationCurrent),
				activationAmountInDays(m_mapAttribObjs, L"aA", param_activationAmountInDays),
				modifiedDate(m_mapAttribObjs, L"mDt", param_modifiedDate),
				productList(m_mapAttribObjs, L"pLt", param_productList),
				licVerificationAttribs(m_mapAttribObjs, L"L_VrfA", L"lVA", param_licVerificationAttribs){;}
		
		Lic_LicenseInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::DwordAttrib &param_customerID,
			const SpdAttribs::DwordAttrib &param_destinationID,
			const SpdAttribs::DwordAttrib &param_softwareGroupLicenseID,
			const TSoftwareLicenseType &param_softwareLicType,
			const SpdAttribs::DwordAttrib &param_softwareLicTypeIndex,
			const SpdAttribs::WStringAttrib &param_activationExpirationDate,
			const SpdAttribs::DwordAttrib &param_activationTotal,
			const SpdAttribs::DwordAttrib &param_activationCurrent,
			const SpdAttribs::DwordAttrib &param_activationAmountInDays,
			const SpdAttribs::WStringAttrib &param_modifiedDate,
			const Lic_ProductInfoAttribsList &param_productList,
			const Lic_VerificationAttribs &param_licVerificationAttribs) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				customerID(m_mapAttribObjs, L"cI", param_customerID),
				destinationID(m_mapAttribObjs, L"dI", param_destinationID),
				softwareGroupLicenseID(m_mapAttribObjs, L"swI", param_softwareGroupLicenseID),
				softwareLicType(m_mapAttribObjs, L"lT", param_softwareLicType),
				softwareLicTypeIndex(m_mapAttribObjs, L"lTI", param_softwareLicTypeIndex),
				activationExpirationDate(m_mapAttribObjs, L"aEDt", param_activationExpirationDate),
				activationTotal(m_mapAttribObjs, L"aT", param_activationTotal),
				activationCurrent(m_mapAttribObjs, L"aC", param_activationCurrent),
				activationAmountInDays(m_mapAttribObjs, L"aA", param_activationAmountInDays),
				modifiedDate(m_mapAttribObjs, L"mDt", param_modifiedDate),
				productList(m_mapAttribObjs, L"pLt", param_productList),
				licVerificationAttribs(m_mapAttribObjs, L"L_VrfA", L"lVA", param_licVerificationAttribs){;}
		
	
	};
	
		
	Lic_LicenseInfoAttribs licLicenseInfoAttribs;
	Lic_SoftwareSpecAttribs licSoftwareSpecAttribs;

	

	Lic_PackageAttribs& operator =(const Lic_PackageAttribs &c)
	{
		licLicenseInfoAttribs = c.licLicenseInfoAttribs;
		licSoftwareSpecAttribs = c.licSoftwareSpecAttribs;
		return *this ;
	}

	Lic_PackageAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		licLicenseInfoAttribs(m_mapAttribObjs, L"L_LIA", L"lIA", Lic_LicenseInfoAttribs()),
		licSoftwareSpecAttribs(m_mapAttribObjs, L"L_SwSpA", L"lSA", Lic_SoftwareSpecAttribs()){;}
	
	Lic_PackageAttribs(const Lic_PackageAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		licLicenseInfoAttribs(m_mapAttribObjs, L"L_LIA", L"lIA", c.licLicenseInfoAttribs),
		licSoftwareSpecAttribs(m_mapAttribObjs, L"L_SwSpA", L"lSA", c.licSoftwareSpecAttribs){;}
	
	Lic_PackageAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		licLicenseInfoAttribs(m_mapAttribObjs, L"L_LIA", L"lIA", Lic_LicenseInfoAttribs()),
		licSoftwareSpecAttribs(m_mapAttribObjs, L"L_SwSpA", L"lSA", Lic_SoftwareSpecAttribs()){;}
	
	Lic_PackageAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_PackageAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		licLicenseInfoAttribs(m_mapAttribObjs, L"L_LIA", L"lIA", c.licLicenseInfoAttribs),
		licSoftwareSpecAttribs(m_mapAttribObjs, L"L_SwSpA", L"lSA", c.licSoftwareSpecAttribs){;}
	
	Lic_PackageAttribs(const CWStringStream &wsAttribsClassName,
		const Lic_LicenseInfoAttribs &param_licLicenseInfoAttribs,
		const Lic_SoftwareSpecAttribs &param_licSoftwareSpecAttribs) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			licLicenseInfoAttribs(m_mapAttribObjs, L"L_LIA", L"lIA", param_licLicenseInfoAttribs),
			licSoftwareSpecAttribs(m_mapAttribObjs, L"L_SwSpA", L"lSA", param_licSoftwareSpecAttribs){;}
	
	Lic_PackageAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const Lic_LicenseInfoAttribs &param_licLicenseInfoAttribs,
		const Lic_SoftwareSpecAttribs &param_licSoftwareSpecAttribs) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			licLicenseInfoAttribs(m_mapAttribObjs, L"L_LIA", L"lIA", param_licLicenseInfoAttribs),
			licSoftwareSpecAttribs(m_mapAttribObjs, L"L_SwSpA", L"lSA", param_licSoftwareSpecAttribs){;}
	

};

#endif //__L_PkgAAttribs_h__
