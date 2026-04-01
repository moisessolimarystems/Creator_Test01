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
		
	SpdAttribs::VectorBufferAttrib layout;
	SpdAttribs::WStringAttrib keyName;
	SpdAttribs::WStringAttrib verificationCode;
	SpdAttribs::WStringAttrib modifiedDate;
	SpdAttribs::DwordAttrib currentActivations;

	

	Lic_KeyAttribs& operator =(const Lic_KeyAttribs &c)
	{
		layout = c.layout;
		keyName = c.keyName;
		verificationCode = c.verificationCode;
		modifiedDate = c.modifiedDate;
		currentActivations = c.currentActivations;
		return *this ;
	}

	Lic_KeyAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		layout(m_mapAttribObjs, L"lo", SpdAttribs::VectorBufferObj()),
		keyName(m_mapAttribObjs, L"kN", SpdAttribs::WStringObj(L"")),
		verificationCode(m_mapAttribObjs, L"vC", SpdAttribs::WStringObj(L"")),
		modifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		currentActivations(m_mapAttribObjs, L"cA", 0){;}
	
	Lic_KeyAttribs(const Lic_KeyAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		layout(m_mapAttribObjs, L"lo", c.layout),
		keyName(m_mapAttribObjs, L"kN", c.keyName),
		verificationCode(m_mapAttribObjs, L"vC", c.verificationCode),
		modifiedDate(m_mapAttribObjs, L"mDt", c.modifiedDate),
		currentActivations(m_mapAttribObjs, L"cA", c.currentActivations){;}
	
	Lic_KeyAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		layout(m_mapAttribObjs, L"lo", SpdAttribs::VectorBufferObj()),
		keyName(m_mapAttribObjs, L"kN", SpdAttribs::WStringObj(L"")),
		verificationCode(m_mapAttribObjs, L"vC", SpdAttribs::WStringObj(L"")),
		modifiedDate(m_mapAttribObjs, L"mDt", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
		currentActivations(m_mapAttribObjs, L"cA", 0){;}
	
	Lic_KeyAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_KeyAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		layout(m_mapAttribObjs, L"lo", c.layout),
		keyName(m_mapAttribObjs, L"kN", c.keyName),
		verificationCode(m_mapAttribObjs, L"vC", c.verificationCode),
		modifiedDate(m_mapAttribObjs, L"mDt", c.modifiedDate),
		currentActivations(m_mapAttribObjs, L"cA", c.currentActivations){;}
	
	Lic_KeyAttribs(const CWStringStream &wsAttribsClassName,
		const SpdAttribs::VectorBufferAttrib &param_layout,
		const SpdAttribs::WStringAttrib &param_keyName,
		const SpdAttribs::WStringAttrib &param_verificationCode,
		const SpdAttribs::WStringAttrib &param_modifiedDate,
		const SpdAttribs::DwordAttrib &param_currentActivations) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			layout(m_mapAttribObjs, L"lo", param_layout),
			keyName(m_mapAttribObjs, L"kN", param_keyName),
			verificationCode(m_mapAttribObjs, L"vC", param_verificationCode),
			modifiedDate(m_mapAttribObjs, L"mDt", param_modifiedDate),
			currentActivations(m_mapAttribObjs, L"cA", param_currentActivations){;}
	
	Lic_KeyAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const SpdAttribs::VectorBufferAttrib &param_layout,
		const SpdAttribs::WStringAttrib &param_keyName,
		const SpdAttribs::WStringAttrib &param_verificationCode,
		const SpdAttribs::WStringAttrib &param_modifiedDate,
		const SpdAttribs::DwordAttrib &param_currentActivations) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			layout(m_mapAttribObjs, L"lo", param_layout),
			keyName(m_mapAttribObjs, L"kN", param_keyName),
			verificationCode(m_mapAttribObjs, L"vC", param_verificationCode),
			modifiedDate(m_mapAttribObjs, L"mDt", param_modifiedDate),
			currentActivations(m_mapAttribObjs, L"cA", param_currentActivations){;}
	

};

#endif //__L_KyAAttribs_h__
