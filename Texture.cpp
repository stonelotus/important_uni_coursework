#include <glad/glad.h>
#include "stb_image.h"

#include "Texture.h"
#include <iostream>

Texture::Texture(BlockType block_type)
    : m_LocalBuffer(nullptr),
    m_Width(0), m_Height(0), m_BPP(0)
{
    switch (block_type) {
        case BlockType::AIR:
			//m_FilePath = "C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\assets\\minecraft\\textures\\block\\dead_bubble_coral_block.png";
            return;
            break;
        case BlockType::DIRT:
			m_FilePath = "C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\assets\\minecraft\\textures\\block\\dirt.png";
			break;
        case BlockType::STONE:
            m_FilePath = "C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\assets\\minecraft\\textures\\block\\stone.png";
            break;
        default:
            m_FilePath = "C:\\Users\\asus\\source\\repos\\minecraft_clone\\mainProj\\assets\\minecraft\\textures\\block\\dead_bubble_coral_block.png";
			break;
    }
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
