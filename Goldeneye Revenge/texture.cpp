//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Texture.cpp - This file handles all texture functions
//
//////////////////////////////////////////////////////////////

//	Include all the needed headers
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the class constructor
//
//////////////////////////////////////////////////////////////
Texture::Texture()
{
	textureId = 0;
	texture = new TGAImg();
}

Texture::~Texture()
{
	delete this->texture;
	this->texture = NULL;
	this->textureId = NULL;
}

//////////////////////////////////////////////////////////////
//
//	This function binds the texture to a given VBO
//
//////////////////////////////////////////////////////////////
void Texture::bindTexture(GLuint &textureVBO)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	
	//	Bind the texture VBO
	glBindBufferARB(GL_ARRAY_BUFFER, textureVBO);     
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);
}

//////////////////////////////////////////////////////////////
//
//	This function returns the bits per pixel of the texture
//
//////////////////////////////////////////////////////////////
GLsizei Texture::getBPP()
{
	return texture->GetBPP();
}

//////////////////////////////////////////////////////////////
//
//	This function returns the height of the texture
//
//////////////////////////////////////////////////////////////
GLsizei Texture::getHeight()
{
	return texture->GetHeight();
}

//////////////////////////////////////////////////////////////
//
//	This function returns the image data of the texture
//
//////////////////////////////////////////////////////////////
const GLvoid *Texture::getImage()
{
	return texture->GetImg();
}


//////////////////////////////////////////////////////////////
//
//	This function returns the image size in a structure
//
//////////////////////////////////////////////////////////////
Size Texture::getSize()
{
	return Size(texture->GetWidth(), texture->GetHeight());
}

//////////////////////////////////////////////////////////////
//
//	This function returns the width of the texture
//
//////////////////////////////////////////////////////////////
GLsizei Texture::getWidth()
{
	return texture->GetWidth();
}

//////////////////////////////////////////////////////////////
//
//	This function loads in a texture from a file. Only accepts
//	TGA format, and returns the generated texture id
//
//////////////////////////////////////////////////////////////
void Texture::loadTexture(char *fileName, bool genTex)
{
	if (texture->Load(fileName) != IMG_OK)
	{
		std::string errorMessage;
		errorMessage = "Could not load texture file: ";
		errorMessage += fileName;

		error((char *)errorMessage.c_str());
		return;
	}

	if (genTex)
	{
		genTexture();
	}
}

//////////////////////////////////////////////////////////////
//
//	This function generates the texture
//
//////////////////////////////////////////////////////////////
GLuint Texture::genTexture(int mode)
{
	glPushMatrix();

	//	Generate and bind the textures
	glGenTextures(1, &this->textureId);
	glBindTexture(GL_TEXTURE_2D, this->textureId);

	//	Set the mode of the texture
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);	

	//	Set the parameters of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);

	//	If the bit depth is 24 bit
	if (getBPP() == 24)
	{		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWidth(), getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, getImage());	
	}
	else if (getBPP() == 32)
	{		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getWidth(), getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, getImage());		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	}
	else
	{
		error("Texture has invalid BPP");
		return -1;
	}

	glPopMatrix();

	return textureId;
}