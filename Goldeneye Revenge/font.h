//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	font.h - This is the header file for the font class
//
//////////////////////////////////////////////////////////////

class FontFace
{
	public:
		GLint fontId;
		int characterWidth, characterHeight;
		int mapWidth, mapHeight;

		FontFace::FontFace();
		FontFace::~FontFace();

		void FontFace::loadFont(char *fileName, int charWidth, int charHeight);
		void FontFace::genFont();
		Texture* FontFace::getTexture();

	private:
		Texture *fontFaceTexture;		
};

class TextString
{
	public:
		float scaleMag;

		TextString::TextString();
		TextString::TextString(string str);
		TextString::~TextString();

		void TextString::checkMouse(int x, int y, int button, void callBack());
		void TextString::color(int r, int g, int b);
		void TextString::color(float r, float g, float b);
		void TextString::color(int r, int g, int b, int a);
		void TextString::color(float r, float g, float b, float a);
		void TextString::color(Colour _colour);
		void TextString::drawString();
		void TextString::genFont();
		Size TextString::getCalculatedSize();
		Vector3 TextString::getPosition();	
		void TextString::scale(Vector3 _scale);
		void TextString::scale(float x, float y, float z);		
		void TextString::setCharacerSpacing(int spacing);
		void TextString::setText(char *str);
		void TextString::setText(string str);
		void TextString::setFontFace(FontFace *face);	
		void TextString::position(Vector3 _position);
		void TextString::position(float x, float y, float z);
		void TextString::position(Vector2 _position);
		void TextString::position(float x, float y);
		void TextString::rotate(Vector4 _rotation);
		void TextString::rotate(float x, float y, float z, float a);

	private:
		Vector3 fontPosition, fontScale;
		Vector4 fontRotation;
		FontFace *fontFace;
		Colour fontColour;

		int charWidth, charHeight;
		string fontStr;
		int characterSpacing;
};