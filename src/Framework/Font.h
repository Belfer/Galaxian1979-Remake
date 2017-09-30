//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	07/07/2015
//	Brief:	Basic Font class for drawing screen-space text
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "Utilities.h"
#include <array>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace NHTV {
struct CharacterBillboard {
  Vertex v0;
  Vertex v1;
  Vertex v2;
  Vertex v3;
};
//////////////////////////////////////////////////////////////////////////
class Font {
public:
  //////////////////////////////////////////////////////////////////////////
  // Character struct used to hold information for each character represented
  // in the font being used
  typedef struct Character_s {
    int id;
    int x;
    int y;
    int width;
    int height;
    int xoffset;
    int yoffset;
    int xadvance;
    int page;
    int channel;
  } Character;
  ///////////////////////////////////////////////////////////////////////////
  // Constructor/Destruction
  Font();
  Font(const char *pszFontName);
  ~Font();
  ///////////////////////////////////////////////////////////////////////////
  uint LoadTexture(const char *pszTextureName);

  uint DrawString(glm::vec4 v4Position, float a_fSize, glm::vec4 v4colour,
                  const std::string &buffer,
                  CharacterBillboard *pVertexDisplayArray);
  uint GetTextureID() { return m_uiTextureID; }
  const std::string &GetFontName() { return m_pFontName; }

private:
  bool LoadFont(const char *pszFontName);
  std::string m_pFontName;

  uint m_uiTextureID;
  uint m_iNumCharacters;
  std::vector<Character> m_Characters;
  // Scale values to account for any difference between texture size and stored
  // font kerning
  float m_TexScale;
  float m_InvTexScale;
  // Font detalis
  std::string m_FaceName;
  int m_Size;
  uint m_IsBold;
  uint m_IsItalic;
  uint m_IsUnicode;
  uint m_IsSmooth;
  uint m_IsAa;
  int m_Outline;
  int m_StretchH;
  int m_Padding[4];
  int m_Spacing[2];
  // Common.
  int m_LineHeight;
  int m_Base;
  int m_ScaleW;
  int m_ScaleH;
  int m_Pages;
  int m_Packed;
  int m_AlphaChannel;
  int m_RedChannel;
  int m_GreenChannel;
  int m_BlueChannel;
  // Pages
  std::string m_PageFilename;
  // Character mapping table
  int m_MappingTable[255];
  // Kerning table.
  bool m_HasKerningInformation;
  std::vector<std::array<int, 256>> m_KerningTable;
  int m_KerningFirst;
  int m_KerningLast;
};

} // Namespace NHTV
