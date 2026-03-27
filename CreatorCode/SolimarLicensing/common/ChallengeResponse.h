#pragma once
#ifndef __Challenge_Response_Header_BFD8FAC8_211E_4e2f_AC12_78BC0EC11BF4__
#define __Challenge_Response_Header_BFD8FAC8_211E_4e2f_AC12_78BC0EC11BF4__

#include "CryptoHelper.h"

class ChallengeResponse
{
public:
	
	class Message
	{
	public:
		// message size measured in bytes
		// changing may cause the message sizes on the challenge and response sides to be out of synch
		enum {MESSAGE_SIZE = 16, MESSAGE_BUFFER_SIZE = 1024};
		
		Message();
		// duplicate the Message o
		Message(const Message& o);
		// reconstruct the message from a VARIANT SafeArray
		Message(const VARIANT &vtMessage);
		
		virtual ~Message();
		
		bool operator==(const Message& o);
		bool operator!=(const Message& o);
		
		operator VARIANT();
		
		DWORD size();
		bool isValid();
		
		HRESULT Sign(CryptoHelper &context, const CryptoHelper::Key &key);
		HRESULT VerifySignature(CryptoHelper &context, const CryptoHelper::Key &key, const Message& signature);
		
	private:
		DWORD data_size;				// for integrity purposes, in case someone changes MESSAGE_SIZE
		BYTE data[MESSAGE_BUFFER_SIZE];	// the message (plus some extra room for the encrypted version)
	};
	
	ChallengeResponse(std::wstring &key_path);
	ChallengeResponse(const BYTE *pKeyBuffer, const DWORD key_buffer_size);
	virtual ~ChallengeResponse();
	
	HRESULT GenerateChallenge(/*[out]*/ Message& challenge);
	HRESULT GenerateResponse(/*[in]*/ const Message& challenge, /*[out]*/ Message& response);
	HRESULT VerifyResponse(/*[in]*/ Message& challenge, /*[in]*/ Message& response);
	
private:
	CryptoHelper m_crypt;
	CryptoHelper::Key m_key;
};

#endif //__Challenge_Response_Header_BFD8FAC8_211E_4e2f_AC12_78BC0EC11BF4__

