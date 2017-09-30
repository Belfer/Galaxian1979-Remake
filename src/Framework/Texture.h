//////////////////////////////////////////////////////////////////////////
// Author:	Jamie Stewart
// Date:	25/06/2015
// Brief:	Texture Handler
//////////////////////////////////////////////////////////////////////////
#pragma once

#include "Utilities.h"

namespace NHTV {
//////////////////////////////////////////////////////////////////////////
// Class to load in texture data
//  A texture is just a buffer with some data in it regardig the pixel colours
//  All texture data is pushed to the GPU on load so that it doesn't occupy
//  memory on the CPU side
//  This texture class uses simplistic reference counting to keep track of how
//  many objects are using the texture
//////////////////////////////////////////////////////////////////////////
class CTexture {
public:
  CTexture();
  CTexture(const char *filename);
  CTexture(CTexture &copy);
  virtual ~CTexture();

  uint Load(const char *filename);
  bool UnLoad();

  uint GetTextureHandle() { return m_textureHandle; }
  const char *GetFileName() { return m_pFilename; }
  void GetTextureSize(uint &iWidth, uint &iHeight);
  uint GetTextureSizeInMemory();

  void IncrememntRefCount();
  void DecrementRefCount();

private:
  uint m_textureHandle;
  int m_iWidth;
  int m_iHeight;
  int m_iBitsPerPixel;
  char *m_pFilename;
  // RefCount is the number of times this texture is being used
  uint m_iRefCount;
  uint m_iTextureMemory;
};
} // end namespace NHTV
