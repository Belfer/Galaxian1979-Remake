////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	25/06/2015
// Brief:	Texture Manager
///////////////////////////////////////////////////////////////
#pragma once

#include "Texture.h"
#include "Utilities.h"
#include <map>
#include <string>

namespace NHTV {
//////////////////////////////////////////////////////////////////////////
// A manager class for the textures currently in memory
//  - Singleton class
//////////////////////////////////////////////////////////////////////////
class CTextureManager {
protected:
  CTextureManager();
  ~CTextureManager();

public:
  static CTextureManager *CreateInstance();
  static CTextureManager *GetInstance();
  static void DestroyInstance();
  //
  uint LoadTexture(const char *a_pTextureName);
  void RemoveTexture(uint &a_uiTextureID);
  void IncrementTextureReference(uint a_uiTextureID);
  void DecrementTextureReference(uint a_uiTextureID);

private:
  static CTextureManager *m_pInstance;
  std::map<std::string, CTexture *> m_pTextureDictionary;
  uint m_pTotalTextureMemory;
};

} // namespace NHTV
