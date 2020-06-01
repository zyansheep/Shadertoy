#pragma once

#include <ctime>
#include <unordered_map>

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

enum ShaderChannelType {
	NullChannel = 0, // No channel selected
	Keyboard, //Texture corresponding to keyboard presses
	Webcam, //Texture showing current webcam image
	Microphone, //Texture containing microphone data
	ImageFile, //Regular Texture
	SoundFile, //Same as Microphone but from a file
	VideoFile, //TODO: figure out how to implement
	CubeMapFile, //Loads texture as cubemap
	VolumeFile, //Loads 3d Texture
	TextureBuffer //Renders texture from separate ShaderToy Script
};

// For displaying necessary combo elements in imgui menu
const std::unordered_map<ShaderChannelType, std::string> ShaderChannelNameMap({
	{ShaderChannelType::NullChannel, "No Texture"},
	{ShaderChannelType::Keyboard, "Keyboard"},
	{ShaderChannelType::Webcam, "Webcam"},
	{ShaderChannelType::Microphone, "Microphone"},
	{ShaderChannelType::ImageFile, "Image"},
	{ShaderChannelType::SoundFile, "Sound"},
	{ShaderChannelType::VideoFile, "Video"},
	{ShaderChannelType::CubeMapFile, "Cube Map"},
	{ShaderChannelType::VolumeFile, "3D Texture"},
	{ShaderChannelType::TextureBuffer, "Buffer"}
});
//Map file type to extension
const std::unordered_map<ShaderChannelType, std::string> ShaderChannelFileDialogExtensions({
	{ShaderChannelType::ImageFile, ".png\0.jpg\0.bmp\0\0"},
	{ShaderChannelType::SoundFile, ".ogg\0\0"},
	{ShaderChannelType::VideoFile, ".mp4\0\0"},
	{ShaderChannelType::CubeMapFile, ".png\0.jpg\0.bmp\0\0"},
	{ShaderChannelType::VolumeFile, ".bmp\0\0"}
});

class ShaderChannel {
public:
	ShaderChannel(ShaderChannelType type = ShaderChannelType::NullChannel, std::string data = "");
	void UpdateUniformLocation(const Shader& shader, const unsigned int& index); //Get uniforms
	std::string GetShaderUniformName() const;
	
	void Reset();
	void Update();
	void Send(const Shader& shader, const unsigned int& index) const; //Send uniforms

	ShaderChannelType Type;
	bool Loaded;
	std::string Data; //Can hold Path or other necessary loading data
	Texture Sampler; //Texture to send to shader
private:
	unsigned int m_UniformLocation;
	static const std::string m_DialogID;
public:
	void DisplayMenu();
	void DisplayFilePickers();
};

class ShaderInput {
public:
	ShaderInput(unsigned int numchannels);

	glm::vec3 					iResolution;
	float 						iTime;
	float 						iGlobalTime;
	glm::vec4 					iMouse;
	glm::vec4 					iDate;
	float 						iSampleRate = 44100 * 1.0f;
	int 						iFrame;
	float 						iTimeDelta;
	float 						iFrameRate;
	std::vector<glm::vec3> 		iChannelResolution;
	std::vector<float> 			iChannelTime;
	std::vector<ShaderChannel> 	iChannel;

	void Reset();
	void UpdateUniformLocations(const Shader& shader);
	void Update(const Window& window);
	void Send(const Shader& shader);

	void DisplayMenu();
	void OpenFilePicker();

	bool Loaded;
private:
	std::time_t time;
	std::tm* now;
	unsigned int m_NumChannels;

	std::vector<int> m_UniformLocations; //23 uniform locations when there are 4 Channels
public:
	unsigned int GetNumChannels(){ return m_NumChannels; }
};
