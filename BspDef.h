/* 
 * File:   BspDef.h
 * Author: dude719
 *
 * Created on September 19, 2015, 6:21 AM
 */

#ifndef BSPDEF_H
#define	BSPDEF_H

#include "glm/glm.hpp"

#define HEADER_LUMPS 64
#define VBSP_HEADER (('P'<<24) + ('S' << 16) + ('B' << 8) + 'V')
#define SPRP 1936749168
#define MINBSPVERSION 19
#define BSPVERSION 21

enum LumpTypes {
    LUMP_ENTITIES = 0,              LUMP_PLANES,                    LUMP_TEXDATA,               LUMP_VERTICES,      LUMP_VISIBILITY,    LUMP_NODES,         LUMP_TEXINFO,                   LUMP_FACES,                 LUMP_LIGHTING,              LUMP_OCCLUSION,                     LUMP_LEAFS,     LUMP_FACEIDS,   LUMP_EDGES,         LUMP_SURFEDGES, LUMP_MODELS, LUMP_WORLDLIGHTS,
    LUMP_LEAFFACES,                 LUMP_LEAFBRUSHES,               LUMP_BRUSHES,               LUMP_BRUSHSIDES,    LUMP_AREAS,         LUMP_AREAPORTALS,   LUMP_PORTALS,                   LUMP_UNUSED0,               LUMP_PROPCOLLISION,         LUMP_CLUSTERS,                      LUMP_UNUSED1,   LUMP_PROPHULLS, LUMP_PORTALVERTS,   LUMP_UNUSED2,
    LUMP_PROPTRIS,                  LUMP_DISPINFO,                  LUMP_ORIGINALFACES,         LUMP_PHYSDISP,      LUMP_PHYSCOLLIDE,   LUMP_VERTNORMALS,   LUMP_VERTNORMALINDICES,         LUMP_DISP_LIGHTMAP_ALPHAS,  LUMP_DISP_VERTS,            LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS,
    LUMP_GAME_LUMP = 35,            LUMP_LEAFWATERDATA,             LUMP_PRIMITIVES,            LUMP_PRIMVERTS,     LUMP_PRIMINDICES,   LUMP_PAKFILE,       LUMP_CLIPPORTALVERTS,           LUMP_CUBEMAPS,              LUMP_TEXDATA_STRING_DATA,   LUMP_TEXDATA_STRING_TABLE,          LUMP_OVERLAYS,  LUMP_LEAFMINDISTTOWATER,
    LUMP_FACE_MACRO_TEXTURE_INFO,   LUMP_DISP_TRIS,                 LUMP_PHYSCOLLIDESURFACE,    LUMP_PROP_BLOB,     LUMP_WATEROVERLAYS, LUMP_LIGHTMAPPAGES, LUMP_LEAF_AMBIENT_INDEX_HDR,    LUMP_LIGHTMAPPAGEINFOS,     LUMP_LEAF_AMBIENT_INDEX,    LUMP_LIGHTING_HDR,
    LUMP_WORLDLIGHTS_HDR,           LUMP_LEAF_AMBIENT_LIGHTING_HDR, LUMP_LEAF_AMBIENT_LIGHTING, LUMP_XZIPPAKFILE,   LUMP_FACES_HDR,     LUMP_MAP_FLAGS,     LUMP_OVERLAY_FADES,             LUMP_OVERLAY_SYSTEM_LEVELS, LUMP_PHYSLEVEL,             LUMP_DISP_MULTIBLEND
};

// Contains the info of every lump (offset from beginning of file, length of lump, version (usually 0), and identification (usually 0)
typedef struct _LumpInfo {
    int offset;
    int length;
    int version;
    char ident[4];
} LumpInfo;

// Main header struct
typedef struct _Header
{
    int magic;
    int version;
    LumpInfo lumps[HEADER_LUMPS];
    int revision;
} Header;

typedef struct _Plane {
    glm::vec3 normal;
    float distance;
    int type;
} Plane;

typedef struct _Node {
    int planeIndex;
    int children[2];
    short mins[3];
    short maxs[3];
    unsigned short faceIndex;
    unsigned short numFaces;
    short area;
    short padding;
} Node;

typedef struct _Leaf {
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
} Leaf;

typedef struct _Edge {
    unsigned short vert[2];
} Edge;

typedef struct _Face {
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
} Face;

typedef struct _Model {
    float nMins[3]; // Defines bounding box mins
    float nMaxs[3]; // Defines bounding box maxs
    glm::vec3 vOrigin; // Coordinates to move the coordinate system before drawing the model
    int iHeadNode; // Index into nodes array
    int iFirstFace, nFaces; // Index and count into face array
} Model;

typedef struct _Brush {
    int	firstside;	// first brushside
    int	numsides;	// number of brushsides
    int	contents;	// contents flags
} Brush;

typedef struct _BrushSide {
    unsigned short	planenum;	// facing out of the leaf
    short           texinfo;	// texture info
    short           dispinfo;	// displacement info
    short           bevel;		// is the side a bevel plane?
} BrushSide;

typedef struct _TexInfo {
    float	textureVecs[2][4];	// [s/t][xyz offset] 
    float	lightmapVecs[2][4];	// [s/t][xyz offset] - length is in units of texels/area
    int     flags;              // miptex flags	overrides
    int     texdata;            // Pointer to texture name, size, etc.
} TexInfo;

typedef struct _TexData {
    glm::vec3	reflectivity;		// RGB reflectivity
    int         nameStringTableID;	// index into TexdataStringTable
    int         width, height;		// source image
    int         view_width, view_height;
} TexData;

typedef struct _GameLumpHeader {
    int lumpCount;
    // dclientlump_ts follow this
} GameLumpHeader;

typedef struct _GameLumpInfo {
    int id;
    unsigned short flags; // currently unused
    unsigned short version;
    int	offset;
    int length;
} GameLumpInfo;

typedef struct _StaticPropLeafLump {
    unsigned short leaf;
} StaticPropLeafLump;

typedef struct _StaticPropDictLump {
    char	name[128]; // model name
} StaticPropDictLump;

typedef struct _StaticPropLump {
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
} StaticPropLump;


#endif	/* BSPDEF_H */

