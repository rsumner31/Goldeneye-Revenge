//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	font.cpp - This is the font face class and will be used
//	in the TextString class as the font map
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the class constructor
//
//////////////////////////////////////////////////////////////
FontFace::FontFace()
{
	fontFaceTexture = NULL;	
}

FontFace::~FontFace()
{
	delete fontFaceTexture;
	fontFaceTexture = NULL;
}

//////////////////////////////////////////////////////////////
//
//	This function loads the font texture
//
//////////////////////////////////////////////////////////////
void FontFace::loadFont(char *fileName, int charWidth, int charHeight)
{
	fontFaceTexture = new Texture();
	fontFaceTexture->loadTexture(fileName);

	characterHeight = charHeight;
	characterWidth = charWidth;
	mapWidth = fontFaceTexture->getWidth();
	mapHeight = fontFaceTexture->getHeight();
}

//////////////////////////////////////////////////////////////
//
//	This function generates the texture id
//
//////////////////////////////////////////////////////////////
void FontFace::genFont()
{
	fontId = fontFaceTexture->genTexture();
}

//////////////////////////////////////////////////////////////
//
//	This function returns the font texture
//
//////////////////////////////////////////////////////////////
Texture* FontFace::getTexture()
{
	return fontFaceTexture;
}

//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	font.cpp - This file will be the file we use to create
//	our custom font faces and display them because OpenGL
//	sucks and doesnt have any documentation on how to load
//	custom bitmap fonts.
//
//	Fonts must be in the format of characters must be in ASCII 
//	order ascending starting from 32 to 126
//
//
//	This is the class constructor
//
//////////////////////////////////////////////////////////////
TextString::TextString()
{
	fontPosition = Vector3(0, 0, 0);
	fontScale = Vector3(1, 1, 1);
	fontRotation = Vector4(0, 0, 0, 0);	
	fontColour = Colour(0, 0, 0, 255);
	fontFace = NULL;
	scaleMag = 1.0f;
}

TextString::TextString(string str)
{
	fontPosition = Vector3(0, 0, 0);
	fontScale = Vector3(1, 1, 1);
	fontRotation = Vector4(0, 0, 0, 0);	
	fontColour = Colour(0, 0, 0, 255);
	fontFace = NULL;
	scaleMag = 1.0f;
	fontStr = str;
}

TextString::~TextString()
{

}

//////////////////////////////////////////////////////////////
//
//	This function checks for a mouse press
//
//////////////////////////////////////////////////////////////
void TextString::checkMouse(int x, int y, int button, void callBack())
{
	//	we need to check the bounds of the object with the
	//	mouse coordinates, returns true if it is within bounds
	//	false if not.
	if (x > fontPosition.x && x < fontPosition.x + (getCalculatedSize().width))
	{
		if (y > fontPosition.y && y < fontPosition.y + (getCalculatedSize().height))
		{
			callBack();
		}
	}
}

//////////////////////////////////////////////////////////////
//
//	This function draws the string
//
//////////////////////////////////////////////////////////////
void TextString::drawString()
{		
	//	We need to calculate the coordinates for each character based on
	//	the char width/height
	for (int charPos = 0, xCounter = 0, yPos = 0; charPos < (int)fontStr.size(); charPos++, xCounter++)
	{
		//	Get each character and convert it to ASCII
		int chr = fontStr[charPos];		

		if (fontStr[charPos] == '\n')
		{
			yPos += charHeight;
			xCounter = -1;

			continue;
		}

		//	Start ascii is 32, end is 126 so how many into the bitmap is this?
		int pos = chr - 32;

		//	Now we find out what row it is on from this
		//	How many chars per row
		int charPerRow = abs((fontFace->mapWidth / charWidth));
		int row = abs(pos / charPerRow) + 1;

		//	How many in is it?
		int col = pos - ((row - 1) * charPerRow);	

		//	Texture coordinates based on 1.0 = max coord (100%)
		//	x = 1 / width * wanted width, y = 1 / height * wanted height
		Vector2 _pos = Vector2		
		(
			(1.0 / fontFace->mapWidth) * (col * charWidth), 
			(1.0 / fontFace->mapHeight) * ((row - 1) * charHeight)	
		);

		Vector2 _pos2 = Vector2		
		(
			(1.0 / fontFace->mapWidth) * ((col * charWidth) + charWidth), 
			(1.0 / fontFace->mapHeight) * (((row - 1) * charHeight) + charHeight)
		);

		//	Set the texture coordinates
		Rect texCoords = Rect(_pos.x, _pos.y, _pos2.x, _pos2.y);
		
		//	Create the object of the character
		Object *character = new Object();

		character->setTexCoord(texCoords);
		character->loadTexture(fontFace->getTexture());
		character->size(charWidth * scaleMag, charHeight * scaleMag);
		character->position(fontPosition.x + (xCounter * characterSpacing), fontPosition.y + yPos);
		character->rotate(fontRotation);
		character->scale(fontScale);
		character->setTransparency(true);
		character->color(fontColour);
		character->drawObject();
		
		delete character;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function sets the character spacing
//
//////////////////////////////////////////////////////////////
void TextString::setCharacerSpacing(int spacing)
{
	characterSpacing = spacing;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the text to be displayed
//
//////////////////////////////////////////////////////////////
void TextString::setText(char *str)
{
	fontStr = string(str);
}

void TextString::setText(string str)
{
	fontStr = str;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the font to use
//
//////////////////////////////////////////////////////////////
void TextString::setFontFace(FontFace *face)
{
	fontFace = face;

	charWidth = fontFace->characterWidth;
	charHeight = fontFace->characterHeight;
	characterSpacing = fontFace->characterWidth;
}

//////////////////////////////////////////////////////////////
//
//	This function scales the text
//
//////////////////////////////////////////////////////////////
void TextString::scale(Vector3 _scale)
{
	fontScale = _scale;
}

void TextString::scale(float x, float y, float z)
{
	fontScale = Vector3(x, y, z);
}

//////////////////////////////////////////////////////////////
//
//	This function positions the text
//
//////////////////////////////////////////////////////////////
void TextString::position(Vector3 _position)
{
	fontPosition = _position;
}

void TextString::position(float x, float y, float z)
{
	fontPosition = Vector3(x, y, z);
}

void TextString::position(Vector2 _position)
{
	fontPosition = Vector3(_position.x, _position.y, 0);
}

void TextString::position(float x, float y)
{
	fontPosition = Vector3(x, y, 0);
}

//////////////////////////////////////////////////////////////
//
//	This function rotates the text
//
//////////////////////////////////////////////////////////////
void TextString::rotate(Vector4 _rotation)
{
	fontRotation = _rotation;
}

void TextString::rotate(float x, float y, float z, float a)
{
	fontRotation = Vector4(x, y, z, a);
}

//////////////////////////////////////////////////////////////
//
//	This function returns the position
//
//////////////////////////////////////////////////////////////
Vector3 TextString::getPosition()
{
	return fontPosition;
}

//////////////////////////////////////////////////////////////
//
//	This function returns the calculated box size of the string
//
//////////////////////////////////////////////////////////////
Size TextString::getCalculatedSize()
{
	return Size(characterSpacing * string(fontStr).size(), charHeight);
}

//////////////////////////////////////////////////////////////
//
//	This function sets the colour of the object
//
//////////////////////////////////////////////////////////////
void TextString::color(int r, int g, int b)
{
	fontColour = Colour(r, g, b, 255);
}

void TextString::color(float r, float g, float b)
{
	fontColour = Colour(r, g, b, 1.0f);
}

void TextString::color(int r, int g, int b, int a)
{
	fontColour = Colour(r, g, b, a);
}

void TextString::color(float r, float g, float b, float a)
{
	fontColour = Colour(r, g, b, a);
}

void TextString::color(Colour _colour)
{
	fontColour = _colour;
}
