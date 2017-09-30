//////////////////////////////////////////////////////////////////////////
//	Author:	Jamie Stewart
//	Date:	08/07/2015
//	Brief:	Class to manage sprites
//////////////////////////////////////////////////////////////////////////
#pragma once

#include <glad/glad.h>

#include "BaseSprite.h"
#include "Utilities.h"
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <queue>
#include <vector>

namespace NHTV {
//////////////////////////////////////////////////////////////////////////
class SpriteManager {
public:
  //////////////////////////////////////////////////////////////////////////
  // This class is a static instance or a singleton class
  //////////////////////////////////////////////////////////////////////////
  static SpriteManager *CreateInstance();
  static SpriteManager *GetInstance();
  static void DestroyInstance();
  //////////////////////////////////////////////////////////////////////////
  void Draw(uint a_uiShaderID);
  //////////////////////////////////////////////////////////////////////////
  uint AddSprite(const char *a_pTexture, glm::vec2 &a_v2Size,
                 glm::vec2 &a_v2Origin, glm::vec4 a_vfUVCoords,
                 glm::vec4 a_v4colour);
  uint DuplicateSprite(uint a_uiSpriteID);
  void MoveSprite(uint a_uiSpriteID, int a_iXPos, int a_iYPos);
  void MoveSprite(uint a_uiSpriteID, const float *a_fVec);
  void GetSpritePosition(uint a_uiSpriteID, glm::vec2 &a_v2Pos) const;
  void SetSpriteMatrix(uint a_uiSpriteID, const float *a_fm4);
  void GetSpriteMatrix(uint a_uiSpriteID, float *a_fm4);
  void RotateSprite(uint a_uiSpriteID, const glm::quat &a_qRotation);
  void RemoveSprite(uint a_uiSpriteID);
  void SetSpriteUVCoordinates(uint a_uiSpriteID, glm::vec2 &a_minUVCoord,
                              glm::vec2 &a_maxUVCoord);
  void GetSpriteUVCoordinates(uint a_uiSpriteID, glm::vec2 &a_minUVCoord,
                              glm::vec2 &a_maxUVCoord);
  void MarkSpriteForDraw(uint a_uiSpriteID);
  void SetSpriteColour(uint a_uiSpriteID, glm::vec4 &a_v4Colour);
  void GetSpriteColour(uint a_uiSpriteID, glm::vec4 &a_v4Colour);
  void SetSpriteScale(uint a_uiSpriteID, glm::vec2 &a_v2SpriteScale);
  void GetSpriteScale(uint a_uiSpriteID, glm::vec2 &a_v2SpriteScale);
  void SetSpriteBlendMode(uint a_uiSpriteID, const uint &a_uiSourceFactor,
                          const uint &a_uiDestinationFactor);
  void GetSpriteBlendMode(uint a_uiSpriteID, uint &a_uiSourceFactor,
                          uint &a_uiDestinationFactor) const;

protected:
  SpriteManager();
  ~SpriteManager();

private:
  std::vector<BaseSprite *> m_Sprites;
  std::queue<BaseSprite *> m_pSpritesToDrawThisFrame;
  static SpriteManager *m_pInstance;

  GLuint m_uiVAO;
  GLuint m_uiVBO;
  GLuint m_uiIBO;

  NHTV::Vertex aoVertices[4];
  NHTV::Vertex aoUpperLeftVertices[4];
};

} // end namespace NHTV
