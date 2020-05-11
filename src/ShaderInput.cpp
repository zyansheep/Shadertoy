#include "ShaderInput.h"

#include "ShaderToy.h"
ShaderInput::ShaderInput(){
    time = std::time(0);
    now = std::localtime(&time);
    
}

#include <regex>
void ShaderInput::GetUniformLocations(Shader& shader){
    uniformLocations.push_back(shader.GetUniformLocation("iChannel0"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel1"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel2"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel3"));

    uniformLocations.push_back(shader.GetUniformLocation("iChannel[0].resolution"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel[1].resolution"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel[2].resolution"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel[3].resolution"));

    uniformLocations.push_back(shader.GetUniformLocation("iChannel[0].time"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel[1].time"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel[2].time"));
    uniformLocations.push_back(shader.GetUniformLocation("iChannel[3].time"));

    std::smatch m;
    std::regex e ("uniform\\s+(\\w+)\\s+(\\w+);"); //Search through Fragment Header using regex

    uniformLocations.reserve(25); //Reserve vector size
    std::string s = FragmentShaderHeader;
    while (std::regex_search (s,m,e)) {
        std::string last;
        for (auto x:m) { std::cout << x << " "; last = x; } 
        std::cout << std::endl;
        s = m.suffix().str();
        uniformLocations.push_back(shader.GetUniformLocation(last.c_str()));
    }
}
void ShaderInput::Update(Window& window, ShaderToy& toy){
    iResolution = {(float)window.GetWidth(), (float)window.GetHeight(), 1.0f};
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
    
    auto& textures = toy.GetTextures();
    auto size = textures.size();
    if(size <= 4){
        for(int i=0;i<size;i++){
            /*iChannelResolution[i] = {
                (float)textures[i]->Width,
                (float)textures[i]->Height, 1.0f};*/
            iChannelTime[i] = 0.0f;
        }
    }
}

void ShaderInput::Send(Shader& shader){
    int i=0;

    shader.Uniform(uniformLocations[i], 0); i++;
    shader.Uniform(uniformLocations[i], 1); i++;
    shader.Uniform(uniformLocations[i], 2); i++;
    shader.Uniform(uniformLocations[i], 3); i++;

    shader.Uniform(uniformLocations[i], iChannelResolution[0]); i++;
    shader.Uniform(uniformLocations[i], iChannelResolution[1]); i++;
    shader.Uniform(uniformLocations[i], iChannelResolution[2]); i++;
    shader.Uniform(uniformLocations[i], iChannelResolution[3]); i++;

    shader.Uniform(uniformLocations[i], iChannelTime[0]); i++;
    shader.Uniform(uniformLocations[i], iChannelTime[1]); i++;
    shader.Uniform(uniformLocations[i], iChannelTime[2]); i++;
    shader.Uniform(uniformLocations[i], iChannelTime[3]); i++;

    //IMPORTANT: Must be in exact order as FragmentShaderHeader
    shader.Uniform(uniformLocations[i], iResolution); i++;
    shader.Uniform(uniformLocations[i], iTime); i++;
    shader.Uniform(uniformLocations[i], iGlobalTime); i++;
    shader.Uniform(uniformLocations[i], iMouse); i++;
    shader.Uniform(uniformLocations[i], iDate); i++;
    shader.Uniform(uniformLocations[i], iSampleRate); i++;
    shader.Uniform(uniformLocations[i], 4, iChannelResolution); i++;
    shader.Uniform(uniformLocations[i], 4, iChannelTime); i++;
    shader.Uniform(uniformLocations[i], iTimeDelta); i++;
    shader.Uniform(uniformLocations[i], iFrame); i++;
    shader.Uniform(uniformLocations[i], iFrameRate); i++;
}