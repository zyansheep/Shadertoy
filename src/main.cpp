#include <iostream>
#include <vector>
#include <array>
#include <math.h>

#include "World/World.h"
#include "World/CameraController.h"
#include "Core/Gui.h"
#include "Model/Model.h"
#include "Core/Functions.h"

#include "ShaderToy.h"

Window window = Window(1280,720, "ZyEngine - Shadertoy");
Gui gui = Gui(window);
ShaderToy toy;

std::string shader_path = "shaders/BallOfFire.shader";
std::vector<std::string> texture_paths;
Shader shader;

void setup(){
  std::cout << "Current Dir: " << GetCurrentWorkingDir() << '\n';
  //AttemptCompile();
  
  toy.SetScript(readFileSync(shader_path));

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  gui.Init();
}
void AttemptCompile(){
  /*if(std::filesystem::exists(shader_path)){
    
  }
  for(std::string& str : texture_paths){
    if(std::filesystem::exists(shader_path)){
      toy.SetScript(readFileSync(shader_path));
    }
  }*/
}

void draw(){
  glClearColor(255,255,255,255);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //Render Shader Here
  toy.Update(window);
  toy.Draw();
  
  gui.Begin("Hello There~");
    ImGui::Button("This is a button");
    ImGuiColorEditFlags misc_flags = (0) | (ImGuiColorEditFlags_NoDragDrop) | (ImGuiColorEditFlags_AlphaPreviewHalf) | (ImGuiColorEditFlags_NoOptions);
    ImGui::Text("FrameTime: %f seconds", window.GetFrameTime());
    ImGui::Text("Mouse Position: (%d, %d)", window.MouseX, window.MouseY);
    ImGui::Text("Current Time: %f", window.RunTime);
  gui.End();
  gui.Render();
  gui.Draw();
}

int main(void){
  printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
  printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
  setup();
  window.Start(draw);
}
