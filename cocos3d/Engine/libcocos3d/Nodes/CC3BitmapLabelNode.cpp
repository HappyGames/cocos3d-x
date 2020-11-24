/*
 * Cocos3D-X 1.0.0
 * Author: Bill Hollings
 * Copyright (c) 2010-2014 The Brenwill Workshop Ltd. All rights reserved.
 * http://www.brenwill.com
 *
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

NS_COCOS3D_BEGIN

CC3BitmapFontConfiguration::CC3BitmapFontConfiguration()
{
	m_characterSet.clear();
	m_atlasName = "";
}

CC3BitmapFontConfiguration::~CC3BitmapFontConfiguration()
{
	purgeCharDefDictionary();
	purgeKerningDictionary();
}

void CC3BitmapFontConfiguration::purgeCharDefDictionary()
{
	CC3BitmapCharDefHashElement *current, *tmp;
	HASH_ITER(hh, m_charDefDictionary, current, tmp) 
	{
		HASH_DEL(m_charDefDictionary, current);
		CC_SAFE_FREE(current);
	}
}

std::string CC3BitmapFontConfiguration::getAtlasName()
{
	return m_atlasName;
}

int CC3BitmapFontConfiguration::getCommonHeight()
{
	return m_commonHeight;
}

float CC3BitmapFontConfiguration::getFontSize()
{
	return m_fontSize;
}

unsigned int CC3BitmapFontConfiguration::getBaseline()
{
	return m_baseline;
}

CCSize CC3BitmapFontConfiguration::getTextureSize()
{
	return m_textureSize;
}

void CC3BitmapFontConfiguration::purgeKerningDictionary()
{
	CC3KerningHashElement *current;
	while(m_kerningDictionary) 
	{
		current = m_kerningDictionary;
		HASH_DEL(m_kerningDictionary, current);
		CC_SAFE_FREE(current);
	}
}

CC3BitmapCharDef* CC3BitmapFontConfiguration::getCharacterSpecFor( unsigned char c )
{
	CC3BitmapCharDefHashElement *element = NULL;
	GLuint key = (GLuint)c;
	HASH_FIND_INT(m_charDefDictionary , &key, element);
	return element ? &(element->charDef) : NULL;
}

int CC3BitmapFontConfiguration::getKerningBetween( unsigned char firstChar, unsigned char secondChar )
{
	if(m_kerningDictionary) 
	{
		unsigned int key = (firstChar << 16) | (secondChar & 0xffff);
		CC3KerningHashElement* element = NULL;
		HASH_FIND_INT(m_kerningDictionary, &key, element);
		if(element) 
			return element->amount;
	}
	return 0;
}

bool CC3BitmapFontConfiguration::initFromFontFile( const std::string& fontFile )
{
	m_kerningDictionary = NULL;
	m_charDefDictionary = NULL;
	std::string validChars = parseConfigFile( fontFile );
	if( validChars.empty() ) 
		return false;

//	_characterSet = characterSetWithCharactersInString( validChars );
	
	return true;
}

static CCDictionary* _fontConfigurations = NULL;

CC3BitmapFontConfiguration* CC3BitmapFontConfiguration::configurationFromFontFile( const std::string& fontFile )
{
	CC3BitmapFontConfiguration *fontConfig = NULL;
	
	if( _fontConfigurations == NULL )
	{
		_fontConfigurations = CCDictionary::create();	// retained
		_fontConfigurations->retain();
	}
	
	fontConfig = (CC3BitmapFontConfiguration*)_fontConfigurations->objectForKey( fontFile );
	if( !fontConfig ) 
	{
		fontConfig = new CC3BitmapFontConfiguration;
		fontConfig->initFromFontFile( fontFile );
		if (fontConfig)
			_fontConfigurations->setObject( fontConfig, fontFile );
		else
			CC3_TRACE( "[bfc]failed loading font file %s", fontFile.c_str() );
		CC_SAFE_RELEASE(fontConfig);
	}

	return fontConfig;
}

void CC3BitmapFontConfiguration::clearFontConfigurations()
{
	if ( _fontConfigurations )
		_fontConfigurations->removeAllObjects(); 

	CC_SAFE_RELEASE( _fontConfigurations );
}

std::string CC3BitmapFontConfiguration::description()
{
	return CC3String::stringWithFormat( (char*)"CC3BitmapFontConfiguration with glphys: %d, kernings:%d, image = %s",
			HASH_COUNT(m_charDefDictionary),
			HASH_COUNT(m_kerningDictionary),
			m_atlasName.c_str() );
}

/** Parses the configuration file, line by line. */
std::string CC3BitmapFontConfiguration::parseConfigFile( const std::string& fontFile )
{
	std::string fullpath = CCFileUtils::sharedFileUtils()->fullPathForFilename( fontFile.c_str() );
		
	CCString* pString = CCString::createWithContentsOfFile( fullpath.c_str() );
	if ( pString == NULL )
		CC3_TRACE( "[bfc]Failed open file %s", fullpath.c_str() );

	CCAssert( pString, "Could not load font file" );

	std::string validCharsString = pString->getCString();
	pString->release();
    
	// Separate the lines into an array and create an enumerator on it
	CLineScanner lineScanner;
	LineArray lines = lineScanner.scan( (char*)validCharsString.c_str(), validCharsString.size() );
	std::string line = "";
    
	// Loop through all the lines in the lines array processing each one based on its first chars
	for ( unsigned int i = 0; i < lines.size(); i++ ) 
	{
		line = lines[i];
		if( CC3String::startsWith( line.c_str(), "char id" ) )
			parseCharacterDefinition( line, validCharsString );
		else if( CC3String::startsWith( line.c_str(), "kerning" ) ) 
			parseKerningEntry( line );
		else if( CC3String::startsWith( line.c_str(), "info" ) ) 
			parseInfoArguments( line );
		else if( CC3String::startsWith( line.c_str(), "common" ) ) 
			parseCommonArguments( line );
		else if( CC3String::startsWith( line.c_str(), "page" ) ) 
			parseImageFileName( line, fontFile );
		else if( CC3String::startsWith( line.c_str(), "chars count" ) ) 
		{}
	}

	return validCharsString;
}

/** Parses a character definition line. */
void CC3BitmapFontConfiguration::parseCharacterDefinition( const std::string& line, std::string& validChars )
{
	CC3BitmapCharDefHashElement *element = (CC3BitmapCharDefHashElement*)calloc(sizeof(CC3BitmapCharDefHashElement), 1);

	LineArray values;
	CC3String::splitString( line, values, "=" );

	std::string propertyValue = "";
    int propertyCursor = 0;

	propertyCursor++;		// Skip line header
    
	propertyValue = values[propertyCursor++];							// Character unicode value
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.charCode = atoi(propertyValue.c_str());
    
	propertyValue = values[propertyCursor++];							// Character rect x
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.rect.origin.x = (float)atof(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];							// Character rect y
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.rect.origin.y = (float)atof(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];							// Character rect width
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.rect.size.width = (float)atof(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];							// Character rect height
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.rect.size.height = (float)atof(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];							// Character xoffset
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.xOffset = atoi(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];							// Character yoffset
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.yOffset = atoi(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];							// Character xadvance
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	element->charDef.xAdvance = atoi(propertyValue.c_str());
	
	element->key = element->charDef.charCode;
	HASH_ADD_INT(m_charDefDictionary, key, element);
	
	validChars += CC3String::stringWithFormat( (char*)"%C", element->charDef.charCode );
}

/** Parses a kerning line. */
void CC3BitmapFontConfiguration::parseKerningEntry( const std::string& line )
{
	if ( CC3String::startsWith( line.c_str(), "kernings" ) )
		return;

	LineArray values;
	CC3String::splitString( line, values, "=" );
	std::string propertyValue = "";
    int propertyCursor = 0;

	propertyCursor++;		// Skip line header
    
	propertyValue = values[propertyCursor++];				// First character
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	int first = CC3String::toInteger( propertyValue );
    
	propertyValue = values[propertyCursor++];				// Second character
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	int second = CC3String::toInteger( propertyValue );
    
	propertyValue = values[propertyCursor++];				// Kerning amount
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	int amount = CC3String::toInteger( propertyValue );
    
	CC3KerningHashElement *element = (CC3KerningHashElement*)calloc(sizeof(CC3KerningHashElement), 1);
	element->key = (first<<16) | (second&0xffff);
	element->amount = amount;
	HASH_ADD_INT(m_kerningDictionary, key, element);
}

/** Parses the info line. */
void CC3BitmapFontConfiguration::parseInfoArguments( const std::string& line )
{
	//
	// possible lines to parse:
	// info face="Script" size=32 bold=0 italic=0 charset="" unicode=1 stretchH=100 smooth=1 aa=1 padding=1,4,3,2 spacing=0,0 outline=0
	// info face="Cracked" size=36 bold=0 italic=0 charset="" unicode=0 stretchH=100 smooth=1 aa=1 padding=0,0,0,0 spacing=1,1
	//
	LineArray values;
	CC3String::splitString( line, values, "=" );
	std::string propertyValue = "";
	int propertyCursor = 0;
	
	propertyCursor++;		// Skip line header
	
	propertyCursor++;								// Font face (ignore)
	
	propertyValue = values[propertyCursor++];				// Font size
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	m_fontSize = (float)atof( propertyValue.c_str() );
	
	propertyCursor++;								// Bold flag (ignore)
	propertyCursor++;								// Italic (ignore)
	propertyCursor++;								// Character set (ignore)
	propertyCursor++;								// Unicode (ignore)
	propertyCursor++;								// Horizontal stretch (ignore)
	propertyCursor++;								// Smoothing (ignore)
	propertyCursor++;								// aa (ignore) 
	
	// Padding is a combined element. Create a parser for it.
	propertyValue = values[propertyCursor++];
	LineArray paddingValues;
	CC3String::splitString( propertyValue, paddingValues, "," );
	int paddingCuror = 0;
	
	propertyValue = paddingValues[paddingCuror++];		// Padding top
	m_padding.top = CC3String::toInteger( propertyValue );
	
	propertyValue = paddingValues[paddingCuror++];		// Padding right
	m_padding.right = CC3String::toInteger( propertyValue );
	
	propertyValue = paddingValues[paddingCuror++];		// Padding bottom
	m_padding.bottom = CC3String::toInteger( propertyValue );
	
	propertyValue = paddingValues[paddingCuror++];		// Padding left
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	m_padding.left = CC3String::toInteger( propertyValue );
	
	propertyCursor++;								// Spacing (ignore)
}

/** Parses the common line. */
void CC3BitmapFontConfiguration::parseCommonArguments( const std::string& line )
{
	//
	// line to parse:
	// common lineHeight=104 base=26 scaleW=1024 scaleH=512 pages=1 packed=0
	//
	LineArray values;
	CC3String::splitString( line, values, "=" );
	std::string propertyValue = "";
	int propertyCursor = 0;
	
	propertyCursor++;		// Skip line header
	
	propertyValue = values[propertyCursor++];				// Line height
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	m_commonHeight = atoi(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];				// Baseline
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	m_baseline = atoi(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];				// Width scale
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	m_textureSize.width = (float)atof(propertyValue.c_str());
	
	propertyValue = values[propertyCursor++];				// Height scale
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	m_textureSize.height = (float)atof(propertyValue.c_str());

	CCAssert(m_textureSize.width <= CCConfiguration::sharedConfiguration()->getMaxTextureSize() &&
			  m_textureSize.height <= CCConfiguration::sharedConfiguration()->getMaxTextureSize(),
			  "Font texture can't be larger than supported");
	
	propertyValue = values[propertyCursor++];				// Pages sanity check
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	int pageNum = atoi(propertyValue.c_str());
	CCAssert( pageNum == 1, "CC3BitmapFontConfiguration does not support font files with multiple pages" );
	
	// packed (ignore) What does this mean ??
}


/** Parses the image file line. */
void CC3BitmapFontConfiguration::parseImageFileName( const std::string& line, const std::string& fontFile )
{
	LineArray values;
	CC3String::splitString( line, values, "=" );
	std::string propertyValue = "";
	int propertyCursor = 0;
    
	propertyCursor++;		// Skip line header
    
	propertyValue = values[propertyCursor++];			// Page ID. Sanity check
	propertyValue = propertyValue.substr( 0, propertyValue.find_first_of( " " ) );
	int pageID = CC3String::toInteger( propertyValue );
	CCAssert( pageID == 0, "CC3BitmapFontConfiguration does not support font files with multiple pages" );
    
	propertyValue = values[propertyCursor++];			// Texture file na,e

	LineArray array;
	CC3String::splitString( propertyValue, array, "\"" );
	propertyValue = array[1];
	CCAssert(!propertyValue.empty(), "CC3BitmapFontConfiguration could not extract font atlas file name");
    
	// Supports subdirectories
	std::string dir = CC3String::getDirectory( fontFile );
	m_atlasName = dir + propertyValue;	// retained
}

CC3BitmapLabelNode::CC3BitmapLabelNode()
{
	m_labelString = "";
	m_fontFileName = "";
	m_fontConfig = NULL;
}

CC3BitmapLabelNode::~CC3BitmapLabelNode()
{
	CC_SAFE_RELEASE( m_fontConfig );
}

void CC3BitmapLabelNode::populateAsBitmapFontLabelFromString( const std::string& lblString, const std::string& fontFileName, GLfloat lineHeight,
						   CCTextAlignment textAlignment, const CCPoint& origin, const CC3Tessellation& divsPerChar )
{
	
	CC3BitmapFontConfiguration* fontConfig = CC3BitmapFontConfiguration::configurationFromFontFile( fontFileName );
	
	populateMeshAsBitmapFontLabelFromString( prepareParametricMesh(), lblString, fontConfig, lineHeight, textAlignment, origin, divsPerChar );

	// Set texture after mesh to avoid mesh setter from clearing texture
	setTexture( CC3Texture::textureFromFile(fontConfig->getAtlasName().c_str()) );

	// By definition, characters have significant transparency, so turn alpha blending on.
	// Since characters can overlap with kerning, don't draw the transparent parts to avoid Z-fighting
	// between the characters. Set the alpha tolerance higher than zero so that non-zero alpha at
	// character edges due to anti-aliasing won't be drawn.
	setIsOpaque( false );
	setShouldDrawLowAlpha( false );
	getMaterial()->setAlphaTestReference( 0.05f );
}

GLfloat CC3BitmapLabelNode::getLineHeight()
{
	return m_lineHeight ? m_lineHeight : (m_fontConfig ? m_fontConfig->getCommonHeight() : 0.f); 
}

void CC3BitmapLabelNode::setLineHeight( GLfloat lineHt )
{
	if (lineHt != m_lineHeight) 
	{
		m_lineHeight = lineHt;
		populateLabelMesh();
	}
}

std::string CC3BitmapLabelNode::getLabelString()
{
	return m_labelString; 
}

void CC3BitmapLabelNode::setLabelString( const std::string& aString )
{
	if ( aString == m_labelString ) 
		return;
	
	m_labelString = aString;
	populateLabelMesh();
}

std::string CC3BitmapLabelNode::getFontFileName()
{
	return m_fontFileName; 
}

void CC3BitmapLabelNode::setFontFileName( const std::string& aFileName )
{
	if ( aFileName == m_fontFileName ) 
		return;

	m_fontFileName = aFileName;

	CC_SAFE_RELEASE( m_fontConfig );
	m_fontConfig = CC3BitmapFontConfiguration::configurationFromFontFile( m_fontFileName );
	m_fontConfig->retain();
	
	populateLabelMesh();
}

CCTextAlignment CC3BitmapLabelNode::getTextAlignment()
{
	return m_textAlignment; 
}

void CC3BitmapLabelNode::setTextAlignment( CCTextAlignment alignment )
{
	if (alignment == m_textAlignment) 
		return;

	m_textAlignment = alignment;
	populateLabelMesh();
}

CCPoint CC3BitmapLabelNode::getRelativeOrigin()
{
	return m_relativeOrigin; 
}

void CC3BitmapLabelNode::setRelativeOrigin( const CCPoint& relOrigin )
{
	if ( relOrigin.equals(m_relativeOrigin) ) 
		return;
	
	m_relativeOrigin = relOrigin;
	populateLabelMesh();
}

CC3Tessellation CC3BitmapLabelNode::getTessellation()
{
	return m_tessellation; 
}

void CC3BitmapLabelNode::setTessellation( const CC3Tessellation& aGrid )
{
	if ( (aGrid.x == m_tessellation.x) && (aGrid.y == m_tessellation.y) )
		return;

	m_tessellation = aGrid;
	populateLabelMesh();
}

GLfloat CC3BitmapLabelNode::getFontSize()
{
	return m_fontConfig ? m_fontConfig->getFontSize() : 0; 
}

GLfloat CC3BitmapLabelNode::getBaseline()
{
	if ( !m_fontConfig ) 
		return 0.0f;

	return 1.0f - (GLfloat)m_fontConfig->getBaseline() / (GLfloat)m_fontConfig->getCommonHeight();
}

void CC3BitmapLabelNode::populateLabelMesh()
{
	if ( !(!m_fontFileName.empty() && !m_labelString.empty()) ) 
		return;

	// If using GL buffers, delete them now, because the population mechanism triggers updates
	// to existing buffers with the new vertex count (which will not match the buffers).
	bool isUsingGLBuffers = false;
	if ( m_pMesh )
	{
		m_pMesh->isUsingGLBuffers();
		m_pMesh->deleteGLBuffers();
	}
	
	populateAsBitmapFontLabelFromString( getLabelString(), getFontFileName(), getLineHeight(), 
		getTextAlignment(), getRelativeOrigin(), getTessellation() );
	
	// If using GL buffers, recreate them now.
	if (isUsingGLBuffers && m_pMesh) 
		m_pMesh->createGLBuffers();

	markBoundingVolumeDirty();
}

void CC3BitmapLabelNode::initWithTag( GLuint aTag, const std::string& aName )
{
	super::initWithTag( aTag, aName );
	setLabelString( "hello, world" );		// Fail-safe to display if nothing set
	m_fontFileName = "";
	m_fontConfig = NULL;
	m_lineHeight = 0;
	m_textAlignment = kCCTextAlignmentLeft;
	m_relativeOrigin = ccp(0,0);
	m_tessellation = CC3TessellationMake(1,1);
}

void CC3BitmapLabelNode::populateFrom( CC3BitmapLabelNode* another )
{
	super::populateFrom( another );

	m_relativeOrigin = another->getRelativeOrigin();
	m_textAlignment = another->getTextAlignment();
	m_tessellation = another->getTessellation();
	m_lineHeight = another->getLineHeight();
	setFontFileName( another->getFontFileName() );
	setLabelString( another->getLabelString() );		// Will trigger repopulation
}

CCObject* CC3BitmapLabelNode::copyWithZone( CCZone* zone )
{
	CC3BitmapLabelNode* pVal = new CC3BitmapLabelNode;
	pVal->init();
	pVal->populateFrom( this );
	pVal->addCopiesOfChildrenFrom( this );

	return pVal;
}

std::string CC3BitmapLabelNode::description()
{
	return CC3String::stringWithFormat( (char*)"%s '%s'", super::description().c_str(), getLabelString().c_str() );
}

typedef struct {
	GLfloat lineWidth;
	GLuint lastVertexIndex;
} CC3BMLineSpec;

/** CC3MeshNode extension to support bitmapped labels. */
//@implementation CC3Mesh (BitmapLabel)

void populateMeshAsBitmapFontLabelFromString( CC3Mesh* mesh, const std::string& lblString, CC3BitmapFontConfiguration* fontConfig, GLfloat lineHeight, 
	CCTextAlignment textAlignment, const CCPoint& origin, const CC3Tessellation& divsPerChar )
{	
	if ( mesh == NULL )
		return;

	CCPoint charPos, adjCharPos;
	CCSize layoutSize;
	int kerningAmount;
	unsigned char prevChar = -1;
	unsigned int strLen = lblString.length();
	
	if (lineHeight == 0.0f) 
		lineHeight = (float)fontConfig->getCommonHeight();

	GLfloat  fontScale = lineHeight / (GLfloat)fontConfig->getCommonHeight();
	
	// Line count needs to be calculated before parsing the lines to get Y position
	GLuint charCount = 0;
	GLuint lineCount = 1;
	for(unsigned int i = 0; i < strLen; i++)
		( lblString[i] == '\n' ) ? lineCount++ : charCount++;
	
	// Create a local array to hold the dimensional characteristics of each line of text
	CC3BMLineSpec* lineSpecs = new CC3BMLineSpec[lineCount];
	
	// We now know the height of the layout. Width will be determined as the lines are laid out.
	layoutSize.width =  0;
	layoutSize.height = lineHeight * lineCount;
	
	// Prepare the vertex content and allocate space for the vertices and indexes.
	mesh->ensureVertexContent();
	GLuint vtxCountPerChar = (divsPerChar.x + 1) * (divsPerChar.y + 1);
	GLuint triCountPerChar = divsPerChar.x * divsPerChar.y * 2;
	mesh->setAllocatedVertexCapacity( vtxCountPerChar * charCount );
	mesh->setAllocatedVertexIndexCapacity( triCountPerChar * 3 * charCount );
	
	CC3_TRACE( "[msh]Creating label mesh with %d (%d) vertices and %d (%d) vertex indices from %d chars on %d lines in text %s",
			 mesh->getVertexCount(), mesh->getAllocatedVertexCapacity(),
			 mesh->getVertexIndexCount(), mesh->getAllocatedVertexIndexCapacity(), charCount, lineCount, lblString.c_str() );
	
	// Start at the top-left corner of the label, above the first line.
	// Place the first character at the left of the first line.
	charPos.x = 0;
	charPos.y = lineCount * lineHeight;
	
	GLuint lineIndx = 0;
	GLuint vIdx = 0;
	GLuint iIdx = 0;
	
	// Iterate through the characters
	for (unsigned int i = 0; i < strLen; i++) 
	{
		unsigned char c = lblString[i];
		
		// If the character is a newline, don't draw anything and move down a line
		if (c == '\n') 
		{
			lineIndx++;
			charPos.x = 0;
			charPos.y -= lineHeight;
			continue;
		}
		
		// Get the font specification and for the character, the kerning between the previous
		// character and this character, and determine a positioning adjustment for the character.
		CC3BitmapCharDef* charSpec = fontConfig->getCharacterSpecFor( c );
		CCAssert(charSpec, "no font specification loaded for character");
		
		kerningAmount = (int)(fontConfig->getKerningBetween( prevChar, c ) * fontScale);
		adjCharPos.x = charPos.x + (charSpec->xOffset * fontScale) + kerningAmount;
		adjCharPos.y = charPos.y - (charSpec->yOffset * fontScale);
		
		// Determine the size of each tesselation division for this character.
		// This is specified in terms of the unscaled font config. It will be scaled later.
		CCSize divSize = CCSizeMake(charSpec->rect.size.width / divsPerChar.x,
									charSpec->rect.size.height / divsPerChar.y);
		
		// Initialize the current line spec
		lineSpecs[lineIndx].lastVertexIndex = 0;
		lineSpecs[lineIndx].lineWidth = 0.0f;
		
		// Populate the tesselated vertex locations, normals & texture coordinates for a single
		// character. Iterate through the rows and columns of the tesselation grid, from the top-left
		// corner downwards. This orientation aligns with the texture coords in the font file.
		// Set the location of each vertex and tex coords to be proportional to its position in the
		// grid, and set the normal of each vertex to point up the Z-axis.
		for (GLint iy = 0; iy <= divsPerChar.y; iy++) 
		{
			for (GLint ix = 0; ix <= divsPerChar.x; ix++, vIdx++) 
			{
				// Cache the index of the last vertex of this line. Since the vertices are accessed
				// in consecutive, ascending order, this is done by simply setting it each time.
				lineSpecs[lineIndx].lastVertexIndex = vIdx;
				
				// Vertex location
				GLfloat vx = adjCharPos.x + (divSize.width * ix * fontScale);
				GLfloat vy = adjCharPos.y - (divSize.height * iy * fontScale);
				mesh->setVertexLocation( cc3v(vx, vy, 0.0), vIdx );
				
				// If needed, expand the line and layout width to account for the vertices
				lineSpecs[lineIndx].lineWidth = MAX(lineSpecs[lineIndx].lineWidth, vx);
				layoutSize.width = MAX(layoutSize.width, vx);
				
				// Vertex normal. Will do nothing if this mesh does not include normals.
				mesh->setVertexNormal( CC3Vector::kCC3VectorUnitZPositive, vIdx );
				
				// Vertex texture coordinates, inverted vertically, because we're working top-down.
				CCSize texSize = fontConfig->getTextureSize();
				GLfloat u = (charSpec->rect.origin.x + (divSize.width * ix)) / texSize.width;
				GLfloat v = (charSpec->rect.origin.y + (divSize.height * iy)) / texSize.height;
				mesh->setVertexTexCoord2F( cc3tc(u, v), vIdx );
				
				// In the grid of division quads for each character, each vertex that is not
				// in either the top-most row or the right-most column is the bottom-left corner
				// of a division. Break the division into two triangles.
				if (iy < divsPerChar.y && ix < divsPerChar.x) 
				{	
					// First triangle of face wound counter-clockwise
					mesh->setVertexIndex( vIdx, iIdx++ );						// TL
					mesh->setVertexIndex( (vIdx + divsPerChar.x + 1), iIdx++ );	// BL
					mesh->setVertexIndex( (vIdx + divsPerChar.x + 2), iIdx++ );	// BR
					
					// Second triangle of face wound counter-clockwise
					mesh->setVertexIndex( (vIdx + divsPerChar.x + 2), iIdx++ );	// BR
					mesh->setVertexIndex( (vIdx + 1), iIdx++ );					// TR
					mesh->setVertexIndex( vIdx, iIdx++ );						// TL
				}
			}
		}
		
		// Horizontal position of the next character
		charPos.x += (charSpec->xAdvance * fontScale) + kerningAmount;
		
		prevChar = c;	// Remember the current character before moving on to the next
	}

	// Iterate through the lines, calculating the width adjustment to correctly align each line,
	// and applying that adjustment to the X-component of the location of each vertex that is
	// contained within that text line.
	for (GLuint i = 0; i < lineCount; i++) 
	{
		GLfloat widthAdj;
		switch (textAlignment) 
		{
			case kCCTextAlignmentCenter:
				// Adjust vertices so half the white space is on each side
				widthAdj = (layoutSize.width - lineSpecs[i].lineWidth) * 0.5f;
				break;
			case kCCTextAlignmentRight:
				// Adjust vertices so all the white space is on the left side
				widthAdj = layoutSize.width - lineSpecs[i].lineWidth;
				break;
			case kCCTextAlignmentLeft:
			default:
				// Leave all vertices where they are
				widthAdj = 0.0f;
				break;
		}
		if ( widthAdj ) 
		{
			GLuint startVtxIdx = (i > 0) ? (lineSpecs[i - 1].lastVertexIndex + 1) : 0;
			GLuint endVtxIdx = lineSpecs[i].lastVertexIndex;
			CC3_TRACE("[msh]Mesh adjusting line %d by %.3f (from line width %.3f in layout width %.3f) from vertex %d to %d",
					 i, widthAdj, lineSpecs[i].lineWidth, layoutSize.width, startVtxIdx, endVtxIdx);
			for ( vIdx = startVtxIdx; vIdx <= endVtxIdx; vIdx++ ) 
			{
				CC3Vector vtxLoc = mesh->getVertexLocationAt( vIdx );
				vtxLoc.x += widthAdj;
				mesh->setVertexLocation( vtxLoc, vIdx );
			}
		}
	}
	
	// Move all vertices so that the origin of the vertex coordinate system is aligned
	// with a location derived from the origin factor.
	GLuint vtxCnt = mesh->getVertexCount();
	CC3Vector originLoc = cc3v((layoutSize.width * origin.x), (layoutSize.height * origin.y), 0);
	for ( vIdx = 0; vIdx < vtxCnt; vIdx++ ) 
	{
		CC3Vector locOld = mesh->getVertexLocationAt( vIdx );
		CC3Vector locNew = locOld - originLoc;
		mesh->setVertexLocation( locNew, vIdx );
	}

	CC_SAFE_DELETE_ARRAY( lineSpecs );
}

NS_COCOS3D_END
