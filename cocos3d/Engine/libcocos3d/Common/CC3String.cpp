/*
 * Cocos3D-X 1.0.0
 * Copyright (c) 2014-2015 Jason Wang
 * http://www.cocos3dx.org/
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * http://en.wikipedia.org/wiki/MIT_License
 */
#include "cocos3d.h"
#include <algorithm>

NS_COCOS3D_BEGIN

#ifndef _WIN32
	#ifndef _MAX_PATH 
		#define _MAX_PATH 256
	#endif
#endif

std::string CC3String::getFileName( const std::string& filePath )
{
	int i;
	bool bFound = false;
	for (i = (int)filePath.size()-1; i>0; i--)
	{
		if ( filePath[i] == '\\' || filePath[i] == '/' )
		{
			bFound = true;
			break;
		}
	}

	return bFound ? filePath.substr(i+1, filePath.size()) : filePath;
}


static inline bool _getDirectory( char* dest, char* src )
{
	char buf[_MAX_PATH] = {0};
	strcpy( buf, src );
	int nLen = (int)strlen(src);

	bool bFound = false;
	for ( int i = nLen-1; i >=0; i--)
	{
		if ( buf[i] == '/' || buf[i] == '\\' )
		{
			buf[i] = 0;

			bFound = true;
			break;
		}
	}

	strcpy(dest, buf);

	return bFound;
}

static inline void addSlash( char* buf )
{
	size_t nLen = strlen(buf);
	if ( buf[nLen-1] != '\\' && buf[nLen-1] != '/' )
		strcat(buf, "/");
}

static inline std::string replaceChar( const std::string& sSrc, const char& cOld, const char& cNew )
{
	std::string sDest = sSrc;

	for (size_t i = 0; i < sDest.size(); i++)
	{
		if ( sDest[i] == cOld )
			sDest[i] = cNew;
	}

	return sDest;
}

static inline std::string trimString( std::string& str, char toTrim = ' ' )
{
	std::string::size_type pos = str.find_last_not_of(toTrim);
	if(pos != std::string::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(toTrim);
		if(pos != std::string::npos) 
			str.erase(0, pos);
	}
	else 
	{
		str.erase(str.begin(), str.end());
	}

	return str;
}

static inline bool isNumber( const char* str, int len )
{
	if ( len == 0 )
		return false;
	for (int i = 0; i < len; i++)
	{
		if ( (int)str[i]<0 )
			return false;

		if ( !isdigit(str[i]) && (str[i]!='-'&&str[i]!='.') )
			return false;
	}
	return true;
}

std::string CC3String::getDirectory( const std::string& filePath )
{
	char buf[_MAX_PATH] = {0};
	if ( _getDirectory(buf, const_cast<char*>(filePath.data())) )
	{
		addSlash(buf);
		std::string sBuf = buf;
		sBuf = replaceChar( sBuf, '\\', '/' );
		return sBuf;
	}

	return "";
}

std::string CC3String::getExtension( const std::string& filePath )
{
	std::string::size_type nPos = filePath.rfind( '.' );
	if ( nPos == std::string::npos )
		return "";

	if ( nPos + 1 < filePath.size() )
		return filePath.substr( nPos + 1 );

	return "";
}

void CC3String::makeLowercase( std::string& str )
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

void CC3String::makeUppercase( std::string& str )
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

std::string CC3String::trim( std::string& str, char toTrim )
{
	return trimString( str, toTrim );
}

int CC3String::splitString( const std::string& src, std::vector<std::string>& destParts, const std::string& seperator )
{
	std::string sTmp = src;
	std::string sRet = "";
    std::string::size_type nPos = sTmp.find( seperator );
	if ( nPos == std::string::npos )
	{
		if ( sTmp.empty() )
			return -1;
		else
		{
			trimString( sTmp );
			destParts.push_back( sTmp );
			return 1;
		}
	}

	while ( true )
	{
		sRet = sTmp.substr( 0, nPos );
		trimString( sRet );
		destParts.push_back( sRet );
		sTmp = sTmp.substr( nPos + seperator.size(), sTmp.size() );
		nPos = sTmp.find( seperator );

		if ( nPos == std::string::npos )
		{
			trimString( sTmp );
			if ( !sTmp.empty() )
				destParts.push_back( sTmp );
			break;
		}
	}

	return (int)destParts.size();
}

std::string CC3String::getSystemTime()
{
	time_t curTime;
	tm today;
	time( &curTime );
	today = *(localtime( &curTime ));
	char szTmp[128] = { 0 };
	strftime( szTmp, 128, "%Y/%m/%d %H:%M:%S", &today );
	
	return std::string( szTmp );
}

int CC3String::toInteger( const std::string& str )
{
	if ( str.empty() )
		return 0;

	if ( !isNumber(str.data(), (int)str.size()) )
		return 0;

	return atoi( str.data() );
}

bool CC3String::startsWith( const char* str, const char* with )
{
	const char* src = str;
	const char* cmp = with;
	size_t nLen = strlen( with );
	int i = 0;
	while( i < nLen )
	{
		if ( (*(src++)) != (*(cmp++)) )
			return false;

		i++;
	}

	return true;
}

void CC3String::stringWithFormat( std::string& sDest, char* szFormat, ... )
{
	char buffer[512];  // large buffers
	GetVarargs(buffer, 512, szFormat);
	sDest = buffer;
}

std::string CC3String::stringWithFormat( char* szFormat, ... )
{
	char buffer[512];  // large buffers
	GetVarargs(buffer, 512, szFormat);
	std::string sDest = buffer;
	return sDest;
}

NS_COCOS3D_END
