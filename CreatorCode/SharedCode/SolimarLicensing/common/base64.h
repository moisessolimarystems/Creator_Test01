#ifndef _BASE64_H_
#define _BASE64_H_

#include <string>

std::string base64_encode(unsigned char const* p, unsigned int len);
std::string base64_decode(std::string const& s);


#ifdef _WIN64
// SLB 21.feb.2024 CR.32662; created.
// inline overload for methods passing size_t as length.
inline std::string base64_encode(unsigned char const* p, size_t len)
{
	return base64_encode(p, (unsigned int)len);
}
#endif

#endif