#pragma once

#include <string>

#include "Rendering/Shader.h"

const std::string VertexShader = std::string(ZY_SHADER_VERSION) + R"(
  precision mediump float;
  precision mediump int;

  layout (location = 0) in vec3 vPosition;
  layout (location = 1) in vec2 vUV;

  void main() {
    gl_Position = vec4(vPosition, 1.0f);
  }
)";
const std::string FragmentShaderFooter = std::string(ZY_SHADER_VERSION) + R"(
  
  void main() {
      mainImage(gl_FragColor.xyzw, gl_FragCoord.xy);
  };
)";