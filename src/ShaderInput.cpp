#include "ShaderInput.h"


ShaderChannel::ShaderChannel(ShaderChannelType type, std::string data)
:Type(type), Data(data){}
const std::string ShaderChannel::m_DialogID = "ShaderChannelDialog";
// Store uniform location
void ShaderChannel::UpdateUniformLocation(const Shader& shader, const unsigned int& index) {
	m_UniformLocation = shader.GetUniformLocation("iChannel" + std::to_string(index));
}

#include <glad/glad.h>
//Get name of shader uniform from the type of the texture loaded
std::string ShaderChannel::GetShaderUniformName() const {
	switch(Sampler.GetType()){
		case GL_TEXTURE_1D:
			return "sampler1D";
		case GL_TEXTURE_2D:
			return "sampler2D";
		case GL_TEXTURE_3D:
			return "sampler3D";
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			return "samplerCube";
		case GL_TEXTURE_1D_ARRAY:
			return "sampler1DArray";
		case GL_TEXTURE_2D_ARRAY:
			return "sampler2DArray";
		default:
			//ZY_LOG_ERROR("ShaderChannel::GetShaderUniformName() Unimplemented OpenGL Texture Type");
			return "samplerUnknown";
	}
}
//Restart any time sensitive streams & update loaded
void ShaderChannel::Reset(){
	Loaded = true;
	switch (Type) {
	case NullChannel: break;
	case ImageFile:
		if(!Sampler.Loaded()){
			Sampler.Load(Data);
		}
		Loaded = Sampler.Loaded();
		break;
	default:
		Loaded = false;
		break;
	}
}
// Update the texture based on the type of shader channel, e.g. keyboard, movie, sound
void ShaderChannel::Update(){
	switch(Type){
		case NullChannel: break;
		case ImageFile:
			break;
		case SoundFile:
			// Update sound spectrum
			break;
		case Keyboard:
			// Get keyboard data from glfw
			break;
		default:
			std::cout << "Unimplemented ShaderChannel::Update Behavoir" << std::endl;
	}
}
// Send SamplerXX to Shader as Uniform
void ShaderChannel::Send(const Shader& shader, const unsigned int& index) const {
	Sampler.BindToUnit(index); //Bind texture set active to unit index
	shader.Uniform(m_UniformLocation, index);
}

#include "imgui.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"
// Display dialog box for ShaderChannel class
void ShaderChannel::DisplayMenu(){
	// Create ImGui Channel Type Selection Dropdown
	if( ImGui::BeginCombo("##Select Channel", ShaderChannelNameMap.at(Type).c_str() ) ){
		for(const std::pair<ShaderChannelType, std::string>& item : ShaderChannelNameMap){
			bool is_selected = (Type == item.first);
			if( ImGui::Selectable(item.second.c_str(), is_selected) ){
				Type = item.first; //Set type
			}
			if(is_selected){ ImGui::SetItemDefaultFocus(); }
		}
		ImGui::EndCombo();
	}

	// If applicable to open dialog...
	if(ShaderChannelFileDialogExtensions.count(Type) == 1){
		ImGui::SameLine();
		if ( ImGui::Button("Select File") ){
			igfd::ImGuiFileDialog::Instance()->CloseDialog(m_DialogID); //Close any open dialogs
			const static std::string dialogTitle = "Choose " + ShaderChannelNameMap.at(Type) + " File";
			igfd::ImGuiFileDialog::Instance()->OpenDialog(m_DialogID,
				dialogTitle.c_str(), ShaderChannelFileDialogExtensions.at(Type).c_str(), ".");
		}
		
		// Display dialog
		if(igfd::ImGuiFileDialog::Instance()->FileDialog( m_DialogID )){
			if (igfd::ImGuiFileDialog::Instance()->IsOk) {
				Data = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
			}
			igfd::ImGuiFileDialog::Instance()->CloseDialog( m_DialogID );
		}
	}
	if(Data != ""){
		ImGui::Text(Data.c_str());
	}
}
// Initialize ShaderInput class
ShaderInput::ShaderInput(unsigned int numchannels)
:m_NumChannels(numchannels){
    time = std::time(0);
    now = std::localtime(&time);
	iChannelResolution.resize(m_NumChannels);
	iChannelTime.resize(m_NumChannels);
	iChannel.resize(m_NumChannels);
	m_UniformLocations.resize(9 + 3 * m_NumChannels);
}

void ShaderInput::Reset(){
	Loaded = true;
	for(auto item : iChannel){
		item.Reset();
		if(!item.Loaded)
			Loaded = false;
	}
	time = std::time(0);
    now = std::localtime(&time);
}
// Retrieve uniform locations from shader by parsing FragmentShaderHeader for uniform names
#include <regex>
void ShaderInput::UpdateUniformLocations(const Shader& shader){
    std::smatch m;
    std::regex e ("uniform\\s+(\\w+)\\s+(\\w+);"); //Search through Fragment Header using regex

    //m_UniformLocations.reserve(9 + 3 * 4 * m_NumChannels); //Reserve vector size
    std::string s = FragmentShaderHeader;
	int i = 0;
    while (std::regex_search (s,m,e)) {
        s = m.suffix().str();
        std::string last;
        for(auto x:m){last = x;}
        m_UniformLocations[i] = shader.GetUniformLocation( last.c_str() );
		i++;
    }

    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[0].resolution");i++;
    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[1].resolution");i++;
    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[2].resolution");i++;
    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[3].resolution");i++;

    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[0].time");i++;
    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[1].time");i++;
    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[2].time");i++;
    m_UniformLocations[i] = shader.GetUniformLocation("iChannel[3].time");i++;
}

// Update uniform data from various sources
void ShaderInput::Update(const Window& window){
    iResolution = glm::vec3{window.GetWidth(), window.GetHeight(), 1.0f};
    iTime = window.RunTime;
    iGlobalTime = iTime;

    now = std::localtime(&time);
    iDate ={(float)now->tm_year, // Year
            (float)now->tm_mon, // Month
            (float)now->tm_mday, // Day
            (float)now->tm_hour * 60.0f * 60.0f + (float)now->tm_min * 60.0f + (float)now->tm_sec}; //Second of the day
    iFrame = window.FrameCount;
    iFrameRate = 1.0f / window.FrameTime;
    iTimeDelta = window.FrameTime;
    iMouse = {window.MouseX, window.MouseY, 0.0f, 0.0f};
    iSampleRate = 44100.0f;
    
    for(int i = 0; i < m_NumChannels; i++){
        iChannel[i].Update();
    }
}


// Send uniform data to shader
void ShaderInput::Send(const Shader& shader){
    int i=0;
    //IMPORTANT: Must be in exact order as FragmentShaderHeader
    shader.Uniform(m_UniformLocations[i], iResolution); i++;
    shader.Uniform(m_UniformLocations[i], iTime); i++;
    shader.Uniform(m_UniformLocations[i], iGlobalTime); i++;
    shader.Uniform(m_UniformLocations[i], iMouse); i++;
    shader.Uniform(m_UniformLocations[i], iDate); i++;
    shader.Uniform(m_UniformLocations[i], iSampleRate); i++;
    shader.Uniform(m_UniformLocations[i], iTimeDelta); i++;
    shader.Uniform(m_UniformLocations[i], iFrame); i++;
    shader.Uniform(m_UniformLocations[i], iFrameRate); i++;
    shader.Uniform(m_UniformLocations[i], 4, &iChannelResolution[0]); i++;
    shader.Uniform(m_UniformLocations[i], 4, &iChannelTime[0]); i++;

	//Bind, Activate and Send Texture Uniforms
	for (int i = 0; i < m_NumChannels; i++){
		iChannel[i].Send(shader, i);
	}

	//Send iChannel object uniforms
    shader.Uniform(m_UniformLocations[i], iChannelResolution[0]); i++;
    shader.Uniform(m_UniformLocations[i], iChannelResolution[1]); i++;
    shader.Uniform(m_UniformLocations[i], iChannelResolution[2]); i++;
    shader.Uniform(m_UniformLocations[i], iChannelResolution[3]); i++;

    shader.Uniform(m_UniformLocations[i], iChannelTime[0]); i++;
    shader.Uniform(m_UniformLocations[i], iChannelTime[1]); i++;
    shader.Uniform(m_UniformLocations[i], iChannelTime[2]); i++;
    shader.Uniform(m_UniformLocations[i], iChannelTime[3]); i++;
}