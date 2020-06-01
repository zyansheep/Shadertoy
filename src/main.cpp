#include <iostream>
#include <vector>
#include <array>
#include <math.h>

#include "Core/Window.h"
#include "Core/Gui.h"
#include "Core/Utils.h"

Window window = Window(1280,720, "ZyEngine - Shadertoy", false);
Gui gui = Gui(window);

#include "ShaderToy.h"
ShaderToy toy;

void setup(){
  std::cout << "Current Dir: " << Utils::GetCurrentWorkingDir() << '\n';

  gui.Init();
}

void draw(){
  glClearColor(255,255,255,255);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //Render Shader Here
  toy.Update(window);
  toy.Draw();
  
  gui.StartFrame();
  ImGui::Begin("Main Window");
    ImGui::Text("FPS: %d", window.GetFrameRate());
    ImGui::Text("Mouse Position: (%d, %d)", window.MouseX, window.MouseY);
    ImGui::Text("Current Time: %f", window.RunTime);

    toy.DisplayMenu();
  ImGui::End();
  
  gui.Render();
  gui.Draw();
}

int main(void){
  printf("OpenGL version is (%s)\n", glGetString(GL_VERSION));
  printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
  setup();
  window.Start(draw);
}
