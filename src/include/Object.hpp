#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "Utils.hpp"
#include "Geometry.hpp"

#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object{
public:
    // Object Constructor, takes in the .obj file to be loaded.
    Object(std::string f);
    // Object destructor
    ~Object();
    // Create a textured quad
    void MakeTexturedQuad();
    // Updates and transformatinos applied to object
    void Update(unsigned int screenWidth, unsigned int screenHeight);
    // How to draw the object
    void Render(bool wireframeMode);
    // Returns an objects transform
    Transform& GetTransform();

private:
    // Filepath to the object
    std::string m_filepath;
    // Directory of the object
    std::string m_obj_dir;
    
    // Store the object's Geometry
    Geometry m_geometry;

    // For now we have one shader per object.
    Shader m_shader;
    // For now we have one buffer per object.
    VertexBufferLayout m_vertexBufferLayout;
    // For now we have one texture per object
    Texture m_texture;
    // We also have one normal map per object
    //Texture m_normalMap;

    // Material information:
    // full filepath to the .mtl file for this Object.
    std::string mtllib;
    // filepath to the diffuse color map.
    std::string map_KD;
    // filepath to the normal map.
    //std::string map_BUMP;


    // Store the objects transformations
    Transform m_transform; 
    // Store the 'camera' projection
    glm::mat4 m_projectionMatrix;

    // sets the filepath for the map_KD ppm and map_Bump ppm.
    void setMaps();
};


#endif
