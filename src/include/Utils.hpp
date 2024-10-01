#ifndef UTILS_HPP
#define UTILS_HPP

/** @file Utils.h
 *  @brief structs for color.
 *  
 *  Note this is implemented as a header only library.
 *  This is to make this code easy to be shared.
 *
 *  Typically in graphics we work with RGB or RGBA.
 *  In other domains color may be stored as
 *  BGR, GRBA, and every other permutation.
 *
 *    
 *  @author Lizzy Hwang
 *  @bug No known bugs.
 */

#include <iostream>


struct VertexData{
    float x,y,z; // position of vertex
    float s,t;   // Texture coordinates
    float u,v,w;  // vert normal coordinates
    float t1,t2,t3;
    float b1,b2,b3;

    // constructor
    //VertexData(float _x, float _y, float _z, float _s, float _t, float _u, float _v, float _w): x(_x),y(_y),z(_z),s(_s),t(_t),u(_u),v(_v),w(_w) { }

    // Tests if two VertexData are equal
	bool operator== (const VertexData &rhs){
		if( (x == rhs.x) && (y == rhs.y) && (z == rhs.z) && (s == rhs.s) && (t == rhs.t) && (u == rhs.u) && (v == rhs.v) && (w == rhs.w)){
			return true;
		}
		return false;
	}
};


#endif
