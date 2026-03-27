#ifndef __COMMUNICATIONLINK_H__
#define __COMMUNICATIONLINK_H__

using namespace System;

/*
	The purpose of this class is to connect to the Solimar License Server
	and to read the key information and module information.
*/
__gc class CommunicationLink
{
public:
	//constructor
	CommunicationLink();

	//connects to the Solimar License Server
	HRESULT Connect();

	//Gets the list of keys and the number of keys
	HRESULT InitializeKeyInfoConnection();

	//Gets the list of modules and the number of modules
	HRESULT InitializeModuleLicenseConnection(Object* TheKeyNumber);

	bool IsModInitialized();

	//Releases the safe array for the module list
	HRESULT UnInitializeModuleLicenseConnection();

	//Free up the memory
	void UnInitializeKeyInfoConnection();

	//returns the number of keys
	int GetNumKeys();

	//returns the number of modules
	int GetNumModules();

	//gets the array containing the Key Info Structures
	void GetKeyInfoStructure(KeyInfoStructure& TheKeyInfoStruct,
							 unsigned int KeyIDIndex
							);

	//gets the array containing the Module Licensing Structure
	void GetModuleLicensingStructureArray(ModuleLicensingStructure& TheModStruct,
										  unsigned int ModuleIndex
										 );

	long GetTotalLicenses(Object* ModuleID, Object* KeyNumber);
	long GetLicensesInUse(Object* ModuleID, Object* KeyNumber);

	//checks to see if the password is valid
	HRESULT CheckPassword(String* InputString);
	HRESULT EnterPasswordPacket(VARIANT* TheData, BSTR* Verification_Code); 

	bool CommunicationLink::KeyIsProgrammed(BSTR* KeyID);

	//disconnects from the solimar license server
	void Disconnect();

	//destructor
	~CommunicationLink();

private:
	//The list of keys returned from the lower layer app
	VARIANT* pKeyList;

	//The number of keys in the key list
	int NumKeys;

	//The list of modules returned from the lower layer app
	VARIANT* ModuleList;

	//the number of modules in the list
	int NumModules;

	//A module in the module list
	VARIANT* Module;

	//The key number that maps to a module in the module list
	Object* KeyNumber;

	//ptr to the server object. Used to access the lower layer code.
	ISolimarLicenseSvr* pTheServer;

	bool ModInitialized;

};
 
#endif //__COMMUNICATIONLINK_H__