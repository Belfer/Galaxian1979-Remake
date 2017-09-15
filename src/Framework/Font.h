//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	07/07/2015
//	Brief:	Basic Font class for drawing screen-space text
//////////////////////////////////////////////////////////////////////////
#ifndef _FONT_H_
#define _FONT_H_
//////////////////////////////////////////////////////////////////////////
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Utilities.h"

namespace NHTV
{
	struct CharacterBillboard
	{
		Vertex v0;
		Vertex v1;
		Vertex v2;
		Vertex v3;
	};
	//////////////////////////////////////////////////////////////////////////
	class Font
	{
	public:

		//////////////////////////////////////////////////////////////////////////
		// Character struct used to hold information for each character represented
		// in the font being used
		typedef struct Character_s
		{
			int	id;
			int	x;
			int	y;
			int	width;
			int	height;
			int	xoffset;
			int	yoffset;
			int	xadvance;
			int	page;
			int	channel;
		} Character;
		///////////////////////////////////////////////////////////////////////////
		//Constructor/Destruction
		Font();
		Font(const char* pszFontName);
		~Font();
		///////////////////////////////////////////////////////////////////////////
		unsigned int LoadTexture(const char* pszTextureName);

		unsigned int DrawString(glm::vec4 v4Position, float a_fSize, glm::vec4 v4colour, const char* pBuffer, CharacterBillboard* pVertexDisplayArray);
		unsigned int GetTextureID() { return m_uiTextureID; }
		const char*		GetFontName() { return m_pFontName; }

	private:

		bool LoadFont(const char* pszFontName);
		char*			m_pFontName;

		unsigned int    m_uiTextureID;
		unsigned int    m_iNumCharacters;
		Character*      m_pCharacters;
		// Scale values to account for any difference between texture size and stored font kerning
		float			m_TexScale;
		float			m_InvTexScale;
		//Font detalis
		char			m_FaceName[256];
		int				m_Size;
		unsigned int    m_IsBold;
		unsigned int    m_IsItalic;
		unsigned int    m_IsUnicode;
		unsigned int    m_IsSmooth;
		unsigned int    m_IsAa;
		int				m_Outline;
		int				m_StretchH;
		int				m_Padding[4];
		int				m_Spacing[2];
		// Common.
		int			m_LineHeight;
		int			m_Base;
		int			m_ScaleW;
		int			m_ScaleH;
		int			m_Pages;
		int			m_Packed;
		int			m_AlphaChannel;
		int			m_RedChannel;
		int			m_GreenChannel;
		int			m_BlueChannel;
		// Pages
		char		m_PageFilename[256];
		// Character mapping table
		int			m_MappingTable[256];
		// Kerning table.
		bool		m_HasKerningInformation;
		int			**m_KerningTable;
		int			m_KerningFirst;
		int			m_KerningLast;
	};

}//Namespace NHTV

 //////////////////////////////////////////////////////////////////////////
#endif //_FONT_H_
 //////////////////////////////////////////////////////////////////////////
