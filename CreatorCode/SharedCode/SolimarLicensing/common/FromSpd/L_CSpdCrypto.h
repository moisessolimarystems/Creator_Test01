#pragma once
#ifndef _CSpdCrypto_h																					// CR.33897; added.
#define _CSpdCrypto_h																					// CR.33897; added.

#include "L_SSILib1DLL.h" // For SSILIB1_EXPORT
#include <string>

/// This class mimics the encryption that the .NET code does.  The encryption parameters are the same that CSpdEncryptedString.cs uses so that 
/// Either language can encrypt or decrypt the values without an issue.
class SSILIB1_EXPORT CSpdCrypto
{
public:
   static HRESULT EncryptString(const std::wstring &_inputData, std::wstring & _outputData);
   static HRESULT DecryptString(const std::wstring &_inputData, std::wstring & _outputData);
private:
   static HRESULT ManipulateString(const std::wstring &_inputData, std::wstring & _outputData, bool bEncrypt);
   static std::string WstringToString(const std::wstring &_wstring);
   static std::wstring StringToWstring(const std::string &_string);
};

#endif
