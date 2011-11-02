//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	object.h - This is the header file for the object class
//	
//////////////////////////////////////////////////////////////

//	Define the different object types
#define OBJECT_BUTTON 1
#define OBJECT_SHAPE 2

class Object
{
	public:		
		Object::Object();
		Object::~Object();				

		list<Vector3> *Object::getVertexList();
		bool Object::checkMouse(int x, int y);
		void Object::checkMouse(int x, int y, int button, void callBack(int buttonType));
		void Object::checkMouse(int x, int y, int button, void callBack());
		void Object::clearVertex();
		void Object::drawObject();		
		void Object::setTransparency(bool setBool);
		void Object::setObjectType(int objectType);		
		void Object::loadTexture(char *fileName);			
		void Object::loadTexture(Texture *texture);
		void Object::genTexture(int mode = GL_CLAMP);		
		GLuint Object::getTextureId();
		Vector3 Object::getPosition();
		void Object::position(Vector3 vector);	
		void Object::position(int x, int y);		
		void Object::position(int x, int y, int z);
		float Object::getSpeed();
		void Object::speed(float speed);
		Size Object::getSize();
		void Object::size(Size _size);	
		void Object::size(int width, int height);	
		void Object::color(int r, int g, int b);
		void Object::color(float r, float g, float b);
		void Object::color(int r, int g, int b, int a);
		void Object::color(float r, float g, float b, float a);
		void Object::color(Colour _colour);		
		void Object::rotate(Vector4 _rotationVector);
		void Object::rotate(float x, float y, float z, float angle);
		void Object::scale(Vector3 _scaleVector);
		void Object::scale(float x, float y, float z);	
		void Object::clearScale();
		void Object::setTexCoord(Rect texCoord);
		void Object::resetOrigin();
		void Object::setOriginToMiddle();
		void Object::setClickable(bool click);
		void Object::vertex(int x, int y);
		void Object::vertex(float x, float y);
		void Object::vertex(Vector2 _vector);
		void Object::vertex(int x, int y, int z);
		void Object::vertex(float x, float y, float z);
		void Object::vertex(Vector3 _vector);	
		void Object::vertex(Rect rect);
		void Object::vertex(list<Vector3> *_vector);

	private:	
		GLuint objectId;
		GLuint objectType;
		Texture *texture;
		GLuint textureId;

		list<Vector3> *vector;
		list<Vector3>::iterator vectorIterator;
		list<Vector3> *scaleVector;
		list<Vector3>::iterator scaleIterator;
		list<Vector4> *rotationVector;
		list<Vector4>::iterator rotationIterator;
		Vector3 positionVector;

		float velocity;
		Colour colour;
		bool transparency;
		bool useTexture;
		bool isClickable;
		bool isDrawn;

		Size objectSize;		
		Vector3 smallestVector;
		Vector3 largestVector;	

		Rect textureCoords;
};