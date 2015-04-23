#include "TerrainPCH.h"
#include "Terrain.h"
#include <btBulletDynamicsCommon.h>
#include "TerrainManager.h"
#include <iostream>
#include <fstream>
#include "stb_image.h"
#include "FlatLandManager.h"
#include "MountainManager.h"
using namespace std;

// Enable mutitexture blending across the terrain
#ifndef ENABLE_MULTITEXTURE
#define ENABLE_MULTITEXTURE 1
#endif

// Enable the blend constants based on the slope of the terrain
#ifndef ENABLE_SLOPE_BASED_BLEND
#define ENABLE_SLOPE_BASED_BLEND 1
#endif 

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

inline float GetPercentage( float value, const float min, const float max )
{
	value = glm::clamp( value, min, max );
	return ( value - min ) / ( max - min );
}

// Get's the size of the file in bytes.
inline int GetFileLength( std::istream& file )
{
	int pos = file.tellg();
	file.seekg(0, std::ios::end );
	int length = file.tellg();
	// Restore the position of the get pointer
	file.seekg(pos);

	return length;
}

inline void DeleteVertexBuffer( GLuint& vboID )
{
	if ( vboID != 0 )
	{
		glDeleteBuffersARB( 1, &vboID );
		vboID = 0;
	}
}

inline void CreateVertexBuffer( GLuint& vboID )
{
	// Make sure we don't loose the reference to the previous VBO if there is one
	DeleteVertexBuffer( vboID );
	glGenBuffersARB( 1, &vboID );
}

inline void DeleteTexture( GLuint& texID )
{
	if ( texID != 0 )
	{
		glDeleteTextures( 1, &texID );
		texID = 0;
	}
}

inline void CreateTexture( GLuint& texID )
{
	DeleteTexture( texID );
	glGenTextures(1, &texID );
}

// Convert data from the char buffer to a floating point value between 0..1
// depending on the storage value of the original data
// NOTE: This only works with (LSB,MSB) data storage.
inline float GetHeightValue( const unsigned char* data, char numBytes )
{
	switch ( numBytes )
	{
	case 1:
		{
			return (char)(data[0]) / (float)0xff;
		}
		break;
	case 2:
		{
			return (short)(data[1] << 8 | data[0] ) / (float)0xffff;
		}
		break;
	case 4:
		{
			return (int)(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0] ) / (float)0xffffffff;
		}
		break;
	default: 
		{
			assert(false);  // Height field with non standard pixle sizes
		}
		break;
	}

	return 0.0f;
}

Terrain::Terrain( float heightScale /* = 500.0f */, float blockScale /* = 2.0f */ )
: m_GLVertexBuffer(0)
, m_GLNormalBuffer(0)
, m_GLColorBuffer(0)
, m_GLTex0Buffer(0)
, m_GLTex1Buffer(0)
, m_GLTex2Buffer(0)
, m_GLIndexBuffer(0)
, m_LocalToWorldMatrix(1)
, m_InverseLocalToWorldMatrix(1)
, m_HeightmapDimensions(0,0)
, m_fHeightScale( heightScale )
, m_fBlockScale( blockScale )
{
	memset( m_GLTextures, 0, sizeof(m_GLTextures) );
}

Terrain::~Terrain()
{
	Terminate();
}

void Terrain::Terminate()
{
	DeleteVertexBuffer(m_GLVertexBuffer);
	DeleteVertexBuffer(m_GLNormalBuffer);
	DeleteVertexBuffer(m_GLColorBuffer);
	DeleteVertexBuffer(m_GLTex0Buffer);
	DeleteVertexBuffer(m_GLTex1Buffer);
	DeleteVertexBuffer(m_GLTex2Buffer);
	DeleteVertexBuffer(m_GLIndexBuffer);

	for ( unsigned int i = 0; i < m_uiNumTextures; ++i )
	{
		DeleteTexture( m_GLTextures[i] );
	}
}

void Terrain::init_FlatGrassLand()
{
	m_TexturePath = "c:/VrCave/Development/VirtualLab/Data/grass4.png";
	m_TerrainManager = new FlatLandManager();
}
void Terrain::init_MountainRange()
{
	m_TexturePath = "c:/VrCave/Development/VirtualLab/Data/rock.png";
	m_TerrainManager = new MountainManager();
}

void Terrain::createVegetation(int amount, btDiscreteDynamicsWorld* world)
{
	m_TerrainManager->createVegetation(amount, world);
}

bool Terrain::LoadTexture( const std::string& filename, unsigned int textureStage /*= 0*/ )
{
	assert( textureStage < m_uiNumTextures );
	DeleteTexture( m_GLTextures[textureStage] );
	int width = 1024, height = 1024, bpp = 32;
	unsigned char * data = stbi_load(m_TexturePath.c_str(), &width, &height, &bpp, 4);
	glGenTextures(1, &m_GLTextures[textureStage]);
	glBindTexture( GL_TEXTURE_2D, m_GLTextures[textureStage] );

	//glTexImage2D(GL_TEXTURE_2D,
	//	0,
	//	GL_RGBA,
	//	width,
	//	height,
	//	0,
	//	GL_RGBA,
	//	GL_UNSIGNED_BYTE,
	//	data);

	gluBuild2DMipmaps(GL_TEXTURE_2D,
		4,
		width,
		height,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);

	stbi_image_free(data);

	GLfloat fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	//glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glBindTexture( GL_TEXTURE_2D, 0 );

	return ( m_GLTextures[textureStage] != 0 );
}

bool Terrain::LoadHeightmap(unsigned int width, unsigned int height )
{
	const int bytesPerPixel = 1;
	unsigned char * heightmap = m_TerrainManager->CreateRandomTerrain(width, height);

	unsigned int numVerts = width * height;
	m_PositionBuffer.resize( numVerts );
	m_ColorBuffer.resize( numVerts );
	m_NormalBuffer.resize( numVerts );
	m_Tex0Buffer.resize( numVerts );
	
	m_HeightmapDimensions = glm::ivec2(width, height);

	// Size of the terrain in world units
	float terrainWidth = ( width - 1 ) * m_fBlockScale;
	float terrainHeight = ( height - 1 ) * m_fBlockScale;

	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f;

	for ( unsigned int j = 0; j < height; ++j )
	{
		for ( unsigned i = 0; i < width; ++i )
		{
			int index = ( j * width ) + i;
			float heightValue = GetHeightValue(&heightmap[index * bytesPerPixel], bytesPerPixel);

			float S = ( i / (float)(width - 1) );
			float T = ( j / (float)(height - 1) );

			float X = ( S * terrainWidth ) - halfTerrainWidth;
			float Y = heightValue * m_fHeightScale;
			float Z = ( T * terrainHeight ) - halfTerrainHeight;

			// Blend 3 textures depending on the height of the terrain
			float tex0Contribution = 1.0f - GetPercentage( heightValue, 0.0f, 0.2f );
			float tex2Contribution = 1.0f - GetPercentage( heightValue, 0.2f, 1.0f );

			m_NormalBuffer[index] = glm::vec3(0);
			m_PositionBuffer[index] = glm::vec3( X, Y, Z );
#if ENABLE_MULTITEXTURE
			m_ColorBuffer[index] = glm::vec4( tex0Contribution, tex0Contribution, tex0Contribution, tex2Contribution );
#else
			m_ColorBuffer[index] = glm::vec4(1.0f);
#endif
			m_Tex0Buffer[index] = glm::vec2( S, T );
		}
	}

	std::cout << "Terrain has been loaded!" << std::endl;
	delete[] heightmap;

	GenerateIndexBuffer();
	GenerateNormals();
	GenerateVertexBuffers();
		
	return true;
}

void Terrain::GenerateIndexBuffer()
{
	if ( m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2 )
	{
		// Terrain hasn't been loaded, or is of an incorrect size
		return;
	}

	const unsigned int terrainWidth = m_HeightmapDimensions.x;
	const unsigned int terrainHeight = m_HeightmapDimensions.y;

	// 2 triangles for every quad of the terrain mesh
	const unsigned int numTriangles = ( terrainWidth - 1 ) * ( terrainHeight - 1 ) * 2;

	// 3 indices for each triangle in the terrain mesh
	m_IndexBuffer.resize( numTriangles * 3 );

	unsigned int index = 0; // Index in the index buffer
	for (unsigned int j = 0; j < (terrainHeight - 1); ++j )
	{
		for (unsigned int i = 0; i < (terrainWidth - 1); ++i )
		{
			int vertexIndex = ( j * terrainWidth ) + i;
			// Top triangle (T0)
			m_IndexBuffer[index++] = vertexIndex;                           // V0
			m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
			m_IndexBuffer[index++] = vertexIndex + 1;                       // V1
			// Bottom triangle (T1)
			m_IndexBuffer[index++] = vertexIndex;                           // V0
			m_IndexBuffer[index++] = vertexIndex + terrainWidth;            // V2
			m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        // V3
		}
	}
}

void Terrain::GenerateNormals()
{
	for ( unsigned int i = 0; i < m_IndexBuffer.size(); i += 3 )
	{
		glm::vec3 v0 = m_PositionBuffer[ m_IndexBuffer[i + 0] ];
		glm::vec3 v1 = m_PositionBuffer[ m_IndexBuffer[i + 1] ];
		glm::vec3 v2 = m_PositionBuffer[ m_IndexBuffer[i + 2] ];

		glm::vec3 normal = glm::normalize( glm::cross( v1 - v0, v2 - v0 ) );

		m_NormalBuffer[ m_IndexBuffer[i + 0] ] += normal;
		m_NormalBuffer[ m_IndexBuffer[i + 1] ] += normal;
		m_NormalBuffer[ m_IndexBuffer[i + 2] ] += normal;
	}

	const glm::vec3 UP( 0.0f, 1.0f, 0.0f );
	for ( unsigned int i = 0; i < m_NormalBuffer.size(); ++i )
	{
		m_NormalBuffer[i] = glm::normalize( m_NormalBuffer[i] );

#if ENABLE_SLOPE_BASED_BLEND
		float temp = glm::dot(m_NormalBuffer[i], UP) - 0.1f;
		if (temp < 0.0)
			temp = 0.0;
		else if (temp > 1.0)
			temp = 1.0;
		float fTexture0Contribution = temp;
		m_ColorBuffer[i] = glm::vec4( fTexture0Contribution, fTexture0Contribution, fTexture0Contribution, m_ColorBuffer[i].w );
#endif

	}
}

void Terrain::GenerateVertexBuffers()
{
	// First generate the buffer object ID's
	CreateVertexBuffer(m_GLVertexBuffer);
	CreateVertexBuffer(m_GLNormalBuffer);
	CreateVertexBuffer(m_GLColorBuffer);
	CreateVertexBuffer(m_GLTex0Buffer);
	CreateVertexBuffer(m_GLTex1Buffer);
	CreateVertexBuffer(m_GLTex2Buffer);
	CreateVertexBuffer(m_GLIndexBuffer);

	// Copy the host data into the vertex buffer objects
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_GLVertexBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * m_PositionBuffer.size(), &(m_PositionBuffer[0]), GL_STATIC_DRAW_ARB ); 

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_GLColorBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec4) * m_ColorBuffer.size(), &(m_ColorBuffer[0]), GL_STATIC_DRAW_ARB ); 

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_GLNormalBuffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec3) * m_NormalBuffer.size(), &(m_NormalBuffer[0]), GL_STATIC_DRAW_ARB ); 

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_GLTex0Buffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW_ARB ); 

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_GLTex1Buffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW_ARB ); 

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_GLTex2Buffer );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_STATIC_DRAW_ARB ); 

	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_GLIndexBuffer );
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(GLuint) * m_IndexBuffer.size(), &(m_IndexBuffer[0]), GL_STATIC_DRAW_ARB ); 
}

float Terrain::GetHeightAt( const glm::vec3& position )
{
	float height = -FLT_MAX;
	// Check if the terrain dimensions are valid
	if ( m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2 ) return height;

	// Width and height of the terrain in world units
	float terrainWidth = ( m_HeightmapDimensions.x - 1) * m_fBlockScale;
	float terrainHeight = ( m_HeightmapDimensions.y - 1) * m_fBlockScale;
	float halfWidth = terrainWidth * 0.5f;
	float halfHeight = terrainHeight * 0.5f;

	// Multiple the position by the inverse of the terrain matrix 
	// to get the position in terrain local space
	glm::vec3 terrainPos = glm::vec3( m_InverseLocalToWorldMatrix * glm::vec4(position, 1.0f) );
	glm::vec3 invBlockScale( 1.0f/m_fBlockScale, 0.0f, 1.0f/m_fBlockScale );

	// Calculate an offset and scale to get the vertex indices
	glm::vec3 offset( halfWidth, 0.0f, halfHeight );

	// Get the 4 vertices that make up the triangle we're over
	glm::vec3 vertexIndices = ( terrainPos + offset ) * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if ( u0 >= 0 && u1 < (int)m_HeightmapDimensions.x && v0 >= 0 && v1 < (int)m_HeightmapDimensions.y )
	{
		glm::vec3 p00 = m_PositionBuffer[ ( v0 * m_HeightmapDimensions.x ) + u0 ];    // Top-left vertex
		glm::vec3 p10 = m_PositionBuffer[ ( v0 * m_HeightmapDimensions.x ) + u1 ];    // Top-right vertex
		glm::vec3 p01 = m_PositionBuffer[ ( v1 * m_HeightmapDimensions.x ) + u0 ];    // Bottom-left vertex
		glm::vec3 p11 = m_PositionBuffer[ ( v1 * m_HeightmapDimensions.x ) + u1 ];    // Bottom-right vertex

		// Which triangle are we over?
		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		glm::vec3 dU, dV;
		if (percentU > percentV)
		{   // Top triangle
			dU = p10 - p00;
			dV = p11 - p10;
		}
		else
		{   // Bottom triangle
			dU = p11 - p01;
			dV = p01 - p00;
		}

		glm::vec3 heightPos = p00 + ( dU * percentU ) + ( dV * percentV );
		// Convert back to world-space by multiplying by the terrain's world matrix
		heightPos = glm::vec3( m_LocalToWorldMatrix * glm::vec4(heightPos, 1) );
		height = heightPos.y;
	}

	return height;
}

void Terrain::Render()
{


//#if _DEBUG
	DebugRender();
//#endif

}

void Terrain::RenderNormals()
{
	//glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	//glMultMatrixf( glm::value_ptr(m_LocalToWorldMatrix) );

	glPushAttrib( GL_ENABLE_BIT );
	glEnable( GL_LIGHTING );

	glColor3f( 1.0f, 1.0f, 0.0f );// Yellow
	//glScalef(0.001, 0.001, 0.001);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_GLTextures[0]);
	glScalef(1, 5, 1);
	glBegin( GL_TRIANGLES );
	{
		for (unsigned long int i = 0; i < m_IndexBuffer.size() ; i+=6)
		{
			//glm::vec3 p1 = ( m_PositionBuffer[i] + m_NormalBuffer[i] );
			glTexCoord2i(0, 0);
			glNormal3fv(glm::value_ptr(m_NormalBuffer[m_IndexBuffer[i + 0]]));
			glVertex3fv(glm::value_ptr(m_PositionBuffer[m_IndexBuffer[i + 0]]));
			glTexCoord2i(1, 0);
			glNormal3fv(glm::value_ptr(m_NormalBuffer[m_IndexBuffer[i + 1]]));
			glVertex3fv(glm::value_ptr(m_PositionBuffer[m_IndexBuffer[i + 1]]));
			glTexCoord2i(0, 1);
			glNormal3fv(glm::value_ptr(m_NormalBuffer[m_IndexBuffer[i + 2]]));
			glVertex3fv(glm::value_ptr(m_PositionBuffer[m_IndexBuffer[i + 2]]));

			glTexCoord2i(0, 1);
			glNormal3fv(glm::value_ptr(m_NormalBuffer[m_IndexBuffer[i + 3]]));
			glVertex3fv(glm::value_ptr(m_PositionBuffer[m_IndexBuffer[i + 3]]));
			glTexCoord2i(1, 0);
			glNormal3fv(glm::value_ptr(m_NormalBuffer[m_IndexBuffer[i + 4]]));
			glVertex3fv(glm::value_ptr(m_PositionBuffer[m_IndexBuffer[i + 4]]));
			glTexCoord2i(1, 1);
			glNormal3fv(glm::value_ptr(m_NormalBuffer[m_IndexBuffer[i + 5]]));
			glVertex3fv(glm::value_ptr(m_PositionBuffer[m_IndexBuffer[i + 5]]));
			//glVertex3fv( glm::value_ptr(p1) );
		}
	}
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPopAttrib();

	glPopMatrix();
}

void Terrain::DebugRender()
{
	RenderNormals();
}