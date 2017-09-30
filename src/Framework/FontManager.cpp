//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	08/07/2015
//	Brief:	The font manager - it is in charge of handling fonts
//          Multiple fonts can be utilised to display different information
//          to the screen
//////////////////////////////////////////////////////////////////////////
#include "FontManager.h"
#include "Config.h"
#include "Font.h"
#include "Utilities.h"
#include <assert.h>

namespace NHTV {
// We're rendering text to the screen for a game not writing an essay
const uint MAX_CHARS = 1024;
//////////////////////////////////////////////////////////////////////////
// Singleton Implementation of Font Manager Class
//////////////////////////////////////////////////////////////////////////
FontManager *FontManager::m_pInstance = 0;
//////////////////////////////////////////////////////////////////////////
FontManager *FontManager::CreateInstance() {
  if (m_pInstance == NULL) {
    m_pInstance = new FontManager();
  }
  return m_pInstance;
}
//////////////////////////////////////////////////////////////////////////
FontManager *FontManager::GetInstance() { return m_pInstance; }
//////////////////////////////////////////////////////////////////////////
void FontManager::DestroyInstance() { delete m_pInstance; }
//////////////////////////////////////////////////////////////////////////
// Create and initialise the font manager and set up the Vertex and index
// buffrers that will be drawn to
///////////////////////////////////////////////////////////////////////////
FontManager::FontManager() {
  m_m4ModelView = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

  m_pDebugFont = new Font((Config::g_ResourcesPath + "/fonts/arial.json.fnt").c_str());
  m_pCurrentFont = m_pDebugFont;

  FontBlock *nextRQFont = new FontBlock();
  nextRQFont->pFont = m_pCurrentFont;
  nextRQFont->iCharacterIndexStart = 0;
  nextRQFont->iNumCharsToRender = 0;
  m_pFontRenderQueue.push_back(nextRQFont);

  m_uiCharactersToDraw = 0;
  m_pCharacterBillboards = new CharacterBillboard[MAX_CHARS];

  glGenVertexArrays(1, &m_uiVAO);
  glBindVertexArray(m_uiVAO);
  glGenBuffers(1, &m_uiVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
  glBufferData(GL_ARRAY_BUFFER, MAX_CHARS * sizeof(CharacterBillboard),
               m_pCharacterBillboards, GL_DYNAMIC_DRAW);

  uint uiIndexCount = MAX_CHARS * 6;
  uint *auiIndices = new uint[uiIndexCount];
  uint index = 0;
  for (uint i = 0; i < uiIndexCount; i += 6, index += 4) {
    auiIndices[i] = index;
    auiIndices[i + 1] = index + 1;
    auiIndices[i + 2] = index + 2;
    auiIndices[i + 3] = index + 1;
    auiIndices[i + 4] = index + 3;
    auiIndices[i + 5] = index + 2;
  }

  glGenBuffers(1, &m_uiIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, uiIndexCount * sizeof(uint), auiIndices,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex),
                        ((char *)0) + 16);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, sizeof(Vertex),
                        ((char *)0) + 32);

  glBindVertexArray(0);
  delete[] auiIndices;
}

//////////////////////////////////////////////////////////////////////////
FontManager::~FontManager() {
  delete m_pDebugFont;
  delete[] m_pCharacterBillboards;
  glDeleteVertexArrays(1, &m_uiVAO);
  glDeleteBuffers(1, &m_uiVBO);
  glDeleteBuffers(1, &m_uiIBO);
}

//////////////////////////////////////////////////////////////////////////
void FontManager::Update(float fTimeStep) {}

//////////////////////////////////////////////////////////////////////////
// Draw - render out any characters that we have been instructed to draw
//     there is a maximum of MAX_CHARS characters allowed to be rendered per
//     frame
///////////////////////////////////////////////////////////////////////////
void FontManager::Draw(uint a_uiProgramID) {
  if (m_uiCharactersToDraw > 0 && m_pFontRenderQueue.size() > 0) {
    glUseProgram(a_uiProgramID);

    GLuint ModelID = glGetUniformLocation(a_uiProgramID, "Model");
    GLuint rendercolour = glGetUniformLocation(a_uiProgramID, "tintColor");
    glUniform4fv(rendercolour, 1,
                 glm::value_ptr(glm::vec4(1.f, 1.f, 1.f, 1.f)));
    GLuint uvOffset = glGetUniformLocation(a_uiProgramID, "uvOffset");
    glUniform2fv(uvOffset, 1, glm::value_ptr(glm::vec2(0.f, 0.f)));

    GLuint uvScale = glGetUniformLocation(a_uiProgramID, "uvScale");
    glUniform2fv(uvScale, 1, glm::value_ptr(glm::vec2(1.f, 1.f)));
    glUniformMatrix4fv(ModelID, 1, false, glm::value_ptr(m_m4ModelView));

    // Ok so here we're going to go through the Fonts that have been put into
    // the render queue and set the
    // appropriate textures and render out the right number of characters
    for (auto iter = m_pFontRenderQueue.begin();
         iter != m_pFontRenderQueue.end(); ++iter) {
      FontBlock *pfrqItem = (*iter);
      if (pfrqItem->iNumCharsToRender > 0) {
        // set active texture, bind the crate quad's buffers and draw it
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, pfrqItem->pFont->GetTextureID());

        // glDepthMask(false);
        glBindBuffer(GL_ARRAY_BUFFER, m_uiVBO);
        glBufferSubData(
            GL_ARRAY_BUFFER, 0,
            pfrqItem->iNumCharsToRender * sizeof(CharacterBillboard),
            &m_pCharacterBillboards[pfrqItem->iCharacterIndexStart]);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(m_uiVAO);
        glDrawElements(GL_TRIANGLES, pfrqItem->iNumCharsToRender * 6,
                       GL_UNSIGNED_INT, 0);
      }
      delete pfrqItem;
    }
  }
  m_uiCharactersToDraw = 0;
  m_pFontRenderQueue.clear();

  m_pCurrentFont = m_pDebugFont;

  FontBlock *nextRQFont = new FontBlock();
  nextRQFont->pFont = m_pCurrentFont;
  nextRQFont->iCharacterIndexStart = 0;
  nextRQFont->iNumCharsToRender = 0;
  m_pFontRenderQueue.push_back(nextRQFont);
}
///////////////////////////////////////////////////////////////////////////
// DrawString - this function is called via a framework accessor to create a new
//      character string to draw this frame, it pushes chars into the draw queue
//////////////////////////////////////////////////////////////////////////
void FontManager::DrawString(glm::vec4 v4Position, float a_fSize,
                             glm::vec4 v4colour, const std::string &buffer) {
  if (m_pCurrentFont != NULL) {
    uint iTotalChars = m_uiCharactersToDraw + buffer.length();
    if (iTotalChars > MAX_CHARS) {
      // Oh noes.. too many characters on the dance floor.
    } else {
      FontBlock *pCurrentItem = m_pFontRenderQueue.back();
      pCurrentItem->iNumCharsToRender += buffer.length();
      m_uiCharactersToDraw += m_pCurrentFont->DrawString(
          v4Position, a_fSize, v4colour, buffer,
          &m_pCharacterBillboards[m_uiCharactersToDraw]);
    }
  }
}

//////////////////////////////////////////////////////////////////////////
Font *FontManager::GetDebugFont() { return m_pDebugFont; }

//////////////////////////////////////////////////////////////////////////
Font *FontManager::GetCurrentFont() { return m_pCurrentFont; }

//////////////////////////////////////////////////////////////////////////
void FontManager::AddFont(Font *pFont) {
  if (pFont) {
    m_pFontList.push_back(pFont);
  }
}
///////////////////////////////////////////////////////////////////////////
// adds a new font to the font manager
//////////////////////////////////////////////////////////////////////////
Font *FontManager::AddFont(const char *pFontName) {
  Font *pFont = new Font(pFontName);
  if (pFont) {
    m_pFontList.push_back(pFont);
  }
  return pFont;
}
///////////////////////////////////////////////////////////////////////////
// SetFont() - sets the currect font if there is not a font available it will
// use the
// debug font
//////////////////////////////////////////////////////////////////////////
void FontManager::SetFont(Font *pFont) {
  m_pCurrentFont = m_pDebugFont;

  if (m_pFontList.size() > 0) {
    if (pFont == nullptr) {
      m_pCurrentFont = (*m_pFontList.begin());
    } else {
      std::list<Font *>::iterator iter;
      for (iter = m_pFontList.begin(); iter != m_pFontList.end(); ++iter) {
        if (*iter == pFont) {
          m_pCurrentFont = pFont;
          break;
        }
      }
    }
  }

  FontBlock *nextRQFont = new FontBlock();
  nextRQFont->pFont = m_pCurrentFont;
  nextRQFont->iCharacterIndexStart = m_uiCharactersToDraw;
  nextRQFont->iNumCharsToRender = 0;
  m_pFontRenderQueue.push_back(nextRQFont);
}
//////////////////////////////////////////////////////////////////////////
// not yet implemented
//////////////////////////////////////////////////////////////////////////
Font *FontManager::SetFont(const char *pFontName) {
  m_pCurrentFont = m_pDebugFont;
  if (pFontName != nullptr && m_pFontList.size() > 0) {
    std::list<Font *>::iterator iter;
    for (iter = m_pFontList.begin(); iter != m_pFontList.end(); ++iter) {
      if ((*iter)->GetFontName() == pFontName) {
        m_pCurrentFont = *iter;
        break;
      }
    }
  }

  FontBlock *nextRQFont = new FontBlock();
  nextRQFont->pFont = m_pCurrentFont;
  nextRQFont->iCharacterIndexStart = m_uiCharactersToDraw;
  nextRQFont->iNumCharsToRender = 0;
  m_pFontRenderQueue.push_back(nextRQFont);

  return m_pCurrentFont;
}
//////////////////////////////////////////////////////////////////////////
// if we dont need the font then we better remove it.
//////////////////////////////////////////////////////////////////////////
void FontManager::RemoveFont(Font *pFont) {
  if (pFont == m_pDebugFont) {
    return;
  }

  bool bGetNextAvailableFont = false;
  std::list<Font *>::iterator iter;
  for (iter = m_pFontList.begin(); iter != m_pFontList.end(); ++iter) {
    if (*iter == pFont) {
      if (pFont == m_pCurrentFont) {
        bGetNextAvailableFont = true;
      }
      iter = m_pFontList.erase(iter);
      break;
    }
  }

  if (bGetNextAvailableFont) {
    Font *pFont = NULL;
    SetFont(pFont);
  }
}
//////////////////////////////////////////////////////////////////////////
// Yet to be implemented
//////////////////////////////////////////////////////////////////////////
void FontManager::RemoveFont(const char *pFontName) {
  bool bGetNextAvailableFont = false;
  std::list<Font *>::iterator iter;
  for (iter = m_pFontList.begin(); iter != m_pFontList.end(); ++iter) {
    if ((*iter)->GetFontName() == pFontName) {
      if (m_pCurrentFont->GetFontName() == pFontName) {
        bGetNextAvailableFont = true;
      }
      iter = m_pFontList.erase(iter);
      break;
    }
  }

  if (bGetNextAvailableFont) {
    Font *pFont = NULL;
    SetFont(pFont);
  }
}

} // namespace NHTV
