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

#define HEADER_LUMPS 64
#define VBSP_HEADER (('P'<<24) + ('S' << 16) + ('B' << 8) + 'V')
#define SPRP 1936749168
#define MINBSPVERSION 19
#define BSPVERSION 21

class Bsp
{
public:
    Bsp(const char* fp);

    void LoadMapDetails(Camera* cam);

    enum LumpTypes
    {
        LUMP_ENTITIES = 0,              LUMP_PLANES,                    LUMP_TEXDATA,               LUMP_VERTICES,      LUMP_VISIBILITY,    LUMP_NODES,         LUMP_TEXINFO,                   LUMP_FACES,                 LUMP_LIGHTING,              LUMP_OCCLUSION,                     LUMP_LEAFS,     LUMP_FACEIDS,   LUMP_EDGES,         LUMP_SURFEDGES, LUMP_MODELS, LUMP_WORLDLIGHTS,
        LUMP_LEAFFACES,                 LUMP_LEAFBRUSHES,               LUMP_BRUSHES,               LUMP_BRUSHSIDES,    LUMP_AREAS,         LUMP_AREAPORTALS,   LUMP_PORTALS,                   LUMP_UNUSED0,               LUMP_PROPCOLLISION,         LUMP_CLUSTERS,                      LUMP_UNUSED1,   LUMP_PROPHULLS, LUMP_PORTALVERTS,   LUMP_UNUSED2,
        LUMP_PROPTRIS,                  LUMP_DISPINFO,                  LUMP_ORIGINALFACES,         LUMP_PHYSDISP,      LUMP_PHYSCOLLIDE,   LUMP_VERTNORMALS,   LUMP_VERTNORMALINDICES,         LUMP_DISP_LIGHTMAP_ALPHAS,  LUMP_DISP_VERTS,            LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS,
        LUMP_GAME_LUMP = 35,                 LUMP_LEAFWATERDATA,             LUMP_PRIMITIVES,            LUMP_PRIMVERTS,     LUMP_PRIMINDICES,   LUMP_PAKFILE,       LUMP_CLIPPORTALVERTS,           LUMP_CUBEMAPS,              LUMP_TEXDATA_STRING_DATA,   LUMP_TEXDATA_STRING_TABLE,          LUMP_OVERLAYS,  LUMP_LEAFMINDISTTOWATER,
        LUMP_FACE_MACRO_TEXTURE_INFO,   LUMP_DISP_TRIS,                 LUMP_PHYSCOLLIDESURFACE,    LUMP_PROP_BLOB,     LUMP_WATEROVERLAYS, LUMP_LIGHTMAPPAGES, LUMP_LEAF_AMBIENT_INDEX_HDR,    LUMP_LIGHTMAPPAGEINFOS,     LUMP_LEAF_AMBIENT_INDEX,    LUMP_LIGHTING_HDR,
        LUMP_WORLDLIGHTS_HDR,           LUMP_LEAF_AMBIENT_LIGHTING_HDR, LUMP_LEAF_AMBIENT_LIGHTING, LUMP_XZIPPAKFILE,   LUMP_FACES_HDR,     LUMP_MAP_FLAGS,     LUMP_OVERLAY_FADES,             LUMP_OVERLAY_SYSTEM_LEVELS, LUMP_PHYSLEVEL,             LUMP_DISP_MULTIBLEND
    };
    
    // Contains the info of every lump (offset from beginning of file, length of lump, version (usually 0), and identification (usually 0)
    struct LumpInfo
    {
        int offset;
        int length;
        int version;
        char ident[4];
    };

    // Main header struct
    struct Header
    {
        int magic;
        int version;
        LumpInfo lumps[HEADER_LUMPS];
        int revision;
    };
    
    struct GameLumpHeader
    {
        int lumpCount;
        // dclientlump_ts follow this
    };
    
    struct GameLumpInfo
    {
        int id;
        unsigned short flags; // currently unused
        unsigned short version;
        int	offset;
        int length;
    };
    
    struct StaticPropLeafLump
    {
        unsigned short leaf;
    };
    
    struct StaticPropDictLump
    {
        char	name[128]; // model name
    };

    // Vertex struct
    //struct Vertex
    //{
    //    // cleaner usage of verts
    //    Vertex() { };
    //    Vertex(float a, float b, float c) { x = a; y = b; z = c; };
    //    Vertex pos() { return Vertex(x, y, z); };
    //    Vertex& operator=(const Vertex& vec) {  x=vec.x; y=vec.y; z=vec.z; }
    //    Vertex& operator=(const glm::vec3& vec) {  x=vec.x; y=vec.y; z=vec.z; }
    //    Vertex& operator+=(const Vertex& vec) {  x+=vec.x; y+=vec.y; z+=vec.z; return *this; }
    //    Vertex& operator-=(const Vertex& vec) {  x-=vec.x; y-=vec.y; z-=vec.z; return *this; }
    //    float& operator[](int i) { switch (i) { case 0: return x; case 1: return y; case 2: return z; } }
    //    Vertex operator+(Vertex vec) const { return Vertex(x + vec.x, y + vec.y, z + vec.z); }
    //    Vertex operator-(Vertex vec) const { return Vertex(x - vec.x, y - vec.y, z - vec.z); }
    //    Vertex operator*(float scalar) const { return Vertex(x * scalar, y * scalar, z * scalar); }
    //    
    //    float x, y, z;  
    //};
    
    std::vector<glm::vec3> vertices; // map verts
    std::vector<glm::vec3> normals; // map normals
    
    struct StaticPropLump
    {
        // v4
        glm::vec3       Origin;		 // origin
        glm::vec3       Angles;		 // orientation (pitch roll yaw)
        unsigned short	PropType;	 // index into model name dictionary
        unsigned short	FirstLeaf;	 // index into leaf array
        unsigned short	LeafCount;
        unsigned char	Solid;		 // solidity type
        unsigned char	Flags;
        int             Skin;		 // model skin numbers
        float           FadeMinDist;
        float           FadeMaxDist;
        glm::vec3       LightingOrigin;  // for lighting
        // since v5
        float           ForcedFadeScale; // fade distance scale
        // v6 and v7 only
        unsigned short  MinDXLevel;      // minimum DirectX version to be visible
        unsigned short  MaxDXLevel;      // maximum DirectX version to be visible
        // CSGO bsp
        int unkshit;
        // v7 only
        int  lightmapResolutionX;
        int  lightmapResolutionY;
        // since v8
        //unsigned char   MinCPULevel;
        //unsigned char   MaxCPULevel;
        //unsigned char   MinGPULevel;
        //unsigned char   MaxGPULevel;
        // since v7
        //int             DiffuseModulation; // per instance color and alpha modulation
        // since v10
        //float           unknown; 
        // since v9
        //bool            DisableX360;     // if true, don't show on XBox 360
    };
    
    struct PropInfo
    {
        const char* name;
        glm::vec3 origin;
        glm::vec3 angles;
        int firstLeaf;
        int leafCount;
        int startIndex;
        int length;
        glm::mat4 matrix;
    };
    
    std::vector<PropInfo> m_propInfo;
    std::map<const char*, PropInfo> m_props;
    
    struct Plane
    {
        glm::vec3 normal;
        float distance;
        int type;
    };

    struct Node
    {
        int planeIndex;
        int children[2];
        short mins[3];
        short maxs[3];
        unsigned short faceIndex;
        unsigned short numFaces;
        short area;
        short padding;
    };
    std::vector<Node> nodes;

    struct Leaf
    {
        int contents;
        short cluster;
        short area : 9; // use only 9 bits
        short flags : 7;
        short mins[3];
        short maxs[3];
        unsigned short firstLeafFace;
        unsigned short numLeafFaces;
        unsigned short firstLeafBrush;
        unsigned short numLeafBrushes;
        short leafWaterDataID;
    };

    std::vector<Leaf> leafs;
    std::vector<unsigned short> leafFaces;

    struct Face
    {
        unsigned short planeIndex;
        unsigned __int8 side;
        unsigned __int8 onNode;
        int firstEdge;
        short numEdges;
        short texInfo;
        short dispInfo;
        short surfaceFogVolumeID;
        unsigned __int8 styles[4];
        int lightOfs;
        float area;
        int lightmapTextureMinsInLuxels[2];
        int lightmapTextureSizeInLuxels[2];
        int origFace;
        unsigned short numPrims;
        unsigned short firstPrimID;
        unsigned int smoothingGroups;
    };
    std::vector<Face> faces;

    struct Model
    {
        float nMins[3]; // Defines bounding box mins
        float nMaxs[3]; // Defines bounding box maxs
        glm::vec3 vOrigin; // Coordinates to move the coordinate system before drawing the model
        int iHeadNode; // Index into nodes array
        int iFirstFace, nFaces; // Index and count into face array
    };
    std::vector<Model> models;

    
    struct Brush
    {
        int	firstside;	// first brushside
        int	numsides;	// number of brushsides
        int	contents;	// contents flags
    };
    std::vector<Brush> brushes;
    
    struct BrushSide
    {
        unsigned short	planenum;	// facing out of the leaf
        short           texinfo;	// texture info
        short           dispinfo;	// displacement info
        short           bevel;		// is the side a bevel plane?
    };
     std::vector<BrushSide> brushsides;
    
    struct Edge
    {
        unsigned short vert[2];
    };

    std::vector<Edge> edges;
    std::vector<int> surfEdges;
    std::vector<Plane> planes;

    std::vector<GLuint> indices; // Final index vector
    int worldSize;
    
    struct ModelInfo
    {
        GLuint startIndex;
        GLuint length;
        glm::vec3 origin;
        glm::mat4 matrix;
    };
    std::map<const char*, ModelInfo> modelInfo; // Final index vector
    
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

    Header header;
    
    void LoadProps();
    void RenderProps(int uniformModel, int uniformColor);
    
    void LoadBrushEntities();
    void RenderBrushEntities(int uniformModel, int uniformColor, int useUserColorUniform);
    void RenderBrushEntityNames(Camera* cam, GLfloat color[]);
    
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

