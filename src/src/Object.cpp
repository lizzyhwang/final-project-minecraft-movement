#include "Object.hpp"
#include "Camera.hpp"
#include "Error.hpp"

#include <map>
#include <utility>
#include <tuple>

Object::Object(std::string f):m_filepath(f) {
	m_obj_dir = f.substr(0, f.find_last_of("/\\") + 1);

	std::cout << "Loading: " << m_filepath << std::endl;

    std::ifstream inFile;  
    inFile.open(m_filepath);

        if(inFile.is_open()) {
			// map of <Faces v/vt/vn, index>
			// each Face v/vt/vn is a face we've seen so far in the file.
			// index is which vertex in m_vertices to use
			// m_vertices holds no overlapping data of x,y,z,s,t,u,v,w
			// m_indices holds indices _i_ of which "x,y,z,s,t,u,v,w" chunk _i_ to use
			// every three m_indices is a triangle!!
			std::map<std::tuple<int,int,int>, int> vertexDataSeenSoFar;
			std::map<std::tuple<int,int,int>, int>::iterator it;
			int indexCounter = 0;

            std::string line;
            while(getline(inFile,line)){
				// get the corresponding MTL file.
				if (line.substr(0,6) == "mtllib") {
					std::stringstream lineStream(line);
                    std::string token;
                    std::vector<std::string> tokens;
					while(lineStream >> token) {
                        tokens.push_back(token);
                    }
					mtllib = m_obj_dir + tokens[1];
				}
                if (line[0] == 'v' || line[0] == 'f') {
                    std::stringstream lineStream(line);
                    std::string token;
                    std::vector<std::string> tokens;

                    while(lineStream >> token) {
                        tokens.push_back(token);
                    }
                    if (line[0] == 'v' && line[1] == ' ') {
                        VertexData vert;
                        vert.x = stof(tokens[1]);
                        vert.y = stof(tokens[2]);
                        vert.z = stof(tokens[3]);
						m_geometry.addVertPos(vert);
                        continue;
                    }
                    if (line[0] == 'v' && line[1] == 'n') {
                        VertexData norm;
                        norm.u = stof(tokens[1]);
                        norm.v = stof(tokens[2]);
                        norm.w = stof(tokens[3]);
						m_geometry.addNormal(norm);
                        continue;
                    }
					if (line[0] == 'v' && line[1] == 't') {
                        VertexData texcoord;
                        texcoord.s = stof(tokens[1]);
                        texcoord.t = stof(tokens[2]);
						m_geometry.addTexCoord(texcoord);
                        continue;
                    }
                    if (line[0] == 'f' && line[1] == ' ') {
                        for (int i = 1; i < tokens.size(); i++) {
                            char str[tokens[i].size()];
                            strcpy(str, tokens[i].c_str());

                            // get which v index for this face corner
                            char *faceVertIndex = strtok(str, "/");
							// get which vt index for this face corner
                            char *faceTexIndex = strtok(NULL, "/");
							// get which vn index for this face corner
                            char *faceNormIndex = strtok(NULL, "/");

							// one vertex information for the current face
                            std::tuple<int,int,int> vertInfoIndices;
							vertInfoIndices = std::make_tuple(atoi(faceVertIndex) - 1, atoi(faceTexIndex) - 1, atoi(faceNormIndex) - 1);

							it = vertexDataSeenSoFar.find(vertInfoIndices);
							
                            if (it != vertexDataSeenSoFar.end()) {
								m_geometry.addIndex(it->second);
							} else {
								vertexDataSeenSoFar[vertInfoIndices] = indexCounter;

								int v = std::get<0>(vertInfoIndices);
								int t = std::get<1>(vertInfoIndices);
								int n = std::get<2>(vertInfoIndices);

								m_geometry.addToFinalVertices(m_geometry.getVert(v).x);
								m_geometry.addToFinalVertices(m_geometry.getVert(v).y);
								m_geometry.addToFinalVertices(m_geometry.getVert(v).z);
								m_geometry.addToFinalVertices(m_geometry.getTex(t).s);
								m_geometry.addToFinalVertices(m_geometry.getTex(t).t);
								m_geometry.addToFinalVertices(m_geometry.getNorm(n).u);
								m_geometry.addToFinalVertices(m_geometry.getNorm(n).v);
								m_geometry.addToFinalVertices(m_geometry.getNorm(n).w);

								m_geometry.MakeTriangle(v,t,n);
								VertexData tan;
								VertexData biTan;
								tan = m_geometry.getTan(indexCounter);
								biTan = m_geometry.getBiTan(indexCounter);
								m_geometry.addToFinalVertices(tan.t1);
								m_geometry.addToFinalVertices(tan.t2);
								m_geometry.addToFinalVertices(tan.t2);
								m_geometry.addToFinalVertices(biTan.b1);
								m_geometry.addToFinalVertices(biTan.b2);
								m_geometry.addToFinalVertices(biTan.b3);

								m_geometry.addIndex(indexCounter);
								indexCounter++;
							}
                        }
                    }                      
                }
            }
        }
    inFile.close();

}

Object::~Object() {
}

// returns the full filepath to the map_kd ppm file specified in the .mtl file.
void Object::setMaps() {
    std::ifstream inFile;
    inFile.open(mtllib);
        if(inFile.is_open()) {
            std::string line;
            while(getline(inFile,line)){
                if(line.substr(0,6) == "map_Kd") {
                    std::stringstream lineStream(line);
                    std::string token;
                    std::vector<std::string> tokens;
					while(lineStream >> token) {
                        tokens.push_back(token);
                    }
                    map_KD =  m_obj_dir + tokens[1];
                }
				// if(line.substr(0,8) == "map_Bump") {
				// 	std::stringstream lineStream(line);
                //     std::string token;
                //     std::vector<std::string> tokens;
				// 	while(lineStream >> token) {
                //         tokens.push_back(token);
                //     }
                //     map_BUMP =  m_obj_dir + tokens[1];
				// }
            }
        }
}

// Initialization of object as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our objects at the correct time
void Object::MakeTexturedQuad()
{
	// Create a buffer and set the stride of information
	// NOTE: We are leveraging our data structure in order to very cleanly
	//       get information into and out of our data structure.
	// this one has stride 14
    m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                    m_geometry.GetIndicesSize(),
                                    m_geometry.GetBufferDataPtr(),
                                    m_geometry.GetIndicesDataPtr());

	// Load our actual texture
	// We are using the input parameter as our texture to load
	setMaps();
	m_texture.LoadTexture(map_KD.c_str());
	// Load the normal map texture
	//m_normalMap.LoadTexture(map_BUMP.c_str());

	// Setup shaders
	std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
	std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");
	// Actually create our shader
	m_shader.CreateShader(vertexShader, fragmentShader);
}

void Object::Update(unsigned int screenWidth, unsigned int screenHeight)
{
	m_texture.Unbind();
	// Make sure we are updating the correct 'buffers'
	m_vertexBufferLayout.Bind();
	m_texture.Bind(0);
	//m_normalMap.Bind(1);
	m_shader.Bind();
	// For our object, we apply the texture in the following way
	// Note that we set the value to 0, because we have bound
	// our texture to slot 0.
	m_shader.SetUniformMatrix1i("u_Texture", 0);
	//m_shader.SetUniformMatrix1i("u_NormalMap", 1);

	// Here we apply the 'view' matrix which creates perspective.
	// The first argument is 'field of view'
	// Then perspective
	// Then the near and far clipping plane.
	// Note I cannot see anything closer than 0.1f units from the screen.
	// TODO: In the future this type of operation would be abstracted away
	//       in a camera class.
	//        modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,-1.0f));
	m_projectionMatrix = glm::perspective(45.5f, (float)screenWidth / (float)screenHeight, 0.01f, 10.0f);
	// Set the uniforms in our current shader
	m_shader.SetUniformMatrix4fv("model", m_transform.GetTransformMatrix());
	m_shader.SetUniformMatrix4fv("view", &Camera::Instance().GetWorldToViewmatrix()[0][0]);
	m_shader.SetUniformMatrix4fv("projection", &m_projectionMatrix[0][0]);

	// Create a first 'light'
    // Set in a light source position
    m_shader.SetUniform3f("lightPos",0.0f,0.0f,0.0f);	
    // Set a view and a vector
    m_shader.SetUniform3f("viewPos",0.0f,0.0f,0.0f);
}

void Object::Render(bool wireframeMode)
{
	// Render data

	if (wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glDrawElements(GL_TRIANGLES,
				   m_geometry.GetIndicesSize(), // The number of indices, not triangles.
				   GL_UNSIGNED_INT,	 // Make sure the data type matches
				   nullptr);		 // Offset pointer to the data. nullptr
									 // because we are currently bound:
}

// Returns the actual transform stored in our object
// which can then be modified
Transform &Object::GetTransform()
{
	return m_transform;
}
