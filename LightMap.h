/* 
 * File:   LightMap.h
 * Author: dude719
 *
 * Created on September 19, 2015, 5:01 AM
 */

#ifndef LIGHTMAP_H
#define	LIGHTMAP_H

#include "Image.h"

#include "GLEW/glew.h"
#include "glm/glm.hpp"

class Node 
{
public:
    Node() : 
    x(0), y(0), 
    width(0), height(0), 
    filled(0) 
    { nodes[0] = NULL; nodes[1] = NULL; }
    
    Node* nodes[2];
    int x, y, width, height;
    bool filled;
    
 };

class LightMap
{
public:
    LightMap();
    virtual ~LightMap();
    
    LightMap* Init();
    
private:
    Node* _allocateRect(int width, int height, Node* node); 
    
    unsigned int texture;
    Node* rectTree;
    
    static int LIGHTMAP_SIZE;
};

#endif	/* LIGHTMAP_H */

