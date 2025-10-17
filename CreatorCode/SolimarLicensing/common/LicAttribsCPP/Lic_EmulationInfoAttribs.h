#ifndef __L_EmInAAttribs_h__
#define __L_EmInAAttribs_h__

#include "LicAttribsBase.h"
class Lic_EmulationInfoAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_EmInA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"License Emulation Info Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_EmulationInfoAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
		
	SpdAttribs::BoolAttrib bDongleEmulatorDetected;
	SpdAttribs::BoolAttrib bBypassDongleEmulatorCheck;
	SpdAttribs::VectorStringAttrib knownEmulatorServicesList;
	SpdAttribs::WStringAttrib excludeService;
	SpdAttribs::BoolAttrib bCheckEmulatorCall_FoundEmulator;
	SpdAttribs::DwordAttrib checkEmulatorCall_StatusCode;

	

	Lic_EmulationInfoAttribs& operator =(const Lic_EmulationInfoAttribs &c)
	{
		bDongleEmulatorDetected = c.bDongleEmulatorDetected;
		bBypassDongleEmulatorCheck = c.bBypassDongleEmulatorCheck;
		knownEmulatorServicesList = c.knownEmulatorServicesList;
		excludeService = c.excludeService;
		bCheckEmulatorCall_FoundEmulator = c.bCheckEmulatorCall_FoundEmulator;
		checkEmulatorCall_StatusCode = c.checkEmulatorCall_StatusCode;
		return *this ;
	}

	Lic_EmulationInfoAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		bDongleEmulatorDetected(m_mapAttribObjs, L"dED", false),
		bBypassDongleEmulatorCheck(m_mapAttribObjs, L"bdEC", false),
		knownEmulatorServicesList(m_mapAttribObjs, L"kEL", SpdAttribs::VectorStringObj()),
		excludeService(m_mapAttribObjs, L"exS", SpdAttribs::WStringObj(L"")),
		bCheckEmulatorCall_FoundEmulator(m_mapAttribObjs, L"ceFE", false),
		checkEmulatorCall_StatusCode(m_mapAttribObjs, L"ceSC", 0){;}
	
	Lic_EmulationInfoAttribs(const Lic_EmulationInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		bDongleEmulatorDetected(m_mapAttribObjs, L"dED", c.bDongleEmulatorDetected),
		bBypassDongleEmulatorCheck(m_mapAttribObjs, L"bdEC", c.bBypassDongleEmulatorCheck),
		knownEmulatorServicesList(m_mapAttribObjs, L"kEL", c.knownEmulatorServicesList),
		excludeService(m_mapAttribObjs, L"exS", c.excludeService),
		bCheckEmulatorCall_FoundEmulator(m_mapAttribObjs, L"ceFE", c.bCheckEmulatorCall_FoundEmulator),
		checkEmulatorCall_StatusCode(m_mapAttribObjs, L"ceSC", c.checkEmulatorCall_StatusCode){;}
	
	Lic_EmulationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		bDongleEmulatorDetected(m_mapAttribObjs, L"dED", false),
		bBypassDongleEmulatorCheck(m_mapAttribObjs, L"bdEC", false),
		knownEmulatorServicesList(m_mapAttribObjs, L"kEL", SpdAttribs::VectorStringObj()),
		excludeService(m_mapAttribObjs, L"exS", SpdAttribs::WStringObj(L"")),
		bCheckEmulatorCall_FoundEmulator(m_mapAttribObjs, L"ceFE", false),
		checkEmulatorCall_StatusCode(m_mapAttribObjs, L"ceSC", 0){;}
	
	Lic_EmulationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_EmulationInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		bDongleEmulatorDetected(m_mapAttribObjs, L"dED", c.bDongleEmulatorDetected),
		bBypassDongleEmulatorCheck(m_mapAttribObjs, L"bdEC", c.bBypassDongleEmulatorCheck),
		knownEmulatorServicesList(m_mapAttribObjs, L"kEL", c.knownEmulatorServicesList),
		excludeService(m_mapAttribObjs, L"exS", c.excludeService),
		bCheckEmulatorCall_FoundEmulator(m_mapAttribObjs, L"ceFE", c.bCheckEmulatorCall_FoundEmulator),
		checkEmulatorCall_StatusCode(m_mapAttribObjs, L"ceSC", c.checkEmulatorCall_StatusCode){;}
	
	Lic_EmulationInfoAttribs(const CWStringStream &wsAttribsClassName,
		const SpdAttribs::BoolAttrib &param_bDongleEmulatorDetected,
		const SpdAttribs::BoolAttrib &param_bBypassDongleEmulatorCheck,
		const SpdAttribs::VectorStringAttrib &param_knownEmulatorServicesList,
		const SpdAttribs::WStringAttrib &param_excludeService,
		const SpdAttribs::BoolAttrib &param_bCheckEmulatorCall_FoundEmulator,
		const SpdAttribs::DwordAttrib &param_checkEmulatorCall_StatusCode) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			bDongleEmulatorDetected(m_mapAttribObjs, L"dED", param_bDongleEmulatorDetected),
			bBypassDongleEmulatorCheck(m_mapAttribObjs, L"bdEC", param_bBypassDongleEmulatorCheck),
			knownEmulatorServicesList(m_mapAttribObjs, L"kEL", param_knownEmulatorServicesList),
			excludeService(m_mapAttribObjs, L"exS", param_excludeService),
			bCheckEmulatorCall_FoundEmulator(m_mapAttribObjs, L"ceFE", param_bCheckEmulatorCall_FoundEmulator),
			checkEmulatorCall_StatusCode(m_mapAttribObjs, L"ceSC", param_checkEmulatorCall_StatusCode){;}
	
	Lic_EmulationInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const SpdAttribs::BoolAttrib &param_bDongleEmulatorDetected,
		const SpdAttribs::BoolAttrib &param_bBypassDongleEmulatorCheck,
		const SpdAttribs::VectorStringAttrib &param_knownEmulatorServicesList,
		const SpdAttribs::WStringAttrib &param_excludeService,
		const SpdAttribs::BoolAttrib &param_bCheckEmulatorCall_FoundEmulator,
		const SpdAttribs::DwordAttrib &param_checkEmulatorCall_StatusCode) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			bDongleEmulatorDetected(m_mapAttribObjs, L"dED", param_bDongleEmulatorDetected),
			bBypassDongleEmulatorCheck(m_mapAttribObjs, L"bdEC", param_bBypassDongleEmulatorCheck),
			knownEmulatorServicesList(m_mapAttribObjs, L"kEL", param_knownEmulatorServicesList),
			excludeService(m_mapAttribObjs, L"exS", param_excludeService),
			bCheckEmulatorCall_FoundEmulator(m_mapAttribObjs, L"ceFE", param_bCheckEmulatorCall_FoundEmulator),
			checkEmulatorCall_StatusCode(m_mapAttribObjs, L"ceSC", param_checkEmulatorCall_StatusCode){;}
	

};

#endif //__L_EmInAAttribs_h__
