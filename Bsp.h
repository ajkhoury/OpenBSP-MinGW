/* 
 * File:   Bsp.h
 * Author: ankhoury
 *
 * Created on April 19, 2015, 6:14 AM
 */

#ifndef BSP_H
#define	BSP_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "GLEW/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Utils.h"
#include "Entity.h"
#include "Camera.h"
#include "Shader.h"
#include "TextRenderer.h"
#include "Image.h"
#include "BspDef.h"

class Bsp
{
public:
    Bsp(const char* fp);

    void LoadMapDetails(Camera* cam);
    
    Header header;
    
    typedef struct _TexCoords {
        float fS; // S coordinate
        float fT; // T coordinate
    } TexCoords;
    
    typedef struct _PropInfo {
        const char* name;
        glm::vec3 origin;
        glm::vec3 angles;
        int firstLeaf;
        int leafCount;
        int startIndex;
        int length;
        glm::mat4 matrix;
    } PropInfo;
   
    typedef struct _ModelInfo {
        GLuint startIndex;
        GLuint length;
        glm::vec3 origin;
        glm::mat4 matrix;
    } ModelInfo;
    
    std::vector<Face> faces;
    std::vector<Node> nodes;
    std::vector<Leaf> leafs;
    std::vector<unsigned short> leafFaces;
    std::vector<Edge> edges;
    std::vector<int> surfEdges;
    std::vector<Plane> planes;
    
    std::vector<TexInfo> texInfo;
    std::vector<TexData> texData;
    
    std::vector<Brush> brushes;
    std::vector<BrushSide> brushsides;
    
    GLuint* pnLightmapLookUp;
    TexCoords* pLightmapCoords;
    
    std::vector<Model> models;
    std::map<const char*, ModelInfo> modelInfo;
    
    std::vector<PropInfo> propInfo;
    std::map<const char*, PropInfo> props;
    
    std::vector<glm::vec3> vertices; // map verts
    std::vector<glm::vec3> normals; // map normals
    std::vector<glm::vec3> lightMaps;
    
    std::vector<GLuint> indices; // Final index vector
    int worldSize;
    
    int nEntities; // Number of entities
    int nBrushEntities;
    int nSpecialEntities;
    Entity* pEntities; // Stores the entities
    Entity** ppBrushEntities; // Pointers to brush entities in *pEntities
    Entity** ppSpecialEntities; // Pointers to special entities in *pEntities
    
    int numPropDictEntries;
    StaticPropDictLump* pStaticPropDict;
    int numPropLeafEntries;
    StaticPropLeafLump* pStaticPropLeaf;
    int numPropLumpEntries;
    StaticPropLump* pStaticPropLumps;

    
    void LoadProps();
    void RenderProps(int uniformModel, int uniformColor);
    
    void LoadBrushEntities();
    void LoadLightMaps(unsigned char* pLightMapData);
    
    void RenderBrushEntities(int uniformModel, int uniformColor, int useUserColorUniform);
    void RenderBrushEntityNames(Camera* cam, GLfloat color[]);
    
    void AdjustTextureToPowerOfTwo(Image* pImg);
    
private:
    std::ifstream map;
    
    int WalkBSPTree(glm::vec3 pos, int node);
    bool PointIsInside(glm::vec3 pos, short min[3], short max[3]);
    void LoadBSP(int node, /*int leaf,*/ glm::vec3 pos);
    void LoadLeaf(int leaf);
    void LoadFace(int face);
    float DotProduct(glm::vec3 normal, glm::vec3 b);
    void CrossProduct(const glm::vec3 v1, const glm::vec3 v2, glm::vec3& cross);
    
    bool IsBrushEntity(Entity* pEnt);
    void ParseEntities(const char* pszEntities);
    Entity* FindEntity(const char* pszNewClassName);
    glm::vec3 GetCoordsFromString(const char* str);

    template<typename T> void ReadLump(std::ifstream& fp, std::vector<T>& elements, unsigned int index);
    
};



#endif	/* BSP_H */

