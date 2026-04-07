#include "L_CSpdCrypto.h"

#include <wincrypt.h>                                             // For Windows Crypto
#include "..\base64.h"   // For base64_encode & base64_decode


// Public - Static
HRESULT CSpdCrypto::EncryptString(const std::wstring &_inputData, std::wstring & _outputData)
{
   return ManipulateString(_inputData, _outputData, true);
}

// Public - Static
HRESULT CSpdCrypto::DecryptString(const std::wstring &_inputData, std::wstring & _outputData)
{
   return ManipulateString(_inputData, _outputData, false);
}

// Private - Static
std::string CSpdCrypto::WstringToString(const std::wstring &_wstring)
{
   int neededSize = WideCharToMultiByte(CP_UTF8, 0, _wstring.c_str(), -1, NULL, 0, NULL, NULL);
   char* tmpCharArray = new char[neededSize+1];
   WideCharToMultiByte(CP_UTF8, 0, _wstring.c_str(), -1, &tmpCharArray[0], neededSize, NULL, NULL);
   tmpCharArray[neededSize] = 0;
   std::string ret = tmpCharArray;
   delete [] tmpCharArray;
   return ret;
}

// Private - Static
std::wstring CSpdCrypto::StringToWstring(const std::string &_string)
{
   int neededSize = MultiByteToWideChar(CP_UTF8, 0, _string.c_str(), -1, NULL, 0);
   wchar_t* tmpCharArray = new wchar_t[neededSize+1];
   MultiByteToWideChar(CP_UTF8, 0, _string.c_str(), -1, &tmpCharArray[0], neededSize);
   tmpCharArray[neededSize] = 0;
   std::wstring ret = tmpCharArray;
   delete [] tmpCharArray;
   return ret;
}

// Private - Static
HRESULT CSpdCrypto::ManipulateString(const std::wstring &_inputData, std::wstring & _outputData, bool bEncrypt)
{
   struct PlainTextKeyBlob {
        BLOBHEADER hdr;
        DWORD cbKeySize;
        BYTE key[16];
   };

   _outputData = L"";
   HRESULT hr (S_OK);
   if (_inputData.length() > 0)
   {
      HCRYPTPROV context = 0;
      HCRYPTKEY hKey(0);
      BYTE *pAllDataBuffer = NULL;
      BYTE *pBuffer = NULL;
      BYTE *pOutputBuffer = NULL;
      try
      {
         if (!CryptAcquireContext(&context, NULL, NULL, PROV_DH_SCHANNEL, CRYPT_MACHINE_KEYSET | CRYPT_VERIFYCONTEXT))
         {
            context = 0;
            throw HRESULT_FROM_WIN32(::GetLastError());
         }

         // The values for the key have to match the C# Key Values.

         // This key is taken from TDESAlgorithm.Key in the CSpdEncryptedString.cs. The value can never change, or it will break older code.
         BYTE importKeyArray[] = { 144, 161, 1, 255, 175, 166, 79, 204, 187, 232, 54, 175, 3, 105, 224, 70 };
         PlainTextKeyBlob keyBlob ={0};
         keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
         keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
         keyBlob.hdr.reserved = 0;
         keyBlob.hdr.aiKeyAlg = CALG_3DES_112;
         keyBlob.cbKeySize =  sizeof(importKeyArray);
         memcpy(keyBlob.key, importKeyArray, keyBlob.cbKeySize);

         DWORD dwSizeBlob = sizeof(BLOBHEADER)+sizeof(DWORD)+sizeof(importKeyArray);
         if(!CryptImportKey(context, (const BYTE*)&keyBlob, dwSizeBlob, 0, CRYPT_EXPORTABLE, &hKey))
            throw HRESULT_FROM_WIN32(::GetLastError());

         DWORD dwMode = 0;

         // Set the IV - Init Vector, copied from the default values from C# code
         BYTE ivByteArray[] = { 74, 93, 103, 82, 91, 38, 213, 230 };
         if (!CryptSetKeyParam(hKey, KP_IV, ivByteArray, 0))
            throw HRESULT_FROM_WIN32(::GetLastError());

         // Set the Crypt Mode
         dwMode = CRYPT_MODE_ECB;
         if (!CryptSetKeyParam(hKey, KP_MODE, (BYTE*)(&dwMode), 0))
            throw HRESULT_FROM_WIN32(::GetLastError());

         // Set the Padding
         // Online - So fundamentally PKCS#5 padding is a subset of PKCS#7 padding for 8 byte block sizes. Hence, 
         // PKCS#5 padding can not be used for AES. PKCS#5 padding was only defined with (triple) DES operation in mind.
         // C# code uses PaddingMode.PKCS7
         //NOTE:Commenting this out seems to make no difference!!!
         dwMode = PKCS5_PADDING;
         if (!CryptSetKeyParam(hKey, KP_PADDING, (BYTE*)(&dwMode), 0))
            throw HRESULT_FROM_WIN32(::GetLastError());

         // encrypt the data
         std::string tmpValue = "";

         if (bEncrypt)
            tmpValue = WstringToString(_inputData);
         else
            tmpValue = base64_decode(WstringToString(_inputData));


         DWORD allDataLength = (DWORD)tmpValue.length();                            // SLB. Avoid Warning msg.
         pAllDataBuffer = new BYTE[allDataLength];
         memset(pAllDataBuffer, 0, allDataLength);
         memcpy(pAllDataBuffer, tmpValue.c_str(), allDataLength);

         const int ENCRYPT_BLOCK_SIZE = 8;
         DWORD block_length(1000 - 1000 % ENCRYPT_BLOCK_SIZE);
         DWORD buffer_length(block_length + ENCRYPT_BLOCK_SIZE);
         DWORD input_cursor(0);
			DWORD count(0), output_buffer_length(0);
         pBuffer = new BYTE[buffer_length];
         bool bLastBuffer = false;
         // encrypt/decrypt the data one block at a time
         do
         {
            count = ((allDataLength-input_cursor)< block_length ? (allDataLength-input_cursor) : block_length);
            memset(pBuffer, 0, buffer_length);
            memcpy(pBuffer, pAllDataBuffer + input_cursor, count);
            bLastBuffer = (input_cursor + count) >= allDataLength;
            input_cursor += count;

            if (bEncrypt)  // Encrypt
            {
               // encrypt, in count and out count are not always equal for CryptEncrypt()
               if (!CryptEncrypt(hKey, 0, (bLastBuffer ? TRUE : FALSE), 0, pBuffer, &count, buffer_length))
                  throw HRESULT_FROM_WIN32(::GetLastError());
            }
            else // Decrypt
            {
                  // encrypt, in count and out count are not always equal for CryptEncrypt()
               if (!CryptDecrypt(hKey, 0, (bLastBuffer ? TRUE : FALSE), 0, pBuffer, &count))
                  throw HRESULT_FROM_WIN32(::GetLastError());
            }

            // copy to output buffer, can't use existing buffer pAllDataBuffer because encrypted data can be larger than original
            if (pOutputBuffer == NULL)
            {
               // This will be the common case
               output_buffer_length = count;
               pOutputBuffer = new BYTE[count];
               memcpy(pOutputBuffer, pBuffer, count);
            }
            else
            {
               // This will not be common, would need to be encrypting a LARGE amount of data

               // Append to exising data
               BYTE* pTmpBuffer = pOutputBuffer;
               pOutputBuffer = new BYTE[output_buffer_length+count];
               memcpy(pOutputBuffer, pTmpBuffer, output_buffer_length);
               memcpy(pOutputBuffer+output_buffer_length, pBuffer, count);
               output_buffer_length += count;
               delete[] pTmpBuffer;
            }
         } while (input_cursor<allDataLength);

         if (bEncrypt)
            _outputData = StringToWstring(base64_encode(pOutputBuffer, (size_t)output_buffer_length)); // CR.32950; modified.
         else
            _outputData = StringToWstring(std::string((const char*)pOutputBuffer, output_buffer_length));
      }
      catch(HRESULT &eHr)
      {
         hr = eHr;
      }
      // Clean up
      if (hKey)
      {
         CryptDestroyKey(hKey);
         hKey = 0;
      }
      if (context != 0)
      {
         CryptReleaseContext(context, 0);
         context = 0;
      }
      if (pBuffer)
      {
         delete[] pBuffer;
         pBuffer = NULL;
      }

      if (pAllDataBuffer)
      {
         delete[] pAllDataBuffer;
         pAllDataBuffer = NULL;
      }
      if (pOutputBuffer)
      {
         delete[] pOutputBuffer;
         pOutputBuffer = NULL;
      }
   }
   //if (FAILED(hr))
   //{
   //   wchar_t wcsMsg[256];
   //   _snwprintf_s(wcsMsg, _TRUNCATE, L"Error - hr: %08X\n", hr);
   //}
   return hr;
}
