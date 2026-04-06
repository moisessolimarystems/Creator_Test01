#ifndef __L_TMSAttribs_h__
#define __L_TMSAttribs_h__

#include "LicAttribsBase.h"
class Lic_TestMailServer : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"L_TMS";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"Test Mail Server Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Lic_TestMailServer& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
		
	SpdAttribs::WStringAttrib subject;
	SpdAttribs::WStringAttrib body;
	SpdAttribs::VectorStringAttrib recipentsList;

	

	Lic_TestMailServer& operator =(const Lic_TestMailServer &c)
	{
		subject = c.subject;
		body = c.body;
		recipentsList = c.recipentsList;
		return *this ;
	}

	Lic_TestMailServer() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		subject(m_mapAttribObjs, L"mS", SpdAttribs::WStringObj(L"Test E-mail")),
		body(m_mapAttribObjs, L"mB", SpdAttribs::WStringObj(L"This is an e-mail sent by Solimar Systems, Inc. testing Mail Server settings.")),
		recipentsList(m_mapAttribObjs, L"rL", SpdAttribs::VectorStringObj()){;}
	
	Lic_TestMailServer(const Lic_TestMailServer &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		subject(m_mapAttribObjs, L"mS", c.subject),
		body(m_mapAttribObjs, L"mB", c.body),
		recipentsList(m_mapAttribObjs, L"rL", c.recipentsList){;}
	
	Lic_TestMailServer(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		subject(m_mapAttribObjs, L"mS", SpdAttribs::WStringObj(L"Test E-mail")),
		body(m_mapAttribObjs, L"mB", SpdAttribs::WStringObj(L"This is an e-mail sent by Solimar Systems, Inc. testing Mail Server settings.")),
		recipentsList(m_mapAttribObjs, L"rL", SpdAttribs::VectorStringObj()){;}
	
	Lic_TestMailServer(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Lic_TestMailServer &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		subject(m_mapAttribObjs, L"mS", c.subject),
		body(m_mapAttribObjs, L"mB", c.body),
		recipentsList(m_mapAttribObjs, L"rL", c.recipentsList){;}
	
	Lic_TestMailServer(const CWStringStream &wsAttribsClassName,
		const SpdAttribs::WStringAttrib &param_subject,
		const SpdAttribs::WStringAttrib &param_body,
		const SpdAttribs::VectorStringAttrib &param_recipentsList) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			subject(m_mapAttribObjs, L"mS", param_subject),
			body(m_mapAttribObjs, L"mB", param_body),
			recipentsList(m_mapAttribObjs, L"rL", param_recipentsList){;}
	
	Lic_TestMailServer(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const SpdAttribs::WStringAttrib &param_subject,
		const SpdAttribs::WStringAttrib &param_body,
		const SpdAttribs::VectorStringAttrib &param_recipentsList) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			subject(m_mapAttribObjs, L"mS", param_subject),
			body(m_mapAttribObjs, L"mB", param_body),
			recipentsList(m_mapAttribObjs, L"rL", param_recipentsList){;}
	

};

#endif //__L_TMSAttribs_h__
