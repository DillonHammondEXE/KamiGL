#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string & path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr),
	m_Width(0), m_Height(0), m_BPP(0)
{
	// OpenGL expects image to start from the bottom left at 0,0 so we have to flip it
	// This is because PNG starts from top left
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4); // 4 is for RGBA (four channels)

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Must specificy these four parameters for all textures
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// void glTexImage2D(GLenum target,GLint level,GLint internalformat,GLsizei width,GLsizei height,GLint border,
	// GLenum format,GLenum type,const void * data);
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer))
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot)) // GL_TEXTURE 0-31 possible slotes, Mobile could have 8
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
