
#include "FlateHelper.h"
#include "..\..\3prty\Flate\Zlib.h"

//static
unsigned char* FlateHelper::UnCompressStream(
	const char* buf,																						// CR.32661.V01; modified.
	long bufSize, 
	long* outBufSize)
{
	z_stream d_stream;
	d_stream.zalloc = (alloc_func)0;
	d_stream.zfree = (free_func)0;
	d_stream.opaque = (voidpf)0;

	int err = inflateInit(&d_stream);
	if (err != Z_OK) 
	{
	  return NULL;
	}
	d_stream.next_in = (unsigned char*)&buf[sizeof(int)];
	d_stream.avail_in = bufSize-sizeof(int);

	// The data's un-compressed size at the beginning of
	// the stream, size of an int.
	memcpy(outBufSize, buf, sizeof(int));
	unsigned char *outBuf = new unsigned char[*outBufSize];

	d_stream.next_out = outBuf;
	d_stream.avail_out = *outBufSize;
	err = inflate(&d_stream, Z_FINISH);

	//Success Case
	if (err == Z_STREAM_END ||
		(err == Z_OK && (*outBufSize == d_stream.total_out))	//CR.FIX.13625 - There is still more data in the buffer, buffer have already read all the data we care about.
		)
	{
		*outBufSize = d_stream.total_out;
		inflateEnd(&d_stream);
		return outBuf;
	}
	//error case
	inflateEnd(&d_stream);
	delete[] outBuf;
	return NULL;
}
unsigned char* FlateHelper::CompressStream(
	const char* origBuf, 																				// CR.32661.V01; modified.
	long origBufSize, 
	long* pOutBufSize)
{
	z_stream c_stream;
	c_stream.zalloc = (alloc_func)0;
	c_stream.zfree = (free_func)0;
	c_stream.opaque = (voidpf)0;

	int err = deflateInit(&c_stream, Z_DEFAULT_COMPRESSION);
	if (err != Z_OK) 
	{
		//m_lastError = "PdfWriter::CompressStream Error: deflateInit failed";
		return NULL;
	}
	c_stream.next_in = (unsigned char*)origBuf;
	c_stream.avail_in = origBufSize;

	// The flate compression states that if deflate is to be
	// called with Z_FINISH directly after deflateInit,
	// then the output buffer needs to be .1% + 12 bytes
	// larger than the input buffer. We also want to store
	// the data's un-compressed size at the beginning of
	// the stream, so make room for another int.
	

	*pOutBufSize = (long)(origBufSize+(origBufSize * .001)+12+sizeof(int));
	unsigned char *outBuf = new unsigned char[*pOutBufSize];
	c_stream.next_out = &outBuf[sizeof(int)];   
	c_stream.avail_out = *pOutBufSize - sizeof(int);
	memcpy(outBuf, &origBufSize, sizeof(int));

	err = deflate(&c_stream, Z_FINISH);
	if (err != Z_STREAM_END) 
	{
	  //m_lastError = "PdfWriter::CompressStream Error: deflateInit failed";
	  deflateEnd(&c_stream);
	  delete[] outBuf;
	  return NULL;
	}
	*pOutBufSize = c_stream.total_out+sizeof(int);

	deflateEnd(&c_stream);
	return outBuf;
}



