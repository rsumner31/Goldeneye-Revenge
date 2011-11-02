//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	model.h - This file handels the model loading and texture
//	mapping / drawing
//	
//////////////////////////////////////////////////////////////

class Model
{
	public:
		
		//	Initialize the mesh and model objects
		Lib3dsFile *model;
		Lib3dsMesh *mesh;			
		Lib3dsVector *vertices;
		Lib3dsVector *normals;
		Lib3dsTexel *textures;

		Vector3 minBounds;
		Vector3 maxBounds;

		Model::Model();
		Model::~Model();
		int Model::getID();
		void Model::clearTranslateVectors();
		void Model::clearRotationVectors();
		void Model::destroy();
		void Model::drawModel(bool drawTextures = true);
		void Model::drawNodes(bool drawTextures);
		void Model::drawVBO(bool drawTextures);
		void Model::renderNode(Lib3dsNode *node, bool drawTextures = true);
		void Model::genBuffers();
		void Model::genTexture();
		void Model::loadSettings(char *filePath);
		void Model::loadModel(char *fileName, bool useVBO = false, bool generateBuffers = false);
		void Model::loadTextures(char *fileName, bool generateTex = false);
		void Model::loadTextures(Settings *textureList, bool generateTex = false);
		void Model::loadTexture(char *fileName, bool generateTex = false);				
		void Model::resize(float x, float y, float z);
		void Model::resize(Vector3 vector);
		void Model::rotate(float angle, float x, float y, float z);
		void Model::rotate(Vector4 vector);
		void Model::translate(float x, float y, float z);
		void Model::translate(Vector3 vector);
		void Model::position(float x, float y, float z);
		void Model::position(Vector3 vector);
		void Model::moveX(float speed);
		void Model::moveY(float speed);
		void Model::updateFrame();
		void Model::setSound(Sound *_soundFile, int _playSoundEvery);
		void Model::clearSound();
		void Model::playFrames(int _startFrame, int _endFrame, bool loop = false, void _callback() = NULL, int callbackFrame = 0);
		void Model::setFrame(int frame);
		int Model::getCurrentFrame();
		
		Setting textureIds[1024];
		GLuint textureId;
		Texture texture;
		Texture multiTextures[1024];
		int texturePointer;
		GLuint vertexVBO;
		GLuint normalVBO;
		GLuint textureVBO;
		Settings *modelSettings;
		
		//	Linked lists for the translate and rotation vectors which
		//	enables us to apply more than one to the model
		list<Vector3> *translateVector;
		list<Vector3>::iterator translateIterator;
		list<Vector4> *modelRotationVector;
		list<Vector4>::iterator modelRotationIterator;
		Vector3 originalPosition;
		Vector3 newPosition;
		Vector4 rotationVector;

	private:		
		void (*callback)();		

		float currentFrame;
		int callbackFrame;
		int totalFaces;
		Vector3 scaleVector;		
						
		char datapath[256];
		bool usingVBO;

		int startFrame;
		int endFrame;
		bool isPlaying;
		bool loopFrames;

		Sound *soundFile;
		int soundFileFrame;
		bool useSound;

		int modelName;
		int modelFPS;

		Locker fpsLocker;
};

