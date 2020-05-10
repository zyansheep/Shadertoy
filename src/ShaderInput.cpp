#include "ShaderInput.h"

#include "ShaderToy.h"
ShaderInput::ShaderInput(){
    time = std::time(0);
    now = std::localtime(&time);
}
void ShaderInput::GetUniformLocations(Shader& shader){
    uniformLocations[0] = shader.GetUniformLocation("iResolution");
    uniformLocations[1] = shader.GetUniformLocation("iTime");
    uniformLocations[2] = shader.GetUniformLocation("iGlobalTime");
    uniformLocations[3] = shader.GetUniformLocation("iMouse");
    uniformLocations[4] = shader.GetUniformLocation("iDate");
    uniformLocations[5] = shader.GetUniformLocation("iSampleRate");
    uniformLocations[6] = shader.GetUniformLocation("iChannelResolution");
    uniformLocations[7] = shader.GetUniformLocation("iChannelTime");
    uniformLocations[8] = shader.GetUniformLocation("iTimeDelta");
    uniformLocations[9] = shader.GetUniformLocation("iFrame");
    uniformLocations[10] = shader.GetUniformLocation("iFrameRate");

    uniformLocations[11] = shader.GetUniformLocation("iChannel0");
    uniformLocations[12] = shader.GetUniformLocation("iChannel1");
    uniformLocations[13] = shader.GetUniformLocation("iChannel2");
    uniformLocations[14] = shader.GetUniformLocation("iChannel3");

    uniformLocations[15] = shader.GetUniformLocation("iChannel[0].resolution");
    uniformLocations[16] = shader.GetUniformLocation("iChannel[1].resolution");
    uniformLocations[17] = shader.GetUniformLocation("iChannel[2].resolution");
    uniformLocations[18] = shader.GetUniformLocation("iChannel[3].resolution");

    uniformLocations[19] = shader.GetUniformLocation("iChannel[0].time");
    uniformLocations[20] = shader.GetUniformLocation("iChannel[1].time");
    uniformLocations[21] = shader.GetUniformLocation("iChannel[2].time");
    uniformLocations[22] = shader.GetUniformLocation("iChannel[3].time");
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
    shader.Uniform(uniformLocations[0], iResolution);
    shader.Uniform(uniformLocations[1], iTime);
    shader.Uniform(uniformLocations[2], iGlobalTime);
    shader.Uniform(uniformLocations[3], iMouse);
    shader.Uniform(uniformLocations[4], iDate);
    shader.Uniform(uniformLocations[5], iSampleRate);
    shader.Uniform(uniformLocations[6], 4, iChannelResolution);
    shader.Uniform(uniformLocations[7], 4, iChannelTime);
    shader.Uniform(uniformLocations[8], iTimeDelta);
    shader.Uniform(uniformLocations[9], iFrame);
    shader.Uniform(uniformLocations[10], iFrameRate);

    shader.Uniform(uniformLocations[11], 0);
    shader.Uniform(uniformLocations[12], 1);
    shader.Uniform(uniformLocations[13], 2);
    shader.Uniform(uniformLocations[14], 3);

    shader.Uniform(uniformLocations[15], iChannelResolution[0]);
    shader.Uniform(uniformLocations[16], iChannelResolution[1]);
    shader.Uniform(uniformLocations[17], iChannelResolution[2]);
    shader.Uniform(uniformLocations[18], iChannelResolution[3]);

    shader.Uniform(uniformLocations[19], iChannelTime[0]);
    shader.Uniform(uniformLocations[20], iChannelTime[1]);
    shader.Uniform(uniformLocations[21], iChannelTime[2]);
    shader.Uniform(uniformLocations[22], iChannelTime[3]);
}