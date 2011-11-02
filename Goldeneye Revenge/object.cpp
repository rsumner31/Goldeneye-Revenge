//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	object.cpp - This file will handle all object drawings
//	because we need to store and manage all the co-ordinates
//	of each object so we can do hit testing and collision
//	detecting.
//
//////////////////////////////////////////////////////////////

//	Let us include the main file for all the system includes and prototypes
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	Player class constructor
//
//////////////////////////////////////////////////////////////
Object::Object()
{
	vector = new list<Vector3>;	
	scaleVector = new list<Vector3>;
	rotationVector = new list<Vector4>;
	positionVector = Vector3(0, 0, 0);
	smallestVector = Vector3(0, 0, 0);
	largestVector = Vector3(0, 0, 0);	
	color(255, 255, 255);
	transparency = false;
	useTexture = false;
	isClickable = true;
	objectType = GL_POLYGON;
	texture = NULL;

	textureCoords = Rect(0.0, 0.0, 1.0, 1.0);
}

Object::~Object()
{
	delete texture;	
	texture = NULL;

	delete vector;
	vector = NULL;

	delete scaleVector;
	scaleVector = NULL;

	delete rotationVector;
	rotationVector = NULL;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the object type
//
//////////////////////////////////////////////////////////////
void Object::setObjectType(int _objectType)
{
	objectType = _objectType;
}

//////////////////////////////////////////////////////////////
//
//	This function adds a vertex to the object, basically adds
//	a point of reference for openGL to draw the polygon
//
//////////////////////////////////////////////////////////////
void Object::vertex(int x, int y)
{
	vertex((float)x, (float)y, 0.0f);
}

void Object::vertex(float x, float y)
{
	vertex((float)x, (float)y, 0.0f);
}

void Object::vertex(Vector2 _vector)
{
	vertex((float)_vector.x, (float)_vector.y, 0.0f);
}

void Object::vertex(Rect rect)
{
	vertex((float)rect.x1, (float)rect.y1, 0.0f);
	vertex((float)rect.x2, (float)rect.y1, 0.0f);
	vertex((float)rect.x2, (float)rect.y2, 0.0f);
	vertex((float)rect.x1, (float)rect.y2, 0.0f);
}

void Object::vertex(Vector3 _vector)
{
	vertex(_vector.x, _vector.y, _vector.z);
}

void Object::vertex(list<Vector3> *_vector)
{
	vector = _vector;
}

void Object::vertex(int x, int y, int z)
{
	vertex((float)x, (float)y, (float)z);
}
	
void Object::vertex(float x, float y, float z)
{
	vector->push_back(Vector3(x, y, z));	

	//	We need to update the width and height and depth based on the
	//	largest variable of the vector
	if (vector->size() > 0)
	{
		for (vectorIterator = vector->begin(); vectorIterator != vector->end(); ++vectorIterator)
		{
			if (vectorIterator->x < smallestVector.x)
			{
				smallestVector.x = vectorIterator->x;
			}

			if (vectorIterator->y < smallestVector.y)
			{
				smallestVector.y = vectorIterator->y;
			}

			if (vectorIterator->z < smallestVector.z)
			{
				smallestVector.z = vectorIterator->z;
			}

			if (vectorIterator->x > largestVector.x)
			{
				largestVector.x = vectorIterator->x;
			}			

			if (vectorIterator->y > largestVector.y)
			{
				largestVector.y = vectorIterator->y;
			}

			if (vectorIterator->z > largestVector.z)
			{
				largestVector.z = vectorIterator->z;
			}
		}
	}

	objectSize.width = largestVector.x - smallestVector.x;
	objectSize.height = largestVector.y - smallestVector.y;
	objectSize.depth = largestVector.z - smallestVector.z;
}

//////////////////////////////////////////////////////////////
//
//	This function returns the object's vector list
//
//////////////////////////////////////////////////////////////
list<Vector3> *Object::getVertexList()
{
	return vector;
}

//////////////////////////////////////////////////////////////
//
//	This function checks the mouse co-ordinates with the 
//	object bounds
//
//////////////////////////////////////////////////////////////
bool Object::checkMouse(int x, int y)
{
	if (isClickable)
	{
		//	we need to check the bounds of the object with the
		//	mouse coordinates, returns true if it is within bounds
		//	false if not.
		if (x > positionVector.x && x < positionVector.x + objectSize.width)
		{
			if (y > positionVector.y && y < positionVector.y + objectSize.height)
			{
				return true;
			}
		}
	}

	return false;
}

void Object::checkMouse(int x, int y, int button, void callBack(int buttonType))
{
	if (isClickable)
	{
		//	we need to check the bounds of the object with the
		//	mouse coordinates, returns true if it is within bounds
		//	false if not.
		if (x > positionVector.x && x < positionVector.x + objectSize.width)
		{
			if (y > positionVector.y && y < positionVector.y + objectSize.height)
			{
				callBack(button);
			}
		}
	}
}

void Object::checkMouse(int x, int y, int button, void callBack())
{
	if (isClickable)
	{
		//	we need to check the bounds of the object with the
		//	mouse coordinates, returns true if it is within bounds
		//	false if not.
		if (x > positionVector.x && x < positionVector.x + objectSize.width)
		{
			if (y > positionVector.y && y < positionVector.y + objectSize.height)
			{
				callBack();
			}
		}
	}
}

//////////////////////////////////////////////////////////////
//
//	This function clears the vertex array
//
//////////////////////////////////////////////////////////////
void Object::clearVertex()
{
	vector->empty();
}

//////////////////////////////////////////////////////////////
//
//	This function sets the position vector
//
//////////////////////////////////////////////////////////////
void Object::position(Vector3 vector)
{
	positionVector = vector;
}

void Object::position(int x, int y)
{
	position(x, y, 0);
}

void Object::position(int x, int y, int z)
{
	positionVector = Vector3(x, y, z);
}

//////////////////////////////////////////////////////////////
//
//	This function returns the current position
//
//////////////////////////////////////////////////////////////
Vector3 Object::getPosition()
{
	return positionVector;
}

void Object::setOriginToMiddle()
{
	Vector3 middlePos = this->positionVector / 2;

	glTranslatef(middlePos.x, middlePos.y, 0);
}

void Object::resetOrigin()
{
	Vector3 middlePos = this->positionVector / 2;

	glTranslatef(-middlePos.x, -middlePos.y, 0);
}

//////////////////////////////////////////////////////////////
//
//	This function draws the object
//
//////////////////////////////////////////////////////////////
void Object::drawObject()
{
	if (transparency)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_ALPHA_TEST);
	}

	if (useTexture && this->textureId > 0)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->textureId);									
	}
	
	glPushMatrix();
				
		//	We need to output any rotation vectors
		if (rotationVector->size() > 0)
		{
			for (rotationIterator = rotationVector->begin(); rotationIterator != rotationVector->end(); ++rotationIterator)
			{
				glRotatef(rotationIterator->angle, rotationIterator->x, rotationIterator->y, rotationIterator->z);
			}
		}		

		//	We need to output any scale vectors
		if (scaleVector->size() > 0)
		{
			for (scaleIterator = scaleVector->begin(); scaleIterator != scaleVector->end(); ++scaleIterator)
			{				
				glScalef(scaleIterator->x, scaleIterator->y, scaleIterator->z);		
			}
		}
		glColor4ub(colour.red, colour.green, colour.blue, colour.alpha);
		glBegin(objectType);		

		//	We need to draw the vertexes
		if (vector->size() > 0)
		{
			for (vectorIterator = vector->begin(); vectorIterator != vector->end(); ++vectorIterator)
			{
				if (this->useTexture && this->textureId > 0)
				{
					glTexCoord2f(vectorIterator->x / objectSize.width, vectorIterator->y / objectSize.height);
				}

				glVertex3i(vectorIterator->x + positionVector.x, vectorIterator->y + positionVector.y, vectorIterator->z + positionVector.z);
			}
		}
		else
		{
			//	check there is a valid vector, if not, default to 0, 0 + the
			//	texture's size
			if (useTexture && this->textureId > 0)
			{
				glTexCoord2f(textureCoords.x1, textureCoords.y1);
			}
			
			glVertex3i(0 + positionVector.x, 0 + positionVector.y, 0 + positionVector.z);
			
			if (useTexture && this->textureId > 0)
			{
				glTexCoord2f(textureCoords.x2, textureCoords.y1);		
			}

			glVertex3i(objectSize.width + positionVector.x, 0 + positionVector.y, 0 + positionVector.z);
			
			if (useTexture && this->textureId > 0)
			{
				glTexCoord2f(textureCoords.x2, textureCoords.y2);
			}
			
			glVertex3i(objectSize.width + positionVector.x, objectSize.height + positionVector.y, 0 + positionVector.z);
			
			if (useTexture && this->textureId > 0)
			{
				glTexCoord2f(textureCoords.x1, textureCoords.y2);
			}

			glVertex3i(0 + positionVector.x, objectSize.height + positionVector.y, 0 + positionVector.z);
		}
		

		glEnd();
	
	glPopMatrix();

	if (transparency)
	{
		glDisable(GL_BLEND);
	}
	else
	{
		glEnable(GL_ALPHA_TEST);
	}

	if (useTexture && this->textureId > 0)
	{
		glDisable(GL_TEXTURE_2D);
	}
}

//////////////////////////////////////////////////////////////
//
//	This function sets the transparency of the object
//
//////////////////////////////////////////////////////////////
void Object::setTransparency(bool setBool)
{
	transparency = setBool;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the colour of the object
//
//////////////////////////////////////////////////////////////
void Object::color(int r, int g, int b)
{
	colour = Colour(r, g, b, 255);
}

void Object::color(float r, float g, float b)
{
	colour = Colour(r, g, b, 1.0f);
}

void Object::color(int r, int g, int b, int a)
{
	colour.red = r;
	colour.green = g;
	colour.blue = b;
	colour.alpha = a;

	if (a < 255)
	{
		setTransparency(true);
	}
}

void Object::color(float r, float g, float b, float a)
{
	colour = Colour(r, g, b, a);

	if (a < 1.0f)
	{
		setTransparency(true);
	}
}

void Object::color(Colour _colour)
{
	colour = _colour;
}

//////////////////////////////////////////////////////////////
//
//	This function returns the velocity of the object
//
//////////////////////////////////////////////////////////////
float Object::getSpeed()
{
	return velocity;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the speed of the object
//
//////////////////////////////////////////////////////////////
void Object::speed(float speed)
{
	velocity = speed;
}

//////////////////////////////////////////////////////////////
//
//	This function gets the size of the object
//
//////////////////////////////////////////////////////////////
Size Object::getSize()
{
	return objectSize;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the size of the object
//
//////////////////////////////////////////////////////////////
void Object::size(Size _size)
{
	objectSize = _size;
}

void Object::size(int width, int height)
{
	objectSize = Size(width, height);
}

//////////////////////////////////////////////////////////////
//
//	This function loads a texture
//
//////////////////////////////////////////////////////////////
void Object::loadTexture(char *fileName)
{
	texture = new Texture();
	texture->loadTexture(fileName);

	objectSize = texture->getSize();
	useTexture = true;
}

void Object::loadTexture(Texture *_texture)
{
	textureId = _texture->textureId;
	useTexture = true;
}

//////////////////////////////////////////////////////////////
//
//	This function generates a texture ID based on the texture
//
//////////////////////////////////////////////////////////////
void Object::genTexture(int mode)
{
	textureId = texture->genTexture(mode);
	useTexture = true;
}

//////////////////////////////////////////////////////////////
//
//	This function returns the texture ID
//
//////////////////////////////////////////////////////////////
GLuint Object::getTextureId()
{
	return textureId;
}

//////////////////////////////////////////////////////////////
//
//	This function adds a rotation vector to the matrix
//
//////////////////////////////////////////////////////////////
void Object::rotate(Vector4 _rotationVector)
{
	rotationVector->push_back(_rotationVector);
}

void Object::rotate(float x, float y, float z, float angle)
{
	rotate(Vector4(x, y, z, angle));
}

//////////////////////////////////////////////////////////////
//
//	This function adds a scale vector to the matrix
//
//////////////////////////////////////////////////////////////
void Object::scale(Vector3 _scaleVector)
{
	scaleVector->push_back(_scaleVector);
}

void Object::scale(float x, float y, float z)
{
	scaleVector->push_back(Vector3(x, y, z));
}

void Object::clearScale()
{
	if (scaleVector->size() > 0)
	{
		scaleVector->empty();
	}
}

//////////////////////////////////////////////////////////////
//
//	This function sets the texture cooridnates
//
//////////////////////////////////////////////////////////////
void Object::setTexCoord(Rect texCoord)
{
	textureCoords = texCoord;
}

//////////////////////////////////////////////////////////////
//
//	This function sets if the object is clickable or not
//
//////////////////////////////////////////////////////////////
void Object::setClickable(bool click)
{
	isClickable = click;
}