//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	texture.h - This is the header file for the texture class
//	
//////////////////////////////////////////////////////////////

class Texture
{
	public:
		TGAImg *texture;
		GLuint textureId;

		Texture::Texture();
		Texture::~Texture();
		void Texture::bindTexture(GLuint &textureVBO);	
		//void Texture::drawTexture(RECT rect);
		//void Texture::drawTexture(int x, int y, int width, int height, int marginTop = 0, int marginLeft = 0);
		void Texture::flipImage();
		GLsizei Texture::getBPP();
		GLsizei Texture::getHeight();
		const GLvoid *Texture::getImage();
		Size Texture::getSize();
		GLsizei Texture::getWidth();				
		void Texture::loadTexture(char *fileName, bool genTex = false);
		GLuint Texture::genTexture(int mode = GL_CLAMP);
};
