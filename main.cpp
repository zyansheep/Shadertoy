#include <iostream>
#include <vector>
#include <array>
#include <math.h>

#include <glm/gtx/color_space.hpp>
#include "glm/gtx/string_cast.hpp"

#include "World/World.h"
#include "World/CameraController.h"
#include "Core/Gui.h"
#include "Model/Model.h"
#include "Core/Functions.h"

Window window = Window(1280,720, "Graphics Engine Testing");

Gui gui = Gui(window);
Camera camera = Camera(&window, glm::vec3{0, 0, 8});
CameraController controller = CameraController(&window);
World world = World(&window, &camera);

Object* plane;
Object* cube;
Object* sphere;
Object* suzanne;
Shader shader;
Shader fileShader;

Model cubeModel = ModelGeneration::Cube();
Model sphereModel = ModelGeneration::Icosphere(4);
Model suzanneModel = ModelGeneration::ObjFile("test/suzanne.obj");

ImVec4 clearColor = ImVec4(0.5,0.5,1.0,1.0);
glm::vec3 color = glm::vec4(1.0,1.0,1.0,1.0);

void setup(){
  std::cout << "Current Dir: " << GetCurrentWorkingDir() << '\n';
  shader = ShaderGeneration::Test();
  fileShader = ShaderGeneration::File("test/shaders/main.vert", "test/shaders/main.frag");
  
  cube = new Object(cubeModel.MakeVertexArray(), shader);
  cube->Translate(glm::vec3(0, 5, 0));
  
  sphere = new Object(sphereModel.MakeVertexArray(), fileShader);
  std::vector<glm::vec3> positions = {glm::vec3{0,0,0}, glm::vec3{5,0,0}, glm::vec3{0,0,5}};
  sphere->MatrixArray.resize(positions.size(), glm::mat4(1.0f));
  for(uint i=0;i<positions.size();i++){
    sphere->Translate(positions[i], i);
  }
  
  suzanne = new Object(suzanneModel.MakeVertexArray(), shader);
  suzanne->Translate(glm::vec3(-5,0,0));
  
  Buffer* plane_attrib_buffer = new Buffer({
    {ShaderType::Float3, "a_position"},
    {ShaderType::Float3, "a_color"}
  });
  float plane_attrib_array[] = {
    0,  0,  0,  255,  0,  0,
    0,  0,100,    0,255,  0,
    0,100,100,    0,  0,255,
    0,100,  0,  255,  0,255
  };
  plane_attrib_buffer->New(plane_attrib_array, sizeof(plane_attrib_array));
  
  Buffer* plane_index_buffer = new Buffer();
  uint plane_index_array[] {
    0,1,2,
    0,2,3
  };
  plane_index_buffer->New(plane_index_array, sizeof(plane_index_array));
  
  plane = new Object(new VertexArray({plane_attrib_buffer}, plane_index_buffer), shader);
  plane->Rotate(90.0f, glm::vec3(0,0,1));
  plane->Translate({-8,-50,-50});
  
  world.AddObject(cube);
  world.AddObject(plane);
  world.AddObject(sphere);
  world.AddObject(suzanne);
  
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  
  gui.Init();
}

void loop(){
  glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  controller.Update();
  
  color = glm::rgbColor(glm::vec3{fmod(window.RunTime*10, 1), 1,1});
  shader.Uniform(shader.GetUniformLocation("u_color"), glm::vec4(color,1.0));
  
  if(window.GetKey(GLFW_KEY_R)){
    controller.Unbind();
    camera = Camera(&window, glm::vec3{0, 0, 8});
    controller.Bind(&camera);
  }
  if(window.GetKey(GLFW_KEY_ESCAPE)){
    controller.Unbind();
  }
  if(window.GetKey(GLFW_KEY_B)){
    controller.Bind(&camera);
  }
  
  cube->Rotate(90.0f/60, glm::vec3(0.0f, 0.0f, 1.0f));
  
  world.Render();
  world.Draw();
  
  gui.Begin("Hello There~");
    ImGui::Button("This is a button");
    ImGuiColorEditFlags misc_flags = (0) | (ImGuiColorEditFlags_NoDragDrop) | (ImGuiColorEditFlags_AlphaPreviewHalf) | (ImGuiColorEditFlags_NoOptions);
    ImGui::ColorPicker4("##picker", (float*)&clearColor, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
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
  window.Start(loop);
}
