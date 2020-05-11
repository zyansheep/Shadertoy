#include "ShaderToy.h"

#include "Model/Model.h"
Model plane = ModelGeneration::Square(1,1);

ShaderToy::ShaderToy(std::string script, std::vector<std::shared_ptr<Texture>>& textures)
: ShaderToy(){
  SetTextures(textures);
  SetScript(script);
}
ShaderToy::ShaderToy(){
  m_VertexArray = plane.MakeVertexArray();
}
ShaderToy::~ShaderToy() {
  m_VertexArray->ClearUnalloc();
}

void ShaderToy::SetTextures(std::vector<std::shared_ptr<Texture>>& texture) {
  m_Textures = texture;
}
std::vector<std::shared_ptr<Texture>>& ShaderToy::GetTextures(){return m_Textures;}

void ShaderToy::SetScript(std::string script){
  std::ostringstream stream;
  stream << FragmentShaderHeader; //From Constants.h
  for (int i = 0; i < m_Textures.size(); i++){
    stream << "uniform sampler2D iChannel" << i << ";\n";
  }
  stream << script << FragmentShaderFooter; //From Constants.h
  //std::cout << stream.str() << std::endl;
  m_Shader = Shader(
    VertexShader, //From Constants.h
    stream.str());
  m_Input.GetUniformLocations(m_Shader); //Save all the uniform locations
}
void ShaderToy::Update(Window& window) {
  if(!m_VertexArray){ return; }
  m_Input.Update(window, *this);
}
void ShaderToy::Draw(){
  if(!m_VertexArray){ return; }
  if(!m_VertexArray->Loaded){ return; }

  m_Shader.Bind();
  for(auto& texture : m_Textures){
    texture->Bind();
  }
  m_Input.Send(m_Shader); // Update Uniforms
  m_VertexArray->Draw();
}

/*
void ShaderToy::KeyCallback(SDL_KeyboardEvent *event) {
	if (event->keysym.sym == SDLK_x)
	{
		if (screenshot() == false)
		{
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Save screenshot failed\n");
		}
	}
}

void ShaderToy::MouseButtonCallback(SDL_MouseButtonEvent *event)
{
    mInput.iMouse.setZ(-Math::abs(mInput.iMouse.z()));
    mInput.iMouse.setW(-Math::abs(mInput.iMouse.w()));
}

void ShaderToy::MouseButtonCallback(SDL_MouseButtonEvent *event)
{
    const Rectangle *rect = drawableRect();
    mInput.iMouse.setZ(Math::floor((event->x - rect->left()) / rect->width() * width()));
    mInput.iMouse.setW(Math::floor(height() - (event->y - rect->top()) / rect->height() * height()));
    mInput.iMouse.setX(mInput.iMouse.z());
    mInput.iMouse.setY(mInput.iMouse.w());
}

void ShaderToy::MouseMotionCallback(SDL_MouseMotionEvent *event)
{
    const Rectangle *winrect = rect();
    const Rectangle *drawrect = drawableRect();

    float widthRatio = drawrect->width() / winrect->width();
    float heightRatio = drawrect->height() / winrect->height();

    mInput.iMouse.setX(Math::floor(event->x - drawrect->left()) * widthRatio);
    mInput.iMouse.setY(Math::floor(height() - (event->y - drawrect->top()) * heightRatio));
}*/
