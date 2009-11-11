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
			
		SpdAttribs::WStringAttrib LicFileName;
		SpdAttribs::WStringAttrib LicFileVerificationCode;
		SpdAttribs::WStringAttrib LicName;
		SpdAttribs::WStringAttrib LicModifiedDate;
		SpdAttribs::DwordAttrib LicCurrentActivations;
	
		
	
		Lic_ServerDataFileInfoAttribs& operator =(const Lic_ServerDataFileInfoAttribs &c)
		{
			LicFileName = c.LicFileName;
			LicFileVerificationCode = c.LicFileVerificationCode;
			LicName = c.LicName;
			LicModifiedDate = c.LicModifiedDate;
			LicCurrentActivations = c.LicCurrentActivations;
			return *this ;
		}
	
		Lic_ServerDataFileInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			LicFileName(m_mapAttribObjs, L"fN", SpdAttribs::WStringObj(L"")),
			LicFileVerificationCode(m_mapAttribObjs, L"fV", SpdAttribs::WStringObj(L"")),
			LicName(m_mapAttribObjs, L"lB", SpdAttribs::WStringObj(L"")),
			LicModifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			LicCurrentActivations(m_mapAttribObjs, L"cA", 0){;}
		
		Lic_ServerDataFileInfoAttribs(const Lic_ServerDataFileInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			LicFileName(m_mapAttribObjs, L"fN", c.LicFileName),
			LicFileVerificationCode(m_mapAttribObjs, L"fV", c.LicFileVerificationCode),
			LicName(m_mapAttribObjs, L"lB", c.LicName),
			LicModifiedDate(m_mapAttribObjs, L"mDt", c.LicModifiedDate),
			LicCurrentActivations(m_mapAttribObjs, L"cA", c.LicCurrentActivations){;}
		
		Lic_ServerDataFileInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			LicFileName(m_mapAttribObjs, L"fN", SpdAttribs::WStringObj(L"")),
			LicFileVerificationCode(m_mapAttribObjs, L"fV", SpdAttribs::WStringObj(L"")),
			LicName(m_mapAttribObjs, L"lB", SpdAttribs::WStringObj(L"")),
			LicModifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			LicCurrentActivations(m_mapAttribObjs, L"cA", 0){;}
		
		Lic_ServerDataFileInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ServerDataFileInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			LicFileName(m_mapAttribObjs, L"fN", c.LicFileName),
			LicFileVerificationCode(m_mapAttribObjs, L"fV", c.LicFileVerificationCode),
			LicName(m_mapAttribObjs, L"lB", c.LicName),
			LicModifiedDate(m_mapAttribObjs, L"mDt", c.LicModifiedDate),
			LicCurrentActivations(m_mapAttribObjs, L"cA", c.LicCurrentActivations){;}
		
		Lic_ServerDataFileInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::WStringAttrib &param_LicFileName,
			const SpdAttribs::WStringAttrib &param_LicFileVerificationCode,
			const SpdAttribs::WStringAttrib &param_LicName,
			const SpdAttribs::WStringAttrib &param_LicModifiedDate,
			const SpdAttribs::DwordAttrib &param_LicCurrentActivations) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				LicFileName(m_mapAttribObjs, L"fN", param_LicFileName),
				LicFileVerificationCode(m_mapAttribObjs, L"fV", param_LicFileVerificationCode),
				LicName(m_mapAttribObjs, L"lB", param_LicName),
				LicModifiedDate(m_mapAttribObjs, L"mDt", param_LicModifiedDate),
				LicCurrentActivations(m_mapAttribObjs, L"cA", param_LicCurrentActivations){;}
		
		Lic_ServerDataFileInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::WStringAttrib &param_LicFileName,
			const SpdAttribs::WStringAttrib &param_LicFileVerificationCode,
			const SpdAttribs::WStringAttrib &param_LicName,
			const SpdAttribs::WStringAttrib &param_LicModifiedDate,
			const SpdAttribs::DwordAttrib &param_LicCurrentActivations) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				LicFileName(m_mapAttribObjs, L"fN", param_LicFileName),
				LicFileVerificationCode(m_mapAttribObjs, L"fV", param_LicFileVerificationCode),
				LicName(m_mapAttribObjs, L"lB", param_LicName),
				LicModifiedDate(m_mapAttribObjs, L"mDt", param_LicModifiedDate),
				LicCurrentActivations(m_mapAttribObjs, L"cA", param_LicCurrentActivations){;}
		
	
	};
	
	typedef std::vector< Lic_ServerDataFileInfoAttribs > TVector_Lic_ServerDataFileInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_ServerDataFileInfoAttribsList  > Vector_Lic_ServerDataFileInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ServerDataFileInfoAttribsList_Obj, TVector_Lic_ServerDataFileInfoAttribsList > Lic_ServerDataFileInfoAttribsList;
	typedef std::vector< Lic_ClockViolationInfoAttribs > TVector_Lic_ClockViolationInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_ClockViolationInfoAttribsList  > Vector_Lic_ClockViolationInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ClockViolationInfoAttribsList_Obj, TVector_Lic_ClockViolationInfoAttribsList > Lic_ClockViolationInfoAttribsList;
		
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
		bInClockViol(m_mapAttribObjs, L"iCV", false){;}
	
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
		bInClockViol(m_mapAttribObjs, L"iCV", c.bInClockViol){;}
	
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
		bInClockViol(m_mapAttribObjs, L"iCV", false){;}
	
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
		bInClockViol(m_mapAttribObjs, L"iCV", c.bInClockViol){;}
	
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
		const SpdAttribs::BoolAttrib &param_bInClockViol) : 
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
			bInClockViol(m_mapAttribObjs, L"iCV", param_bInClockViol){;}
	
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
		const SpdAttribs::BoolAttrib &param_bInClockViol) : 
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
			bInClockViol(m_mapAttribObjs, L"iCV", param_bInClockViol){;}
	

};

#endif //__L_SvrDtAAttribs_h__
