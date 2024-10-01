#include "../include/Geometry.hpp"
#include <assert.h>
#include <iostream>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/glm.hpp"

// Constructor
Geometry::Geometry(){

}

// Destructor
Geometry::~Geometry(){

}


// Retrieves a pointer to our data.
float* Geometry::GetBufferDataPtr(){
	return m_finalVertices.data();
}

// Retrieves the size of our data 
unsigned int Geometry::GetBufferDataSize(){
	return m_finalVertices.size();
}

// Retrieves the number of bytes of our data
unsigned int Geometry::GetBufferSizeInBytes(){
	return m_finalVertices.size()*sizeof(float);
}

// Adds to m_vertexPositions
void Geometry::addVertPos(VertexData v) {
	m_vertexPositions.push_back(v);
}
// Adds to m_textureCoords
void Geometry::addTexCoord(VertexData t) {
	m_textureCoords.push_back(t);
}
// Adds to m_normals
void Geometry::addNormal(VertexData n) {
	m_normals.push_back(n);
}
// Adds to m_indices
void Geometry::addIndex(GLuint i) {
	m_indices.push_back(i);
}

// Returns the vertPos at the given index
VertexData Geometry::getVert(GLuint i) {
	return m_vertexPositions[i];
}

// Returns the texCoord at the given index
VertexData Geometry::getTex(GLuint i) {
	return m_textureCoords[i];
}

// Returns the normal at the given index
VertexData Geometry::getNorm(GLuint i) {
	return m_normals[i];
}

// Returns the normal at the given index
GLuint Geometry::getIndex(GLuint i) {
	return m_indices[i];
}

// Push float to flattened list of x,y,z,s,t,u,v,w
void Geometry::addToFinalVertices(float f) {
	m_finalVertices.push_back(f);
}

// Returns the tangent at the given index
VertexData Geometry::getTan(GLuint i) {
	return m_tangents[i];
}
// Returns the bitangent at the given index
VertexData Geometry::getBiTan(GLuint i) {
	return m_biTangents[i];
}

// The big trick here, is that when we make a triangle
// We also need to update our m_normals, tangents, and bi-tangents.
void Geometry::MakeTriangle(unsigned int vert0, unsigned int vert1, unsigned int vert2){

	// Look up the actual vertex positions
	glm::vec3 pos0(m_vertexPositions[vert0*3 +0].x, m_vertexPositions[vert0*3 + 1].y, m_vertexPositions[vert0*3 + 2].z); 
	glm::vec3 pos1(m_vertexPositions[vert1*3 +0].x, m_vertexPositions[vert1*3 + 1].y, m_vertexPositions[vert1*3 + 2].z); 
	glm::vec3 pos2(m_vertexPositions[vert2*3 +0].x, m_vertexPositions[vert2*3 + 1].y, m_vertexPositions[vert2*3 + 2].z); 

	// Look up the texture coordinates
	glm::vec2 tex0(m_textureCoords[vert0*2 +0].s, m_textureCoords[vert0*2 + 1].t); 
	glm::vec2 tex1(m_textureCoords[vert1*2 +0].s, m_textureCoords[vert1*2 + 1].t); 
	glm::vec2 tex2(m_textureCoords[vert2*2 +0].s, m_textureCoords[vert2*2 + 1].t); 

	// Now create an edge
	// With two edges
	// This section is inspired by: https://learnopengl.com/Advanced-Lighting/Normal-Mapping
	glm::vec3 edge0 = pos1 - pos0;
	glm::vec3 edge1 = pos2 - pos0;
	// Question to ask yourself is what is going on here?
    // The difference of y's and x's? Hmm.
	glm::vec2 deltaUV0 = tex1-tex0;
	glm::vec2 deltaUV1 = tex2-tex0;

	float f = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

	VertexData tangent;
	VertexData bitangent;

	tangent.t1 = f * (deltaUV1.y * edge0.x - deltaUV0.y* edge1.x);
	tangent.t2 = f * (deltaUV1.y * edge0.y - deltaUV0.y* edge1.y);
	tangent.t3 = f * (deltaUV1.y * edge0.z - deltaUV0.y* edge1.z);
	//tangent = glm::normalize(tangent);

	bitangent.b1 = f * (-deltaUV1.x * edge0.x + deltaUV0.x* edge1.x);
	bitangent.b2 = f * (-deltaUV1.x * edge0.y + deltaUV0.x* edge1.y);
	bitangent.b3 = f * (-deltaUV1.x * edge0.z + deltaUV0.x* edge1.z);
	//bitangent = glm::normalize(bitangent);
	
	// Compute a normal
	// For now we sort of 'cheat' since this is a quad the 'z' axis points straight out
    // glm::vec3 normal1{m_normals[vert0*3+0] ,m_normals[vert0*3+1], m_normals[vert0*3+2]};
    // glm::vec3 normal2{m_normals[vert1*3+0] ,m_normals[vert1*3+1], m_normals[vert1*3+2]};
    // glm::vec3 normal3{m_normals[vert2*3+0] ,m_normals[vert2*3+1], m_normals[vert2*3+2]};

	// basic push_back for m_tangents and m_biTangents , which are lists of VertexData	
	m_tangents.push_back(tangent);
	m_biTangents.push_back(bitangent);

	// Compute a tangent
	// m_tangents[vert0*3+0] = tangent.x; m_tangents[vert0*3+1] = tangent.y; m_tangents[vert0*3+2] = tangent.z;	
	// m_tangents[vert1*3+0] = tangent.x; m_tangents[vert1*3+1] = tangent.y; m_tangents[vert1*3+2] = tangent.z;	
	// m_tangents[vert2*3+0] = tangent.x; m_tangents[vert2*3+1] = tangent.y; m_tangents[vert2*3+2] = tangent.z;	

	// // Compute a bi-tangent
	// m_biTangents[vert0*3+0] = bitangent.x; m_biTangents[vert0*3+1] = bitangent.y; m_biTangents[vert0*3+2] = bitangent.z;	
	// m_biTangents[vert1*3+0] = bitangent.x; m_biTangents[vert1*3+1] = bitangent.y; m_biTangents[vert1*3+2] = bitangent.z;	
	// m_biTangents[vert2*3+0] = bitangent.x; m_biTangents[vert2*3+1] = bitangent.y; m_biTangents[vert2*3+2] = bitangent.z;	
}

// Retrieves the number of indices that we have.
unsigned int Geometry::GetIndicesSize(){
	return m_indices.size();
}

// Retrieves a pointer to the indices that we have
unsigned int* Geometry::GetIndicesDataPtr(){
	return m_indices.data();
}
