#include "ProcessorUtils.h"

#if _MSC_VER > 1200	//For only greater than VS6
#include <intrin.h>
#endif

const int HT_ENABLED		= 0x10000000; // CPUID:1 EDX[28]
const int LOGICAL_COUNT		= 0x00FF0000; // CPUID:1 EBX[23:16]
const int INTEL_CORE_COUNT	= 0xFC000000; // CPUID:4 EAX[31:26]
const int AMD_CORE_COUNT	= 0x000000FF; // CPUID:80000008 ECX[7:0]
const int AMD_CMPLEGACY		= 0x00000002; // CPUID:1 ECX[1]
const int APIC_ID			= 0xFF000000; // CPUID:1 EBX[31:24]

/*
 * ProcessorUtils()
 * Thread-safe utility class for querying detailed 
 * processor information.
 */
ProcessorUtils::ProcessorUtils() : 
	m_initializeResult(-1),
	m_numCoresPerPackage(0),
	m_numLogicalPerCore(0),
	m_numLogicalPerPackage(0),
	m_numAvailablePackages(0),
	m_numAvailableCores(0)
{
	m_hInitializeMutex = CreateMutex(NULL, FALSE, NULL);
}

/*
 * ~ProcessorUtils()
 */
ProcessorUtils::~ProcessorUtils()
{
	if (m_hInitializeMutex != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hInitializeMutex);
		m_hInitializeMutex = NULL;
	}
}

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
DWORD ProcessorUtils::GetProcessorLicenseCount()
{
	const double coreFactor = 1; 
	DWORD licensedProcessors = 0;

	if (Initialize())
	{
		licensedProcessors = (DWORD)(m_numAvailablePackages * ceil( coreFactor * ( double(m_numAvailableCores) / double(m_numAvailablePackages))));
	}

	return licensedProcessors;
}

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
DWORD ProcessorUtils::GetProcessorCoreCount()
{
	DWORD coreCount = 0;

	if (Initialize())
	{
		coreCount = m_numAvailableCores;
	}

	return coreCount;
}

/*
 * Initialize()
 *
 * Returns >0 if successful or 0 if an error occurred.
 */
int ProcessorUtils::Initialize()
{
	SafeMutex xlock(m_hInitializeMutex);

	if (m_initializeResult < 0)
	{ 
		m_initializeResult = 0;

		m_numLogicalPerPackage	= GetNumLogicalPerPackage();
		m_numCoresPerPackage	= GetNumCoresPerPackage();
		m_numLogicalPerCore		= m_numLogicalPerPackage / m_numCoresPerPackage;
		
		if (GetNumAvailablePackagesAndCores(
												m_numLogicalPerCore, 
												m_numCoresPerPackage, 
												m_numAvailablePackages, 
												m_numAvailableCores))
		{
			m_initializeResult = 1;
		}
	}

	return m_initializeResult;
}

/*
 * ParseAPICID()
 *
 * Parses the BIOS assigned processor APIC ID into its
 * relevant sub-IDs.
 */
void ProcessorUtils::ParseAPICID(
					DWORD apicID, 
					DWORD numLogicalPerCore, 
					DWORD numCoresPerPackage,
					BYTE& smtID,
					BYTE& coreID,
					BYTE& pkgID)
{
	BYTE smtIDWidth = BYTE( ceil( log2(numLogicalPerCore)));
	BYTE coreIDWidth = BYTE( ceil( log2(numCoresPerPackage)));
	BYTE pkgIDWidth = BYTE( ceil( log2(numLogicalPerCore * numCoresPerPackage)));
	BYTE smtIDMask = 0xFF ^ (0xFF << smtIDWidth);
	BYTE coreIDMask	= (0xFF << smtIDWidth) ^ (0xFF << (coreIDWidth + smtIDWidth));
	BYTE pkgIDMask = 0xFF << pkgIDWidth;
	smtID = apicID & smtIDMask;
	coreID = apicID & coreIDMask;
	pkgID = apicID & pkgIDMask;
}

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
BYTE ProcessorUtils::GetApicID()
{
	DWORD c_ebx;

#if _MSC_VER > 1200	//For only greater than VS6
	int CPUInfo[4] = { 0, 0, 0, 0 };
	__cpuid(CPUInfo, 1);
	c_ebx = CPUInfo[1];	//ebx
#else
	__asm
	{
		mov eax,1
		cpuid
		mov c_ebx, ebx
	}
#endif

	

	return (BYTE)((c_ebx & APIC_ID) >> 24);
}

/*
 * GetMaxStandardCPUID()
 *
 * Returns the highest standard cpuid function supported 
 * by the processor. 
 */
DWORD ProcessorUtils::GetMaxStandardCPUID()
{
	DWORD max_cpuid;

#if _MSC_VER > 1200	//For only greater than VS6
	int CPUInfo[4] = { 0, 0, 0, 0 };
	__cpuid(CPUInfo, 0);
	max_cpuid = CPUInfo[0]; //eax
#else
	__asm
	{
		mov eax,0
		cpuid
		mov max_cpuid, eax
	}
#endif
	return max_cpuid;

	
}

/*
 * GetMaxExtendedCPUID()
 *
 * Returns the highest extended cpuid function supported 
 * by the processor. 
 */
DWORD ProcessorUtils::GetMaxExtendedCPUID()
{
	DWORD max_x_cpuid;

#if _MSC_VER > 1200	//For only greater than VS6
	int CPUInfo[4] = { 0, 0, 0, 0 };
	__cpuid(CPUInfo, 0x80000000);
	max_x_cpuid = CPUInfo[0]; //eax
#else
	__asm
	{
		mov eax,0x80000000
		cpuid
		mov max_x_cpuid, eax
	}
#endif

	return max_x_cpuid;
}

/*
 * GetProcessorVendor()
 *
 * Returns the processors vendor if known,
 * otherwise returns VENDOR_UNKOWN.
 */
ProcessorUtils::ProcessorVendor ProcessorUtils::GetProcessorVendor()
{
	ProcessorVendor vendor = VENDOR_UNKNOWN;
	
	DWORD c_ebx, c_ecx, c_edx;
	
#if _MSC_VER > 1200	//For only greater than VS6
	int CPUInfo[4] = { 0, 0, 0, 0 };
	__cpuid(CPUInfo, 0);
	c_ebx = CPUInfo[1]; //ebx
	c_ecx = CPUInfo[2]; //ecx
	c_edx = CPUInfo[3]; //edx
#else
	__asm
	{
		mov eax,0
		cpuid
		mov c_ebx, ebx
		mov c_ecx, ecx
		mov c_edx, edx
	}
#endif
	
	if (IsGenuineIntel(c_ebx, c_ecx, c_edx))
	{
		vendor = VENDOR_INTEL;
	}
	else if (IsAuthenticAMD(c_ebx, c_ecx, c_edx))
	{
		vendor = VENDOR_AMD;
	}

	return vendor;
}

/*
 * IsGenuineIntel()
 *
 * Test registers returned from cpuid:0 for vendor: Intel.
 */
BOOL ProcessorUtils::IsGenuineIntel(DWORD ebx, DWORD ecx, DWORD edx)
{
	return (ebx == 'uneG' && ecx == 'letn' && edx == 'Ieni') ? TRUE : FALSE;
}

/*
 * IsAuthenticAMD()
 *
 * Test registers returned from cpuid:0 for vendor: AMD.
 */
BOOL ProcessorUtils::IsAuthenticAMD(DWORD ebx, DWORD ecx, DWORD edx)
{
	return (ebx == 'htuA' && ecx == 'DMAc' && edx == 'itne') ? TRUE : FALSE;
}

/*
 * GetNumLogicalPerCore()
 *
 * Returns the maximum number of logical processors (threads) that can run
 * in a single core.
 */
DWORD ProcessorUtils::GetNumLogicalPerCore()
{
	DWORD logicalPerPackage = 1;
	DWORD logicalPerCore = 1;
	DWORD c_ebx, c_edx;

	ProcessorVendor vendor = GetProcessorVendor();

	switch (vendor)
	{
		case VENDOR_INTEL:
			if (GetMaxStandardCPUID() >= 1)
			{
#if _MSC_VER > 1200	//For only greater than VS6
				int CPUInfo[4] = { 0, 0, 0, 0 };
				__cpuid(CPUInfo, 1);
				c_ebx = CPUInfo[1]; //ebx
				c_edx = CPUInfo[3]; //edx
#else
				__asm 
				{
					mov eax,1
					cpuid
					mov c_ebx, ebx
					mov c_edx, edx
				}
#endif
				if (c_edx & HT_ENABLED)
				{
					logicalPerPackage = ((c_ebx & LOGICAL_COUNT) >> 16);
					if (logicalPerPackage > 0)
					{
						logicalPerCore = logicalPerPackage / GetNumCoresPerPackage();
					}
				}
			}
			break;
		case VENDOR_AMD:
		default:
			// We know AMD only supports 1 logical processor per core.
			// For all others we'll assume 1 logical per core.
			// Default logicalPerCore = 1
			break;
	}
	return logicalPerCore;
}

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
DWORD ProcessorUtils::GetNumLogicalPerPackage()
{
	DWORD logicalPerPackage = 1;
	DWORD c_ebx, c_ecx, c_edx;

	if (GetMaxStandardCPUID() >= 1)
	{
#if _MSC_VER > 1200	//For only greater than VS6
		int CPUInfo[4] = { 0, 0, 0, 0 };
		__cpuid(CPUInfo, 1);
		c_ebx = CPUInfo[1]; //ebx
		c_ecx = CPUInfo[2]; //ecx
		c_edx = CPUInfo[3]; //edx
#else
		__asm 
		{
			mov eax,1
			cpuid
			mov c_ebx, ebx
			mov c_ecx, ecx
			mov c_edx, edx
		}
#endif
		
		if (c_edx & HT_ENABLED)
		{
			logicalPerPackage = ((c_ebx & LOGICAL_COUNT) >> 16);
		}
	}

	return logicalPerPackage;
}

/*
 * GetNumCoresPerPackage()
 *
 * Returns the maximum number of available cores per package. This 
 * does not mean that all of the cores will be available to the application
 * due to BIOS settings, virtualization, or affinity settings.
 */
DWORD ProcessorUtils::GetNumCoresPerPackage()
{
	DWORD coresPerPackage = 1;
	DWORD c_eax, c_ecx;

	ProcessorVendor vendor = GetProcessorVendor();

	switch (vendor)
	{
		case VENDOR_INTEL:
			if (GetMaxStandardCPUID() >= 4)
			{
#if _MSC_VER > 1200	//For only greater than VS6
				int CPUInfo[4] = { 0, 0, 0, 0 };
				__cpuidex(CPUInfo, 4, 0);
				c_eax = CPUInfo[0]; //eax
#else
				__asm 
				{
					mov eax,4
					mov ecx,0
					cpuid
					mov c_eax, eax
				}
#endif
				coresPerPackage = ((c_eax & INTEL_CORE_COUNT) >> 26) + 1;
			}
			break;
		case VENDOR_AMD:
			if (GetMaxExtendedCPUID() >= 0x80000008)
			{
#if _MSC_VER > 1200	//For only greater than VS6
				int CPUInfo[4] = { 0, 0, 0, 0 };
				__cpuid(CPUInfo, 0x80000008);
				c_ecx = CPUInfo[2]; //ecx
#else
				__asm
				{
					mov eax, 0x80000008
					cpuid
					mov c_ecx, ecx
				}			
#endif
				coresPerPackage = (c_ecx & AMD_CORE_COUNT) + 1;
			}
			break;
	}

	return coresPerPackage > 0 ? coresPerPackage : 1;
}

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
BOOL ProcessorUtils::GetNumAvailablePackagesAndCores(
										DWORD numLogicalPerCore,
										DWORD numCoresPerPackage, 
										DWORD& numAvailablePackages, 
										DWORD& numAvailableCores)
{
	DWORD_PTR processAffinity = 0;
	DWORD_PTR systemAffinity = 0;
	if (!GetProcessAffinityMask(GetCurrentProcess(), &processAffinity, &systemAffinity))
	{
		return FALSE;
	}

	BYTE smtID, coreID, pkgID;
	set<BYTE> coreIDSet;
	set<BYTE> pkgIDSet;
	BYTE apicID = 0;
	DWORD_PTR affinityMask = 1;
	while (affinityMask && affinityMask <= systemAffinity)
	{
		if (SetThreadAffinityMask(GetCurrentThread(), affinityMask))
		{
			Sleep(0); // give up time-slice to insure cpu switch.
			ParseAPICID(
				GetApicID(), 
				numLogicalPerCore, 
				numCoresPerPackage, 
				smtID,
				coreID,
				pkgID);
			pkgIDSet.insert(pkgID);
			coreIDSet.insert(pkgID | coreID);
		}
		else
		{
			return FALSE;
		}
		affinityMask <<= 1;
	}
	
	//
	// Restore affinity.
	//

	SetThreadAffinityMask(GetCurrentThread(), processAffinity);
	Sleep(0); // give up time-slice to insure cpu switch.

	numAvailablePackages = (DWORD)pkgIDSet.size();
	numAvailableCores = (DWORD)coreIDSet.size();

	return TRUE;
}
