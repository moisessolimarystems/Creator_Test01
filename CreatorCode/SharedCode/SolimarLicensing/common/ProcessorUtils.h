#ifndef __PROCESSORUTILS_H
#define __PROCESSORUTILS_H

#include <windows.h>
#include <math.h>

#pragma warning(disable:4786) // Suppress Identifier trunc in debug info.
#include <set>
using namespace std;

/*
 * ProcessorUtils
 *
 * Thread-safe utility class for querying detailed processor information.
 */
class ProcessorUtils
{
	enum ProcessorVendor
	{ 
		VENDOR_UNKNOWN	= 0, 
		VENDOR_INTEL	= 1, 
		VENDOR_AMD		= 2
	};

	/*
	 * SafeMutex
	 */
	class SafeMutex
	{
	public:
		SafeMutex(HANDLE hMut) : m_hMut(hMut) { if (m_hMut != INVALID_HANDLE_VALUE) WaitForSingleObject(m_hMut, INFINITE); }
		~SafeMutex() { if (m_hMut != INVALID_HANDLE_VALUE) ReleaseMutex(m_hMut); }
	private:
		HANDLE m_hMut;
		SafeMutex() {} 
	};

public:

	/*
	 * ProcessorUtils()
	 */
	ProcessorUtils();

	/*
	 * ~ProcessorUtils()
	 */
	virtual ~ProcessorUtils();

	/*
	 * GetProcessorLicenseCount()
	 *
	 * Returns the number of processor licenses the calling machine
	 * should consume. Due to BIOS and affinity settings, not all 
	 * processors/cores may be available for an application to use. 
	 * The returned processor license count is calculated using only 
	 * processors/cores that are immediately available to the calling 
	 * process at the time of the call.
	 * 
	 * Current Licensing Implementation:
	 * Uses a core factor to weight cores differently than physical 
	 * packages. If the core factor is 1 then cores will carry as much
	 * weight as a physical package.
	 *
	 * The current formula used to compute the processor license count is:
	 * 
	 * licensedProcessors = numAvailablePackages * ceil( coreFactor * (numAvailableCores / numAvailablePackages))
	 *
	 * Returns 0 on failure.
	 */
	DWORD GetProcessorLicenseCount();

	/*
	 * GetProcessorCoreCount()
	 *
	 * Returns the total number of available cores made available 
	 * to the calling process. This number may be different than
	 * the number returned by GetProcessorLicenseCount due to 
	 * the core factor (see notes for GetProcessorLicenseCount).
	 * Use the value returned by this function to determine the
	 * the number of optimal threads for your application.
	 *
	 * Returns 0 on failure.
	 */
	DWORD GetProcessorCoreCount();

private:

	int		m_initializeResult;
	HANDLE	m_hInitializeMutex;

	//
	// These values represent the maximum that the processor(s) can
	// support, they don't represent what's available to the
	// system/application. The actual resources available to
	// an application will depend on BIOS and affinity settings.
	//

	DWORD m_numCoresPerPackage;
	DWORD m_numLogicalPerCore; 
	DWORD m_numLogicalPerPackage;

	//
	// These values represent what's available to the application due
	// to BIOS or processor affinity settings.
	//

	DWORD m_numAvailablePackages;
	DWORD m_numAvailableCores;

	/*
	 * Initialize()
	 *
	 * Returns >0 if successful or 0 if an error occurred.
	 */
	int Initialize();

	/*
	 * log2() 
	 * 
	 * Calculates log base 2.
	 */
	double log2(double n) { return log(n) / log((double)2); }

	/*
	 * ParseAPICID()
	 *
	 * Parses the BIOS assigned processor APIC ID into its
	 * relevant sub-IDs.
	 */
	void ParseAPICID(
						DWORD apicID, 
						DWORD numLogicalPerCore, 
						DWORD numCoresPerPackage,
						BYTE& smtID,
						BYTE& coreID,
						BYTE& pkgID);

	/*
	 * GetApicID()
	 *
	 * Retreives a processors initial APIC ID assigned
	 * to it at startup. This function returns the ID
	 * associated with the processor the calling thread 
	 * is running on at the time of this call, so before
	 * calling, set the threads processor affinity 
	 * appropriately.
	 */
	BYTE GetApicID();
	
	/*
	 * GetMaxStandardCPUID()
	 *
	 * Returns the highest standard cpuid function supported 
	 * by the processor. 
	 */
	DWORD GetMaxStandardCPUID();

	/*
	 * GetMaxExtendedCPUID()
	 *
	 * Returns the highest extended cpuid function supported 
	 * by the processor. 
	 */
	DWORD GetMaxExtendedCPUID();

	/*
	 * GetProcessorVendor()
	 *
	 * Returns the processors vendor if known,
	 * otherwise returns VENDOR_UNKOWN.
	 */
	ProcessorVendor GetProcessorVendor();

	/*
	 * IsGenuineIntel()
	 *
	 * Test registers returned from cpuid:0 for vendor: Intel.
	 */
	BOOL IsGenuineIntel(DWORD ebx, DWORD ecx, DWORD edx);

	/*
	 * IsAuthenticAMD()
	 *
	 * Test registers returned from cpuid:0 for vendor: AMD.
	 */
	BOOL IsAuthenticAMD(DWORD ebx, DWORD ecx, DWORD edx);

	/*
	 * GetNumLogicalPerCore()
	 *
	 * Returns the maximum number of logical processors (threads) that can run
	 * in a single core.
	 */
	DWORD GetNumLogicalPerCore();

	/*
	 * GetNumLogicalPerPackage()
	 *
	 * Returns the maximum number of logical processors (threads) that can run
	 * in a single package.
	 *
	 * Note from AMD CPUID Specification:
	 * When HT_ENABLED is false, LogicalProcessorCount is reserved, and
	 * the package contains 1 core and that core is single threaded.
	 */
	DWORD GetNumLogicalPerPackage();

	/*
	 * GetNumCoresPerPackage()
	 *
	 * Returns the maximum number of available cores per package. This 
	 * does not mean that all of the cores will be available to the application
	 * due to BIOS settings, virtualization, or affinity settings.
	 */
	DWORD GetNumCoresPerPackage();

	/*
	 * GetNumAvailablePackagesAndCores()
	 *
	 * Calculates the number of physical packages and cores
	 * available to the calling application. Takes into account
	 * disablement in the BIOS as well as virtual machine and
	 * process level CPU affinity.
	 *
	 * Returns FALSE if an error occurs.
	 */
	BOOL GetNumAvailablePackagesAndCores(
											DWORD numLogicalPerCore,
											DWORD numCoresPerPackage, 
											DWORD& numAvailablePackages, 
											DWORD& numAvailableCores);
};

#endif