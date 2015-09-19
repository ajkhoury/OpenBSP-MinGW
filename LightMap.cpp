/* 
 * File:   LightMap.cpp
 * Author: dude719
 * 
 * Created on September 19, 2015, 5:01 AM
 */

#include "LightMap.h"

int LightMap::LIGHTMAP_SIZE = 512;

LightMap::LightMap()
{
}

LightMap::~LightMap()
{
}

LightMap* LightMap::Init()
{
    glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, LIGHTMAP_SIZE, LIGHTMAP_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    unsigned char whitePixels[16] = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
    glTexSubImage2D(GL_TEXTURE_2D, 0, LIGHTMAP_SIZE-2, LIGHTMAP_SIZE-2, 2, 2, GL_RGBA, GL_UNSIGNED_BYTE, whitePixels);
    
    rectTree = new Node();
    rectTree->width = LIGHTMAP_SIZE;
    rectTree->height = LIGHTMAP_SIZE;
    
    return this;
}

//Node LightMap::loadFaceLighting(Face face, ) 
//{
//    value: function(gl, face, lighting, lightingExp) {
//        var width = face.m_LightmapTextureSizeInLuxels[0] + 1;
//        var height = face.m_LightmapTextureSizeInLuxels[1] + 1;
//        
//        if(height <= 0 || width <= 0) { return null; }
//        
//        var styleCount;
//        for(styleCount = 0; styleCount < face.styles.length; styleCount++) { 
//            if(face.styles[styleCount] == 255) { break; }
//        }
//        
//        function clamp(value) {
//            return value > 255 ? 255 : value < 0 ? 0 : value;
//        }
//
//        // Navigate lightmap BSP to find correctly sized space
//        // Allocate room for a 1 pixel border to prevent bleeding from other lightmaps
//        Node node = this._allocateRect(width+2, height+2);
//
//        if(node) {
//            // Read the lightmap from the BSP file
//            var byteCount = width * height * 4;
//            var borderedByteCount = (width+2) * (height+2) * 4; // includes border
//            var rowBytes = (width+2) * 4;
//            
//            var lightmap = new Uint8Array(borderedByteCount);
//            
//            for(var j = 0; j < styleCount; ++j) {
//                var lightOffset = face.lightofs + (byteCount*j);
//                var lightbuffer = lighting.subarray(lightOffset, lightOffset + byteCount);
//                var expbuffer = lightingExp.subarray(lightOffset + 3, lightOffset + byteCount);
//                
//                var i = 0;
//                
//                // Fill out the lightmap, minus borders
//                for(var y = 0; y < height; ++y) {
//                    var o = (rowBytes * (y+1)) + 4;
//                    for(var x = 0; x < width; ++x) {
//                        var exp = Math.pow(2, expbuffer[i]);
//                        lightmap[o] = clamp(lightmap[o] + (lightbuffer[i] * exp)); ++i; ++o;
//                        lightmap[o] = clamp(lightmap[o] + (lightbuffer[i] * exp)); ++i; ++o;
//                        lightmap[o] = clamp(lightmap[o] + (lightbuffer[i] * exp)); ++i; ++o;
//                        lightmap[o] = 255; ++i; ++o;
//                    }
//                }
//                
//                // Generate the borders
//                this._fillBorders(lightmap, width+2, height+2);
//            }
//            
//            // Copy the lightmap into the allocated rectangle
//            gl.bindTexture(gl.TEXTURE_2D, this.texture);
//            gl.texSubImage2D(gl.TEXTURE_2D, 0, node.x, node.y, width+2, height+2, gl.RGBA, gl.UNSIGNED_BYTE, lightmap);
//            
//            face.lightmapOffsetX = (node.x+1) / LIGHTMAP_SIZE;
//            face.lightmapOffsetY = (node.y+1) / LIGHTMAP_SIZE;
//            face.lightmapScaleX = width / LIGHTMAP_SIZE;
//            face.lightmapScaleY = height / LIGHTMAP_SIZE;
//        }
//        return node;
//    }
//}

Node* LightMap::_allocateRect(int width, int height, Node* node = NULL) 
{
    if(!node)
        node = this->rectTree;
    
    // Check children node
    if(node->nodes[0] != NULL && node->nodes[1] != NULL)
    { 
        Node* retNode = this->_allocateRect(width, height, node->nodes[0]);
        if(retNode) 
            return retNode;
        return this->_allocateRect(width, height, node->nodes[1]);
    }

    // Already used
    if(node->filled)
        return NULL;

    // Too small
    if(node->width < width || node->height < height)
        return NULL;

    // Perfect fit. Allocate without splitting
    if(node->width == width && node->height == height) 
    {
        node->filled = true;
        return node;
    }

    // We need to split if we've reached here
    //Node nodes[2];
    Node node1;
    Node node2;

    // Which way do we split?
    if ((node->width - width) > (node->height - height)) 
    {
        node1.x = node->x;
        node1.y = node->y;
        node1.width = width;
        node1.height = node->height;
        
        node->nodes[0] = &node1;
        
        node2.x = node->x + width;
        node2.y = node->y;
        node2.width = node->width - width;
        node2.height = node->height;
       
        node->nodes[1] = &node2;
        
    } 
    else 
    {
        node1.x = node->x;
        node1.y = node->y;
        node1.width = node->width;
        node1.height = height;
        
        node->nodes[0] = &node1;
        
        node2.x = node->x;
        node2.y = node->y + height;
        node2.width = node->width;
        node2.height = node->height - height;
       
        node->nodes[1] = &node2;
        
    }
    
    return this->_allocateRect(width, height, node->nodes[0]);
}



