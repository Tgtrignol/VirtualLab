#include <gl/glew.h>
#include "ObjModel.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "stb_image.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <map>
#include <string>
#include "Hydra.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

std::map<std::string, ObjModel::ObjModelCore *> buffer;

std::string replace(std::string str, std::string toReplace, std::string replacement)
{
	size_t index = 0;
	while (true) 
	{
		 index = str.find(toReplace, index);
		 if (index == std::string::npos) 
			 break;
		 str.replace(index, toReplace.length(), replacement);
		 ++index;
	}
	return str;
}

std::vector<std::string> split(std::string str, std::string sep)
{
	std::vector<std::string> ret;
	size_t index;
	while(true)
	{
		index = str.find(sep);
		if(index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index+1);
	}
	ret.push_back(str);
	return ret;
}

inline std::string toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}



glm::vec4 calcTangentVector(
	glm::vec3 pos1,		glm::vec3 pos2,		glm::vec3 pos3, 
	glm::vec2 texCoord1,	glm::vec2 texCoord2,	glm::vec2 texCoord3, glm::vec3 normal)
{
	// Given the 3 vertices (position and texture coordinates) of a triangle
	// calculate and return the triangle's tangent vector.

	// Create 2 vectors in object space.
	//
	// edge1 is the vector from vertex positions pos1 to pos2.
	// edge2 is the vector from vertex positions pos1 to pos3.
	glm::vec3 edge1(pos2-pos1);//Vector3 edge1(pos2[0] - pos1[0], pos2[1] - pos1[1], pos2[2] - pos1[2]);
	glm::vec3 edge2(pos3-pos1);//Vector3 edge2(pos3[0] - pos1[0], pos3[1] - pos1[1], pos3[2] - pos1[2]);

	glm::normalize(edge1);
	glm::normalize(edge2);

	// Create 2 vectors in tangent (texture) space that point in the same
	// direction as edge1 and edge2 (in object space).
	//
	// texEdge1 is the vector from texture coordinates texCoord1 to texCoord2.
	// texEdge2 is the vector from texture coordinates texCoord1 to texCoord3.
	glm::vec2 texEdge1(texCoord2 - texCoord1);
	glm::vec2 texEdge2(texCoord3 - texCoord1);

	glm::normalize(texEdge1);
	glm::normalize(texEdge2);

	// These 2 sets of vectors form the following system of equations:
	//
	//  edge1 = (texEdge1.x * tangent) + (texEdge1.y * bitangent)
	//  edge2 = (texEdge2.x * tangent) + (texEdge2.y * bitangent)
	//
	// Using matrix notation this system looks like:
	//
	//  [ edge1 ]     [ texEdge1.x  texEdge1.y ]  [ tangent   ]
	//  [       ]  =  [                        ]  [           ]
	//  [ edge2 ]     [ texEdge2.x  texEdge2.y ]  [ bitangent ]
	//
	// The solution is:
	//
	//  [ tangent   ]        1     [ texEdge2.y  -texEdge1.y ]  [ edge1 ]
	//  [           ]  =  -------  [                         ]  [       ]
	//  [ bitangent ]      det A   [-texEdge2.x   texEdge1.x ]  [ edge2 ]
	//
	//  where:
	//        [ texEdge1.x  texEdge1.y ]
	//    A = [                        ]
	//        [ texEdge2.x  texEdge2.y ]
	//
	//    det A = (texEdge1.x * texEdge2.y) - (texEdge1.y * texEdge2.x)
	//
	// From this solution the tangent space basis vectors are:
	//
	//    tangent = (1 / det A) * ( texEdge2.y * edge1 - texEdge1.y * edge2)
	//  bitangent = (1 / det A) * (-texEdge2.x * edge1 + texEdge1.x * edge2)
	//     normal = cross(tangent, bitangent)

	glm::vec3 t;
	glm::vec3 b;
	

	float det = (texEdge1[0] * texEdge2[1]) - (texEdge1[1] * texEdge2[0]);

	if (det == 0)
	{
		t = glm::vec3(1.0f, 0.0f, 0.0f);
		b = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else
	{
		det = 1.0f / det;

		t[0] = (texEdge2[1] * edge1[0] - texEdge1[1] * edge2[0]) * det;
		t[1] = (texEdge2[1] * edge1[1] - texEdge1[1] * edge2[0]) * det;
		t[2] = (texEdge2[1] * edge1[2] - texEdge1[1] * edge2[0]) * det;

		b[0] = (-texEdge2[0] * edge1[0] + texEdge1[0] * edge2[0]) * det;
		b[1] = (-texEdge2[0] * edge1[1] + texEdge1[0] * edge2[1]) * det;
		b[2] = (-texEdge2[0] * edge1[2] + texEdge1[0] * edge2[2]) * det;

		glm::normalize(t);
		glm::normalize(b);
	}

	// Calculate the handedness of the local tangent space.
	// The bitangent vector is the cross product between the triangle face
	// normal vector and the calculated tangent vector. The resulting bitangent
	// vector should be the same as the bitangent vector calculated from the
	// set of linear equations above. If they point in different directions
	// then we need to invert the cross product calculated bitangent vector. We
	// store this scalar multiplier in the tangent vector's 'w' component so
	// that the correct bitangent vector can be generated in the normal mapping
	// shader's vertex shader	.

	glm::vec3 bitangent;
	bitangent = glm::cross(normal, t);
	float handedness = (glm::dot(bitangent, b) < 0.0f) ? -1.0f : 1.0f;

	return glm::vec4(t[0], t[1], t[2], handedness);

/*	tangent[0] = t.x;
	tangent[1] = t.y;
	tangent[2] = t.z;
	tangent[3] = handedness;*/
}

void ObjModel::createRigidBody(btScalar mass, const btVector3 &origin, const std::vector<float> *vertices, const btVector3 &scale)
{
	groundShape = new btConvexHullShape();
	for (int i = 0; i < vertices->size(); i += 3)
	{
		btVector3 vertex((*vertices)[i] * scale.x(), (*vertices)[i + 1] * scale.y(), (*vertices)[i+2] * scale.z());
		((btConvexHullShape *)groundShape)->addPoint(vertex);
	}

	btVector3 localInertia(0, 0, 0);
	groundShape->calculateLocalInertia(mass, localInertia);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(origin);

	myMotionState = new btDefaultMotionState(groundTransform); //motionstate provides interpolation capabilities, and only synchronizes 'active' objects
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
	rigidBody = new btRigidBody(rbInfo);
	rigidBody->setUserPointer(this);
}

ObjModel::ObjModel(std::string fileName, btScalar mass, const btVector3 &origin, const btVector3 &scale)
{
	//If allready in buffer, load that instead.
	objModelCore = buffer[fileName];
	if (objModelCore == nullptr)
	{
		objModelCore = new ObjModelCore();
		buffer[fileName] = objModelCore;
	}
	else
	{
		createRigidBody(mass, origin, objModelCore->vertices, scale);
		return;
	}

	std::string dirName = fileName;
	if(dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if(dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if(fileName == dirName)
		dirName = "";


	std::ifstream pFile(fileName.c_str());

	std::vector<float>	vertices;
	std::vector<float>	normals;
	std::vector<float>	texcoords;

	std::vector<float> finalVertices;


	ObjGroup* currentGroup = new ObjGroup();
	currentGroup->end = -1;
	currentGroup->start = 0;
	currentGroup->materialIndex = -1;


	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		
		line = replace(line, "\t", " ");
		while(line.find("  ") != std::string::npos)
			line = replace(line, "  ", " ");
		if(line == "")
			continue;
		if(line[0] == ' ')
			line = line.substr(1);
		if(line == "")
			continue;
		if(line[line.length()-1] == ' ')
			line = line.substr(0, line.length()-1);
		if(line == "")
			continue;
		if(line[0] == '#')
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		if(params[0] == "v")
		{
			vertices.push_back(atof(params[1].c_str()));
			vertices.push_back(atof(params[2].c_str()));
			vertices.push_back(atof(params[3].c_str()));
		}
		else if(params[0] == "vn")
		{
			normals.push_back(atof(params[1].c_str()));
			normals.push_back(atof(params[2].c_str()));
			normals.push_back(atof(params[3].c_str()));
		}
		else if(params[0] == "vt")
		{
			texcoords.push_back(atof(params[1].c_str()));
			texcoords.push_back(atof(params[2].c_str()));
		}
		else if(params[0] == "f")
		{
			std::vector<std::string> indices1 = split(params[1], "/");
			std::vector<std::string> indices2 = split(params[2], "/");
			std::vector<std::string> indices3 = split(params[3], "/");
			glm::vec3 p1(vertices[(atoi(indices1[0].c_str())-1)*3+0],vertices[(atoi(indices1[0].c_str())-1)*3+1],vertices[(atoi(indices1[0].c_str())-1)*3+2]);
			glm::vec3 p2(vertices[(atoi(indices2[0].c_str())-1)*3+0],vertices[(atoi(indices2[0].c_str())-1)*3+1],vertices[(atoi(indices2[0].c_str())-1)*3+2]);
			glm::vec3 p3(vertices[(atoi(indices3[0].c_str())-1)*3+0],vertices[(atoi(indices3[0].c_str())-1)*3+1],vertices[(atoi(indices3[0].c_str())-1)*3+2]);
			glm::vec2 t1;
			glm::vec2 t2;
			glm::vec2 t3;
			if (texcoords.size() > 0)
			{
				t1 = glm::vec2(texcoords[(atoi(indices1[1].c_str()) - 1) * 2 + 0], texcoords[(atoi(indices1[1].c_str()) - 1) * 2 + 1]);
				t2 = glm::vec2(texcoords[(atoi(indices2[1].c_str()) - 1) * 2 + 0], texcoords[(atoi(indices2[1].c_str()) - 1) * 2 + 1]);
				t3 = glm::vec2(texcoords[(atoi(indices3[1].c_str()) - 1) * 2 + 0], texcoords[(atoi(indices3[1].c_str()) - 1) * 2 + 1]);
			}
			else
			{
				t1 = glm::vec2(0, 0);
				t2 = glm::vec2(0, 0);
				t3 = glm::vec2(0, 0);
			}
			glm::vec3 n;
			
			if (!normals.empty())
				n = glm::vec3(normals[(atoi(indices1[2].c_str()) - 1) * 3 + 0], normals[(atoi(indices1[2].c_str()) - 1) * 3 + 1], normals[(atoi(indices1[2].c_str()) - 1) * 3 + 2]);
			else
				n = glm::vec3(0.0);

			glm::vec4 tangent(calcTangentVector(p1,p2,p3,t1,t2,t3,n));

			for(size_t ii = 4; ii <= params.size(); ii++)
			{

				for(size_t i = ii-3; i < ii; i++)
				{
					std::vector<std::string> indices = split(params[i == (ii-3) ? 1 : i], "/");
					glm::vec3 p;
					glm::vec2 t;
					glm::vec3 n;

					float px = 0, py = 0, pz = 0, tx = 0, ty = 0, nx = 0, ny = 0, nz = 0;
					if(indices.size() >= 1)
					{
						p[0] = vertices[(atoi(indices[0].c_str())-1) * 3+0];
						p[1] = vertices[(atoi(indices[0].c_str())-1) * 3+1];
						p[2] = vertices[(atoi(indices[0].c_str())-1) * 3+2];
					}
					if(indices.size() == 2) //texture 
					{
						if (texcoords.size() > 0)
						{
							t[0] = texcoords[(atoi(indices[1].c_str()) - 1) * 2 + 0];
							t[1] = texcoords[(atoi(indices[1].c_str()) - 1) * 2 + 1];
						}
						else
						{
							t[0] = 0;
							t[1] = 0;
						}
					}
					if(indices.size() == 3)
					{
						if( indices[1] != "")
						{
							if (texcoords.size() > 0)
							{
								t[0] = texcoords[(atoi(indices[1].c_str()) - 1) * 2 + 0];
								t[1] = texcoords[(atoi(indices[1].c_str()) - 1) * 2 + 1];
							}
							else
							{
								t[0] = 0;
								t[1] = 0;
							}
						}
						n[0] = normals[(atoi(indices[2].c_str())-1) * 3+0];
						n[1] = normals[(atoi(indices[2].c_str())-1) * 3+1];
						n[2] = normals[(atoi(indices[2].c_str())-1) * 3+2];
					}
					finalVertices.push_back(p[0]);
					finalVertices.push_back(p[1]);
					finalVertices.push_back(p[2]);
					finalVertices.push_back(n[0]);
					finalVertices.push_back(n[1]);
					finalVertices.push_back(n[2]);
					finalVertices.push_back(t[0]);
					finalVertices.push_back(t[1]);

					finalVertices.push_back(tangent[0]);
					finalVertices.push_back(tangent[1]);
					finalVertices.push_back(tangent[2]);
					finalVertices.push_back(tangent[3]);
					

					currentGroup->end = finalVertices.size() / 12;
				}
			}
		}
		else if(params[0] == "s")
		{
		}
        else if(params[0] == "mtllib")
        {
            loadMaterialFile(dirName + "/" + params[1], dirName);
        }
		else if(params[0] == "usemtl")
		{
			if(currentGroup->end != -1)
				objModelCore->groups.push_back(currentGroup);
			currentGroup = new ObjGroup();
			currentGroup->start = finalVertices.size()/12;
			currentGroup->materialIndex = -1;

			for (size_t i = 0; i < objModelCore->materials.size(); i++)
			{
				MaterialInfo* info = objModelCore->materials[i];
				if(info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if(currentGroup->materialIndex == -1)
				std::cout<<"Could not find material name "<<params[1]<<std::endl;
		}
		//else
		//	std::cout<<line<<std::endl;

	}

	objModelCore->groups.push_back(currentGroup);


	glGenVertexArrays(1, &objModelCore->_vertexArray);
	glBindVertexArray(objModelCore->_vertexArray);
        
    GLuint _vertexBuffer;
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, finalVertices.size()*sizeof(GLfloat), &finalVertices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(12));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(24));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 48, BUFFER_OFFSET(32));
        
    glBindVertexArray(0);

	objModelCore->vertices = new std::vector<float>(vertices);
	createRigidBody(mass, origin, objModelCore->vertices, scale);
}

ObjModel::~ObjModel(void)
{
}

void ObjModel::draw(unsigned int shaderID)
{
	glBindVertexArray(objModelCore->_vertexArray);
	glBindAttribLocation(shaderID, 0, "InVertex");
	glBindAttribLocation(shaderID, 1, "InNormal");
	glBindAttribLocation(shaderID, 2, "InTexCoord0");
	GLint loc = glGetUniformLocation(shaderID, "texture");

	for (size_t i = 0; i < objModelCore->groups.size(); i++)
	{
		ObjGroup* group = objModelCore->groups[i];
		MaterialInfo* material = objModelCore->materials[group->materialIndex];
		if(material->hasTexture)
		{
			//glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, material->texture);
			glUniform1f(loc, material->texture);
		}
		//if(material->bumpMap != NULL)
		//{
		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_2D, material->bumpMap);
		//}
		
		glDrawArrays(GL_TRIANGLES, group->start, group->end - group->start);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

unsigned int textureCreate(std::string filename)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width = 1024, height = 1024, bpp = 32;
	unsigned char* imgData = stbi_load(filename.c_str(), &width, &height, &bpp, 4);

	glTexImage2D(GL_TEXTURE_2D,
		0,					//Scene
		GL_RGBA,			//internal format
		width,				//width
		height,				//height
		0,					//border
		GL_RGBA,			//data format
		GL_UNSIGNED_BYTE,	//data type
		imgData);			//data

	stbi_image_free(imgData);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void ObjModel::loadMaterialFile( std::string fileName, std::string dirName )
{
	std::ifstream pFile(fileName.c_str());

	MaterialInfo* currentMaterial = NULL;

	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		
		line = replace(line, "\t", " ");
		while(line.find("  ") != std::string::npos)
			line = replace(line, "  ", " ");
		if(line == "")
			continue;
		if(line[0] == ' ')
			line = line.substr(1);
		if(line == "")
			continue;
		if(line[line.length()-1] == ' ')
			line = line.substr(0, line.length()-1);
		if(line == "")
			continue;
		if(line[0] == '#')
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		if(params[0] == "newmtl")
		{
			if(currentMaterial != NULL)
			{
				objModelCore->materials.push_back(currentMaterial);
			}
			currentMaterial = new MaterialInfo();
			currentMaterial->name = params[1];
		}
		else if(params[0] == "map_kd")
		{
			currentMaterial->hasTexture = true;
			currentMaterial->textureStr = dirName + "/" + params[1];
			currentMaterial->texture = textureCreate(currentMaterial->textureStr);
		}
		else if(params[0] == "map_bump")
		{
			currentMaterial->bumpMapStr = dirName + "/" + params[1];
			currentMaterial->bumpMap = textureCreate(currentMaterial->bumpMapStr);
		}
		else
			std::cout<<"Didn't parse "<<params[0]<<" in material file"<<std::endl;
	}
	if(currentMaterial != NULL)
		objModelCore->materials.push_back(currentMaterial);

}

ObjModel::MaterialInfo::MaterialInfo()
{
	texture = NULL;
	bumpMap = NULL;
	hasTexture = false;
}
