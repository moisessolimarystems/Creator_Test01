//
// Lic_KeyAttribs_Helper.h
// Wrapper class for the Lic_KeyAttribs class.
//
#pragma once
#include "Lic_KeyAttribs.h"

/* class Lic_KeyAttribs_Helper
 * 
 * This wrapper class the the Lic_KeyAttribs was key to resolving the CR.34040
 * issue. The problem here is that the LicenseCodes generated are GUIDs.
 * However, in licensing 3.3, and before, the license codes are wrapped in 
 * curly brackets. In Licesing 3.4+ they are not.
 * This prevented license packet validation from succeeding.
 * 
 * This helper class provides are wrapper method for SetlicenseCode() which 
 * ensures that the new LicenseCode will not include the curly brackets... 
 * Hence, normalizing the LicenseCodes stored by this attribs class.
 */
class Lic_KeyAttribs_Helper : public Lic_KeyAttribs
{
public:
	Lic_KeyAttribs_Helper() :
		Lic_KeyAttribs()
	{
	}
	Lic_KeyAttribs_Helper(const Lic_KeyAttribs& cpy) :
		Lic_KeyAttribs(cpy)
	{
		NormalizeLicenseCode();
	}

	/* SetlicenseCode() - over-rides attribs default method and normalizes GUID
	 * before assigning to attribs.
	 *
	 * SLB 18.jun.2025 CR.34040; Created.
	 * SLB 20.jun.2025 CR.34041; Fix for removing first char when it doesn't start 
	 * with the '{' char.
	 * SLB 25.jun.2025 CR.34180; moved normalization code into new method
	 * NormalizeFileLicenseCode(). Converted this method to use that.
	 --------------------------------------------------------------------------------*/
	virtual void SetlicenseCode(
		const CSpdCString& val)
	{
		// If no curly bracket for this GUID, then just thunk attribs method.
		if (IsNormalizedCode(val)) { Lic_KeyAttribs::SetlicenseCode(val); return; }	// CR.34180; modified. CR.34040.V01; modified.

		Lic_KeyAttribs::SetlicenseCode(GetNormalizedCode(val));								// CR.34180; modified.
	}

	/* DoesLicenseCodeMatch() - Convenience helper method for consuming code.
	 * Note: at some point, may need to normalize he GUIDs here.
	 *
	 * SLB 18.jun.2025 CR.34040; Created.
	 * SLB 25.jun.2025 CR.34180; Normalize the passed in LicenseCode before comparing
	 * to the stored version.
	 --------------------------------------------------------------------------------*/
	bool DoesLicenseCodeMatch(
		const CSpdCString& wsLicenseCode) const
	{
		CSpdCString wsNormalized(GetNormalizedCode(wsLicenseCode));							// CR.34180; added.

		// Do case insensitive compare and return true if they match.
		return wsNormalized.compare(GetlicenseCode(), false) == 0;
	}

private:
	/* NormalizeLicenseCode() - reads LicenseCode and rewrites it to ensure
	 * it gets normalized (curly brackets removed)
	 *
	 * SLB 18.jun.2025 CR.34040; Created.
	 --------------------------------------------------------------------------------*/
	void NormalizeLicenseCode()
	{
		SetlicenseCode(GetlicenseCode());
	}

	/* IsNormalizedCode() - returns true if wsCode starts with open curly bracket.
	 *
	 * SLB 18.jun.2025 CR.34180; Created.
	 --------------------------------------------------------------------------------*/
	static bool IsNormalizedCode(
		const CSpdCString& wsCode)
	{
		return wsCode[0] != L'{';
	}

	/* GetNormalizedCode() - returns version of wsCode without curly brackets.
	 *
	 * SLB 18.jun.2025 CR.34180; Created.
	 --------------------------------------------------------------------------------*/
	static CSpdCString GetNormalizedCode(
		const CSpdCString& wsCode)
	{
		if (IsNormalizedCode(wsCode)) return wsCode;

		CEnterExitMsg::TraceMsg(L"%s() normalized code <%s> CR.34180.", __WFUNCTION__, wsCode.c_str());

		// remove curly brackets at begining and end, before calling attribs method.
		CSpdCString wsNormalized(wsCode);
		wsNormalized.erase(0, 1);
		size_t stFind = wsNormalized.find_first_of(L'}', 10);
		wsNormalized.erase(stFind, 1);

		return wsNormalized;
	}

};
