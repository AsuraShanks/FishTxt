#include <string.h>
#include "EncodingMapper.h"
#include "..\include\Scintilla\Scintilla.h"


static EncodingUnit encodings[] = {
	{SC_CP_UTF8, "utf-8 utf8"},																	//utf8
    {936,   "gb2312 gbk csGB2312 gb18030"},                                                     //IDM_FORMAT_GB2312
};

bool isInListA(const char *token, const char *list)
{
	if ((!token) || (!list))
		return false;

	char word[64] = { '\0' };
	size_t i = 0;
	size_t j = 0;
	for (size_t len = strlen(list); i <= len; ++i)
	{
		if ((list[i] == ' ')||(list[i] == '\0'))
		{
			if (j != 0)
			{
				word[j] = '\0';
				j = 0;
				
				if (!_stricmp(token, word))
					return true;
			}
		}
		else 
		{
			word[j] = list[i];
			++j;
		}
	}
	return false;
}

int EncodingMapper::getEncodingFromString(const char *encodingAlias) const
{
	if (isInListA(encodingAlias, "utf-8 utf8"))
		return SC_CP_UTF8;

	size_t nbItem = sizeof(encodings)/sizeof(EncodingUnit);
	int enc = -1;
	for (size_t i = 0 ; i < nbItem ; ++i)
	{
		if (isInListA(encodingAlias, encodings[i]._aliasList))
		{
			enc = encodings[i]._codePage;
			break;
		}
	}
	return enc;
}
