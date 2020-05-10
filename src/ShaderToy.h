#pragma once

#include <memory>
#include <string>
#include <vector>
#include <sstream>

#include "Rendering/Texture.h"
#include "Rendering/VertexArray.h"

//All the Vertex Shader constants and stuff
#include "Constants.h"
#include "ShaderInput.h"

#include "Core/Window.h"

class ShaderToy {
public:
  ShaderToy(std::string script, std::vector<std::shared_ptr<Texture>>& textures);
  ShaderToy();
  ~ShaderToy();

  void SetTextures(std::vector<std::shared_ptr<Texture>>& texture);
  std::vector<std::shared_ptr<Texture>>& GetTextures();
  void SetScript(std::string script);

  void Update(Window& window);
  void Draw();

  /*void KeyCallback(int key, int scancode, int action, int mods);
  void MouseButtonCallback(int button, int action, int mods);
  void MouseMotionCallback(double xpos, double ypos);*/

private:
  ShaderInput m_Input; //Internal utility class
  VertexArray* m_VertexArray; //Generated for display

  Shader m_Shader;
  std::vector<std::shared_ptr<Texture>> m_Textures;
};
