#pragma once

#include <ctime>

#include <glm/glm.hpp>

#include "Core/Window.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"

const std::string FragmentShaderHeader = std::string(ZY_SHADER_VERSION) + R"(
  precision mediump float;
  precision mediump int;
  uniform vec3       iResolution;
  uniform float      iTime;
  uniform float      iGlobalTime;
  uniform vec4       iMouse;
  uniform vec4       iDate;
  uniform float      iSampleRate;
  uniform int        iFrame;
  uniform float      iTimeDelta;
  uniform float      iFrameRate;
  uniform vec3       iChannelResolution[4];
  uniform float      iChannelTime[4];
)";

class ShaderToy;
class ShaderInput {
public:
    ShaderInput();

    glm::vec3 iResolution;
    float iTime;
    float iGlobalTime;
    glm::vec4 iMouse;
    glm::vec4 iDate;
    float iSampleRate = 44100 * 1.0f;
    int iFrame;
    float iTimeDelta;
    float iFrameRate;
    glm::vec3 iChannelResolution[4];
    float iChannelTime[4];

    void GetUniformLocations(Shader& shader);
    void Update(Window& window, ShaderToy& toy);
    void Send(Shader& shader);
private:
    std::time_t time;
    std::tm* now;

    unsigned int uniformLocations[21]; //21 uniform locations
};
