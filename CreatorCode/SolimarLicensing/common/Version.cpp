#include "Version.h"
#include <string>
#include <stdio.h>
#include <tchar.h>
Version::ModuleVersion::ModuleVersion(std::wstring versionstr)
{
	long output[4]={0,0,0,0};
	wchar_t *pToken;
	wchar_t *pNextToken;
	size_t sizeofVersionBuffer = versionstr.length()+1;
	wchar_t *pVersionBuffer=new wchar_t[sizeofVersionBuffer];
	
	
	// copy the string so we can modify the copy using strtok
	wcscpy_s(pVersionBuffer, sizeofVersionBuffer, versionstr.c_str());
	
	// parse the string
	unsigned int uiVersionCount=0;
	pToken=wcstok_s(pVersionBuffer, L",. \t\n", &pNextToken);
	while (pToken!=NULL && uiVersionCount<4)
	{
		output[uiVersionCount]= _wtol(pToken);
		++uiVersionCount;
		pToken=wcstok_s(NULL,L",. \t\n", &pNextToken);
	}
	
	major = output[0];
	minor = output[1];
	submajor = output[2];
	subminor = output[3];
	delete [] pVersionBuffer;
}

Version::ModuleVersion::ModuleVersion(const TinyVersion &t)
{
	major=(long)((t.program & 0xF000) >> 12);
	minor=(long)((((t.program & 0x0F00) >> 8) * 10) + (((t.program & 0x00F0) >> 4) * 1));
	submajor=0;
	subminor=(long)(t.build);
}
		
std::wstring Version::ModuleVersion::ToString()
{
	wchar_t *versionstring=new wchar_t[64];
	_snwprintf_s(versionstring, sizeof(versionstring)/sizeof(wchar_t), _TRUNCATE, L"%d.%d.%d.%d", major, minor, submajor, subminor);
	versionstring[63]=0;	// prevent string buffer overflows
	return versionstring;
}

bool Version::ModuleVersion::operator<(const Version::ModuleVersion& o)
{
	if (major>o.major)
		return false;
	if (o.major>major)
		return true;

	if (minor>o.minor)
		return false;
	if (o.minor>minor)
		return true;

	if (submajor>o.submajor)
		return false;
	if (o.submajor>submajor)
		return true;

	if (subminor>o.subminor)
		return false;
	if (o.subminor>subminor)
		return true;

	return false;
}

bool Version::ModuleVersion::operator<=(const Version::ModuleVersion& o)
{
	return (*this<o || *this==o);
}

bool Version::ModuleVersion::operator>(const Version::ModuleVersion& o)
{
	return (!(*this<=o));
}

bool Version::ModuleVersion::operator>=(const Version::ModuleVersion& o)
{
	return (!(*this<o));
}

bool Version::ModuleVersion::operator==(const Version::ModuleVersion& o)
{
	return (major==o.major && minor==o.minor && submajor==o.submajor && subminor==o.subminor);
}



Version::TinyVersion::TinyVersion(std::wstring versionstr)
{
	TinyVersion tv;
	tv = ModuleVersion(versionstr);
	program = tv.program;
	build = tv.build;
}

Version::TinyVersion::TinyVersion(const ModuleVersion &mv)
{
	program = (short)( ((mv.major & 0x0000000F) << 12) | ( ((((mv.minor % 100)/10) & 0x0000000F) << 8) | (((mv.minor % 10) & 0x0000000F) << 4)) );
	build = (short)(mv.subminor & 0x0000FFFF);
}

std::wstring Version::TinyVersion::ToString()
{
	return ModuleVersion(*this).ToString();
}

bool Version::TinyVersion::operator<(const Version::TinyVersion& o)		{return ModuleVersion(*this)<ModuleVersion(o);}
bool Version::TinyVersion::operator<=(const Version::TinyVersion& o)	{return ModuleVersion(*this)<=ModuleVersion(o);}
bool Version::TinyVersion::operator>(const Version::TinyVersion& o)		{return ModuleVersion(*this)>ModuleVersion(o);}
bool Version::TinyVersion::operator>=(const Version::TinyVersion& o)	{return ModuleVersion(*this)>=ModuleVersion(o);}
bool Version::TinyVersion::operator==(const Version::TinyVersion& o)	{return ModuleVersion(*this)==ModuleVersion(o);}

