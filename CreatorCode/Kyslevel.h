//------------------------------------------------------------------------------
// Filename:    Kyslevel.h
//------------------------------------------------------------------------------

#ifndef KYSLEVEL_H
#define KYSLEVEL_H
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------------
#define MAX_KEY_DESCRIPTION         30
#define MAX_COMPANY_NAME_LENGTH     50
#define KD_PASSWORD_MAX             28
#include <list>

class SKeyRecord;

#include "keymaster.h" // Common class

enum ModType
{
   mtNone=0,
   mtInitiator=1,
   mtGateway=2,
   mtTerminator=3,
   mtLUType=4,
   mtJobEvent=5,
   mtEmailer=6,
   mtPathOk=7,
   mtIlleagal=8,
   mtCounter=9,
};
enum KeyType
{
   ktBASE,
   ktCUSTOM,
   ktADDON,
   ktREPLACEMENT,
   ktSOLIMAR,
   ktRELICENSE,
   ktBACKUP,
   ktEMERGENCY,
   ktINVENTORY,
   ktUNASSIGNED = 0xFFFF,         //Unassigned
};


extern const char* status_text[];

//*--------------------------------------------------------------------*
// *                                                                    *
// *   SKeyRecord Class:                                                *
// *                                                                    *
// *   Contains database data for one protection key.                   *
// *                                                                    *
// *--------------------------------------------------------------------*
class SKeyRecord
{
private:
   ushort passwordNumber;   // Password Count
   ushort appInstances;     // Application Instances

public:
   SKeyRecord();
   SKeyRecord(const SKeyRecord&);
   SKeyRecord& operator=(const SKeyRecord&);
   ~SKeyRecord();

   void setCustomerNumber(ushort);
   void setKeyNumber(ushort);
   void setStatus(ushort);
   void setAppInstances(ushort);
   void setPasswordNumber(ushort);
   int getKeyTypeIntegerValue(unsigned short);
   unsigned short getKeyNumber();
   unsigned short getCustomerNumber();
   unsigned short getStatus();
   unsigned short getUses();
   unsigned short getProductId();
   unsigned short getNextExtension(); //helper function that i do not want to put at key lvl
   unsigned short getNextExtensionP();
   unsigned short getKeyTypeEnumValue(int);
   unsigned short getAppInstances(){return appInstances;};
   unsigned short getPasswordNumber(){return passwordNumber;};
   const char* getVersionText();
   const char* getProductText();
   const char* getKeyTypeText(unsigned short iEnumValue);
   const char* getStatusText(unsigned short status);
   const char* getStatusText(){return getStatusText(pkey->status);}
   bool loadKeyRow(char*);
   bool hasAssociatedKey(){return parent_skr_id;}
   unsigned short incrementPasswordNumber();
   unsigned short decrementPasswordNumber();


   ProtectionKey* pkey;
   static char *text;

   //
   //DATABASE MEMBERS
   int skr_id;              // primary key into SKeyRecord - each key has a unqiue skrID
   int parent_skr_id;
   int system_id;
   char description[MAX_KEY_DESCRIPTION+1]; //database member only
   AnsiString customerName;
   AnsiString summary;
   time_t activationDate;   //date key is programmed
   time_t deactivationDate; //date key is cleared
   time_t expirationDate;   //date key is due to expirer
   //SPasswordHistory pHistory[MAX_PASSWORDS];

   //
   // Database Tracking Numbers
   int po;                  // PK Number for DacEasy
   AnsiString eBOnumber;    // epicore BackOffice order number

   //

   //
   // Key Information
   int key_class;           // USB key or Parallel key
   int non_perm_ktf;        // Test/Dev, Addon, or WarmBackup

   //
   // Pages Per Minute
   int ppm_extensions;      // Pages Per Minute Extensions
   int xch_ipds_ppm;        // Xchange::IPDS pages per minute
   int xch_ps_ppm;          // Xchange::PS pages per minute
   int xch_ps_dbcs_ppm;     // Xchange::PS DBCS pages per minute
   int xch_pcl_ppm;         // Xchange::PCL pages per minute
   int afpds_ps_ppm;        // AFPDS::PS pages per minute

   //
   // SOLsearcher Enterprise Key Information
   int index_servers;
   int report_servers;
   int concurrent_users;
   bool slave;

   int num_days;

   //these two arrays match up such that the license count for mod_id[0] is = license_count[0];
   int module_id_array[128];
   int license_count_array[128];

};

#endif

