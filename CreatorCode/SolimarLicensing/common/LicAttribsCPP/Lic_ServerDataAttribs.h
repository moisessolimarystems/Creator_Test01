#ifndef __L_SvrDtAAttribs_h__
#define __L_SvrDtAAttribs_h__

#include "LicAttribsBase.h"
class Lic_ServerDataAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_SvrDtA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"License Server Data Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_ServerDataAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
	class Lic_ClockViolationInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"cV";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"Clock Violation Info Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ClockViolationInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
			
		SpdAttribs::WStringAttrib fileDate;
		SpdAttribs::WStringAttrib systemDate;
	
		
	
		Lic_ClockViolationInfoAttribs& operator =(const Lic_ClockViolationInfoAttribs &c)
		{
			fileDate = c.fileDate;
			systemDate = c.systemDate;
			return *this ;
		}
	
		Lic_ClockViolationInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			fileDate(m_mapAttribObjs, L"fDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			systemDate(m_mapAttribObjs, L"sDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")){;}
		
		Lic_ClockViolationInfoAttribs(const Lic_ClockViolationInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			fileDate(m_mapAttribObjs, L"fDt", c.fileDate),
			systemDate(m_mapAttribObjs, L"sDt", c.systemDate){;}
		
		Lic_ClockViolationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			fileDate(m_mapAttribObjs, L"fDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			systemDate(m_mapAttribObjs, L"sDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")){;}
		
		Lic_ClockViolationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ClockViolationInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			fileDate(m_mapAttribObjs, L"fDt", c.fileDate),
			systemDate(m_mapAttribObjs, L"sDt", c.systemDate){;}
		
		Lic_ClockViolationInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::WStringAttrib &param_fileDate,
			const SpdAttribs::WStringAttrib &param_systemDate) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				fileDate(m_mapAttribObjs, L"fDt", param_fileDate),
				systemDate(m_mapAttribObjs, L"sDt", param_systemDate){;}
		
		Lic_ClockViolationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::WStringAttrib &param_fileDate,
			const SpdAttribs::WStringAttrib &param_systemDate) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				fileDate(m_mapAttribObjs, L"fDt", param_fileDate),
				systemDate(m_mapAttribObjs, L"sDt", param_systemDate){;}
		
	
	};
	
	class Lic_ServerDataFileInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"fi";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Server Data File Info Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_ServerDataFileInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		class Lic_ActivationInfoAttribs : public SpdAttribs::CAttribsBase
		{
		public:
			static wchar_t* GetAttribsClassName()
			{
				 return L"ai";
			}
			
		
			static const wchar_t* DescriptiveName()
			{
				return L"Activation Slot Info Attributes";
			}
		
			virtual const wchar_t* GetDescriptiveName() const
			{
				return DescriptiveName();
			}
		
			Lic_ActivationInfoAttribs& operator =(const CWStringStream &wsData)
			{
				InitFromString(wsData);
				return *this ;
			}
				
			SpdAttribs::WStringAttrib productLicenseNumber;
			SpdAttribs::WordAttrib activationSlotId;
			SpdAttribs::WordAttrib activationSlotCurrentActivation;
			SpdAttribs::WordAttrib activationSlotHoursToExpire;
		
			
		
			Lic_ActivationInfoAttribs& operator =(const Lic_ActivationInfoAttribs &c)
			{
				productLicenseNumber = c.productLicenseNumber;
				activationSlotId = c.activationSlotId;
				activationSlotCurrentActivation = c.activationSlotCurrentActivation;
				activationSlotHoursToExpire = c.activationSlotHoursToExpire;
				return *this ;
			}
		
			Lic_ActivationInfoAttribs() : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				productLicenseNumber(m_mapAttribObjs, L"pLB", SpdAttribs::WStringObj(L"")),
				activationSlotId(m_mapAttribObjs, L"aI", 0),
				activationSlotCurrentActivation(m_mapAttribObjs, L"aC", 0),
				activationSlotHoursToExpire(m_mapAttribObjs, L"aE", 0){;}
			
			Lic_ActivationInfoAttribs(const Lic_ActivationInfoAttribs &c) : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				productLicenseNumber(m_mapAttribObjs, L"pLB", c.productLicenseNumber),
				activationSlotId(m_mapAttribObjs, L"aI", c.activationSlotId),
				activationSlotCurrentActivation(m_mapAttribObjs, L"aC", c.activationSlotCurrentActivation),
				activationSlotHoursToExpire(m_mapAttribObjs, L"aE", c.activationSlotHoursToExpire){;}
			
			Lic_ActivationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				productLicenseNumber(m_mapAttribObjs, L"pLB", SpdAttribs::WStringObj(L"")),
				activationSlotId(m_mapAttribObjs, L"aI", 0),
				activationSlotCurrentActivation(m_mapAttribObjs, L"aC", 0),
				activationSlotHoursToExpire(m_mapAttribObjs, L"aE", 0){;}
			
			Lic_ActivationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ActivationInfoAttribs &c) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				productLicenseNumber(m_mapAttribObjs, L"pLB", c.productLicenseNumber),
				activationSlotId(m_mapAttribObjs, L"aI", c.activationSlotId),
				activationSlotCurrentActivation(m_mapAttribObjs, L"aC", c.activationSlotCurrentActivation),
				activationSlotHoursToExpire(m_mapAttribObjs, L"aE", c.activationSlotHoursToExpire){;}
			
			Lic_ActivationInfoAttribs(const CWStringStream &wsAttribsClassName,
				const SpdAttribs::WStringAttrib &param_productLicenseNumber,
				const SpdAttribs::WordAttrib &param_activationSlotId,
				const SpdAttribs::WordAttrib &param_activationSlotCurrentActivation,
				const SpdAttribs::WordAttrib &param_activationSlotHoursToExpire) : 
					SpdAttribs::CAttribsBase(wsAttribsClassName),
					productLicenseNumber(m_mapAttribObjs, L"pLB", param_productLicenseNumber),
					activationSlotId(m_mapAttribObjs, L"aI", param_activationSlotId),
					activationSlotCurrentActivation(m_mapAttribObjs, L"aC", param_activationSlotCurrentActivation),
					activationSlotHoursToExpire(m_mapAttribObjs, L"aE", param_activationSlotHoursToExpire){;}
			
			Lic_ActivationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
				const SpdAttribs::WStringAttrib &param_productLicenseNumber,
				const SpdAttribs::WordAttrib &param_activationSlotId,
				const SpdAttribs::WordAttrib &param_activationSlotCurrentActivation,
				const SpdAttribs::WordAttrib &param_activationSlotHoursToExpire) : 
					SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
					productLicenseNumber(m_mapAttribObjs, L"pLB", param_productLicenseNumber),
					activationSlotId(m_mapAttribObjs, L"aI", param_activationSlotId),
					activationSlotCurrentActivation(m_mapAttribObjs, L"aC", param_activationSlotCurrentActivation),
					activationSlotHoursToExpire(m_mapAttribObjs, L"aE", param_activationSlotHoursToExpire){;}
			
		
		};
		
			
		SpdAttribs::WStringAttrib LicFileName;
		SpdAttribs::WStringAttrib LicFileLicenseCode;
		SpdAttribs::WStringAttrib LicName;
		SpdAttribs::WStringAttrib LicModifiedDate;
		SpdAttribs::BoolAttrib bHideFromUI;
		SpdAttribs::WStringAttrib LicCurrentDate;
		SpdAttribs::WStringAttrib Streamed_ActivationAttribs;
	
		
	
		Lic_ServerDataFileInfoAttribs& operator =(const Lic_ServerDataFileInfoAttribs &c)
		{
			LicFileName = c.LicFileName;
			LicFileLicenseCode = c.LicFileLicenseCode;
			LicName = c.LicName;
			LicModifiedDate = c.LicModifiedDate;
			bHideFromUI = c.bHideFromUI;
			LicCurrentDate = c.LicCurrentDate;
			Streamed_ActivationAttribs = c.Streamed_ActivationAttribs;
			return *this ;
		}
	
		Lic_ServerDataFileInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			LicFileName(m_mapAttribObjs, L"fN", SpdAttribs::WStringObj(L"")),
			LicFileLicenseCode(m_mapAttribObjs, L"fV", SpdAttribs::WStringObj(L"")),
			LicName(m_mapAttribObjs, L"lB", SpdAttribs::WStringObj(L"")),
			LicModifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			bHideFromUI(m_mapAttribObjs, L"bHide", false),
			LicCurrentDate(m_mapAttribObjs, L"cDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			Streamed_ActivationAttribs(m_mapAttribObjs, L"saA", SpdAttribs::WStringObj(L"")){;}
		
		Lic_ServerDataFileInfoAttribs(const Lic_ServerDataFileInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			LicFileName(m_mapAttribObjs, L"fN", c.LicFileName),
			LicFileLicenseCode(m_mapAttribObjs, L"fV", c.LicFileLicenseCode),
			LicName(m_mapAttribObjs, L"lB", c.LicName),
			LicModifiedDate(m_mapAttribObjs, L"mDt", c.LicModifiedDate),
			bHideFromUI(m_mapAttribObjs, L"bHide", c.bHideFromUI),
			LicCurrentDate(m_mapAttribObjs, L"cDt", c.LicCurrentDate),
			Streamed_ActivationAttribs(m_mapAttribObjs, L"saA", c.Streamed_ActivationAttribs){;}
		
		Lic_ServerDataFileInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			LicFileName(m_mapAttribObjs, L"fN", SpdAttribs::WStringObj(L"")),
			LicFileLicenseCode(m_mapAttribObjs, L"fV", SpdAttribs::WStringObj(L"")),
			LicName(m_mapAttribObjs, L"lB", SpdAttribs::WStringObj(L"")),
			LicModifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			bHideFromUI(m_mapAttribObjs, L"bHide", false),
			LicCurrentDate(m_mapAttribObjs, L"cDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			Streamed_ActivationAttribs(m_mapAttribObjs, L"saA", SpdAttribs::WStringObj(L"")){;}
		
		Lic_ServerDataFileInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ServerDataFileInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			LicFileName(m_mapAttribObjs, L"fN", c.LicFileName),
			LicFileLicenseCode(m_mapAttribObjs, L"fV", c.LicFileLicenseCode),
			LicName(m_mapAttribObjs, L"lB", c.LicName),
			LicModifiedDate(m_mapAttribObjs, L"mDt", c.LicModifiedDate),
			bHideFromUI(m_mapAttribObjs, L"bHide", c.bHideFromUI),
			LicCurrentDate(m_mapAttribObjs, L"cDt", c.LicCurrentDate),
			Streamed_ActivationAttribs(m_mapAttribObjs, L"saA", c.Streamed_ActivationAttribs){;}
		
		Lic_ServerDataFileInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::WStringAttrib &param_LicFileName,
			const SpdAttribs::WStringAttrib &param_LicFileLicenseCode,
			const SpdAttribs::WStringAttrib &param_LicName,
			const SpdAttribs::WStringAttrib &param_LicModifiedDate,
			const SpdAttribs::BoolAttrib &param_bHideFromUI,
			const SpdAttribs::WStringAttrib &param_LicCurrentDate,
			const SpdAttribs::WStringAttrib &param_Streamed_ActivationAttribs) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				LicFileName(m_mapAttribObjs, L"fN", param_LicFileName),
				LicFileLicenseCode(m_mapAttribObjs, L"fV", param_LicFileLicenseCode),
				LicName(m_mapAttribObjs, L"lB", param_LicName),
				LicModifiedDate(m_mapAttribObjs, L"mDt", param_LicModifiedDate),
				bHideFromUI(m_mapAttribObjs, L"bHide", param_bHideFromUI),
				LicCurrentDate(m_mapAttribObjs, L"cDt", param_LicCurrentDate),
				Streamed_ActivationAttribs(m_mapAttribObjs, L"saA", param_Streamed_ActivationAttribs){;}
		
		Lic_ServerDataFileInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::WStringAttrib &param_LicFileName,
			const SpdAttribs::WStringAttrib &param_LicFileLicenseCode,
			const SpdAttribs::WStringAttrib &param_LicName,
			const SpdAttribs::WStringAttrib &param_LicModifiedDate,
			const SpdAttribs::BoolAttrib &param_bHideFromUI,
			const SpdAttribs::WStringAttrib &param_LicCurrentDate,
			const SpdAttribs::WStringAttrib &param_Streamed_ActivationAttribs) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				LicFileName(m_mapAttribObjs, L"fN", param_LicFileName),
				LicFileLicenseCode(m_mapAttribObjs, L"fV", param_LicFileLicenseCode),
				LicName(m_mapAttribObjs, L"lB", param_LicName),
				LicModifiedDate(m_mapAttribObjs, L"mDt", param_LicModifiedDate),
				bHideFromUI(m_mapAttribObjs, L"bHide", param_bHideFromUI),
				LicCurrentDate(m_mapAttribObjs, L"cDt", param_LicCurrentDate),
				Streamed_ActivationAttribs(m_mapAttribObjs, L"saA", param_Streamed_ActivationAttribs){;}
		
	
	};
	
	typedef std::vector< Lic_ServerDataFileInfoAttribs > TVector_Lic_ServerDataFileInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_ServerDataFileInfoAttribsList  > Vector_Lic_ServerDataFileInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ServerDataFileInfoAttribsList_Obj, TVector_Lic_ServerDataFileInfoAttribsList > Lic_ServerDataFileInfoAttribsList;
	typedef std::vector< Lic_ClockViolationInfoAttribs > TVector_Lic_ClockViolationInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_ClockViolationInfoAttribsList  > Vector_Lic_ClockViolationInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ClockViolationInfoAttribsList_Obj, TVector_Lic_ClockViolationInfoAttribsList > Lic_ClockViolationInfoAttribsList;
	class Lic_AlertInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"aia";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"License Server Alert Info Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Lic_AlertInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
		class Lic_EmailAlertMailAttribs : public SpdAttribs::CAttribsBase
		{
		public:
			static wchar_t* GetAttribsClassName()
			{
				 return L"eA";
			}
			
		
			static const wchar_t* DescriptiveName()
			{
				return L"Email Alert Attributes";
			}
		
			virtual const wchar_t* GetDescriptiveName() const
			{
				return DescriptiveName();
			}
		
			Lic_EmailAlertMailAttribs& operator =(const CWStringStream &wsData)
			{
				InitFromString(wsData);
				return *this ;
			}
				
			SpdAttribs::WStringAttrib id;
			SpdAttribs::WStringAttrib name;
			SpdAttribs::BoolAttrib bActive;
			SpdAttribs::VectorStringAttrib recipentsList;
			SpdAttribs::VectorDwordAttrib eventIdList;
		
			
		
			Lic_EmailAlertMailAttribs& operator =(const Lic_EmailAlertMailAttribs &c)
			{
				id = c.id;
				name = c.name;
				bActive = c.bActive;
				recipentsList = c.recipentsList;
				eventIdList = c.eventIdList;
				return *this ;
			}
		
			Lic_EmailAlertMailAttribs() : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				id(m_mapAttribObjs, L"eID", SpdAttribs::WStringObj(L"")),
				name(m_mapAttribObjs, L"eN", SpdAttribs::WStringObj(L"")),
				bActive(m_mapAttribObjs, L"bA", true),
				recipentsList(m_mapAttribObjs, L"rL", SpdAttribs::VectorStringObj()),
				eventIdList(m_mapAttribObjs, L"eL", SpdAttribs::VectorDwordObj()){;}
			
			Lic_EmailAlertMailAttribs(const Lic_EmailAlertMailAttribs &c) : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				id(m_mapAttribObjs, L"eID", c.id),
				name(m_mapAttribObjs, L"eN", c.name),
				bActive(m_mapAttribObjs, L"bA", c.bActive),
				recipentsList(m_mapAttribObjs, L"rL", c.recipentsList),
				eventIdList(m_mapAttribObjs, L"eL", c.eventIdList){;}
			
			Lic_EmailAlertMailAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				id(m_mapAttribObjs, L"eID", SpdAttribs::WStringObj(L"")),
				name(m_mapAttribObjs, L"eN", SpdAttribs::WStringObj(L"")),
				bActive(m_mapAttribObjs, L"bA", true),
				recipentsList(m_mapAttribObjs, L"rL", SpdAttribs::VectorStringObj()),
				eventIdList(m_mapAttribObjs, L"eL", SpdAttribs::VectorDwordObj()){;}
			
			Lic_EmailAlertMailAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_EmailAlertMailAttribs &c) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				id(m_mapAttribObjs, L"eID", c.id),
				name(m_mapAttribObjs, L"eN", c.name),
				bActive(m_mapAttribObjs, L"bA", c.bActive),
				recipentsList(m_mapAttribObjs, L"rL", c.recipentsList),
				eventIdList(m_mapAttribObjs, L"eL", c.eventIdList){;}
			
			Lic_EmailAlertMailAttribs(const CWStringStream &wsAttribsClassName,
				const SpdAttribs::WStringAttrib &param_id,
				const SpdAttribs::WStringAttrib &param_name,
				const SpdAttribs::BoolAttrib &param_bActive,
				const SpdAttribs::VectorStringAttrib &param_recipentsList,
				const SpdAttribs::VectorDwordAttrib &param_eventIdList) : 
					SpdAttribs::CAttribsBase(wsAttribsClassName),
					id(m_mapAttribObjs, L"eID", param_id),
					name(m_mapAttribObjs, L"eN", param_name),
					bActive(m_mapAttribObjs, L"bA", param_bActive),
					recipentsList(m_mapAttribObjs, L"rL", param_recipentsList),
					eventIdList(m_mapAttribObjs, L"eL", param_eventIdList){;}
			
			Lic_EmailAlertMailAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
				const SpdAttribs::WStringAttrib &param_id,
				const SpdAttribs::WStringAttrib &param_name,
				const SpdAttribs::BoolAttrib &param_bActive,
				const SpdAttribs::VectorStringAttrib &param_recipentsList,
				const SpdAttribs::VectorDwordAttrib &param_eventIdList) : 
					SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
					id(m_mapAttribObjs, L"eID", param_id),
					name(m_mapAttribObjs, L"eN", param_name),
					bActive(m_mapAttribObjs, L"bA", param_bActive),
					recipentsList(m_mapAttribObjs, L"rL", param_recipentsList),
					eventIdList(m_mapAttribObjs, L"eL", param_eventIdList){;}
			
		
		};
		
		typedef std::vector< Lic_EmailAlertMailAttribs > TVector_Lic_EmailAlertMailAttribsList;
		typedef SpdAttribs::CStreamableVectors< TVector_Lic_EmailAlertMailAttribsList  > Vector_Lic_EmailAlertMailAttribsList_Obj;
		typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_EmailAlertMailAttribsList_Obj, TVector_Lic_EmailAlertMailAttribsList > Lic_EmailAlertMailAttribsList;
		class Lic_AlertMailServerAttribs : public SpdAttribs::CAttribsBase
		{
		public:
			static wchar_t* GetAttribsClassName()
			{
				 return L"aMS";
			}
			
		
			static const wchar_t* DescriptiveName()
			{
				return L"License Server Alert Mail Server Attributes";
			}
		
			virtual const wchar_t* GetDescriptiveName() const
			{
				return DescriptiveName();
			}
		
			Lic_AlertMailServerAttribs& operator =(const CWStringStream &wsData)
			{
				InitFromString(wsData);
				return *this ;
			}
			typedef enum {
				ttAnonymous = 0,
				ttBasic = 1,
			} TAuthenticationType;
			
				
			SpdAttribs::WStringAttrib mailServerName;
			SpdAttribs::WStringAttrib fromEmail;
			SpdAttribs::WStringAttrib fromDisplayName;
			SpdAttribs::DwordAttrib portNumber;
			SpdAttribs::AttribEnumTypeMember<TAuthenticationType> authenticationType;
			SpdAttribs::WStringAttrib authBasicUserName;
			SpdAttribs::WStringAttrib authBasicUserPassword;
		
			
		
			Lic_AlertMailServerAttribs& operator =(const Lic_AlertMailServerAttribs &c)
			{
				mailServerName = c.mailServerName;
				fromEmail = c.fromEmail;
				fromDisplayName = c.fromDisplayName;
				portNumber = c.portNumber;
				authenticationType = c.authenticationType;
				authBasicUserName = c.authBasicUserName;
				authBasicUserPassword = c.authBasicUserPassword;
				return *this ;
			}
		
			Lic_AlertMailServerAttribs() : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				mailServerName(m_mapAttribObjs, L"msn", SpdAttribs::WStringObj(L"")),
				fromEmail(m_mapAttribObjs, L"fe", SpdAttribs::WStringObj(L"")),
				fromDisplayName(m_mapAttribObjs, L"fD", SpdAttribs::WStringObj(L"")),
				portNumber(m_mapAttribObjs, L"pn", 25),
				authenticationType(m_mapAttribObjs, L"aT", ttAnonymous),
				authBasicUserName(m_mapAttribObjs, L"uN", SpdAttribs::WStringObj(L"")),
				authBasicUserPassword(m_mapAttribObjs, L"uP", SpdAttribs::WStringObj(L"")){;}
			
			Lic_AlertMailServerAttribs(const Lic_AlertMailServerAttribs &c) : 
				SpdAttribs::CAttribsBase(GetAttribsClassName()),
				mailServerName(m_mapAttribObjs, L"msn", c.mailServerName),
				fromEmail(m_mapAttribObjs, L"fe", c.fromEmail),
				fromDisplayName(m_mapAttribObjs, L"fD", c.fromDisplayName),
				portNumber(m_mapAttribObjs, L"pn", c.portNumber),
				authenticationType(m_mapAttribObjs, L"aT", c.authenticationType),
				authBasicUserName(m_mapAttribObjs, L"uN", c.authBasicUserName),
				authBasicUserPassword(m_mapAttribObjs, L"uP", c.authBasicUserPassword){;}
			
			Lic_AlertMailServerAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				mailServerName(m_mapAttribObjs, L"msn", SpdAttribs::WStringObj(L"")),
				fromEmail(m_mapAttribObjs, L"fe", SpdAttribs::WStringObj(L"")),
				fromDisplayName(m_mapAttribObjs, L"fD", SpdAttribs::WStringObj(L"")),
				portNumber(m_mapAttribObjs, L"pn", 25),
				authenticationType(m_mapAttribObjs, L"aT", ttAnonymous),
				authBasicUserName(m_mapAttribObjs, L"uN", SpdAttribs::WStringObj(L"")),
				authBasicUserPassword(m_mapAttribObjs, L"uP", SpdAttribs::WStringObj(L"")){;}
			
			Lic_AlertMailServerAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_AlertMailServerAttribs &c) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				mailServerName(m_mapAttribObjs, L"msn", c.mailServerName),
				fromEmail(m_mapAttribObjs, L"fe", c.fromEmail),
				fromDisplayName(m_mapAttribObjs, L"fD", c.fromDisplayName),
				portNumber(m_mapAttribObjs, L"pn", c.portNumber),
				authenticationType(m_mapAttribObjs, L"aT", c.authenticationType),
				authBasicUserName(m_mapAttribObjs, L"uN", c.authBasicUserName),
				authBasicUserPassword(m_mapAttribObjs, L"uP", c.authBasicUserPassword){;}
			
			Lic_AlertMailServerAttribs(const CWStringStream &wsAttribsClassName,
				const SpdAttribs::WStringAttrib &param_mailServerName,
				const SpdAttribs::WStringAttrib &param_fromEmail,
				const SpdAttribs::WStringAttrib &param_fromDisplayName,
				const SpdAttribs::DwordAttrib &param_portNumber,
				const TAuthenticationType &param_authenticationType,
				const SpdAttribs::WStringAttrib &param_authBasicUserName,
				const SpdAttribs::WStringAttrib &param_authBasicUserPassword) : 
					SpdAttribs::CAttribsBase(wsAttribsClassName),
					mailServerName(m_mapAttribObjs, L"msn", param_mailServerName),
					fromEmail(m_mapAttribObjs, L"fe", param_fromEmail),
					fromDisplayName(m_mapAttribObjs, L"fD", param_fromDisplayName),
					portNumber(m_mapAttribObjs, L"pn", param_portNumber),
					authenticationType(m_mapAttribObjs, L"aT", param_authenticationType),
					authBasicUserName(m_mapAttribObjs, L"uN", param_authBasicUserName),
					authBasicUserPassword(m_mapAttribObjs, L"uP", param_authBasicUserPassword){;}
			
			Lic_AlertMailServerAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
				const SpdAttribs::WStringAttrib &param_mailServerName,
				const SpdAttribs::WStringAttrib &param_fromEmail,
				const SpdAttribs::WStringAttrib &param_fromDisplayName,
				const SpdAttribs::DwordAttrib &param_portNumber,
				const TAuthenticationType &param_authenticationType,
				const SpdAttribs::WStringAttrib &param_authBasicUserName,
				const SpdAttribs::WStringAttrib &param_authBasicUserPassword) : 
					SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
					mailServerName(m_mapAttribObjs, L"msn", param_mailServerName),
					fromEmail(m_mapAttribObjs, L"fe", param_fromEmail),
					fromDisplayName(m_mapAttribObjs, L"fD", param_fromDisplayName),
					portNumber(m_mapAttribObjs, L"pn", param_portNumber),
					authenticationType(m_mapAttribObjs, L"aT", param_authenticationType),
					authBasicUserName(m_mapAttribObjs, L"uN", param_authBasicUserName),
					authBasicUserPassword(m_mapAttribObjs, L"uP", param_authBasicUserPassword){;}
			
		
		};
		
			
		Lic_EmailAlertMailAttribsList emailAlertsList;
		Lic_AlertMailServerAttribs mailServer;
	
		
	
		Lic_AlertInfoAttribs& operator =(const Lic_AlertInfoAttribs &c)
		{
			emailAlertsList = c.emailAlertsList;
			mailServer = c.mailServer;
			return *this ;
		}
	
		Lic_AlertInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			emailAlertsList(m_mapAttribObjs, L"emLt", Vector_Lic_EmailAlertMailAttribsList_Obj()),
			mailServer(m_mapAttribObjs, L"aMS", L"mS", Lic_AlertMailServerAttribs()){;}
		
		Lic_AlertInfoAttribs(const Lic_AlertInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			emailAlertsList(m_mapAttribObjs, L"emLt", c.emailAlertsList),
			mailServer(m_mapAttribObjs, L"aMS", L"mS", c.mailServer){;}
		
		Lic_AlertInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			emailAlertsList(m_mapAttribObjs, L"emLt", Vector_Lic_EmailAlertMailAttribsList_Obj()),
			mailServer(m_mapAttribObjs, L"aMS", L"mS", Lic_AlertMailServerAttribs()){;}
		
		Lic_AlertInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_AlertInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			emailAlertsList(m_mapAttribObjs, L"emLt", c.emailAlertsList),
			mailServer(m_mapAttribObjs, L"aMS", L"mS", c.mailServer){;}
		
		Lic_AlertInfoAttribs(const CWStringStream &wsAttribsClassName,
			const Lic_EmailAlertMailAttribsList &param_emailAlertsList,
			const Lic_AlertMailServerAttribs &param_mailServer) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				emailAlertsList(m_mapAttribObjs, L"emLt", param_emailAlertsList),
				mailServer(m_mapAttribObjs, L"aMS", L"mS", param_mailServer){;}
		
		Lic_AlertInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const Lic_EmailAlertMailAttribsList &param_emailAlertsList,
			const Lic_AlertMailServerAttribs &param_mailServer) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				emailAlertsList(m_mapAttribObjs, L"emLt", param_emailAlertsList),
				mailServer(m_mapAttribObjs, L"aMS", L"mS", param_mailServer){;}
		
	
	};
	
		
	Lic_ServerDataFileInfoAttribsList fileInfoList;
	Lic_ClockViolationInfoAttribsList clockViolHistoryList;
	SpdAttribs::DwordAttrib versionMajor;
	SpdAttribs::DwordAttrib versionMinor;
	SpdAttribs::DwordAttrib versionSubMajor;
	SpdAttribs::DwordAttrib versionSubMinor;
	SpdAttribs::WStringAttrib lastTouchDate;
	SpdAttribs::DwordAttrib clockViolCount;
	SpdAttribs::WStringAttrib clockViolLastDate;
	SpdAttribs::BoolAttrib bInClockViol;
	Lic_AlertInfoAttribs alertInfo;
	SpdAttribs::WStringAttrib streamed_EmulationInfoAttribs;

	

	Lic_ServerDataAttribs& operator =(const Lic_ServerDataAttribs &c)
	{
		fileInfoList = c.fileInfoList;
		clockViolHistoryList = c.clockViolHistoryList;
		versionMajor = c.versionMajor;
		versionMinor = c.versionMinor;
		versionSubMajor = c.versionSubMajor;
		versionSubMinor = c.versionSubMinor;
		lastTouchDate = c.lastTouchDate;
		clockViolCount = c.clockViolCount;
		clockViolLastDate = c.clockViolLastDate;
		bInClockViol = c.bInClockViol;
		alertInfo = c.alertInfo;
		streamed_EmulationInfoAttribs = c.streamed_EmulationInfoAttribs;
		return *this ;
	}

	Lic_ServerDataAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		fileInfoList(m_mapAttribObjs, L"fiLt", Vector_Lic_ServerDataFileInfoAttribsList_Obj()),
		clockViolHistoryList(m_mapAttribObjs, L"cVLt", Vector_Lic_ClockViolationInfoAttribsList_Obj()),
		versionMajor(m_mapAttribObjs, L"vMj", 1),
		versionMinor(m_mapAttribObjs, L"vMn", 0),
		versionSubMajor(m_mapAttribObjs, L"vSMj", 0),
		versionSubMinor(m_mapAttribObjs, L"vSMn", 0),
		lastTouchDate(m_mapAttribObjs, L"lDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		clockViolCount(m_mapAttribObjs, L"cVC", 0),
		clockViolLastDate(m_mapAttribObjs, L"cVD", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		bInClockViol(m_mapAttribObjs, L"iCV", false),
		alertInfo(m_mapAttribObjs, L"aia", L"aI", Lic_AlertInfoAttribs()),
		streamed_EmulationInfoAttribs(m_mapAttribObjs, L"sEA", SpdAttribs::WStringObj(L"")){;}
	
	Lic_ServerDataAttribs(const Lic_ServerDataAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		fileInfoList(m_mapAttribObjs, L"fiLt", c.fileInfoList),
		clockViolHistoryList(m_mapAttribObjs, L"cVLt", c.clockViolHistoryList),
		versionMajor(m_mapAttribObjs, L"vMj", c.versionMajor),
		versionMinor(m_mapAttribObjs, L"vMn", c.versionMinor),
		versionSubMajor(m_mapAttribObjs, L"vSMj", c.versionSubMajor),
		versionSubMinor(m_mapAttribObjs, L"vSMn", c.versionSubMinor),
		lastTouchDate(m_mapAttribObjs, L"lDt", c.lastTouchDate),
		clockViolCount(m_mapAttribObjs, L"cVC", c.clockViolCount),
		clockViolLastDate(m_mapAttribObjs, L"cVD", c.clockViolLastDate),
		bInClockViol(m_mapAttribObjs, L"iCV", c.bInClockViol),
		alertInfo(m_mapAttribObjs, L"aia", L"aI", c.alertInfo),
		streamed_EmulationInfoAttribs(m_mapAttribObjs, L"sEA", c.streamed_EmulationInfoAttribs){;}
	
	Lic_ServerDataAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		fileInfoList(m_mapAttribObjs, L"fiLt", Vector_Lic_ServerDataFileInfoAttribsList_Obj()),
		clockViolHistoryList(m_mapAttribObjs, L"cVLt", Vector_Lic_ClockViolationInfoAttribsList_Obj()),
		versionMajor(m_mapAttribObjs, L"vMj", 1),
		versionMinor(m_mapAttribObjs, L"vMn", 0),
		versionSubMajor(m_mapAttribObjs, L"vSMj", 0),
		versionSubMinor(m_mapAttribObjs, L"vSMn", 0),
		lastTouchDate(m_mapAttribObjs, L"lDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		clockViolCount(m_mapAttribObjs, L"cVC", 0),
		clockViolLastDate(m_mapAttribObjs, L"cVD", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		bInClockViol(m_mapAttribObjs, L"iCV", false),
		alertInfo(m_mapAttribObjs, L"aia", L"aI", Lic_AlertInfoAttribs()),
		streamed_EmulationInfoAttribs(m_mapAttribObjs, L"sEA", SpdAttribs::WStringObj(L"")){;}
	
	Lic_ServerDataAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ServerDataAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		fileInfoList(m_mapAttribObjs, L"fiLt", c.fileInfoList),
		clockViolHistoryList(m_mapAttribObjs, L"cVLt", c.clockViolHistoryList),
		versionMajor(m_mapAttribObjs, L"vMj", c.versionMajor),
		versionMinor(m_mapAttribObjs, L"vMn", c.versionMinor),
		versionSubMajor(m_mapAttribObjs, L"vSMj", c.versionSubMajor),
		versionSubMinor(m_mapAttribObjs, L"vSMn", c.versionSubMinor),
		lastTouchDate(m_mapAttribObjs, L"lDt", c.lastTouchDate),
		clockViolCount(m_mapAttribObjs, L"cVC", c.clockViolCount),
		clockViolLastDate(m_mapAttribObjs, L"cVD", c.clockViolLastDate),
		bInClockViol(m_mapAttribObjs, L"iCV", c.bInClockViol),
		alertInfo(m_mapAttribObjs, L"aia", L"aI", c.alertInfo),
		streamed_EmulationInfoAttribs(m_mapAttribObjs, L"sEA", c.streamed_EmulationInfoAttribs){;}
	
	Lic_ServerDataAttribs(const CWStringStream &wsAttribsClassName,
		const Lic_ServerDataFileInfoAttribsList &param_fileInfoList,
		const Lic_ClockViolationInfoAttribsList &param_clockViolHistoryList,
		const SpdAttribs::DwordAttrib &param_versionMajor,
		const SpdAttribs::DwordAttrib &param_versionMinor,
		const SpdAttribs::DwordAttrib &param_versionSubMajor,
		const SpdAttribs::DwordAttrib &param_versionSubMinor,
		const SpdAttribs::WStringAttrib &param_lastTouchDate,
		const SpdAttribs::DwordAttrib &param_clockViolCount,
		const SpdAttribs::WStringAttrib &param_clockViolLastDate,
		const SpdAttribs::BoolAttrib &param_bInClockViol,
		const Lic_AlertInfoAttribs &param_alertInfo,
		const SpdAttribs::WStringAttrib &param_streamed_EmulationInfoAttribs) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			fileInfoList(m_mapAttribObjs, L"fiLt", param_fileInfoList),
			clockViolHistoryList(m_mapAttribObjs, L"cVLt", param_clockViolHistoryList),
			versionMajor(m_mapAttribObjs, L"vMj", param_versionMajor),
			versionMinor(m_mapAttribObjs, L"vMn", param_versionMinor),
			versionSubMajor(m_mapAttribObjs, L"vSMj", param_versionSubMajor),
			versionSubMinor(m_mapAttribObjs, L"vSMn", param_versionSubMinor),
			lastTouchDate(m_mapAttribObjs, L"lDt", param_lastTouchDate),
			clockViolCount(m_mapAttribObjs, L"cVC", param_clockViolCount),
			clockViolLastDate(m_mapAttribObjs, L"cVD", param_clockViolLastDate),
			bInClockViol(m_mapAttribObjs, L"iCV", param_bInClockViol),
			alertInfo(m_mapAttribObjs, L"aia", L"aI", param_alertInfo),
			streamed_EmulationInfoAttribs(m_mapAttribObjs, L"sEA", param_streamed_EmulationInfoAttribs){;}
	
	Lic_ServerDataAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const Lic_ServerDataFileInfoAttribsList &param_fileInfoList,
		const Lic_ClockViolationInfoAttribsList &param_clockViolHistoryList,
		const SpdAttribs::DwordAttrib &param_versionMajor,
		const SpdAttribs::DwordAttrib &param_versionMinor,
		const SpdAttribs::DwordAttrib &param_versionSubMajor,
		const SpdAttribs::DwordAttrib &param_versionSubMinor,
		const SpdAttribs::WStringAttrib &param_lastTouchDate,
		const SpdAttribs::DwordAttrib &param_clockViolCount,
		const SpdAttribs::WStringAttrib &param_clockViolLastDate,
		const SpdAttribs::BoolAttrib &param_bInClockViol,
		const Lic_AlertInfoAttribs &param_alertInfo,
		const SpdAttribs::WStringAttrib &param_streamed_EmulationInfoAttribs) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			fileInfoList(m_mapAttribObjs, L"fiLt", param_fileInfoList),
			clockViolHistoryList(m_mapAttribObjs, L"cVLt", param_clockViolHistoryList),
			versionMajor(m_mapAttribObjs, L"vMj", param_versionMajor),
			versionMinor(m_mapAttribObjs, L"vMn", param_versionMinor),
			versionSubMajor(m_mapAttribObjs, L"vSMj", param_versionSubMajor),
			versionSubMinor(m_mapAttribObjs, L"vSMn", param_versionSubMinor),
			lastTouchDate(m_mapAttribObjs, L"lDt", param_lastTouchDate),
			clockViolCount(m_mapAttribObjs, L"cVC", param_clockViolCount),
			clockViolLastDate(m_mapAttribObjs, L"cVD", param_clockViolLastDate),
			bInClockViol(m_mapAttribObjs, L"iCV", param_bInClockViol),
			alertInfo(m_mapAttribObjs, L"aia", L"aI", param_alertInfo),
			streamed_EmulationInfoAttribs(m_mapAttribObjs, L"sEA", param_streamed_EmulationInfoAttribs){;}
	

};

#endif //__L_SvrDtAAttribs_h__
