#ifndef __S_ElInAAttribs_h__
#define __S_ElInAAttribs_h__

#include "LicAttribsBase.h"
class Sys_EventLogInfoAttribs : public SpdAttribs::CAttribsBase
{
public:
	static wchar_t* GetAttribsClassName()
	{
		 return L"S_ElInA";
	}
	

	static const wchar_t* DescriptiveName()
	{
		return L"System Event Log Info Attributes";
	}

	virtual const wchar_t* GetDescriptiveName() const
	{
		return DescriptiveName();
	}

	Sys_EventLogInfoAttribs& operator =(const CWStringStream &wsData)
	{
		InitFromString(wsData);
		return *this ;
	}
	class Sys_EventLogEntriesInfoAttribs : public SpdAttribs::CAttribsBase
	{
	public:
		static wchar_t* GetAttribsClassName()
		{
			 return L"S_EEA";
		}
		
	
		static const wchar_t* DescriptiveName()
		{
			return L"System - Event Log Entries Info Attributes";
		}
	
		virtual const wchar_t* GetDescriptiveName() const
		{
			return DescriptiveName();
		}
	
		Sys_EventLogEntriesInfoAttribs& operator =(const CWStringStream &wsData)
		{
			InitFromString(wsData);
			return *this ;
		}
			
		SpdAttribs::WStringAttrib category;
		SpdAttribs::DwordAttrib categoryNumber;
		SpdAttribs::BufferAttrib data;
		SpdAttribs::DwordAttrib entryType;
		SpdAttribs::DwordAttrib eventID;
		SpdAttribs::DwordAttrib index;
		SpdAttribs::DwordAttrib instanceId;
		SpdAttribs::WStringAttrib machineName;
		SpdAttribs::WStringAttrib message;
		SpdAttribs::VectorStringAttrib replacementStrings;
		SpdAttribs::WStringAttrib source;
		SpdAttribs::WStringAttrib timeGenerated;
		SpdAttribs::WStringAttrib timeWritten;
		SpdAttribs::WStringAttrib userName;
	
		
	
		Sys_EventLogEntriesInfoAttribs& operator =(const Sys_EventLogEntriesInfoAttribs &c)
		{
			category = c.category;
			categoryNumber = c.categoryNumber;
			data = c.data;
			entryType = c.entryType;
			eventID = c.eventID;
			index = c.index;
			instanceId = c.instanceId;
			machineName = c.machineName;
			message = c.message;
			replacementStrings = c.replacementStrings;
			source = c.source;
			timeGenerated = c.timeGenerated;
			timeWritten = c.timeWritten;
			userName = c.userName;
			return *this ;
		}
	
		Sys_EventLogEntriesInfoAttribs() : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			category(m_mapAttribObjs, L"c", SpdAttribs::WStringObj(L"")),
			categoryNumber(m_mapAttribObjs, L"cN", 0),
			data(m_mapAttribObjs, L"d", SpdAttribs::BufferObj(L"<CBuffer><Size>0</Size><Data></Data></CBuffer>")),
			entryType(m_mapAttribObjs, L"eT", 0),
			eventID(m_mapAttribObjs, L"eI", 0),
			index(m_mapAttribObjs, L"i", 0),
			instanceId(m_mapAttribObjs, L"iI", 0),
			machineName(m_mapAttribObjs, L"mN", SpdAttribs::WStringObj(L"")),
			message(m_mapAttribObjs, L"mG", SpdAttribs::WStringObj(L"")),
			replacementStrings(m_mapAttribObjs, L"rLt", SpdAttribs::VectorStringObj()),
			source(m_mapAttribObjs, L"sR", SpdAttribs::WStringObj(L"")),
			timeGenerated(m_mapAttribObjs, L"tG", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			timeWritten(m_mapAttribObjs, L"tW", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			userName(m_mapAttribObjs, L"uN", SpdAttribs::WStringObj(L"")){;}
		
		Sys_EventLogEntriesInfoAttribs(const Sys_EventLogEntriesInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(GetAttribsClassName()),
			category(m_mapAttribObjs, L"c", c.category),
			categoryNumber(m_mapAttribObjs, L"cN", c.categoryNumber),
			data(m_mapAttribObjs, L"d", c.data),
			entryType(m_mapAttribObjs, L"eT", c.entryType),
			eventID(m_mapAttribObjs, L"eI", c.eventID),
			index(m_mapAttribObjs, L"i", c.index),
			instanceId(m_mapAttribObjs, L"iI", c.instanceId),
			machineName(m_mapAttribObjs, L"mN", c.machineName),
			message(m_mapAttribObjs, L"mG", c.message),
			replacementStrings(m_mapAttribObjs, L"rLt", c.replacementStrings),
			source(m_mapAttribObjs, L"sR", c.source),
			timeGenerated(m_mapAttribObjs, L"tG", c.timeGenerated),
			timeWritten(m_mapAttribObjs, L"tW", c.timeWritten),
			userName(m_mapAttribObjs, L"uN", c.userName){;}
		
		Sys_EventLogEntriesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			category(m_mapAttribObjs, L"c", SpdAttribs::WStringObj(L"")),
			categoryNumber(m_mapAttribObjs, L"cN", 0),
			data(m_mapAttribObjs, L"d", SpdAttribs::BufferObj(L"<CBuffer><Size>0</Size><Data></Data></CBuffer>")),
			entryType(m_mapAttribObjs, L"eT", 0),
			eventID(m_mapAttribObjs, L"eI", 0),
			index(m_mapAttribObjs, L"i", 0),
			instanceId(m_mapAttribObjs, L"iI", 0),
			machineName(m_mapAttribObjs, L"mN", SpdAttribs::WStringObj(L"")),
			message(m_mapAttribObjs, L"mG", SpdAttribs::WStringObj(L"")),
			replacementStrings(m_mapAttribObjs, L"rLt", SpdAttribs::VectorStringObj()),
			source(m_mapAttribObjs, L"sR", SpdAttribs::WStringObj(L"")),
			timeGenerated(m_mapAttribObjs, L"tG", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			timeWritten(m_mapAttribObjs, L"tW", SpdAttribs::WStringObj(L"1900-01-01 00:00:00.0000")),
			userName(m_mapAttribObjs, L"uN", SpdAttribs::WStringObj(L"")){;}
		
		Sys_EventLogEntriesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Sys_EventLogEntriesInfoAttribs &c) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			category(m_mapAttribObjs, L"c", c.category),
			categoryNumber(m_mapAttribObjs, L"cN", c.categoryNumber),
			data(m_mapAttribObjs, L"d", c.data),
			entryType(m_mapAttribObjs, L"eT", c.entryType),
			eventID(m_mapAttribObjs, L"eI", c.eventID),
			index(m_mapAttribObjs, L"i", c.index),
			instanceId(m_mapAttribObjs, L"iI", c.instanceId),
			machineName(m_mapAttribObjs, L"mN", c.machineName),
			message(m_mapAttribObjs, L"mG", c.message),
			replacementStrings(m_mapAttribObjs, L"rLt", c.replacementStrings),
			source(m_mapAttribObjs, L"sR", c.source),
			timeGenerated(m_mapAttribObjs, L"tG", c.timeGenerated),
			timeWritten(m_mapAttribObjs, L"tW", c.timeWritten),
			userName(m_mapAttribObjs, L"uN", c.userName){;}
		
		Sys_EventLogEntriesInfoAttribs(const CWStringStream &wsAttribsClassName,
			const SpdAttribs::WStringAttrib &param_category,
			const SpdAttribs::DwordAttrib &param_categoryNumber,
			const SpdAttribs::BufferAttrib &param_data,
			const SpdAttribs::DwordAttrib &param_entryType,
			const SpdAttribs::DwordAttrib &param_eventID,
			const SpdAttribs::DwordAttrib &param_index,
			const SpdAttribs::DwordAttrib &param_instanceId,
			const SpdAttribs::WStringAttrib &param_machineName,
			const SpdAttribs::WStringAttrib &param_message,
			const SpdAttribs::VectorStringAttrib &param_replacementStrings,
			const SpdAttribs::WStringAttrib &param_source,
			const SpdAttribs::WStringAttrib &param_timeGenerated,
			const SpdAttribs::WStringAttrib &param_timeWritten,
			const SpdAttribs::WStringAttrib &param_userName) : 
				SpdAttribs::CAttribsBase(wsAttribsClassName),
				category(m_mapAttribObjs, L"c", param_category),
				categoryNumber(m_mapAttribObjs, L"cN", param_categoryNumber),
				data(m_mapAttribObjs, L"d", param_data),
				entryType(m_mapAttribObjs, L"eT", param_entryType),
				eventID(m_mapAttribObjs, L"eI", param_eventID),
				index(m_mapAttribObjs, L"i", param_index),
				instanceId(m_mapAttribObjs, L"iI", param_instanceId),
				machineName(m_mapAttribObjs, L"mN", param_machineName),
				message(m_mapAttribObjs, L"mG", param_message),
				replacementStrings(m_mapAttribObjs, L"rLt", param_replacementStrings),
				source(m_mapAttribObjs, L"sR", param_source),
				timeGenerated(m_mapAttribObjs, L"tG", param_timeGenerated),
				timeWritten(m_mapAttribObjs, L"tW", param_timeWritten),
				userName(m_mapAttribObjs, L"uN", param_userName){;}
		
		Sys_EventLogEntriesInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
			const SpdAttribs::WStringAttrib &param_category,
			const SpdAttribs::DwordAttrib &param_categoryNumber,
			const SpdAttribs::BufferAttrib &param_data,
			const SpdAttribs::DwordAttrib &param_entryType,
			const SpdAttribs::DwordAttrib &param_eventID,
			const SpdAttribs::DwordAttrib &param_index,
			const SpdAttribs::DwordAttrib &param_instanceId,
			const SpdAttribs::WStringAttrib &param_machineName,
			const SpdAttribs::WStringAttrib &param_message,
			const SpdAttribs::VectorStringAttrib &param_replacementStrings,
			const SpdAttribs::WStringAttrib &param_source,
			const SpdAttribs::WStringAttrib &param_timeGenerated,
			const SpdAttribs::WStringAttrib &param_timeWritten,
			const SpdAttribs::WStringAttrib &param_userName) : 
				SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
				category(m_mapAttribObjs, L"c", param_category),
				categoryNumber(m_mapAttribObjs, L"cN", param_categoryNumber),
				data(m_mapAttribObjs, L"d", param_data),
				entryType(m_mapAttribObjs, L"eT", param_entryType),
				eventID(m_mapAttribObjs, L"eI", param_eventID),
				index(m_mapAttribObjs, L"i", param_index),
				instanceId(m_mapAttribObjs, L"iI", param_instanceId),
				machineName(m_mapAttribObjs, L"mN", param_machineName),
				message(m_mapAttribObjs, L"mG", param_message),
				replacementStrings(m_mapAttribObjs, L"rLt", param_replacementStrings),
				source(m_mapAttribObjs, L"sR", param_source),
				timeGenerated(m_mapAttribObjs, L"tG", param_timeGenerated),
				timeWritten(m_mapAttribObjs, L"tW", param_timeWritten),
				userName(m_mapAttribObjs, L"uN", param_userName){;}
		
	
	};
	
	typedef std::vector< Sys_EventLogEntriesInfoAttribs > TVector_Sys_EventLogEntriesInfoAttribsList;
	typedef SpdAttribs::CStreamableVectors< TVector_Sys_EventLogEntriesInfoAttribsList  > Vector_Sys_EventLogEntriesInfoAttribsList_Obj;
	typedef SpdAttribs::CAttribMemberBaseU< Vector_Sys_EventLogEntriesInfoAttribsList_Obj, TVector_Sys_EventLogEntriesInfoAttribsList > Sys_EventLogEntriesInfoAttribsList;
		
	Sys_EventLogEntriesInfoAttribsList entryList;
	SpdAttribs::WStringAttrib log;
	SpdAttribs::WStringAttrib logName;
	SpdAttribs::WStringAttrib machineName;
	SpdAttribs::DwordAttrib maximumKilobytes;
	SpdAttribs::DwordAttrib minimumRetentionDays;
	SpdAttribs::DwordAttrib enumOverflowAction;
	SpdAttribs::WStringAttrib source;

	

	Sys_EventLogInfoAttribs& operator =(const Sys_EventLogInfoAttribs &c)
	{
		entryList = c.entryList;
		log = c.log;
		logName = c.logName;
		machineName = c.machineName;
		maximumKilobytes = c.maximumKilobytes;
		minimumRetentionDays = c.minimumRetentionDays;
		enumOverflowAction = c.enumOverflowAction;
		source = c.source;
		return *this ;
	}

	Sys_EventLogInfoAttribs() : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		entryList(m_mapAttribObjs, L"eLt", Vector_Sys_EventLogEntriesInfoAttribsList_Obj()),
		log(m_mapAttribObjs, L"l", SpdAttribs::WStringObj(L"")),
		logName(m_mapAttribObjs, L"lN", SpdAttribs::WStringObj(L"")),
		machineName(m_mapAttribObjs, L"mN", SpdAttribs::WStringObj(L"")),
		maximumKilobytes(m_mapAttribObjs, L"mK", 0),
		minimumRetentionDays(m_mapAttribObjs, L"mR", 0),
		enumOverflowAction(m_mapAttribObjs, L"oA", 0),
		source(m_mapAttribObjs, L"s", SpdAttribs::WStringObj(L"")){;}
	
	Sys_EventLogInfoAttribs(const Sys_EventLogInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(GetAttribsClassName()),
		entryList(m_mapAttribObjs, L"eLt", c.entryList),
		log(m_mapAttribObjs, L"l", c.log),
		logName(m_mapAttribObjs, L"lN", c.logName),
		machineName(m_mapAttribObjs, L"mN", c.machineName),
		maximumKilobytes(m_mapAttribObjs, L"mK", c.maximumKilobytes),
		minimumRetentionDays(m_mapAttribObjs, L"mR", c.minimumRetentionDays),
		enumOverflowAction(m_mapAttribObjs, L"oA", c.enumOverflowAction),
		source(m_mapAttribObjs, L"s", c.source){;}
	
	Sys_EventLogInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		entryList(m_mapAttribObjs, L"eLt", Vector_Sys_EventLogEntriesInfoAttribsList_Obj()),
		log(m_mapAttribObjs, L"l", SpdAttribs::WStringObj(L"")),
		logName(m_mapAttribObjs, L"lN", SpdAttribs::WStringObj(L"")),
		machineName(m_mapAttribObjs, L"mN", SpdAttribs::WStringObj(L"")),
		maximumKilobytes(m_mapAttribObjs, L"mK", 0),
		minimumRetentionDays(m_mapAttribObjs, L"mR", 0),
		enumOverflowAction(m_mapAttribObjs, L"oA", 0),
		source(m_mapAttribObjs, L"s", SpdAttribs::WStringObj(L"")){;}
	
	Sys_EventLogInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName, const Sys_EventLogInfoAttribs &c) : 
		SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
		entryList(m_mapAttribObjs, L"eLt", c.entryList),
		log(m_mapAttribObjs, L"l", c.log),
		logName(m_mapAttribObjs, L"lN", c.logName),
		machineName(m_mapAttribObjs, L"mN", c.machineName),
		maximumKilobytes(m_mapAttribObjs, L"mK", c.maximumKilobytes),
		minimumRetentionDays(m_mapAttribObjs, L"mR", c.minimumRetentionDays),
		enumOverflowAction(m_mapAttribObjs, L"oA", c.enumOverflowAction),
		source(m_mapAttribObjs, L"s", c.source){;}
	
	Sys_EventLogInfoAttribs(const CWStringStream &wsAttribsClassName,
		const Sys_EventLogEntriesInfoAttribsList &param_entryList,
		const SpdAttribs::WStringAttrib &param_log,
		const SpdAttribs::WStringAttrib &param_logName,
		const SpdAttribs::WStringAttrib &param_machineName,
		const SpdAttribs::DwordAttrib &param_maximumKilobytes,
		const SpdAttribs::DwordAttrib &param_minimumRetentionDays,
		const SpdAttribs::DwordAttrib &param_enumOverflowAction,
		const SpdAttribs::WStringAttrib &param_source) : 
			SpdAttribs::CAttribsBase(wsAttribsClassName),
			entryList(m_mapAttribObjs, L"eLt", param_entryList),
			log(m_mapAttribObjs, L"l", param_log),
			logName(m_mapAttribObjs, L"lN", param_logName),
			machineName(m_mapAttribObjs, L"mN", param_machineName),
			maximumKilobytes(m_mapAttribObjs, L"mK", param_maximumKilobytes),
			minimumRetentionDays(m_mapAttribObjs, L"mR", param_minimumRetentionDays),
			enumOverflowAction(m_mapAttribObjs, L"oA", param_enumOverflowAction),
			source(m_mapAttribObjs, L"s", param_source){;}
	
	Sys_EventLogInfoAttribs(SpdAttribs::SAttribMemberMap &mapAttribObjs, const CWStringStream &wsAttribsClassName, const CWStringStream &wsAttribsKeyName,
		const Sys_EventLogEntriesInfoAttribsList &param_entryList,
		const SpdAttribs::WStringAttrib &param_log,
		const SpdAttribs::WStringAttrib &param_logName,
		const SpdAttribs::WStringAttrib &param_machineName,
		const SpdAttribs::DwordAttrib &param_maximumKilobytes,
		const SpdAttribs::DwordAttrib &param_minimumRetentionDays,
		const SpdAttribs::DwordAttrib &param_enumOverflowAction,
		const SpdAttribs::WStringAttrib &param_source) : 
			SpdAttribs::CAttribsBase(mapAttribObjs,wsAttribsClassName,wsAttribsKeyName),
			entryList(m_mapAttribObjs, L"eLt", param_entryList),
			log(m_mapAttribObjs, L"l", param_log),
			logName(m_mapAttribObjs, L"lN", param_logName),
			machineName(m_mapAttribObjs, L"mN", param_machineName),
			maximumKilobytes(m_mapAttribObjs, L"mK", param_maximumKilobytes),
			minimumRetentionDays(m_mapAttribObjs, L"mR", param_minimumRetentionDays),
			enumOverflowAction(m_mapAttribObjs, L"oA", param_enumOverflowAction),
			source(m_mapAttribObjs, L"s", param_source){;}
	

};

#endif //__S_ElInAAttribs_h__
