#pragma once;
#include "TerrainManager.h"
#include <string>

class Terrain
{
public:
	typedef std::vector<glm::vec3>  PositionBuffer;
	typedef std::vector<unsigned int>     IndexBuffer;
	PositionBuffer  m_PositionBuffer;
	IndexBuffer     m_IndexBuffer;
	std::string m_TexturePath;
	TerrainManager* m_TerrainManager;

	Terrain( float heightScale = 500.0f, float blockScale = 2.0f );
	virtual ~Terrain();

	void Terminate();
	void init_FlatGrassLand();
	void init_MountainRange();
	bool LoadHeightmap(unsigned int width, unsigned int height );
	bool LoadTexture( const std::string& filename, unsigned int textureStage = 0 );
	void createVegetation(int amount, btDiscreteDynamicsWorld* world);

	// Get the height of the terrain at a position in world space
	float GetHeightAt( const glm::vec3& position );

	void Render();
	// In debug builds, the terrain normals will be rendered.
	void DebugRender();

protected:
	void GenerateIndexBuffer();
	void GenerateNormals();
	// Generates the vertex buffer objects from the 
	// position, normal, texture, and color buffers
	void GenerateVertexBuffers();

	void RenderNormals();

private:
	typedef std::vector<glm::vec4>  ColorBuffer;
	typedef std::vector<glm::vec3>  NormalBuffer;
	typedef std::vector<glm::vec2>  TexCoordBuffer;

	ColorBuffer     m_ColorBuffer;
	NormalBuffer    m_NormalBuffer;
	TexCoordBuffer  m_Tex0Buffer;

	// ID's for the VBO's
	unsigned int m_GLNormalBuffer;
	unsigned int m_GLColorBuffer;
	unsigned int m_GLTex0Buffer;
	unsigned int m_GLTex1Buffer;
	unsigned int m_GLTex2Buffer;
	unsigned int m_GLIndexBuffer;
	unsigned int m_GLVertexBuffer;
	

	static const unsigned int m_uiNumTextures = 3;
	unsigned int m_GLTextures[m_uiNumTextures];

	glm::mat4x4 m_LocalToWorldMatrix;
	glm::mat4x4 m_InverseLocalToWorldMatrix;

	// The dimensions of the heightmap texture
	glm::ivec2 m_HeightmapDimensions;

	// The height-map value will be multiplied by this value
	// before it is assigned to the vertex's Y-coordinate
	float m_fHeightScale;
	// The vertex's X and Z coordinates will be multiplied by this
	// for each step when building the terrain
	float m_fBlockScale;
};
