// ==================================================================
#version 330 core

layout(location=0)in vec3 position; // We explicitly state which is the vertex
                                    // information (The first 3 floats are
                                    // positional data, we are putting in
                                    // our vector)
// Take 'in' the texture coordinates from our
// vertex buffer object (VBO) layout.
// TODO:
layout(location=1)in vec2 texCoord;

// If we have texture coordinates we will need
// to pass these into the fragment shader. 
// We create a 'vec2' and the 'out' qualifier 
// implies that we will read this variable 'in'
// a later stage of the graphics
// pipeline (i.e. our fragment shader)
out vec2 v_texCoord;

// If we are applying our camera, then we need to add some uniforms.
// Recall that the vertex positions 'vec3 postion' are the objects
// positions in 'local space'
// Then we have the 'modelTransformMatrix' which is part of the model view
// transformation.
// And finally the 'projectionMatrix' which will transform our vertices
// into our chosen projection (i.e. for us, a perspective view).
//
// Note: that the syntax nicely matches glm's mat4!
//
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  // gl_Position is a special glsl variable that tells us what
  // position to put things in.
  // It takes in exactly 4 things.
  // Note that 'w' (the 4th dimension) should be 1.
  // vec4 oldPosition = vec4(position.x, position.y, position.z, 1.0f);
  // vec4 newPositon = modelTransformMatrix * oldPosition;
  // vec4 projectedPosition = projectionMatrix * newPositon;

  // gl_Position = projectedPosition;

  gl_Position = projection * view * model * vec4(position, 1.0f);

  // Store the texture coordinaets which we will output to
  // the next stage in the graphics pipeline.
  v_texCoord = texCoord;
}
// ==================================================================
