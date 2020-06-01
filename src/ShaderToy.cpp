#include "ShaderToy.h"

#include "Model/Model.h"
Model plane = ModelGeneration::Square(1,1);

ShaderToy::ShaderToy(unsigned  int channels)
: m_Input(channels){
	m_VertexArray = plane.MakeVertexArray();
}

// Compile passed ShaderToy Script and set m_Ready flag
void ShaderToy::Compile(const std::string& script){
	std::ostringstream stream;
	stream << FragmentShaderHeader; //From Constants.h
	//Setup ShaderInput and other ShaderChannels + reset various uniforms
	m_Input.Reset();

	//Get neccessary input channels
	for (int i = 0; i < m_Input.iChannel.size(); i++){
		if(m_Input.iChannel[i].Type != ShaderChannelType::NullChannel){
			stream << "uniform " << m_Input.iChannel[i].GetShaderUniformName() << " iChannel" << i << ";\n";
		}
	}
	stream << script << FragmentShaderFooter; //From Constants.h
	//std::cout << stream.str() << std::endl;
	m_Shader = Shader(
	VertexShader, //From Constants.h
	stream.str());

	m_Input.UpdateUniformLocations(m_Shader); //Save all the uniform locations
	
	m_Ready = (m_Shader.Loaded() && m_Input.Loaded && m_VertexArray->Loaded);
}
void ShaderToy::Update(Window& window) {
	if(!m_VertexArray){ return; }
	m_Input.Update(window);
}
void ShaderToy::Draw(){
	if(!m_Ready){ return; }

	m_Shader.Bind();
	m_Input.Send(m_Shader); // Update Uniforms & Bind Textures
	m_VertexArray->Draw();
}

#include "imgui.h"
#include "ImGuiFileDialog/ImGuiFileDialog.h"

// Display ImGui Menu
void ShaderToy::DisplayMenu(){
	// Main Shader Selection Button
	if (ImGui::Button("Select Shader")){
		igfd::ImGuiFileDialog::Instance()->OpenDialog("ShaderToyDialog",
			"Choose Shader File", ".shader\0.frag\0.glsl\0\0", ".");
	}

	// Display all channel Entries (combos and selectors)
	for(ShaderChannel& channel : m_Input.iChannel){
		channel.DisplayMenu();
	}

	//Detect Dialog Selection
	if (igfd::ImGuiFileDialog::Instance()->FileDialog("ShaderToyDialog"))	{
		if (igfd::ImGuiFileDialog::Instance()->IsOk) {
			std::string path = igfd::ImGuiFileDialog::Instance()->GetFilepathName();
			std::cout << "Loading ShaderToy Script: " << path << std::endl;
			
			Compile(Utils::ReadFileSync(path));
		}
		// close
		igfd::ImGuiFileDialog::Instance()->CloseDialog("ShaderToyDialog");
	}
	ImGui::Text("ShaderToy Ready: %d", m_Ready);
	ImGui::Text("ShaderInput Ready: %d", m_Input.Loaded);
	for(int i=0;i<m_Input.iChannel.size();i++){
		ImGui::Text("Shader iChannel%d Ready: %d", i, m_Input.iChannel[i].Loaded);
	}
}