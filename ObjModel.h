#pragma once

#include <string>
#include <vector>

#include <windows.h>
#include <gl/GL.h>

#include <map>
#include <string>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class Texture;
class btRigidBody;
class btCollisionShape;
struct btDefaultMotionState;

class ObjModel
{
public: //private:

	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		unsigned int texture;
		std::string textureStr;
		unsigned int bumpMap;
		std::string bumpMapStr;

		bool hasTexture;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int start;
		int end;
		int materialIndex;
	};

	class ObjModelCore
	{
	public:
		std::vector<ObjGroup*> groups;
		std::vector<MaterialInfo*> materials;
		GLuint _vertexArray;
		std::vector<float> *vertices;
	};

	btRigidBody *rigidBody;
	btCollisionShape *groundShape;
	btDefaultMotionState *myMotionState;
	ObjModelCore *objModelCore;


	void loadMaterialFile(std::string fileName, std::string dirName);
//public:
	ObjModel::ObjModel(std::string fileName, btScalar mass, const btVector3 &origin, const btVector3 &scale);
	void ObjModel::createRigidBody(btScalar mass, const btVector3 &origin, const std::vector<float> *vertices, const btVector3 &scale);
	~ObjModel(void);

	void draw(unsigned int shaderID);
};