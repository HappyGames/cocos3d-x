/*
 * Cocos3D-X 1.0.0
 * Copyright (c) 2014-2015 Jason Wong
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

NS_COCOS3D_BEGIN

COCOS_SINGLETON_IMPL( Loggers );

static const char* logLevels[] =
{
	"[I]",
	"[E]",
	"[W]",
	"UNKNOWN",
};

CLoggers::CLoggers()
{
	m_bShowSystemTime = true;
}

CLoggers::~CLoggers()
{

}

void CLoggers::logMessage( int level, const char* format, ... )
{
	char buffer[512];  // large buffers
	GetVarargs(buffer, 512, format);
	std::string prefix = "";
	if ( m_bShowSystemTime )
		prefix = "[" + CC3String::getSystemTime() + "]";

	prefix += level <= CC3_LOG_MAX ? logLevels[level] : logLevels[CC3_LOG_MAX];
	prefix += ":";

	for( unsigned int i = 0; i < m_allLogger.size(); i++ )
	{
		m_allLogger[i]->logMessage( prefix + buffer );
	}
}

void CLoggers::logMessageDirectly( int level, const std::string& msg )
{
	std::string prefix = "";
	if ( m_bShowSystemTime )
		prefix = "[" + CC3String::getSystemTime() + "]";

	prefix += level <= CC3_LOG_MAX ? logLevels[level] : logLevels[CC3_LOG_MAX];
	prefix += ":";

	for( unsigned int i = 0; i < m_allLogger.size(); i++ )
	{
		m_allLogger[i]->logMessage( prefix + msg );
	}
}

void CLoggers::addLogger( CLogDelegate* pLogger )
{
	m_allLogger.push_back( pLogger );
}

void CLoggers::setShowTime( bool showTime )
{
	m_bShowSystemTime = showTime;
}

bool CLoggers::isShowTime()
{
	return m_bShowSystemTime;
}

NS_COCOS3D_END
