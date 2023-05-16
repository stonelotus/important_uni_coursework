#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "structures.h"

class Texture
{
public:
    Texture(BlockType block_type);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;



private:
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;

    unsigned int m_RendererID;
};

#endif
