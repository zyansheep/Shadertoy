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
#include "Core/Utils.h"

#include "Core/Window.h"

class ShaderToy {
public:
	ShaderToy(unsigned int channels = 4);

	void Compile(const std::string& script);

	void Update(Window& window);
	void Draw();

	void DisplayMenu();
private:
	bool m_Ready;
	ShaderInput m_Input; //Manages Inputs to the shader
	std::shared_ptr<VertexArray> m_VertexArray; //Generated for display

	Shader m_Shader;
};
