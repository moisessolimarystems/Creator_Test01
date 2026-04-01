#pragma once

#include <windows.h>

//Have to link flatelib.lib
class FlateHelper
{
public:
	//Up to caller to free returned buffer, returns NULL on error
	static unsigned char* UnCompressStream(char* buf, long bufSize, long* outBufSize);

	//Up to caller to free returned buffer, returns NULL on error
	static unsigned char* CompressStream(char* buf, long bufSize, long* outBufSize);
};