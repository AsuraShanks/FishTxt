// pch.cpp: 与预编译标头对应的源文件

#include "pch.h"
#include <vector>

// 当使用预编译的头时，需要使用此源文件，编译才能成功。

CString RCG_UTF82ASCII(CString strUtf8)
{
	//先把 utf8 转为 unicode  
	CStringW strW = RCG_UTF82UNICODE(strUtf8);
	//最后把 unicode 转为 ascii 
	CString strRet = RCG_WideByte2Asic(strW);
	return strRet;
}

CStringW RCG_UTF82UNICODE(CString strUtf8)
{
	int nWideSize = ::MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, NULL, 0);
	if (ERROR_NO_UNICODE_TRANSLATION == nWideSize)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (0 == nWideSize)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<wchar_t> resultstring(nWideSize);
	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, strUtf8, -1, &resultstring[0], nWideSize);
	if (convresult != nWideSize)
	{
		throw std::exception("La falla!");
	}

	return CStringW(&resultstring[0]);
}

CString RCG_ASCII2UTF8(CString strAscii)
{
	//先把 utf8 转为 unicode
	CStringW strW = RCG_ASCII2WideByte(strAscii);
	CString strRet = RCG_UNICODE2UTF8(strW);
	return strRet;
}

CStringW RCG_ASCII2WideByte(CString strAscii)
{
	int widesize = MultiByteToWideChar(CP_ACP, 0, strAscii, -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar(CP_ACP, 0, strAscii, -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return CStringW(&resultstring[0]);
}

CString RCG_UNICODE2UTF8(CStringW strUnicode)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<char> resultstring(utf8size);

	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, strUnicode, -1, &resultstring[0], utf8size, NULL, NULL);

	if (convresult != utf8size)
	{
		throw std::exception("La falla!");
	}

	return CString(&resultstring[0]);
}

CString RCG_WideByte2Asic(CStringW strUnicode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult = ::WideCharToMultiByte(CP_OEMCP, 0, strUnicode, -1, &resultstring[0], asciisize, NULL, NULL);

	if (convresult != asciisize)
	{
		throw std::exception("La falla!");
	}

	return CString(&resultstring[0]);
}