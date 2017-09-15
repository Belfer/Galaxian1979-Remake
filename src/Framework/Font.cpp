//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	07/07/2015
//	Brief:	Basic Font class for drawing screen-space text
//////////////////////////////////////////////////////////////////////////
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tinyxml.h>

#include "Font.h"
#include "Utilities.h"
#include "tinyxmlHelper.h"

using namespace NHTV;
//////////////////////////////////////////////////////////////////////////
Font::Font(){}
//////////////////////////////////////////////////////////////////////////
Font::Font(const char* pszFontName) : m_pCharacters(nullptr), m_KerningTable(nullptr)
{
	m_pFontName = nullptr;
	if (pszFontName != NULL)
	{
		int len = (int)strlen(pszFontName);
		m_pFontName = new char[len + 1];
		strcpy(m_pFontName, pszFontName);
		if (!LoadFont(m_pFontName))
		{
		}
	}
}
//////////////////////////////////////////////////////////////////////////
bool Font::LoadFont(const char* pszFontName)
{
	//Test to see if the FontFile Exists first
	FILE* fh = fopen(pszFontName, "rb");
	if (fh != NULL)
	{
		TiXmlDocument doc(pszFontName);
		doc.LoadFile();

		TiXmlNode *xmlNodeFont = doc.FirstChild("font");
		if (!xmlNodeFont)
		{
			printf("Could not find 'font' node in XML!\n");
			return NULL;
		}

		TiXmlNode *xmlNodeInfo = xmlNodeFont->FirstChild("info");
		if (!xmlNodeInfo)
		{
			printf("Could not find 'info' node in XML!\n");
			return NULL;
		}
		///////////////////////////////////////////////////////////////////////////
		//Load in font elements form the xml file associated with this font
		// - the xml contains all the details that we will need for this font
		// - including spacing/kernings
		// - uv coordinates
		// - scale, itallics, bold etc
		TiXmlElement *xmlElementInfo = xmlNodeInfo->ToElement();
		if (false == GetAttributeString(xmlElementInfo, "face", m_FaceName))				{ return false; }
		if (false == GetAttributeInt<int>(xmlElementInfo, "size", m_Size))					{ return false; }
		if (false == GetAttributeInt<unsigned int>(xmlElementInfo, "bold", m_IsBold))		{ return false; }
		if (false == GetAttributeInt<unsigned int>(xmlElementInfo, "italic", m_IsItalic))	{ return false; }
		if (false == GetAttributeInt<unsigned int>(xmlElementInfo, "unicode", m_IsUnicode)) { return false; }
		if (false == GetAttributeInt<int>(xmlElementInfo, "stretchH", m_StretchH))			{ return false; }
		if (false == GetAttributeInt<unsigned int>(xmlElementInfo, "smooth", m_IsSmooth))	{ return false; }
		if (false == GetAttributeInt<unsigned int>(xmlElementInfo, "aa", m_IsAa))			{ return false; }
		if (false == GetAttributeIntArray4(xmlElementInfo, "padding", m_Padding))			{ return true; }
		if (false == GetAttributeIntArray2(xmlElementInfo, "spacing", m_Spacing))			{ return true; }
		if (false == GetAttributeInt<int>(xmlElementInfo, "outline", m_Outline))			{ return false; }
		
		TiXmlNode *xmlNodeCommon = xmlNodeFont->FirstChild("common");
		if (!xmlNodeCommon)
		{
			printf("Could not find 'common' node in XML!\n");
			return false;
		}

		TiXmlElement *xmlElementCommon = xmlNodeCommon->ToElement();
		if (false == GetAttributeInt(xmlElementCommon, "lineHeight", m_LineHeight))			{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "base", m_Base))						{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "scaleW", m_ScaleW))					{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "scaleH", m_ScaleH))					{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "pages", m_Pages))					{return false;}

		if (m_Pages > 1)
		{
			printf("Only supporting fonts made from one page for performance reasons!\n");
			return false;
		}

		if (false == GetAttributeInt(xmlElementCommon, "packed", m_Packed))					{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "alphaChnl", m_AlphaChannel))		{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "redChnl", m_RedChannel))			{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "greenChnl", m_GreenChannel))		{return false;}
		if (false == GetAttributeInt(xmlElementCommon, "blueChnl", m_BlueChannel))			{return false;}
		///////////////////////////////////////////////////////////////////////////
		//Pages refers to the number of sprite sheets that this font is spread over
		// for intents there is only one sprite sheet 
		//////////////////////////////////////////////////////////////////////////
		TiXmlNode *xmlNodePages = xmlNodeFont->FirstChild("pages");
		if (!xmlNodePages)
		{
			printf("Could not find 'pages' node in XML!\n");
			return false;
		}

		TiXmlNode *xmlNodePage = xmlNodePages->FirstChild("page");
		if (!xmlNodePage)
		{
			printf("Could not find 'page' node in XML!\n");
			return false;
		}

		TiXmlElement *xmlElementPage = xmlNodePage->ToElement();
		char temp[256];
		if (false == GetAttributeString(xmlElementPage, "file", temp))						{ return false; }
        StrHandler::SPrintf(m_PageFilename, 256, "../resources/fonts/%s", temp);
		
		TiXmlNode *xmlNodeChars = xmlNodeFont->FirstChild("chars");
		if (!xmlNodeChars)
		{
			printf("Could not find 'chars' node in XML!\n");
			return false;
		}

		TiXmlElement *xmlElementChars = xmlNodeChars->ToElement();
		if (false == GetAttributeInt(xmlElementChars, "count", m_iNumCharacters))			{ return false; }
		m_pCharacters = (Character*)malloc(sizeof(Character) * m_iNumCharacters);
		///////////////////////////////////////////////////////////////////////////
		// Get each characters attributes and uv coordinates in the associated 
		// sprite for the font
		///////////////////////////////////////////////////////////////////////////
		TiXmlNode *xmlNodeChar = xmlNodeChars->FirstChild("char");
		if (!xmlNodeChar)
		{
			printf("Didn't find the specified number of characters in file!\n");
			return false;
		}
		for (unsigned int i = 0; i < m_iNumCharacters; ++i)
		{
			TiXmlElement *element = xmlNodeChar->ToElement();
			if (false == GetAttributeInt(element, "id", m_pCharacters[i].id))				{return false;}
			if (false == GetAttributeInt(element, "x", m_pCharacters[i].x))					{return false;}
			if (false == GetAttributeInt(element, "y", m_pCharacters[i].y))					{return false;}
			if (false == GetAttributeInt(element, "width", m_pCharacters[i].width))			{return false;}
			if (false == GetAttributeInt(element, "height", m_pCharacters[i].height))		{return false;}
			if (false == GetAttributeInt(element, "xoffset", m_pCharacters[i].xoffset))		{return false;}
			if (false == GetAttributeInt(element, "yoffset", m_pCharacters[i].yoffset))		{return false;}
			if (false == GetAttributeInt(element, "xadvance", m_pCharacters[i].xadvance))	{return false;}
			if (false == GetAttributeInt(element, "page", m_pCharacters[i].page))			{return false;}
			if (false == GetAttributeInt(element, "chnl", m_pCharacters[i].channel))		{return false;}

			xmlNodeChar = xmlNodeChar->NextSibling("char");
			if (!xmlNodeChar && i != m_iNumCharacters - 1)
			{
				printf("Didn't find the specified number of characters in file!\n");
				return false;
			}
		}
		/////////////////////////////////////////////////////////////////////////// 
		// Sort out mapping table.
		for (unsigned int i = 0; i < 256; ++i)
		{
			m_MappingTable[i] = -1;
		}
		for (unsigned int i = 0; i < m_iNumCharacters; ++i)
		{
			m_MappingTable[m_pCharacters[i].id] = i;
		}
		///////////////////////////////////////////////////////////////////////////    
		// Do we have kerning information?
		// Kerning refers to how fonts are spaced when they are displayed such as 
		// having characters next under a capital T or space closer to a l than a w
		///////////////////////////////////////////////////////////////////////////
		m_HasKerningInformation = false;
		TiXmlNode *xmlNodeKernings = xmlNodeFont->FirstChild("kernings");
		if (xmlNodeKernings)
		{
			// If kerning information was found...
			TiXmlNode *xmlNodeKerningFirst = xmlNodeKernings->FirstChild("kerning");
			TiXmlNode *xmlNodeKerningLast = xmlNodeKernings->LastChild("kerning");
			if (xmlNodeKerningFirst && xmlNodeKerningLast)
			{
				m_HasKerningInformation = true;
				TiXmlElement *xmlElementKerningFirst = xmlNodeKerningFirst->ToElement();
				TiXmlElement *xmlElementKerningLast = xmlNodeKerningLast->ToElement();
				if (false == GetAttributeInt(xmlElementKerningFirst, "first", m_KerningFirst))	{return false;}
				if (false == GetAttributeInt(xmlElementKerningLast, "first", m_KerningLast))	{return false;}
				// Allocate pointers for kerning information.
				int iSizeOfKerningTable = (m_KerningLast - m_KerningFirst + 1);
				m_KerningTable = (int**)malloc(iSizeOfKerningTable * sizeof(int*));
				// Allocate the memory for each entry...
				for (int i = 0; i < iSizeOfKerningTable; ++i)
				{
					m_KerningTable[i] = (int*)malloc(sizeof(int) * 256);
					// Fill in 0 for now.
					for (int j = 0; j < 256; ++j)
					{
						m_KerningTable[i][j] = 0;
					}
				}
				// Now fill in valid entires from XML.
				for (TiXmlNode *xmlNodeKerningCurrent = xmlNodeKerningFirst;
				xmlNodeKerningCurrent != NULL;
				xmlNodeKerningCurrent = xmlNodeKernings->IterateChildren(xmlNodeKerningCurrent))
				{
					int first, second, amount;
					TiXmlElement *element = xmlNodeKerningCurrent->ToElement();
					if (false == GetAttributeInt(element, "first", first))					{return false;}
					if (false == GetAttributeInt(element, "second", second))				{return false;}
					if (false == GetAttributeInt(element, "amount", amount))				{return false;}
					m_KerningTable[first - m_KerningFirst][second] = amount;
				}
			}
		}
		// Load graphic.
		unsigned int iFontTexture = LoadTexture(m_PageFilename);
		if (iFontTexture <= 0) { return false; }
		
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
Font::~Font()
{
	if (m_pFontName != nullptr)
	{
		delete[] m_pFontName;
	}
	if (m_pCharacters != nullptr)
	{
		free(m_pCharacters);
		m_pCharacters = nullptr;
	}

	if (m_KerningTable != nullptr)
	{
		int iSizeOfKerningTable = (m_KerningLast - m_KerningFirst + 1);
		for (int i = 0; i < iSizeOfKerningTable; ++i)
		{
			free(m_KerningTable[i]);
		}
		free(m_KerningTable);
		m_KerningTable = nullptr;
	}
}
//////////////////////////////////////////////////////////////////////////
unsigned int Font::LoadTexture(const char* pszTextureName)
{

	int width = 0, height = 0, bpp = 0;
    m_uiTextureID = NHTV::Utility::loadTexture(pszTextureName, width, height, bpp);
	m_TexScale = width / (float)m_ScaleW;
	m_InvTexScale = 1.0f / m_TexScale;

	return m_uiTextureID;
}
//////////////////////////////////////////////////////////////////////////
unsigned int Font::DrawString(glm::vec4 v4Position, float a_fSize, glm::vec4 v4colour, const char* pBuffer, CharacterBillboard* pVertexDisplayArray)
{
	//Then push this geometry to the render queue
	int prevC = -1;
	char c = -1;
	//////////////////////////////////////////////////////////////////////////
	//\ Set up the memory for our vertex and index buffers
	//////////////////////////////////////////////////////////////////////////
	unsigned int iSize = StrHandler::Length(pBuffer);
	//////////////////////////////////////////////////////////////////////////
	float fWScale = a_fSize;
	float fHScale = a_fSize;
	glm::vec4 vRight = glm::vec4(1.f, 0.f, 0.f, 0.f);
	glm::vec4 vHeight = glm::vec4(0.f, 1.f, 0.f, 0.f);
	float fYOrigin = v4Position.y;
	for (unsigned int i = 0; i < iSize; ++i)
	{
		prevC = c;
		c = pBuffer[i];

		int charNum = m_MappingTable[c];
		// If character wasn't found....
		if (charNum == -1)
		{
			// Just use space.
			charNum = m_MappingTable[' '];
		}
		if (charNum == -1)
		{
			// If space not found!
			continue;
		}
		// Get this characters information.
		Character *pCharacter = &m_pCharacters[charNum];
		v4Position.x = (float)(v4Position.x + ((float)pCharacter->xoffset * fWScale));
		v4Position.y = fYOrigin;
		// If we have kerning information from previous character to this character, then apply it.
		if (m_HasKerningInformation && prevC != -1 && prevC >= m_KerningFirst && prevC <= m_KerningLast)
		{
			v4Position.x += ((float)m_KerningTable[prevC - m_KerningFirst][c] * fWScale);
		}
		v4Position.y = (float)(v4Position.y - ((float)pCharacter->yoffset) * fHScale);

		//Generate coordinates for this Character
		float fWidthWithScale = pCharacter->width * fWScale;
		float fHeightWithScale = pCharacter->height * fHScale;
		pVertexDisplayArray[i].v0.position = v4Position - (vHeight *  fHeightWithScale);// + (vHeight * -fHeightWithScale);
		pVertexDisplayArray[i].v1.position = v4Position - (vHeight *  fHeightWithScale) + (vRight *  fWidthWithScale);
		pVertexDisplayArray[i].v2.position = v4Position;// +  vHeight * -fHeightWithScale;
		pVertexDisplayArray[i].v3.position = v4Position + (vRight *  fWidthWithScale);

		// Colour
		pVertexDisplayArray[i].v0.colour = v4colour;
		pVertexDisplayArray[i].v1.colour = v4colour;
		pVertexDisplayArray[i].v2.colour = v4colour;
		pVertexDisplayArray[i].v3.colour = v4colour;

		//UV Co-ordinates
		pVertexDisplayArray[i].v0.texCoord = glm::vec2((float)pCharacter->x / m_ScaleW, (float)((m_ScaleH - pCharacter->y) - pCharacter->height) / m_ScaleH);
		pVertexDisplayArray[i].v1.texCoord = glm::vec2((float)(pCharacter->x + pCharacter->width) / m_ScaleW, (float)((m_ScaleH - pCharacter->y) - pCharacter->height) / m_ScaleH);
		pVertexDisplayArray[i].v2.texCoord = glm::vec2((float)pCharacter->x / m_ScaleW, (float)(m_ScaleH - pCharacter->y) / m_ScaleH);
		pVertexDisplayArray[i].v3.texCoord = glm::vec2((float)(pCharacter->x + pCharacter->width) / m_ScaleW, (float)(m_ScaleH - pCharacter->y) / m_ScaleH);

		v4Position.x = (v4Position.x + ((float)pCharacter->xadvance * fWScale));
	}
	return iSize;
}

