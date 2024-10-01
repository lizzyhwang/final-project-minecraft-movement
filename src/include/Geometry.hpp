/** @file Geometry.hpp
 *  @brief Organizes vertex and triangle information.
 *  
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <glad/glad.h>
#include <vector>
#include "Utils.hpp"

// Purpose of this class is to store vertice and triangle information
class Geometry{
public:
	// Constructor
	Geometry();
	// Destructor
	~Geometry();
	
	// Functions for working with individual vertices
	unsigned int GetBufferSizeInBytes();
    // Retrieve the 
	unsigned int GetBufferDataSize();
    // Retrieve the Buffer Data Size
	float* GetBufferDataPtr();

	// Functions that update our various lists of vertex information
	// Adds to m_vertexPositions
	void addVertPos(VertexData v);
	// Adds to m_textureCoords
	void addTexCoord(VertexData t);
	// Adds to m_normals
	void addNormal(VertexData n);
	// Adds to m_indices
	void addIndex(GLuint i);
	// Push float to flattened list of x,y,z,s,t,u,v,w
	void addToFinalVertices(float f);
	// Returns the vertPos at the given index
	VertexData getVert(GLuint i);
	// Returns the texCoord at the given index
	VertexData getTex(GLuint i);
	// Returns the normal at the given index
	VertexData getNorm(GLuint i);
	// Returns the index at the given index
	GLuint getIndex(GLuint i);
	// Returns the tangent at the given index
	VertexData getTan(GLuint i);
	// Returns the bitangent at the given index
	VertexData getBiTan(GLuint i);

	// Functions for working with Indices
	// Creates a triangle from 3 indices
	// When a triangle is made, the tangents and bi-tangents are also
	// computed
	void MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2);  
    // Retrieve how many indices there are
	unsigned int GetIndicesSize();
    // Retrieve the pointer to the indices
	unsigned int* GetIndicesDataPtr();

private:
	// m_bufferData stores all of the vertexPositons, coordinates, normals, etc.
	// This is all of the information that should be sent to the vertex Buffer Object
	// list of floats x,y,z,s,t,u,v,w,t1,t2,t3,b1,b2,b3
	std::vector<float> m_finalVertices;

    // // Individual components of 
	// std::vector<float> m_vertexPositions;
	// std::vector<float> m_textureCoords;
	// std::vector<float> m_normals;
	// std::vector<VertexData> m_indices;
	// std::vector<float> m_tangents;
	// std::vector<float> m_biTangents;

	// Object vertices (one VertexData stores its x,y,z)
    std::vector<VertexData> m_vertexPositions;
    // Object vertex normals (one VertexData stores its u,v,w)
    std::vector<VertexData> m_normals;
    // Object texture coordinates (one VertexData stores its s,t)
    std::vector<VertexData> m_textureCoords;
    // Object indices: list of indices of vertices, where every three is a face.
    std::vector<GLuint> m_indices;
	// Object tangent vectors (one VertexData stores its t1,t2,t3)
	std::vector<VertexData> m_tangents;
	// Object tangent vectors (one VertexData stores its b1,b2,b3)
	std::vector<VertexData> m_biTangents;    

	// The indices for a indexed-triangle mesh
	//std::vector<unsigned int> m_indices;
};





#endif
