//////////////////////////////////////////////////////////////
//
//	Golden Eye Reveng - 2011 Games design project by
//	Callum Taylor
//
//	Model.cpp - this file is the class constructor for the model
//	class.
//
//////////////////////////////////////////////////////////////

//	Include all the needed headers
#include "main.h"

//////////////////////////////////////////////////////////////
//
//	This is the default class constructor
//
//////////////////////////////////////////////////////////////
Model::Model()
{ 
	totalFaces = 0;
	currentFrame = 0;
	startFrame = 0;
	endFrame = 0;
	modelFPS = 60;
	isPlaying = false;
	useSound = false;
	scaleVector = Vector3(1, 1, 1);
	translateVector = new list<Vector3>;
	modelRotationVector = new list<Vector4>;
	rotationVector = Vector4(0, 0, 0, 0);

	//	Generate a random name for the model
	modelName = getNextId();
	__modelList->push_back(this);
	modelSettings = NULL;
}

Model::~Model()
{
	delete &model;
	delete &mesh;
	delete &normals;
	delete &vertices;
	delete &textures;
	delete modelSettings;
	normalVBO = NULL;
	vertexVBO = NULL;
	textureVBO = NULL;
}

//////////////////////////////////////////////////////////////
//
//	This function returns the model name we assigned at 
//	registration
//
//////////////////////////////////////////////////////////////
int Model::getID()
{
	return modelName;
}

//////////////////////////////////////////////////////////////
//
//	This function clears the translate vectors
//
//////////////////////////////////////////////////////////////
void Model::clearTranslateVectors()
{
	translateVector->empty();
}

void Model::clearRotationVectors()
{
	modelRotationVector->empty();
}

//////////////////////////////////////////////////////////////
//
//	This function destroys the model
//
//////////////////////////////////////////////////////////////
void Model::destroy()
{
	//	Reset the variables
	scaleVector = Vector3(1, 1, 1);
	modelRotationVector->empty();
	translateVector->empty();	
}

//////////////////////////////////////////////////////////////
//
//	This function draws the model
//
//////////////////////////////////////////////////////////////
void Model::drawModel(bool drawTextures)
{		
	updateFrame();

	//	Push the model ID as its name
	glInitNames();
	glPushName(modelName);
	glPushMatrix();

	//	If there are any rotational vectors in the array, loop
	//	through and apply them to the matrix
	if (modelRotationVector->size() > 0)
	{	
		for (modelRotationIterator = modelRotationVector->begin(); modelRotationIterator != modelRotationVector->end(); ++modelRotationIterator)
		{
			glRotatef(modelRotationIterator->angle, modelRotationIterator->x, modelRotationIterator->y, modelRotationIterator->z);
		}
	}		

	glScalef(scaleVector.x, scaleVector.y, scaleVector.z);

	//	If there are any translational vectors in the array, loop
	//	through and apply them to the matrix
	if (translateVector->size() > 0)
	{	
		for (translateIterator = translateVector->begin(); translateIterator != translateVector->end(); ++translateIterator)
		{
			glTranslatef(translateIterator->x, translateIterator->y, translateIterator->z);
		}
	}	

	//	Translate the original position
	glTranslatef(originalPosition.x, originalPosition.y, originalPosition.z);	

	glEnable(GL_TEXTURE_2D);

	if (usingVBO)
	{
		drawVBO(drawTextures);
	}
	else
	{
		drawNodes(drawTextures);
	}	

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopName();
}

//////////////////////////////////////////////////////////////
//
//	This function draws the model from nodes
//
//////////////////////////////////////////////////////////////
void Model::drawNodes(bool drawTextures)
{
	glBindTexture(GL_TEXTURE_2D, 0);

	Lib3dsNode *node;	

	//	If there is only one texture ID and there is none in 
	//	the multi texture array, bind the texture ID once
	if (drawTextures && textureId > 0 && texturePointer < 0)
	{		
		glBindTexture(GL_TEXTURE_2D, textureId);	
	}

	glPushMatrix();

	string lastNodeName = "";
	//	Loop through each node and display it
	for (node = model->nodes; node != 0; node = node->next) 
	{		
		//	If there are any elements in the multi texture array
		//	loop through the array and find the correct texture
		//	id to display
		if (texturePointer > 0 && string(node->name) != lastNodeName)
		{
			//	We use lastNodeName to detect changes in the 
			//	node to make the code more efficient
			lastNodeName = string(node->name);
			for each(Setting setting in this->textureIds)
			{
				if (setting.data == "")
				{
					break;
				}

				if (setting.name == string(node->name))
				{
					int id = toInt(setting.data);
					glBindTexture(GL_TEXTURE_2D, id);	

					break;
				}
			}						
		}

		renderNode(node, drawTextures);
	}	

	glPopMatrix();
}

//////////////////////////////////////////////////////////////
//
//	This function draws the model using a VBO
//
//////////////////////////////////////////////////////////////
void Model::drawVBO(bool drawTextures)
{
	if (drawTextures && textureId > 0)
	{		
		//	Bind the texture with the texture VBO
		texture.bindTexture(textureVBO);
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);  
		
	//	Bind the normal VBO
	glBindBufferARB(GL_ARRAY_BUFFER, normalVBO);
	glNormalPointer(GL_FLOAT, 0, NULL);

	//	Bind the vertex VBO
	glBindBufferARB(GL_ARRAY_BUFFER, vertexVBO);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	//	Draw the triangle array
	glDrawArrays(GL_TRIANGLES, 0, totalFaces * 3);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);  	
}

//////////////////////////////////////////////////////////////
//
//	This function renders a node
//
//////////////////////////////////////////////////////////////
void Model::renderNode(Lib3dsNode *node, bool drawTextures)
{
	//	If there are any child nodes, recursively render them
	Lib3dsNode *nodeChild;
  	for (nodeChild = node->childs; nodeChild != 0; nodeChild = nodeChild->next) 
	{
		renderNode(nodeChild, drawTextures);
	}

	glPushMatrix();

	//	Make sure the node is actually a node
	if (node->type == LIB3DS_OBJECT_NODE) 
	{		
		//	If the node is a dummy node, then return
		if (strcmp(node->name, "$$$DUMMY") == 0) 
		{
			return;
		}

		//	Get the mesh details if there is any morphs
		Lib3dsMesh *mesh = lib3ds_file_mesh_by_name(model, node->data.object.morph);
		if (mesh == NULL)
		{			
			//	If there is no morphing, just load the mesh by node name
			mesh = lib3ds_file_mesh_by_name(model, node->name);
		}

		//	This gets called to draw the initial points, matricies are then created
		//	to handle the changes and to maintain the model
		if (!mesh->user.d) 
		{			
			if (!mesh) 
			{
				return;
			}

			mesh->user.d = glGenLists(1);
			glNewList(mesh->user.d, GL_COMPILE);
			{
				unsigned faces;
				//	Init a memory allocation for the normal vectors
				Lib3dsVector *normalL = (Lib3dsVector*) malloc(3 * sizeof(Lib3dsVector) * mesh->faces);
				Lib3dsMaterial *oldmat = (Lib3dsMaterial *) -1;
				{
					//	Copy all the matricies to the new matrix 
					//	variable for use with translations
					Lib3dsMatrix matrix;
					lib3ds_matrix_copy(matrix, mesh->matrix);
					lib3ds_matrix_inv(matrix);
					glMultMatrixf(&matrix[0][0]);
				}

				//	Calculate the mesh normals and store in the normal
				//	vectory variable
				lib3ds_mesh_calculate_normals(mesh, normalL);

				//	Loop through the faces 
				for (faces = 0; faces < mesh->faces; ++faces) 
				{					
					Lib3dsFace *face = &mesh->faceL[faces];																	

					glBegin(GL_TRIANGLES);
					glNormal3fv(face->normal);											

					//	Loop through a tri range (3) and draw each
					//	vertex and texture coordinate
					for (int loop = 0; loop < 3; ++loop) 
					{							
						glNormal3fv(normalL[3 * faces + loop]);			
							
						if (mesh->texels && drawTextures && textureId > 0)
						{
							glTexCoord2f(mesh->texelL[face->points[loop]][0], mesh->texelL[face->points[loop]][1]);
						}

						glVertex3fv(mesh->pointL[face->points[loop]].pos);
					}
          					
					glEnd();					
				}

				free(normalL);
			}

			glEndList();
		}

		//	If the mesh is just a transformation
		if (mesh->user.d) 
		{
			Lib3dsObjectData *objectData;

			glPushMatrix();
				
				objectData = &node->data.object;
				glMultMatrixf(&node->matrix[0][0]);
				glTranslatef(-objectData->pivot[0], -objectData->pivot[1], -objectData->pivot[2]);					
				glCallList(mesh->user.d);									

			glPopMatrix();

			glFlush();
		}
	}

	glPopMatrix();
}

//////////////////////////////////////////////////////////////
//
//	This function generates the buffers. We need to call this
//	seperatly when doing a loading thread because openGL
//	cant be used from external contexes. so we need to firstly
//	load up the model data, process it, then generate the buffers
//
//////////////////////////////////////////////////////////////
void Model::genBuffers()
{
	//	Now we assign the buffers for the vertices
	glGenBuffersARB(1, &vertexVBO);
	glBindBufferARB(GL_ARRAY_BUFFER, vertexVBO);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * totalFaces, vertices, GL_STATIC_DRAW);
	
	//	Assign the buffers for the normals
	glGenBuffersARB(1, &normalVBO);
	glBindBufferARB(GL_ARRAY_BUFFER, normalVBO);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(Lib3dsVector) * 3 * totalFaces, normals, GL_STATIC_DRAW);
	
	//	Assign the texel buffers
	glGenBuffersARB(1, &textureVBO);
	glBindBufferARB(GL_ARRAY_BUFFER, textureVBO);
	glBufferDataARB(GL_ARRAY_BUFFER, sizeof(Lib3dsTexel) * 3 * totalFaces, textures, GL_STATIC_DRAW);
}

//////////////////////////////////////////////////////////////
//
//	This function generates the texture ID
//
//////////////////////////////////////////////////////////////
void Model::genTexture()
{			
	if (texturePointer > 0)
	{
		for (int textureCount = 0; textureCount < texturePointer; textureCount++)
		{
			if (multiTextures[textureCount].getWidth() > 0 && multiTextures[textureCount].getHeight() > 0)
			{
				GLint id = multiTextures[textureCount].genTexture();
				if (id < 0)
				{
					error("An invalid texture ID was generated.");
					return;
				}
				else
				{
					textureIds[textureCount].data = toString((int)id);
				}
			}
		}
	}
	else
	{
		if (texture.getWidth() > 0 && texture.getHeight() > 0)
		{
			if ((textureId = texture.genTexture()) < 0)
			{
				error("An invalid texture ID was generated.");
				return;
			}
		}
	}
}

//////////////////////////////////////////////////////////////
//
//	This function will load a model from a settings file
//
//////////////////////////////////////////////////////////////
void Model::loadSettings(char *filePath) 
{
	modelSettings = new Settings(filePath);
	loadModel((char *)modelSettings->getSetting("main", "model").data.c_str());
	
	string texturesPath = modelSettings->getSetting("main", "textures").data;
	if (texturesPath != "")
	{
		Settings *textures = new Settings((char *)texturesPath.c_str());
		
		loadTextures(textures);
	}
	else
	{
		string texturePath = modelSettings->getSetting("main", "texture").data;
		if (texturePath != "")
		{
			loadTexture((char *)texturePath.c_str());
		}
	}
	
	//	Get the pos data
	vector<string> posx = split(modelSettings->getSetting("position", "x").data, ',');
	vector<string> posy = split(modelSettings->getSetting("position", "y").data, ',');
	vector<string> posz = split(modelSettings->getSetting("position", "z").data, ',');

	for (int pos = 0; pos < (int)posx.size(); pos++)
	{
		float posX = atof(posx.at(pos).c_str());
		float posY = atof(posy.at(pos).c_str());
		float posZ = atof(posz.at(pos).c_str());
		position(Vector3(posX, posY, posZ));
	}	

	//	Get the rotation data
	vector<string> rotx = split(modelSettings->getSetting("rotate", "x").data, ',');
	vector<string> roty = split(modelSettings->getSetting("rotate", "y").data, ',');
	vector<string> rotz = split(modelSettings->getSetting("rotate", "z").data, ',');
	vector<string> rota = split(modelSettings->getSetting("rotate", "a").data, ',');

	for (int rotPos = 0; rotPos < (int)rotx.size(); rotPos++)
	{
		float rotX = atof(rotx.at(rotPos).c_str());
		float rotY = atof(roty.at(rotPos).c_str());
		float rotZ = atof(rotz.at(rotPos).c_str());
		float rotA = atof(rota.at(rotPos).c_str());
		rotate(Vector4(rotX, rotY, rotZ, rotA));
	}	

	//	Get the scale data
	vector<string> rezx = split(modelSettings->getSetting("resize", "x").data, ',');
	vector<string> rezy = split(modelSettings->getSetting("resize", "y").data, ',');
	vector<string> rezz = split(modelSettings->getSetting("resize", "z").data, ',');

	for (int rezPos = 0; rezPos < (int)rezx.size(); rezPos++)
	{
		float rezX = atof(rezx.at(rezPos).c_str());
		float rezY = atof(rezy.at(rezPos).c_str());
		float rezZ = atof(rezz.at(rezPos).c_str());
		resize(Vector3(rezX, rezY, rezZ));
	}	

	int fps = atoi(modelSettings->getSetting("main", "modelfps").data.c_str());
	if (fps > 0)
	{
		modelFPS = fps;
	}
}

//////////////////////////////////////////////////////////////
//
//	This function loads the model from a given filename. It
//	only acceps .3ds files.
//
//////////////////////////////////////////////////////////////
void Model::loadModel(char *fileName, bool useVBO, bool generateBuffers)
{
	//	Load the model
	if (!(model = lib3ds_file_load(fileName)))
	{
		std::string errorMessage;
		errorMessage = "Could not load model: ";
		errorMessage += fileName;

		error((char *)errorMessage.c_str());
		return;
	}

	if (useVBO)
	{
		usingVBO = true;

		//	Calculate the total faces	
		totalFaces = 0;	
		for (mesh = model->meshes; mesh != NULL; mesh = mesh->next)
		{
			totalFaces += mesh->faces;
		}

		vertices = new Lib3dsVector[totalFaces * 3];
		normals = new Lib3dsVector[totalFaces * 3];
		textures = new Lib3dsTexel[totalFaces * 3];				

		int finishedFaces = 0;
		for (mesh = model->meshes; mesh != NULL; mesh = mesh->next)
		{		
			lib3ds_mesh_calculate_normals(mesh, &normals[finishedFaces * 3]);			

			//	Now we loop through each face
			for (int faces = 0; faces < (int)mesh->faces; faces++)
			{
				Lib3dsFace *face = &mesh->faceL[faces];

				//	Because each face is a triangle, loop through
				//	and get the verticies
				for (int tri = 0; tri < 3; tri++)
				{
					if (mesh->texels)
					{
						memcpy(&textures[finishedFaces * 3 + tri], mesh->texelL[face->points[tri]], sizeof(Lib3dsTexel));
					}

					memcpy(&vertices[finishedFaces * 3 + tri], mesh->pointL[face->points[tri]].pos, sizeof(Lib3dsVector));				
				}

				finishedFaces++;
			}
		}
	}
	else
	{
		usingVBO = false;

		//	If no nodes are set, create some faux ones
		if (!model->nodes)
		{
			Lib3dsMesh *mesh;
			Lib3dsNode *node;
		
			for (mesh = model->meshes; mesh != NULL; mesh = mesh->next)
			{					
				node = lib3ds_node_new_object();
				strcpy_s(node->name, mesh->name);
				node->parent_id = LIB3DS_NO_PARENT;
				lib3ds_file_insert_node(model, node);				
			}
		}

		lib3ds_file_eval(model, 0.0f);			
	}

	Lib3dsVector minVector;
	Lib3dsVector maxVector;		
	lib3ds_file_bounding_box_of_objects(model, true, false, false, minVector, maxVector);	

	minBounds = Vector3(minVector[0], minVector[1], minVector[2]);
	maxBounds = Vector3(maxVector[0], maxVector[1], maxVector[2]);

	if (generateBuffers)
	{
		genBuffers();
	}		
}

//////////////////////////////////////////////////////////////
//
//	This function loads the texture. Only support file is .TGA
//
//////////////////////////////////////////////////////////////
void Model::loadTextures(char *fileName, bool generateTex)
{
	multiTextures[texturePointer++].loadTexture(fileName);

	if (generateTex)
	{
		genTexture();
	}
}

void Model::loadTextures(Settings *textureList, bool generateTex)
{
	list<string> *textureNames = textureList->getGroupList();
	list<string>::iterator it;

	texturePointer = 0;
	int settingCount = 0;
	for (it = textureNames->begin(); it != textureNames->end(); ++it)
	{
		textureIds[settingCount].groupName = "texture";
		textureIds[settingCount].name = (char *)textureList->getSetting((char *)it->c_str(), "name").data.c_str();			
		loadTextures((char *)textureList->getSetting((char *)it->c_str(), "texture").data.c_str());		

		++settingCount;
	}
}

void Model::loadTexture(char *fileName, bool generateTex)
{
	texture.loadTexture(fileName);
	
	if (generateTex)
	{
		genTexture();
	}
}

//////////////////////////////////////////////////////////////
//
//	This function sets the new position of the model
//
//////////////////////////////////////////////////////////////
void Model::position(Vector3 vector)
{
	originalPosition = vector;

	//	We need to apply the new translations to the bounding box
	maxBounds = maxBounds + vector;
	minBounds = minBounds + vector;
}

void Model::position(float x, float y, float z)
{
	originalPosition = Vector3(x, y, z);

	//	We need to apply the new translations to the bounding box
	maxBounds = maxBounds + Vector3(x, y, z);
	minBounds = minBounds + Vector3(x, y, z);
}

//////////////////////////////////////////////////////////////
//
//	This function sets the resize vector
//
//////////////////////////////////////////////////////////////
void Model::resize(Vector3 vector)
{
	scaleVector = vector;

	//maxBounds = maxBounds * vector;
	//minBounds = minBounds * vector;
}

void Model::resize(float x, float y, float z)
{
	scaleVector = Vector3(x, y, z);

	//maxBounds = maxBounds * Vector3(x, y, z);
	//minBounds = minBounds * Vector3(x, y, z);
}

//////////////////////////////////////////////////////////////
//
//	This function sets the rotation vector
//
//////////////////////////////////////////////////////////////
void Model::rotate(Vector4 vector)
{
	modelRotationVector->push_back(vector);
}

void Model::rotate(float angle, float x, float y, float z)
{
	modelRotationVector->push_back(Vector4(x, y, z, angle));
}

//////////////////////////////////////////////////////////////
//
//	This function sets the translate vector
//
//////////////////////////////////////////////////////////////
void Model::translate(Vector3 vector)
{		
	newPosition = vector;
	translateVector->push_back(vector);
}

void Model::translate(float x, float y, float z)
{	
	newPosition = Vector3(x, y, z);
	translateVector->push_back(Vector3(x, y, z));
}

//////////////////////////////////////////////////////////////
//
//	This function will handle the frame in the animation
//
//////////////////////////////////////////////////////////////
void Model::updateFrame()
{	 
	if (isPlaying && fpsLocker.lockFPS(60))
	{
		if (currentFrame > endFrame)
		{
			currentFrame = startFrame;
			
			if (!loopFrames)
			{
				isPlaying = false;

				//	Set to the original model state which is frame 0
				currentFrame = 0;
			}
		}
		else
		{
			if (useSound)
			{
				if ((int)currentFrame == soundFileFrame)
				{
					soundFile->playSound();					
				}
			}

			if (callback != NULL && (int)currentFrame == callbackFrame)
			{
				callback();
			}

			currentFrame += 1.0;
    
			if (currentFrame > model->frames) 
			{
				currentFrame = 0;
			}			
		}

		lib3ds_file_eval(model, currentFrame);
	}
}

void Model::setFrame(int frame)
{
	currentFrame = frame;
	playFrames(frame, frame);
	lib3ds_file_eval(model, currentFrame);
}

//////////////////////////////////////////////////////////////
//
//	This function sets a sound to be played
//	@soundFile: this is a sound file associated with the model
//	@playSoundEvery: This tells the model to play the sound
//	after every nth frame
//
//////////////////////////////////////////////////////////////
void Model::setSound(Sound *_soundFile, int _playSoundEvery)
{
	soundFile = _soundFile;
	soundFileFrame = _playSoundEvery;
	useSound = true;
}

void Model::clearSound()
{
	useSound = false;	
	soundFileFrame = 0;
}

//////////////////////////////////////////////////////////////
//
//	This function sets the frames to be played
//
//////////////////////////////////////////////////////////////
void Model::playFrames(int _startFrame, int _endFrame, bool loop, void (*_callback)(), int _callbackFrame)
{
	callbackFrame = _callbackFrame;
	callback = _callback;
	currentFrame = _startFrame;
	startFrame = _startFrame;
	endFrame = _endFrame;	
	loopFrames = loop;
	isPlaying = true;
}

int Model::getCurrentFrame()
{
	return currentFrame;
}