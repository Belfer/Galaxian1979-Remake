#include "Application.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <imgui.h>
#include <iostream>
#include <json.hpp>

#include "Config.h"
#include "FontManager.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "Utilities.h"

using json = nlohmann::json;

namespace NHTV {
bool Application::create(int a_argc, char *a_argv[]) {
  if (a_argc < 2)
    assert(false && "Please specify resource path!");
  Config::ResPath = std::string(a_argv[1]);

#pragma region CreateGLFWWindow
  // Load the application data from our appData.xml
  if (!loadApplicationData()) {
    std::cout << "Unable to load Application Data" << std::endl;
    return false;
  }
  // initialise glfw systems
  if (glfwInit() != GL_TRUE) {
    std::cout << "Unable to initialize GLFW" << std::endl;
    return false;
  }

  // create a windowed mode window and its OpenGL context
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  m_windowHandle.glfwWindowHandle =
      glfwCreateWindow(m_windowHandle.width, m_windowHandle.height,
                       m_windowHandle.title.c_str(), nullptr, nullptr);
  if (m_windowHandle.glfwWindowHandle == nullptr) {
    std::cout << "Unable to create a GLFW Window" << std::endl;
    glfwTerminate();
    return false;
  }
  auto window = m_windowHandle.glfwWindowHandle;

  glfwSetWindowPos(window, m_windowHandle.xPos, m_windowHandle.yPos);
  glfwShowWindow(window);

  int major = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR);
  int minor = glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR);
  int revision = glfwGetWindowAttrib(window, GLFW_CONTEXT_REVISION);
  std::cout << "OpenGL Version " << major << "." << minor << "." << revision
            << std::endl;

  // make the window's context current
  glfwMakeContextCurrent(window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#pragma endregion

  // Create some of the components we require
  SpriteManager::CreateInstance();
  FontManager::CreateInstance();
  CTextureManager::CreateInstance();

  int iScreenWidth = 0, iScreenHeight = 0;
  GetScreenSize(iScreenWidth, iScreenHeight);

  // Generate the framebuffer
  glGenFramebuffers(1, &m_frameBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

  // Generate the texture to render to
  glGenTextures(1, &m_renderTarget);
  glBindTexture(GL_TEXTURE_2D, m_renderTarget);

  // Sumbit an empty image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iScreenWidth, iScreenHeight, 0, GL_RGB,
               GL_UNSIGNED_BYTE, 0);

  // Use nearest filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // Set render target as our colour attachement num 0
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTarget, 0);

  // Set the list of draw buffers
  glDrawBuffers(1, m_drawBuffers);

  // Check if framebuffer is ok
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cout << "Unable to setup framebuffer!" << std::endl;
    return false;
  }

  // Generate the fullscreen quad's VAO
  glGenVertexArrays(1, &m_screenQuadVAO);
  glBindVertexArray(m_screenQuadVAO);

  // Generate the fullscreen quads's VBO
  glGenBuffers(1, &m_screenQuadVBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_screenQuadVBO);
  static const GLfloat screenQuadData[] = {
      -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
      -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadData), screenQuadData,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  m_pLineRenderer = new LineRenderer();

  // load shader
  const char *aszInputs[] = {"Position", "Colour", "UV"};
  const char *aszOutputs[] = {"vUV", "vColour"};
  m_ShaderID = NHTV::Utility::loadShaderFromFile(
      3, aszInputs, 2, aszOutputs,
      (Config::ResPath + "/shaders/default.vsh").c_str(),
      (Config::ResPath + "/shaders/default.fsh").c_str());

  // set the texture to use slot 0 in the shader
  GLuint texUniformID = glGetUniformLocation(m_ShaderID, "diffuseTexture");
  glUniform1i(texUniformID, 0);

  // set clear colour
  glClearColor(0.25f, 0.25f, 0.25f, 1);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  // start our timer
  NHTV::Utility::resetTimer();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_fDeltaTime = NHTV::Utility::tickTimer() * m_fTimeScale;

  // Now call our overridden onCreate function to set up application specifics
  bool result = onCreate(a_argc, a_argv);
  if (result == false) {
    std::cout << "Application onCreate failure, terminating application"
              << std::endl;
    glfwTerminate();
  }
  return result;
}

void Application::run() {
  // Utility::resetTimer();
  m_running = true;
  do {
    glfwPollEvents();
    m_fDeltaTime = NHTV::Utility::tickTimer() * m_fTimeScale;
    double mX;
    double mY;
    GetMousePos(mX, mY);

    onUpdate(m_fDeltaTime);

    int iScreenWidth = 0, iScreenHeight = 0;
    GetScreenSize(iScreenWidth, iScreenHeight);
    glViewport(0, 0, iScreenWidth, iScreenHeight);

    if (m_postProcess) {
      glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
      glBindTexture(GL_TEXTURE_2D, m_renderTarget);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iScreenWidth, iScreenHeight, 0,
                   GL_RGB, GL_UNSIGNED_BYTE, 0);
      glBindTexture(GL_TEXTURE_2D, 0);
    }

    onDraw(m_fDeltaTime);

    // set active shader
    glUseProgram(m_ShaderID);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    for (auto &camera : m_cameras) {
      camera.second.update();

      // set current transforms in the shader
      GLint ProjectionID = glGetUniformLocation(m_ShaderID, "Projection");
      GLint ViewID = glGetUniformLocation(m_ShaderID, "View");

      glUniformMatrix4fv(ProjectionID, 1, false,
                         glm::value_ptr(camera.second.getProjection()));
      glUniformMatrix4fv(ViewID, 1, false,
                         glm::value_ptr(camera.second.getView()));

      // Call to draw sprites
      SpriteManager::GetInstance()->Draw(m_ShaderID);

      // Switch blending draw the font
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      FontManager::GetInstance()->Draw(m_ShaderID);

      // Draw any lines that we have in the scene
      m_pLineRenderer->draw(camera.second.getView(),
                            camera.second.getProjection());
    }

    // clear all lines for this frame
    m_pLineRenderer->clear();

    if (m_postProcess) {
      // Render to the screen
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      // Render on the whole framebuffer, complete from the lower left corner to
      // the upper right
      glViewport(0, 0, iScreenWidth, iScreenHeight);

      // Clear the screen
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // Use screen quad shader
      glUseProgram(m_postFxShader);

      // Bind our texture in Texture Unit 0
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_renderTarget);

      // Set our "renderedTexture" sampler to use Texture Unit 0
      GLuint texID = glGetUniformLocation(m_postFxShader, "renderTarget");
      GLuint resolutionID = glGetUniformLocation(m_postFxShader, "resolution");
      GLuint bloomKID = glGetUniformLocation(m_postFxShader, "bloomK");
      GLuint waveParamsID = glGetUniformLocation(m_postFxShader, "waveParams");
      GLuint timeID = glGetUniformLocation(m_postFxShader, "time");
      GLuint colorID = glGetUniformLocation(m_postFxShader, "color");
      GLuint posID = glGetUniformLocation(m_postFxShader, "position");
      GLuint offID = glGetUniformLocation(m_postFxShader, "offset");
      glUniform1i(texID, 0);
      glUniform2fv(
          resolutionID, 1,
          glm::value_ptr(glm::vec2(1.f / iScreenWidth, 1.f / iScreenHeight)));

      if (m_fxBloomK <= 0)
        m_fxBloomK = 0.001f;
      glUniform1f(bloomKID, m_fxBloomK);
      glUniform4fv(waveParamsID, 1, glm::value_ptr(m_fxWaveParams));

      glUniform1f(timeID, m_fxTime);
      glUniform4fv(colorID, 1, glm::value_ptr(m_fxColor));
      glUniform4fv(posID, 1, glm::value_ptr(m_fxPosition));
      glUniform4fv(offID, 1, glm::value_ptr(m_fxOffset));

      // Draw the triangles !
      glBindVertexArray(m_screenQuadVAO);
      glDrawArrays(GL_TRIANGLES, 0,
                   6); // 2*3 indices starting at 0 -> 2 triangles
      glBindVertexArray(0);
    }

    onEditor();

    // bring the back-buffer to the monitor
    glfwSwapBuffers(m_windowHandle.glfwWindowHandle);

  } while (m_running == true &&
           glfwWindowShouldClose(m_windowHandle.glfwWindowHandle) == 0);

  onDestroy();
}

bool Application::loadApplicationData() {
  std::ifstream ifs(Config::ResPath + "/appData.json", std::ios::in);
  std::string res = "";

  if (!ifs.is_open()) {
    std::cerr << "App data file does not exist!\n";
    return false;
  } else {
    std::string line = "";
    while (!ifs.eof()) {
      std::getline(ifs, line);
      res.append(line + "\n");
    }
  }

  auto appData = json::parse(res);
  m_windowHandle.title = appData["title"];
  m_windowHandle.width = appData["width"];
  m_windowHandle.height = appData["height"];
  m_windowHandle.xPos = appData["xPos"];
  m_windowHandle.yPos = appData["yPos"];
  m_windowHandle.glfwWindowHandle = nullptr;

  return true;
}
//////////////////////////////////////////////////////////////////////////
// Gets the screen size of the default window
//////////////////////////////////////////////////////////////////////////
void Application::GetScreenSize(int &a_iWidth, int &a_iHeight) const {
  glfwGetWindowSize(m_windowHandle.glfwWindowHandle, &a_iWidth, &a_iHeight);
}
//////////////////////////////////////////////////////////////////////////
// Does what it says on the tin clears the render buffer or screen
//////////////////////////////////////////////////////////////////////////
void Application::ClearScreen() {
  m_pLineRenderer->clear();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//////////////////////////////////////////////////////////////////////////////
// Changes the background colour of the render window
//////////////////////////////////////////////////////////////////////////////
void Application::SetBackgroundColor(SColour a_bgColor) {
  glm::vec4 bgColor = glm::vec4(
      a_bgColor.argb.colours.r / 255.f, a_bgColor.argb.colours.g / 255.f,
      a_bgColor.argb.colours.b / 255.f, a_bgColor.argb.colours.a / 255.f);

  glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
}

#pragma region Sprite creation and mainpulation functionality
//////////////////////////////////////////////////////////////////////////
// Sprite creation, copy and destruction
//////////////////////////////////////////////////////////////////////////
uint Application::CreateSprite(
    const char *a_pTextureName, float a_fWidth, float a_fHeight,
    bool a_bDrawFromCenter,
    SColour a_sColour /* = SColour( 0xFF, 0xFF, 0xFF, 0xFF )*/) {
  glm::vec2 v2Size = glm::vec2(a_fWidth, a_fHeight);
  glm::vec2 v2Origin =
      a_bDrawFromCenter ? glm::vec2(0.5f, 0.5f) : glm::vec2(0.f, 1.f);
  glm::vec4 uvCoords = glm::vec4(0.f, 0.f, 1.f, 1.f);
  return CreateSprite(a_pTextureName, glm::value_ptr(v2Size),
                      glm::value_ptr(v2Origin), glm::value_ptr(uvCoords),
                      a_sColour);
}

uint Application::CreateSprite(const char *a_pTextureName,
                               const float *a_fv2Size, const float *a_fv2Origin,
                               const float *a_fv4UVCoords /*= nullptr*/,
                               SColour a_sColour) {
  // Set up the UV Coordinates
  glm::vec4 v4UVCoords = (a_fv4UVCoords)
                             ? glm::vec4(a_fv4UVCoords[0], a_fv4UVCoords[1],
                                         a_fv4UVCoords[2], a_fv4UVCoords[3])
                             : glm::vec4(0.f, 0.f, 1.f, 1.f);
  // Set up the colour
  glm::vec4 v4Color = glm::vec4(
      a_sColour.argb.colours.r / 255.f, a_sColour.argb.colours.g / 255.f,
      a_sColour.argb.colours.b / 255.f, a_sColour.argb.colours.a / 255.f);
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    glm::vec2 v2Size = glm::vec2(a_fv2Size[0], a_fv2Size[1]);
    glm::vec2 v2Origin = glm::vec2(a_fv2Origin[0], a_fv2Origin[1]);
    return pSpriteManager->AddSprite(a_pTextureName, v2Size, v2Origin,
                                     v4UVCoords, v4Color);
  }
  return 0;
}

uint Application::DuplicateSprite(uint a_uiSpriteID) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    return pSpriteManager->DuplicateSprite(a_uiSpriteID);
  }
  return 0;
}

void Application::DestroySprite(uint a_uiSpriteID) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->RemoveSprite(a_uiSpriteID);
  }
}
//////////////////////////////////////////////////////////////////////////
// Drawing
//////////////////////////////////////////////////////////////////////////
void Application::DrawSprite(uint a_uiSpriteID) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->MarkSpriteForDraw(a_uiSpriteID);
  }
}
//////////////////////////////////////////////////////////////////////////
// Movement, Rotation and Scale
//////////////////////////////////////////////////////////////////////////
void Application::MoveSprite(uint a_uiSpriteID, float a_fXPos, float a_fYPos) {
  glm::vec2 v2Pos = glm::vec2(a_fXPos, a_fYPos);
  MoveSprite(a_uiSpriteID, glm::value_ptr(v2Pos));
}

void Application::MoveSprite(uint a_uiSpriteID, const float *a_vFloatVec) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->MoveSprite(a_uiSpriteID, a_vFloatVec);
  }
}

void Application::GetSpritePosition(uint a_uiSpriteID, float &a_xPos,
                                    float &a_YPos) const {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    glm::vec2 v2Pos;
    pSpriteManager->GetSpritePosition(a_uiSpriteID, v2Pos);
    a_xPos = v2Pos.x;
    a_YPos = v2Pos.y;
  }
}

void Application::SetSpriteMatrix(uint a_uiSpriteID, const float *a_fm4) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->SetSpriteMatrix(a_uiSpriteID, a_fm4);
  }
}

void Application::GetSpriteMatrix(uint a_uiSpriteID, float *a_fm4) const {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->GetSpriteMatrix(a_uiSpriteID, a_fm4);
  }
}

void Application::RotateSprite(uint a_uiSpriteID,
                               const glm::quat &a_qRotation) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->RotateSprite(a_uiSpriteID, a_qRotation);
  }
}

void Application::SetSpriteScale(uint a_uiSpriteID, const float &a_fSpriteWidth,
                                 const float &a_fSpriteHeight) {
  glm::vec2 v2Scale = glm::vec2(a_fSpriteWidth, a_fSpriteHeight);
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->SetSpriteScale(a_uiSpriteID, v2Scale);
  }
}

void Application::GetSpriteScale(uint a_uiSpriteID, float &a_fSpriteWidth,
                                 float &a_fSpriteHeight) const {
  glm::vec2 v2Scale = glm::vec2(a_fSpriteWidth, a_fSpriteHeight);
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->GetSpriteScale(a_uiSpriteID, v2Scale);
  }
}
//////////////////////////////////////////////////////////////////////////
// UV Coordinates
//////////////////////////////////////////////////////////////////////////
void Application::SetSpriteUVCoordinates(uint a_uiSpriteID, float a_minUCoord,
                                         float a_minVCoord, float a_maxUCoord,
                                         float a_maxVCoord) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    glm::vec2 minUV(a_minUCoord, a_minVCoord);
    glm::vec2 maxUV(a_maxUCoord, a_maxVCoord);
    pSpriteManager->SetSpriteUVCoordinates(a_uiSpriteID, minUV, maxUV);
  }
}

void Application::SetSpriteUVCoordinates(uint a_uiSpriteID,
                                         const float *a_fUVVec4) {
  glm::vec4 v4SpriteCoordinates =
      glm::vec4(a_fUVVec4[0], a_fUVVec4[1], a_fUVVec4[2], a_fUVVec4[3]);
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    glm::vec2 minUV(v4SpriteCoordinates.x, v4SpriteCoordinates.y);
    glm::vec2 maxUV(v4SpriteCoordinates.z, v4SpriteCoordinates.w);
    pSpriteManager->SetSpriteUVCoordinates(a_uiSpriteID, minUV, maxUV);
  }
}

void Application::GetSpriteUVCoordinates(uint a_uiSpriteID, float &a_minUCoord,
                                         float &a_minVCoord, float &a_maxUCoord,
                                         float &a_maxVCoord) const {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    glm::vec2 minUVCoord;
    glm::vec2 maxUVCoord;
    pSpriteManager->GetSpriteUVCoordinates(a_uiSpriteID, minUVCoord,
                                           maxUVCoord);

    a_minUCoord = minUVCoord.x;
    a_minVCoord = minUVCoord.y;
    a_maxUCoord = maxUVCoord.x;
    a_maxVCoord = maxUVCoord.y;
  }
}

void Application::GetSpriteUVCoordinates(uint a_uiSpriteID,
                                         float *a_fUVVec4) const {
  glm::vec4 v4SpriteCoordinates = glm::vec4(0.f, 0.f, 0.f, 0.f);
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    glm::vec2 minUVCoord;
    glm::vec2 maxUVCoord;
    pSpriteManager->GetSpriteUVCoordinates(a_uiSpriteID, minUVCoord,
                                           maxUVCoord);

    v4SpriteCoordinates.x = minUVCoord.x;
    v4SpriteCoordinates.y = minUVCoord.y;
    v4SpriteCoordinates.z = maxUVCoord.x;
    v4SpriteCoordinates.w = maxUVCoord.y;
  }
  a_fUVVec4 = glm::value_ptr(v4SpriteCoordinates);
}
//////////////////////////////////////////////////////////////////////////
// Color
//////////////////////////////////////////////////////////////////////////
void Application::SetSpriteColour(uint a_uiSpriteID, const SColour &a_sColour) {
  glm::vec4 v4colour = glm::vec4(
      a_sColour.argb.colours.r / 255.f, a_sColour.argb.colours.g / 255.f,
      a_sColour.argb.colours.b / 255.f, a_sColour.argb.colours.a / 255.f);
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->SetSpriteColour(a_uiSpriteID, v4colour);
  }
}
void Application::GetSpriteColour(uint a_uiSpriteID, SColour &a_sColour) const {
  glm::vec4 v4colour;
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->GetSpriteColour(a_uiSpriteID, v4colour);
  }
  a_sColour.argb.colours.r = (char)(255.f * v4colour.x);
  a_sColour.argb.colours.g = (char)(255.f * v4colour.y);
  a_sColour.argb.colours.b = (char)(255.f * v4colour.z);
  a_sColour.argb.colours.a = (char)(255.f * v4colour.w);
}
//////////////////////////////////////////////////////////////////////////
// Blend mode settings
//////////////////////////////////////////////////////////////////////////
void Application::SetSpriteBlendMode(uint a_uiSpriteID,
                                     const uint &a_uiSourceFactor,
                                     const uint &a_uiDestinationFactor) {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->SetSpriteBlendMode(a_uiSpriteID, a_uiSourceFactor,
                                       a_uiDestinationFactor);
  }
}

void Application::GetSpriteBlendMode(uint a_uiSpriteID, uint &a_uiSourceFactor,
                                     uint &a_uiDestinationFactor) const {
  SpriteManager *pSpriteManager = SpriteManager::GetInstance();
  if (pSpriteManager) {
    pSpriteManager->GetSpriteBlendMode(a_uiSpriteID, a_uiSourceFactor,
                                       a_uiDestinationFactor);
  }
}
#pragma endregion
#pragma region DrawString Functionality
/// @name Text Rendering
/**@{**/
//////////////////////////////////////////////////////////////////////////
void Application::DrawString(
    const char *a_pText, int a_iXPos, int a_iYPos, float a_fSize /*= 1.0f*/,
    SColour a_sColour /*= SColour(0xFF,0xFF,0xFF,0xFF)*/) {
  glm::vec4 v4Position = glm::vec4((float)a_iXPos, (float)a_iYPos, 0.f, 1.f);
  glm::vec4 v4Colour = glm::vec4(
      a_sColour.argb.colours.r / 255.f, a_sColour.argb.colours.g / 255.f,
      a_sColour.argb.colours.b / 255.f, a_sColour.argb.colours.a / 255.f);

  if (FontManager::GetInstance()) {
    FontManager::GetInstance()->DrawString(v4Position, a_fSize, v4Colour,
                                           a_pText);
  }
}
/**@}**/

void Application::AddFont(const char *a_pFontName) {
  if (FontManager::GetInstance()) {
    FontManager::GetInstance()->AddFont(a_pFontName);
  }
}

void Application::SetFont(const char *a_pFontName) {
  if (FontManager::GetInstance()) {
    FontManager::GetInstance()->SetFont(a_pFontName);
  }
}

void Application::RemoveFont(const char *a_pFontName) {
  if (FontManager::GetInstance()) {
    FontManager::GetInstance()->RemoveFont(a_pFontName);
  }
}

#pragma endregion
#pragma region Post - processing rendering functionality
void Application::SetPostProcessFx(const char *a_pShaderName) {
  const char *pfxInputs[] = {"Position"};
  const char *pfxOutputs[] = {"vUV"};
  m_postFxShader = NHTV::Utility::loadShaderFromFile(
      1, pfxInputs, 1, pfxOutputs,
      (Config::ResPath + "/shaders/fullpass.vsh").c_str(), a_pShaderName);
  m_postProcess = true;
}
#pragma endregion
#pragma region Line Drawring Functionality
//////////////////////////////////////////////////////////////////////////
/// @name Basic Line Drawing Functionality
//////////////////////////////////////////////////////////////////////////
/**@{**/

//////////////////////////////////////////////////////////////////////////
void Application::DrawLine(int a_iStartX, int a_iStartY, int a_iEndX,
                           int a_iEndY) {
  m_pLineRenderer->addLine(
      glm::vec4((float)a_iStartX, (float)a_iStartY, 0.f, 1.f),
      glm::vec4((float)a_iEndX, (float)a_iEndY, 0.f, 1.f),
      glm::vec4(1.f, 1.f, 1.f, 1.f));
}

//////////////////////////////////////////////////////////////////////////
void Application::DrawLine(int a_iStartX, int a_iStartY, int a_iEndX,
                           int a_iEndY, SColour a_sColour) {
  glm::vec4 lineColor = glm::vec4(
      a_sColour.argb.colours.r / 255.f, a_sColour.argb.colours.g / 255.f,
      a_sColour.argb.colours.b / 255.f, a_sColour.argb.colours.a / 255.f);

  m_pLineRenderer->addLine(
      glm::vec4((float)a_iStartX, (float)a_iStartY, 0.f, 1.f),
      glm::vec4((float)a_iEndX, (float)a_iEndY, 0.f, 1.f), lineColor);
}

//////////////////////////////////////////////////////////////////////////
void Application::DrawLine(int a_iStartX, int a_iStartY, int a_iEndX,
                           int a_iEndY, SColour a_sStartColour,
                           SColour a_sEndColour) {
  glm::vec4 startColor = glm::vec4(a_sStartColour.argb.colours.r / 255.f,
                                   a_sStartColour.argb.colours.g / 255.f,
                                   a_sStartColour.argb.colours.b / 255.f,
                                   a_sStartColour.argb.colours.a / 255.f);

  glm::vec4 endColor = glm::vec4(
      a_sEndColour.argb.colours.r / 255.f, a_sEndColour.argb.colours.g / 255.f,
      a_sEndColour.argb.colours.b / 255.f, a_sEndColour.argb.colours.a / 255.f);

  m_pLineRenderer->addLine(
      glm::vec4((float)a_iStartX, (float)a_iStartY, 0.f, 1.f),
      glm::vec4((float)a_iEndX, (float)a_iEndY, 0.f, 1.f), startColor,
      endColor);
}
/**@}**/
#pragma endregion
#pragma region Input handling functionality
//////////////////////////////////////////////////////////////////////////
/// @name Input handling functionality
//////////////////////////////////////////////////////////////////////////
/**@{**/

//////////////////////////////////////////////////////////////////////////
bool Application::IsKeyDown(uint a_uiKey) {
  return (glfwGetKey(m_windowHandle.glfwWindowHandle, a_uiKey) == GLFW_PRESS);
}

//////////////////////////////////////////////////////////////////////////
void Application::GetMousePos(double &a_uiMousePosX, double &a_uiMousePosY) {
  glfwGetCursorPos(m_windowHandle.glfwWindowHandle, &a_uiMousePosX,
                   &a_uiMousePosY);
}

//////////////////////////////////////////////////////////////////////////
bool Application::GetMouseButtonDown(int a_uiMouseButton) {
  return (glfwGetMouseButton(m_windowHandle.glfwWindowHandle,
                             a_uiMouseButton) == GLFW_PRESS);
}

bool Application::GetMouseButtonReleased(int a_uiMouseButton) {
  return (glfwGetMouseButton(m_windowHandle.glfwWindowHandle,
                             a_uiMouseButton) == GLFW_RELEASE);
}
/**@}**/
#pragma endregion
float Application::GetDeltaTime() { return NHTV::Utility::getDeltaTime(); }
//////////////////////////////////////////////////////////////////////////////
// Destructor for the Application
//////////////////////////////////////////////////////////////////////////
Application::~Application() {
  glDeleteShader(m_ShaderID);
  glfwTerminate();
}
} // end namespace NHTV
