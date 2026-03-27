#pragma once

#include <string>

namespace Version
{
	class TinyVersion;

	/** Standard version structure. Four version places (major, minor, submajor, and subminor). */
	class ModuleVersion
	{
	public:
		long major, minor, submajor, subminor;
		
		ModuleVersion() : major(0), minor(0), submajor(0), subminor(0) {;}
		ModuleVersion(std::wstring versionstr);
		ModuleVersion(long ver_major, long ver_minor, long ver_submajor, long ver_subminor) : major(ver_major), minor(ver_minor), submajor(ver_submajor), subminor(ver_subminor) {;}
		ModuleVersion(const TinyVersion &t);
		
		std::wstring ToString();
		
		bool operator<(const ModuleVersion& o);
		bool operator<=(const ModuleVersion& o);
		bool operator>(const ModuleVersion& o);
		bool operator>=(const ModuleVersion& o);
		bool operator==(const ModuleVersion& o);
	};
	
	/** Compacted version structure. Mimics the version number compression used on the protection keys. */
	class TinyVersion
	{
	public:
		short program, build;
		
		TinyVersion() : program(0), build(0) {;}
		TinyVersion(std::wstring versionstr);
		TinyVersion(short prog, short buildnum) : program(prog), build(buildnum) {;}
		TinyVersion(const ModuleVersion &mv);

		std::wstring ToString();

		bool operator<(const TinyVersion& o);
		bool operator<=(const TinyVersion& o);
		bool operator>(const TinyVersion& o);
		bool operator>=(const TinyVersion& o);
		bool operator==(const TinyVersion& o);
	};
};

