#ifndef __L_KyAAttribs_h__
#define __L_KyAAttribs_h__

#include "LicAttribsBase.h"
class Lic_KeyAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_KyA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"License Key Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_KeyAttribs& operator =(const CWStringStream &wsData)
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
			
		SpdAttribs::WordAttrib activationSlotId;
		SpdAttribs::WordAttrib activationSlotCurrentActivation;
		SpdAttribs::WordAttrib activationSlotHoursToExpire;
	
		
	
		Lic_ActivationInfoAttribs& operator =(const Lic_ActivationInfoAttribs &c)
		{
			activationSlotId = c.activationSlotId;
			activationSlotCurrentActivation = c.activationSlotCurrentActivation;
			activationSlotHoursToExpire = c.activationSlotHoursToExpire;
			return *this ;
		}
	
		Lic_ActivationInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			activationSlotId(m_mapAttribObjs, L"aS", 0),
			activationSlotCurrentActivation(m_mapAttribObjs, L"aC", 0),
			activationSlotHoursToExpire(m_mapAttribObjs, L"aE", 0){;}
		
		Lic_ActivationInfoAttribs(const Lic_ActivationInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			activationSlotId(m_mapAttribObjs, L"aS", c.activationSlotId),
			activationSlotCurrentActivation(m_mapAttribObjs, L"aC", c.activationSlotCurrentActivation),
			activationSlotHoursToExpire(m_mapAttribObjs, L"aE", c.activationSlotHoursToExpire){;}
		
		Lic_ActivationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			activationSlotId(m_mapAttribObjs, L"aS", 0),
			activationSlotCurrentActivation(m_mapAttribObjs, L"aC", 0),
			activationSlotHoursToExpire(m_mapAttribObjs, L"aE", 0){;}
		
		Lic_ActivationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_ActivationInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			activationSlotId(m_mapAttribObjs, L"aS", c.activationSlotId),
			activationSlotCurrentActivation(m_mapAttribObjs, L"aC", c.activationSlotCurrentActivation),
			activationSlotHoursToExpire(m_mapAttribObjs, L"aE", c.activationSlotHoursToExpire){;}
		
		Lic_ActivationInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::WordAttrib &param_activationSlotId,
			const SpdAttribs::WordAttrib &param_activationSlotCurrentActivation,
			const SpdAttribs::WordAttrib &param_activationSlotHoursToExpire) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				activationSlotId(m_mapAttribObjs, L"aS", param_activationSlotId),
				activationSlotCurrentActivation(m_mapAttribObjs, L"aC", param_activationSlotCurrentActivation),
				activationSlotHoursToExpire(m_mapAttribObjs, L"aE", param_activationSlotHoursToExpire){;}
		
		Lic_ActivationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::WordAttrib &param_activationSlotId,
			const SpdAttribs::WordAttrib &param_activationSlotCurrentActivation,
			const SpdAttribs::WordAttrib &param_activationSlotHoursToExpire) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				activationSlotId(m_mapAttribObjs, L"aS", param_activationSlotId),
				activationSlotCurrentActivation(m_mapAttribObjs, L"aC", param_activationSlotCurrentActivation),
				activationSlotHoursToExpire(m_mapAttribObjs, L"aE", param_activationSlotHoursToExpire){;}
		
	
	};
	
	typedef std::vector< Lic_ActivationInfoAttribs > TVector_Lic_ActivationInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Lic_ActivationInfoAttribsList  > Vector_Lic_ActivationInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Lic_ActivationInfoAttribsList_Obj, TVector_Lic_ActivationInfoAttribsList > Lic_ActivationInfoAttribsList;
		
	SpdAttribs::VectorBufferAttrib layout;
	SpdAttribs::WStringAttrib keyName;
	SpdAttribs::DwordAttrib keyVersion;
	SpdAttribs::WStringAttrib licenseCode;
	SpdAttribs::WStringAttrib packetCreationDate;
	SpdAttribs::WStringAttrib currentDate;
	SpdAttribs::WordAttrib historyNumber;
	Lic_ActivationInfoAttribsList activationInfoList;

	

	Lic_KeyAttribs& operator =(const Lic_KeyAttribs &c)
	{
		layout = c.layout;
		keyName = c.keyName;
		keyVersion = c.keyVersion;
		licenseCode = c.licenseCode;
		packetCreationDate = c.packetCreationDate;
		currentDate = c.currentDate;
		historyNumber = c.historyNumber;
		activationInfoList = c.activationInfoList;
		return *this ;
	}

	Lic_KeyAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		layout(m_mapAttribObjs, L"lo", SpdAttribs::VectorBufferObj()),
		keyName(m_mapAttribObjs, L"kN", SpdAttribs::WStringObj(L"")),
		keyVersion(m_mapAttribObjs, L"kV", 1),
		licenseCode(m_mapAttribObjs, L"lC", SpdAttribs::WStringObj(L"")),
		packetCreationDate(m_mapAttribObjs, L"pDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		currentDate(m_mapAttribObjs, L"cDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		historyNumber(m_mapAttribObjs, L"hN", 0),
		activationInfoList(m_mapAttribObjs, L"aiLt", Vector_Lic_ActivationInfoAttribsList_Obj()){;}
	
	Lic_KeyAttribs(const Lic_KeyAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		layout(m_mapAttribObjs, L"lo", c.layout),
		keyName(m_mapAttribObjs, L"kN", c.keyName),
		keyVersion(m_mapAttribObjs, L"kV", c.keyVersion),
		licenseCode(m_mapAttribObjs, L"lC", c.licenseCode),
		packetCreationDate(m_mapAttribObjs, L"pDt", c.packetCreationDate),
		currentDate(m_mapAttribObjs, L"cDt", c.currentDate),
		historyNumber(m_mapAttribObjs, L"hN", c.historyNumber),
		activationInfoList(m_mapAttribObjs, L"aiLt", c.activationInfoList){;}
	
	Lic_KeyAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		layout(m_mapAttribObjs, L"lo", SpdAttribs::VectorBufferObj()),
		keyName(m_mapAttribObjs, L"kN", SpdAttribs::WStringObj(L"")),
		keyVersion(m_mapAttribObjs, L"kV", 1),
		licenseCode(m_mapAttribObjs, L"lC", SpdAttribs::WStringObj(L"")),
		packetCreationDate(m_mapAttribObjs, L"pDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		currentDate(m_mapAttribObjs, L"cDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		historyNumber(m_mapAttribObjs, L"hN", 0),
		activationInfoList(m_mapAttribObjs, L"aiLt", Vector_Lic_ActivationInfoAttribsList_Obj()){;}
	
	Lic_KeyAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_KeyAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		layout(m_mapAttribObjs, L"lo", c.layout),
		keyName(m_mapAttribObjs, L"kN", c.keyName),
		keyVersion(m_mapAttribObjs, L"kV", c.keyVersion),
		licenseCode(m_mapAttribObjs, L"lC", c.licenseCode),
		packetCreationDate(m_mapAttribObjs, L"pDt", c.packetCreationDate),
		currentDate(m_mapAttribObjs, L"cDt", c.currentDate),
		historyNumber(m_mapAttribObjs, L"hN", c.historyNumber),
		activationInfoList(m_mapAttribObjs, L"aiLt", c.activationInfoList){;}
	
	Lic_KeyAttribs(const CWStringStream &wsAttribsClassName,
		const SpdAttribs::VectorBufferAttrib &param_layout,
		const SpdAttribs::WStringAttrib &param_keyName,
		const SpdAttribs::DwordAttrib &param_keyVersion,
		const SpdAttribs::WStringAttrib &param_licenseCode,
		const SpdAttribs::WStringAttrib &param_packetCreationDate,
		const SpdAttribs::WStringAttrib &param_currentDate,
		const SpdAttribs::WordAttrib &param_historyNumber,
		const Lic_ActivationInfoAttribsList &param_activationInfoList) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			layout(m_mapAttribObjs, L"lo", param_layout),
			keyName(m_mapAttribObjs, L"kN", param_keyName),
			keyVersion(m_mapAttribObjs, L"kV", param_keyVersion),
			licenseCode(m_mapAttribObjs, L"lC", param_licenseCode),
			packetCreationDate(m_mapAttribObjs, L"pDt", param_packetCreationDate),
			currentDate(m_mapAttribObjs, L"cDt", param_currentDate),
			historyNumber(m_mapAttribObjs, L"hN", param_historyNumber),
			activationInfoList(m_mapAttribObjs, L"aiLt", param_activationInfoList){;}
	
	Lic_KeyAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const SpdAttribs::VectorBufferAttrib &param_layout,
		const SpdAttribs::WStringAttrib &param_keyName,
		const SpdAttribs::DwordAttrib &param_keyVersion,
		const SpdAttribs::WStringAttrib &param_licenseCode,
		const SpdAttribs::WStringAttrib &param_packetCreationDate,
		const SpdAttribs::WStringAttrib &param_currentDate,
		const SpdAttribs::WordAttrib &param_historyNumber,
		const Lic_ActivationInfoAttribsList &param_activationInfoList) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			layout(m_mapAttribObjs, L"lo", param_layout),
			keyName(m_mapAttribObjs, L"kN", param_keyName),
			keyVersion(m_mapAttribObjs, L"kV", param_keyVersion),
			licenseCode(m_mapAttribObjs, L"lC", param_licenseCode),
			packetCreationDate(m_mapAttribObjs, L"pDt", param_packetCreationDate),
			currentDate(m_mapAttribObjs, L"cDt", param_currentDate),
			historyNumber(m_mapAttribObjs, L"hN", param_historyNumber),
			activationInfoList(m_mapAttribObjs, L"aiLt", param_activationInfoList){;}
	

};

#endif //__L_KyAAttribs_h__
