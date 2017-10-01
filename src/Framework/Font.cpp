//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	07/07/2015
//	Brief:	Basic Font class for drawing screen-space text
//////////////////////////////////////////////////////////////////////////
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <fstream>
#include <json.hpp>

#include "Font.h"
#include "Utilities.h"

using json = nlohmann::json;

using namespace NHTV;
//////////////////////////////////////////////////////////////////////////
Font::Font() {}
//////////////////////////////////////////////////////////////////////////
Font::Font(const char *pszFontName) { LoadFont(pszFontName); }
//////////////////////////////////////////////////////////////////////////
bool Font::LoadFont(const char *pszFontName) {
  m_pFontName = pszFontName;

  std::ifstream ifs(pszFontName, std::ios::in);
  std::string res = "";

  if (!ifs.is_open()) {
    std::cerr << "Font file does not exist!\n";
    return false;
  } else {
    std::string line = "";
    while (!ifs.eof()) {
      std::getline(ifs, line);
      res.append(line + "\n");
    }
  }

  auto font = json::parse(res)["font"];

  // Font detalis
  auto info = font["info"];
  m_FaceName = info["face"];
  m_Size = info["size"];
  m_IsBold = info["bold"];
  m_IsItalic = info["italic"];
  m_IsUnicode = info["unicode"];
  m_StretchH = info["stretchH"];
  m_IsSmooth = info["smooth"];
  m_IsAa = info["aa"];

  uint tmp = 0;
  for (auto &e : info["padding"]) {
    m_Padding[tmp++] = e;
  }

  tmp = 0;
  for (auto &e : info["spacing"]) {
    m_Spacing[tmp++] = e;
  }

  m_Outline = info["outline"];

  // Common.
  auto common = font["common"];
  m_LineHeight = common["lineHeight"];
  m_Base = common["base"];
  m_ScaleW = common["scaleW"];
  m_ScaleH = common["scaleH"];
  m_Pages = common["pages"];
  m_Packed = common["packed"];
  m_AlphaChannel = common["alphaChnl"];
  m_RedChannel = common["redChnl"];
  m_GreenChannel = common["greenChnl"];
  m_BlueChannel = common["blueChnl"];

  // Pages
  auto pages = font["pages"];
  m_PageFilename = pages["file"];

  // Character mapping table
  auto chars = font["chars"];
  size_t charCount = chars["count"];
  m_Characters.reserve(charCount);
  for (auto &e : chars["char"]) {
    Character c;
    c.id = e["id"];
    c.x = e["x"];
    c.y = e["y"];
    c.width = e["width"];
    c.height = e["height"];
    c.xoffset = e["xoffset"];
    c.yoffset = e["yoffset"];
    c.xadvance = e["xadvance"];
    c.page = e["page"];
    c.channel = e["chnl"];
    m_Characters.emplace_back(c);
  }

  for (uint i = 0; i < 256; ++i)
    m_MappingTable[i] = -1;
  for (uint i = 0; i < charCount; ++i)
    m_MappingTable[m_Characters[i].id] = i;

  // Kerning table.
  auto kernings = font["kernings"];
  if (!kernings.is_null()) {
    m_HasKerningInformation = true;
    size_t kerningsCount = kernings["count"];
    m_HasKerningInformation = true;
    m_KerningTable.reserve(kerningsCount);

    // m_KerningTable;
    // m_KerningFirst;
    // m_KerningLast;

    for (auto &e : kernings["kerning"]) {
      int first, second, amount;
      // m_KerningTable[first - m_KerningFirst][second] = amount;
    }
  }

  // Load graphic.
  uint iFontTexture = LoadTexture(m_PageFilename.c_str());
  if (iFontTexture <= 0) {
    return false;
  }

  return true;
}
//////////////////////////////////////////////////////////////////////////
Font::~Font() {
  m_Characters.clear();
  m_KerningTable.clear();
}
//////////////////////////////////////////////////////////////////////////
uint Font::LoadTexture(const char *pszTextureName) {

  int width = 0, height = 0, bpp = 0;
  m_uiTextureID =
      NHTV::Utility::loadTexture(pszTextureName, width, height, bpp);
  m_TexScale = width / (float)m_ScaleW;
  m_InvTexScale = 1.0f / m_TexScale;

  return m_uiTextureID;
}
//////////////////////////////////////////////////////////////////////////
uint Font::DrawString(glm::vec4 v4Position, float a_fSize, glm::vec4 v4colour,
                      const std::string &buffer,
                      CharacterBillboard *pVertexDisplayArray) {
  // Then push this geometry to the render queue
  int prevC = -1;
  char c = -1;
  //////////////////////////////////////////////////////////////////////////
  //\ Set up the memory for our vertex and index buffers
  //////////////////////////////////////////////////////////////////////////
  uint iSize = buffer.size();
  //////////////////////////////////////////////////////////////////////////
  float fWScale = a_fSize;
  float fHScale = a_fSize;
  glm::vec4 vRight = glm::vec4(1.f, 0.f, 0.f, 0.f);
  glm::vec4 vHeight = glm::vec4(0.f, 1.f, 0.f, 0.f);
  float fYOrigin = v4Position.y;
  for (uint i = 0; i < iSize; ++i) {
    prevC = c;
    c = buffer[i];

    int charNum = m_MappingTable[c];
    // If character wasn't found....
    if (charNum == -1) {
      // Just use space.
      charNum = m_MappingTable[' '];
    }
    if (charNum == -1) {
      // If space not found!
      continue;
    }
    // Get this characters information.
    const Character &charRef = m_Characters[charNum];
    v4Position.x = (float)(v4Position.x + ((float)charRef.xoffset * fWScale));
    v4Position.y = fYOrigin;
    // If we have kerning information from previous character to this character,
    // then apply it.
    if (m_HasKerningInformation && prevC != -1 && prevC >= m_KerningFirst &&
        prevC <= m_KerningLast) {
      v4Position.x +=
          ((float)m_KerningTable[prevC - m_KerningFirst][c] * fWScale);
    }
    v4Position.y = (float)(v4Position.y - ((float)charRef.yoffset) * fHScale);

    // Generate coordinates for this Character
    float fWidthWithScale = charRef.width * fWScale;
    float fHeightWithScale = charRef.height * fHScale;
    pVertexDisplayArray[i].v0.position =
        v4Position -
        (vHeight * fHeightWithScale); // + (vHeight * -fHeightWithScale);
    pVertexDisplayArray[i].v1.position =
        v4Position - (vHeight * fHeightWithScale) + (vRight * fWidthWithScale);
    pVertexDisplayArray[i].v2.position =
        v4Position; // +  vHeight * -fHeightWithScale;
    pVertexDisplayArray[i].v3.position =
        v4Position + (vRight * fWidthWithScale);

    // Colour
    pVertexDisplayArray[i].v0.colour = v4colour;
    pVertexDisplayArray[i].v1.colour = v4colour;
    pVertexDisplayArray[i].v2.colour = v4colour;
    pVertexDisplayArray[i].v3.colour = v4colour;

    // UV Co-ordinates
    pVertexDisplayArray[i].v0.texCoord =
        glm::vec2((float)charRef.x / m_ScaleW,
                  (float)((m_ScaleH - charRef.y) - charRef.height) / m_ScaleH);
    pVertexDisplayArray[i].v1.texCoord =
        glm::vec2((float)(charRef.x + charRef.width) / m_ScaleW,
                  (float)((m_ScaleH - charRef.y) - charRef.height) / m_ScaleH);
    pVertexDisplayArray[i].v2.texCoord = glm::vec2(
        (float)charRef.x / m_ScaleW, (float)(m_ScaleH - charRef.y) / m_ScaleH);
    pVertexDisplayArray[i].v3.texCoord =
        glm::vec2((float)(charRef.x + charRef.width) / m_ScaleW,
                  (float)(m_ScaleH - charRef.y) / m_ScaleH);

    v4Position.x = (v4Position.x + ((float)charRef.xadvance * fWScale));
  }
  return iSize;
}
