//\====

#pragma once

#include <glad/glad.h>

#include "Camera.h"
#include "Config.h"
#include "Enumerations.h"
#include "LineRenderer.h"
#include "Utilities.h"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

struct GLFWwindow;

namespace NHTV {

#pragma region SColor structure to describe a colour
/**
* @brief A Structure to describe a colour value as an uint, and as
*individual 8 bit components (char values)
**/
struct SColour {
  /// @brief Default constructor for a SColour object
  SColour() {}
  /// @brief Constructor for an SColour obejct that constructs the SColour
  /// object from an uinteger value
  /// @param a_iColor the colour that the SColour will be created to specify
  SColour(uint a_iColour) { argb.colour = a_iColour; }
  /// @brief Constructor to create an SColor object from four char values RGBA
  /// @param r uchar to represent the red component of an SColour object
  /// @param g uchar to represent the red component of an SColour object
  /// @param b uchar to represent the red component of an SColour object
  /// @param a uchar to represent the red component of an SColour object
  SColour(uchar r, uchar g, uchar b, uchar a) {
    argb.colours.r = r;
    argb.colours.g = g;
    argb.colours.b = b;
    argb.colours.a = a;
  }
  /// and unnamed union that causes SColours member variables to use the same
  /// area of memory.
  /// this means that @param colour and @param colours occupy the same space, a
  /// change to one is
  /// reflected in the other.
  union {
    uint colour;
    struct SColours {
      uchar a, r, g, b;
    } colours;
  } argb;
};
#pragma endregion

class Application {
public:
  Application() : m_running(false) {}
  virtual ~Application();

  bool create(int a_argc, char *a_argv[]);
  void run();
  void destroy() { m_running = false; }

protected:
  // To be implemented in a child class of our application
  virtual bool onCreate(int a_argc, char *a_argv[]) = 0;
  virtual void onUpdate(float a_deltaTime) = 0;
  virtual void onDraw() = 0;
  virtual void onEditor() = 0;
  virtual void onDestroy() = 0;

  bool m_running;

public:
  // Functionality that we need in our base application
  void GetScreenSize(int &a_iWidth, int &a_iHeight) const;
  void clearScreen();
  void setBackgroundColor(SColour a_bgColor);

#pragma region Line Drawing functionality

  /// @brief Function for drawing a simple line
  /// @param a_iStartX the starting X Position for the Line
  /// @param a_iStartY the starting Y Position for the line
  /// @param a_iEndX the end X Position for the line
  /// @param a_iEndY the end Y Position for the line
  void DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY);
  /// @brief Function for drawing a line with a specific uniform colour
  /// @param a_iStartX the starting X Position for the Line
  /// @param a_iStartY the starting Y Position for the line
  /// @param a_iEndX the end X Position for the line
  /// @param a_iEndY the end Y Position for the line
  /// @param a_sColour a Vec4 representing the color for the line segment
  void DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY,
                SColour a_sColour);
  /// @brief Function for drawing a line that starts as one colour and finishes
  /// as another colour.
  /// @param a_iStartX the starting X Position for the Line
  /// @param a_iStartY the starting Y Position for the line
  /// @param a_iEndX the end X Position for the line
  /// @param a_iEndY the end Y Position for the line
  /// @param a_sStartColour a Vec4 denoting the colour of the starting point of
  /// the line segment
  /// @param a_sEndColour a Vec4 denoting the colour of the end point of the
  /// line segment
  void DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY,
                SColour a_sStartColour, SColour a_sEndColour);

#pragma endregion

#pragma region Input handling functionality
  /// @brief Function returns weather a key is currently being pressed
  /// @param a_uiKey the unique identifier for the key, these are kept in an
  /// enum referenced from GLFW
  /// @return boolean value to represent wheather a key is held down or not
  bool IsKeyDown(uint a_uiKey);
  /// @brief Function to get the position of the mouse, relative to the display
  /// window. Value is returned via function parameters passed in as reference
  /// arguments
  ///			the returned values are given in pixels from the windows
  /// origin.
  /// @param a_uiMousePosX the reference argument that is to be given the value
  /// for the mouse cursors X position
  /// @param a_uiMousePosY the reference argument that is to be given the value
  /// for the mouse cursors Y position
  void GetMousePos(double &a_uiMousePosX, double &a_uiMousePosY);
  /// @brief Function to test if a mouse button is being pressed.
  /// @param a_iMouseButton and integer identifier to identify the button to
  /// test for
  /// @return boolean value indicating if button is currently being triggered
  bool GetMouseButtonDown(int a_iMouseButton);
  /// @brief Function to Test to see if the mouse button has been released
  /// @param a_iMoustButton an integer identifier to identify the button to test
  /// for release
  /// @return boolean value to indicate weather button has just been released.
  bool GetMouseButtonReleased(int a_iMouseButton);
#pragma endregion

#pragma region Sprite creation and mainpulation functionality
  /// Function to create a Sprite
  /// @param a_pTextureName - the name of the texture that the sprite image
  /// resides in
  /// @param a_fWidth - the width that the sprite object is to be draw on the
  /// screen, this width is in pixels
  /// @param a_fHeight - the height that the sprite object is to be draw on the
  /// screen, this height is in pixels
  /// @param a_bDrawFromCenter - weather or not the sprite is to be drawn from
  /// the center position, defaults to true
  /// @param a_v4color - the colour tint that the sprite is to be given,
  /// defaults to white.
  /// @return the ID for the sprite, this is an uinteger.
  ///
  uint CreateSprite(const char *a_pTextureName, float a_fWidth, float a_fHeight,
                    bool a_bDrawFromCenter = true,
                    SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF));
  /// Function to create a Sprite
  /// @param a_pTextureName - the name of the texture that the sprite image
  /// resides in
  /// @param a_fv2Size - the width and height of the sprite in Pixels as a float
  /// vector
  /// @param a_fv2Origin - The origin position for the sprite matrix, this
  /// dictates where the sprite will be drawn from (Top Left, Top Right etc)
  /// @param a_v4UVCoords - The UV Coordinates for the sprite as a float 4,
  /// components  1 & 2 correspond to origin point, components 3 & 4 correspond
  /// to end point.
  /// @param a_v4color - the colour tint that the sprite is to be given,
  /// defaults to white.
  /// @return the ID for the sprite, this is an uinteger.
  ///
  uint CreateSprite(const char *a_pTextureName, const float *a_fv2Size,
                    const float *a_fv2Origin,
                    const float *a_fv4UVCoords = nullptr,
                    SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF));
  /// Function to Duplicate an existing Sprite
  /// @param a_uiSpriteID This is ID of the sprite that is to be duplicated
  /// @return Returns a new sprite ID.
  uint DuplicateSprite(uint a_uiSpriteID);
  /// This function is used to free up the memory associated with the Sprite ID
  /// passed in. After this function is called the sprite can no longer be drawn
  /// as it ceases to exist
  /// @param a_uiSpriteID the ID of the sprite to be destroyed.
  void DestroySprite(uint a_uiSpriteID);
  /// This funciton moves a Sprite
  /// @param a_uiSpriteID the ID of the sprite to be moved
  /// @param a_fXPos the X co-ordinate that the sprite is to be moved to in
  /// screenspace
  /// @param a_fYPos the Y co-ordinate that the sprite is to be moved to in
  /// screenspace
  void MoveSprite(uint a_uiSpriteID, float a_fXPos, float a_fYPos);
  /// This funciton moves a Sprite
  /// @param a_uiSpriteID the ID of the sprite to be moved
  /// @param a_fVec a float vector that contains two values (X & Y) to relocate
  /// the sprite to
  void MoveSprite(uint a_uiSpriteID, const float *a_fVec);
  /// @brief This function gets the x y post of the Sprite
  /// @param a_uiSpriteID the unique identifier for the sprite
  /// @param a_fXPos the X co-ordinate that the sprite is at
  /// @param a_fYPos the Y co-ordinate that the sprite is at
  void GetSpritePosition(uint a_uiSpriteID, float &a_xPos, float &a_YPos) const;
  /// @brief This function sets the 4x4 matrix for the sprite
  /// @param a_uiSpriteID the unique identifier for the sprite
  /// @param a_fm4 a float pointer indicating the start of a float array
  /// containing 16 values representing a 4 x 4 matrix
  void SetSpriteMatrix(uint a_uiSpriteID, const float *a_fm4);
  /// @brief This function gets the 4x4 matrix for the sprite
  /// @param a_uiSpriteID the unique identifier for the sprite
  /// @param a_fm4 a float pointer indicating the start of a float array that is
  /// to be used to hold the 16 float values representing a 4 x 4 matrix
  void GetSpriteMatrix(uint a_uiSpriteID, float *a_fm4) const;
  /// @brief Function to draw a sprite to the screen
  /// @param a_uiSpriteID the unique identifier for the Sprite
  void DrawSprite(uint a_uiSpriteID);
  /// @brief Function to rotate a sprite by a_fRotation number of degrees,
  /// positive rotation is in a clockwise direction
  /// @param a_uiSpriteID the unique ID for the sprite
  /// @param a_qRotation the quaternion to rotate the sprite by, this value is
  /// non-cumulative E.G passing in a constant rotation value every frame will
  /// not incrementally rotate the sprite each frame.
  void RotateSprite(uint a_uiSpriteID, const glm::quat &a_qRotation);
  /// @brief This function sets the sprites UV coordinates
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_minUCoord this represents the starting point for the UV
  /// coordinates on the U Texture axis
  /// @param a_minVCoord this represents the starting point for the UV
  /// coordinates on the V Texture axis
  /// @param a_maxUCoord this represents the end point for the UV coordinates on
  /// the U Texture axis
  /// @param a_maxVCoord this represents the end point for the UV coordinates on
  /// the V Texture axis
  void SetSpriteUVCoordinates(uint a_uiSpriteID, float a_minUCoord,
                              float a_minVCoord, float a_maxUCoord,
                              float a_maxVCoord);
  /// @brief This function sets the sprites UV coordinates
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_v4UVVoordinates a float vector4 representing the minimum and
  /// maximum UV Coordinates for the sprite, the X and Y components of the Vec4
  /// correspond to the minimum UV coordinates
  ///			the Z & W components correspond to the maximum UV
  /// Coordinates
  void SetSpriteUVCoordinates(uint a_uiSpriteID, const float *a_fUVVec4);
  /// @brief This functions retrieves the Sprites current UV coordinates
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_minUCoord this represents the starting point for the UV
  /// coordinates on the U Texture axis
  /// @param a_minVCoord this represents the starting point for the UV
  /// coordinates on the V Texture axis
  /// @param a_maxUCoord this represents the end point for the UV coordinates on
  /// the U Texture axis
  /// @param a_maxVCoord this represents the end point for the UV coordinates on
  /// the V Texture axis
  void GetSpriteUVCoordinates(uint a_uiSpriteID, float &a_minUCoord,
                              float &a_minVCoord, float &a_maxUCoord,
                              float &a_maxVCoord) const;
  /// @brief This functions retrieves the Sprites current UV coordinates
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_v4UVVoordinates a float vector4 representing the minimum and
  /// maximum UV Coordinates for the sprite, the X and Y components of the Vec4
  /// correspond to the minimum UV coordinates
  ///			the Z & W components correspond to the maximum UV
  /// Coordinates
  void GetSpriteUVCoordinates(uint a_uiSpriteID, float *a_fUVVec4) const;
  /// @brief Used to set the colour for the sprite
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_sColour the colour that the sprite is to be tinted with, this
  /// colour is passed in a vec4 with xyzw values mapping to rgba values
  void SetSpriteColour(uint a_uiSpriteID, const SColour &a_sColour);
  /// @brief Used to get the current colour for the sprite
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_sColour the colour that the sprite is tinted with, this colour is
  /// accessed via a vec4 with xyzw values mapping to rgba values
  void GetSpriteColour(uint a_uiSpriteID, SColour &a_sColour) const;
  /// @brief Used to set the current scale for the sprite
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_v2Scale a Vec2 representing the scale in the X and Y Directions
  /// that the sprite is to be scaled by
  void SetSpriteScale(uint a_uiSpriteID, const float &a_fSpriteWidth,
                      const float &a_fSpriteHeight);
  /// @brief Used to get the current scale for the sprite
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_v2Scale a Vec2 representing the scale in the X and Y Directions
  /// that the sprite is to currently scaled by
  void GetSpriteScale(uint a_uiSpriteID, float &a_fSpriteWidth,
                      float &a_fSpriteHeight) const;
  /// @brief Used to set the current blend mode for the sprite
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_uiSourceFactor the blend factor for the source image
  /// @param a_uiDestinationFactor the blend mode that is to be used by the
  /// destination image.
  void SetSpriteBlendMode(uint a_uiSpriteID, const uint &a_uiSourceFactor,
                          const uint &a_uiDestinationFactor);
  /// @brief Used to get the current blend mode for the sprite
  /// @param a_uiSpriteID the unique identifier for the given sprite
  /// @param a_uiSourceFactor the current blend factor for the source image
  /// @param a_uiDestinationFactor the current blend mode that is used by the
  /// destination image.
  void GetSpriteBlendMode(uint a_uiSpriteID, uint &a_uiSourceFactor,
                          uint &a_uiDestinationFactor) const;

#pragma endregion

#pragma region Font Rendering functionality
  /// @brief This function is used to display text output to the screen.  This
  /// text is renedered as series of simple sprites who's texture is an area of
  /// a font character sheet.
  /// @param a_v4Position this is the location that the origin of the String is
  /// to be drawn to the screen, this is a Vec4 to account for the underlying
  /// Mat4 calculations that take place in
  ///			the deeper parts of the font renderering functionality
  /// @param a_v4Color the color that the text is to be rendered in
  /// @param a_pTextToDisplay the text that is to be written to the screen. This
  /// is to be passed through as a pointer to a char array.
  void DrawString(const char *a_pText, int a_iXPos, int a_iYPos,
                  float fSize = 1.0f,
                  SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF));

  void AddFont(const char *a_pFontName);
  void SetFont(const char *a_pFontName);
  void RemoveFont(const char *a_pFontName);

#pragma endregion

#pragma region Camera - camera functionality
  using CameraHandle = std::pair<size_t, Camera>;

  inline CameraHandle &CreateCamera() {
    m_cameras.emplace_back(std::make_pair(m_cameraCounter++, Camera()));
    return m_cameras.at(0);
  }

  inline void RemoveCamera(size_t id) {
    for (auto it = m_cameras.begin(); it != m_cameras.end(); ++it) {
      if (it->first == id) {
        m_cameras.erase(it);
        break;
      }
    }
  }

  inline void ClearCameras() { m_cameras.clear(); }
#pragma endregion

#pragma region Post - processing rendering functionality
  inline void SetPostProcessEnabled(bool enabled) { m_postProcess = enabled; }

  inline void SetTimePostFxVar(float time) { m_fxTime = time; }

  inline void SetColorPostFxVar(const glm::vec4 &color) { m_fxColor = color; }

  inline void SetPositionPostFxVar(const glm::vec4 &pos) { m_fxPosition = pos; }

  inline void SetOffsetPostFxVar(const glm::vec4 &off) { m_fxOffset = off; }

  void SetPostProcessFx(const char *a_pShaderName);
#pragma endregion
  //////////////////////////////////////////////////////////////////////////
  /// @brief GetDeltaTime() a function to get the amount of time that has passed
  /// since the last time the CPU was polled
  ///	@return the amount of time that has passed in seconds since the CPU was
  /// last polled.
  //////////////////////////////////////////////////////////////////////////
  float GetDeltaTime();

private:
  // For the naming and creation of our application window
  bool loadApplicationData();
  char m_ApplicationTitle[256];

  typedef struct sApplicationWindow {
    std::string title;
    int width;
    int height;
    int xPos;
    int yPos;
    GLFWwindow *glfwWindowHandle;
  } ApplicationWindow;

  ApplicationWindow m_windowHandle;

public:
  inline ApplicationWindow &GetWindowHandle() { return m_windowHandle; }

private:
  // Rendering specific member variables that any child classes should not need
  // to concern themselves
  // with
  GLuint m_ShaderID = 0;

  // Post-processing vars
  bool m_postProcess = true;
  GLuint m_postFxShader = 0;
  float m_fxTime = 0;
  glm::vec4 m_fxColor;
  glm::vec4 m_fxPosition;
  glm::vec4 m_fxOffset;

  GLenum m_drawBuffers[1] = {GL_COLOR_ATTACHMENT0};

  GLuint m_screenQuadVAO = 0;
  GLuint m_screenQuadVBO = 0;

  GLuint m_frameBuffer = 0;
  GLuint m_renderTarget = 0;

  // Viewing matrices
  size_t m_cameraCounter = 0;
  std::vector<CameraHandle> m_cameras;

  LineRenderer *m_pLineRenderer;

  float m_fDeltaTime;
};

} // end namespace NHTV

//////////////////////////////////////////////////////////////////////////
#define APPLICATION_ENTRY(appClass)                                            \
  int main(int a_argc, char *a_argv[]) {                                       \
    NHTV::Application *app = new appClass();                                   \
    if (app->create(a_argc, a_argv) == true) {                                 \
      app->run();                                                              \
      app->destroy();                                                          \
    } else {                                                                   \
      /*NHTV::logError("Failed to setup application!\n");*/                    \
    }                                                                          \
    delete app;                                                                \
    return 0;                                                                  \
  }
//////////////////////////////////////////////////////////////////////////
