==============================================================================================
 Creator - Protection Key database program
  ***** NOTES FROM THE UNDERGROUND *****
==============================================================================================


------------------------------------------------------
***** Version 2.16 REVISION 0 - November 22, 2002 *****
------------------------------------------------------

FUNCTIONALITY FIX:
I corrected a dialog that allowed users to update the database even though they didn't have permission to do so. This dialog was to be used when Creator was to be used with back office and part numbers were issued, etc. Since this is no longer the case, the user will automatically return from the DblClick() event in the ModuleEditor dialog. CustomerForm->Tools->ModuleEditor. The dialog is now read only.

SEARCHING FUNCTIONALITY ADDITION:
This has been a long awaited addition to Creator. From the KeyForm, the user now has the option to search by Customer Number, Key Number, Version, License, Key Type, Activation Date, Expiration Date, & Deactivation Date. In addition to this, the sorting by columns implemented in the previous version is now much more useful for searching keys.


------------------------------------------------------
***** Version 2.15 REVISION 2 - November 7, 2002 *****
------------------------------------------------------

FUNCTIONALITY FIX:
All Add-on/Backup keys of the "Permanent Key Type" from this revision on, cannot be made permanent. The only way the key can go permanent is if the user consciously makes the key a base key, and then proceeds to make the key permanent. The new "NON-Permanent Key Types" that are Add-On/Backup keys, do not have the ability to be issued Pages Per Minute passwords now. The reason is that they are always initial keys, and therefore are unlimited in this regard. This makes things much easier to manage.

FUNCTIONALITY FIX:
On the keyform page, you can now sort by columns. Previous versions didn't allow this. Click on the title, for example, "License", and all keys in that range will be sorted by License.

BUG FIX:   
If the user tries to issue a module password on an initial key, and the module is a default module for another key type. For example, if the user is trying to issue a PCL::Metacode module password for an SP/D Platinum key, this particular module is a default module for the Connectivity edition of SP/D. You would normally get a message saying it is a default module, but for the SP/D Platinum key, it is not.You would have to create a permanent password, and then issue the module password. Now the product type is checked, so you would be able to issue this password.


------------------------------------------------------
***** Version 2.15 REVISION 1 - October 28, 2002 *****
------------------------------------------------------

FUNCTIONALITY FIX:
Changed the way that the test/dev keys are programmed & extended. The keys can now be programmed for fixed values other than 365 days/1000 hours. These values are located in Pkey.cpp. The extension days (ex. 5 days) is the index into the list where we will get the expiration days and counter hours. The 10 day grace period should be removed from this key type as well.

BUG FIX:
Also corrected pages per minute bug. If a key was already permanent, and an output password was generated prior to generating a pages per minute password, the output value would be reset to 1. This bug has been fixed, and the output password will retain the value it was changed to previously. The output value will be set to 1 if the key is a trial key going permanent.

--------------------------------------------
***** Version 2.15 - September 4, 2002 *****
--------------------------------------------

FUNCTIONALITY FIX:   	
Added a new key type called a 'Development' key. The key description is listed as "Test/Dev". The new key type has an initial counter set to 1000 hours (+ 10 day grace period), and an expiration date 365 days (+10 day grace period) from the date it was created. When an extension password is created, the password is created for 1 day as per instructions from Bert. In addition, this is a new key type that can not go permanent, even though module passwords, output passwords can be generated, the key will never become permanent. This feature also applies to Backup & Add-on keys from now on. On the KeyForm page, there is a disabled checkbox that shows whether the key is a "Non Permanent Key Type" or not. This allows for users to tell the difference between old keys & new keys (Add-on & Backup Only).

-----------------------------------------------------
***** Version 2.14 REVISION 1 - August 29, 2002 *****
-----------------------------------------------------

BUG FIX:
Fixed bug for accounting. I enabled the EBO description field. Was disabled.

----------------------------------------
***** Version 2.14 - June 11, 2002 *****
----------------------------------------

FUNCTIONALITY FIX:
This version of creator allows the user to create a pages per minute password for the end user. The password can be viewed in the 5.11 release of SpdNT in the Licensing Dialog. To create a pages per minute password, in the keyForm, the user must right click on the module and select "Pages Per Minute". The password will be generated and the key will become PERMANENT!!! The only modules that can have a pages per minute password are:
		AFPDS::PS
		XCHANGE::PS
		XCHANGE::PS (DBCS)
		XCHANGE::IPDS
		XCHANGE::PCL
All others will fail to generate a password.
