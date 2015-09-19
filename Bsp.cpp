/* 
 * File:   Bsp.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:14 AM
 */

#include "Bsp.h"

Bsp::Bsp(const char* fp)
{
	/* Opening file */
	printf("Opening file...");
	map.open(fp, std::ios::binary);
	if (!map)
	{
		printf(" Could not open file!\n");
		return;
	}
	printf(" done.\n");

	/* Reading Header */
	printf("Reading header...");
	map.seekg(0);
	map.read((char*)&header, sizeof(Header));
	printf(" done.\n");

	if (header.magic == VBSP_HEADER)
		printf("File matches VBSP.\n");

	if (header.version == BSPVERSION)
		printf("BSP version %i matches CSGO BSP version.\n", header.version);
	else
		printf("BSP version %i is not a CSGO BSP.\n", header.version);

	/* Verify Lump data */
	printf("Verifying lumps...\n");
	for (int x = 0; x < 64; x++)
	{
		if (header.lumps[x].length == 0)
			printf("Lump %i is not included in this file.\n", x);
	}

	/* Grab Lump data */
	printf("Reading node lump... ");
	ReadLump(map, nodes, LUMP_NODES);
	printf("done.\n");

	printf("Reading leaf lump... ");
	ReadLump(map, leafs, LUMP_LEAFS);
	printf("done.\n");

	printf("Reading face lump... ");
	ReadLump(map, faces, LUMP_FACES);
	printf("done.\n");

	printf("Reading edge lump... ");
	ReadLump(map, edges, LUMP_EDGES);
	printf("done.\n");

	printf("Reading surfedge lump... ");
	ReadLump(map, surfEdges, LUMP_SURFEDGES);
	printf("done.\n");

	printf("Creating leafface lump... ");
	ReadLump(map, leafFaces, LUMP_LEAFFACES);
	printf("done.\n");

	printf("Reading vertex lump... ");
	ReadLump(map, vertices, LUMP_VERTICES);
	printf("done.\n");

	printf("Reading plane lump... ");
	ReadLump(map, planes, LUMP_PLANES);
	printf("done.\n");

	printf("Reading model lump... ");
	ReadLump(map, models, LUMP_MODELS);
	printf("done.\n");
    
    printf("Reading texture info lump... ");
    ReadLump(map, texInfo, LUMP_TEXINFO);
    printf("done.\n");
    
    printf("Reading texture data lump... ");
    ReadLump(map, texData, LUMP_TEXDATA);
    printf("done.\n");
    
    pnLightmapLookUp = (GLuint*)malloc(sizeof(GLuint) * faces.size());
    
    unsigned char* pLightMapData = (unsigned char*)malloc(header.lumps[LUMP_LIGHTING].length);
    // Seek to the position in the file that stores the lightmap data
    map.seekg(header.lumps[LUMP_LIGHTING].offset, std::ios_base::beg);
    // Read in the lightmap data
    map.read((char*)pLightMapData, header.lumps[LUMP_LIGHTING].length);
    
    LoadLightMaps(pLightMapData);
    free(pLightMapData);

	/* Entity Operations */
	char* pszEntityBuffer = (char*)malloc(header.lumps[LUMP_ENTITIES].length);
	// Seek to the position in the file that stores the entity information
	map.seekg(header.lumps[LUMP_ENTITIES].offset, std::ios_base::beg);
	// Read in the Entity data
	map.read(pszEntityBuffer, header.lumps[LUMP_ENTITIES].length);
    
	// Parse the string and create the pEntities
	ParseEntities(pszEntityBuffer);
	// delete the allocated entity buffer
	free(pszEntityBuffer);

	
	/* Game Lumps */
	//map.seekg(header.lumps[LUMP_GAME_LUMP].offset, std::ios_base::beg);
	//GameLumpHeader* pGameLumpHdr = (GameLumpHeader*)malloc(header.lumps[LUMP_GAME_LUMP].length);
	//map.read((char*)pGameLumpHdr, header.lumps[LUMP_GAME_LUMP].length);
	//
	//GameLumpInfo* pGameLumps = (GameLumpInfo*)(pGameLumpHdr + 1);
	//
	//for (int i = 0; i < pGameLumpHdr->lumpCount; i++)
	//{
	//	if (pGameLumps[i].id == SPRP) // static props
	//	{
	//		printf("SPRP offset: 0x%X  length: 0x%X\n", pGameLumps[i].offset, pGameLumps[i].length);
	//		int* dictEntries = (int*)((char*)pGameLumpHdr + (pGameLumps[i].offset - header.lumps[LUMP_GAME_LUMP].offset));
	//		numPropDictEntries = *dictEntries;
	//		printf("numPropDictEntries: %i\n", numPropDictEntries);
	//		pStaticPropDict = (StaticPropDictLump*)(dictEntries + 1);
	//		
	//		printf("StaticPropDictLump: \n");
	//		for (int i = 0; i < numPropDictEntries; i++)
	//		{
	//			printf("   [%i] %s\n", i, pStaticPropDict[i].name);
	//		}
	//		
	//		int* leafEntries = (int*)(pStaticPropDict + numPropDictEntries);
	//		numPropLeafEntries = *leafEntries;
	//		pStaticPropLeaf = (StaticPropLeafLump*)(leafEntries + 1);
	//		
	//		int* lumpEntries = (int*)(pStaticPropLeaf + numPropLeafEntries);
	//		numPropLumpEntries = *lumpEntries;
	//		printf("StaticPropLump: 0x%X  numPropLumpEntries: %i\n", lumpEntries, numPropLumpEntries);
	//		pStaticPropLumps = (StaticPropLump*)(lumpEntries + 1);
	//		
	//		for (int j = 0; j < numPropLumpEntries; j++)
	//		{
	//			PropInfo info;
	//			info.name = pStaticPropDict[pStaticPropLumps[j].PropType].name;
	//			printf("Prop name: %s\n", info.name);
	//			info.origin = glm::vec3(pStaticPropLumps[j].Origin.x, pStaticPropLumps[j].Origin.y, pStaticPropLumps[j].Origin.z);
	//			info.angles = glm::vec3(pStaticPropLumps[j].Angles.x, pStaticPropLumps[j].Angles.y, pStaticPropLumps[j].Angles.z);
	//			info.firstLeaf = pStaticPropLumps[j].FirstLeaf;
	//			info.leafCount = pStaticPropLumps[j].LeafCount;
	//			m_propInfo.push_back(info);
	//		}
	//	}
	//}
	
	map.close();
	
	normals.resize(vertices.size(), glm::vec3(0,0,0));
}

void Bsp::AdjustTextureToPowerOfTwo(Image* pImg)
{
    //if (GLEE_ARB_texture_non_power_of_two)
        //return;

    if (((pImg->nWidth & (pImg->nWidth - 1)) == 0) && ((pImg->nHeight & (pImg->nHeight - 1)) == 0))
        return;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int nPOT = 1;
    while (nPOT < pImg->nHeight || nPOT < pImg->nWidth)
        nPOT *= 2;

    // Scale image
    unsigned char* pNewData = (unsigned char*)malloc(nPOT * nPOT * pImg->nChannels * sizeof(unsigned char));

    gluScaleImage(pImg->nChannels == 4 ? GL_RGBA : GL_RGB, pImg->nWidth, pImg->nHeight, GL_UNSIGNED_BYTE, pImg->pData, nPOT, nPOT, GL_UNSIGNED_BYTE, pNewData);

    free(pImg->pData);
    pImg->nWidth = nPOT;
    pImg->nHeight = nPOT;
    pImg->pData = pNewData;
}

void Bsp::LoadLightMaps(unsigned char* pLightMapData)
{
    int nLoadedData = 0;
    int nLoadedLightmaps = 0;
    //int nErrors = 0;

    for (int i = 0; i < faces.size(); i++)
    {
        if (faces[i].styles[0] == 0 && (signed)faces[i].lightOfs >= -1)
        {
            //Allocate pLightmapCoords
            pLightmapCoords = (TexCoords*)malloc(sizeof(TexCoords) * faces[i].numEdges);

            /* *********** QRAD ********** */
            float fMinU = 999999;
            float fMinV = 999999;
            float fMaxU = -99999;
            float fMaxV = -99999;

            TexInfo info = texInfo[faces[i].texInfo];
            
            for (int j = 0; j < faces[i].numEdges; j++)
            {
                int iEdge = surfEdges[faces[i].firstEdge + j];
                glm::vec3 vertex;
                if (iEdge >= 0)
                    vertex = vertices[edges[iEdge].vert[0]];
                else
                    vertex = vertices[edges[-iEdge].vert[1]];

                float fU = info.lightmapVecs[0][0] * vertex.x + info.lightmapVecs[0][1] * vertex.y + info.lightmapVecs[0][2] * vertex.z + info.lightmapVecs[0][3]; //DotProduct(texInfo.vS, vertex) + texInfo.fSShift;
                if (fU < fMinU)
                    fMinU = fU;
                if (fU > fMaxU)
                    fMaxU = fU;

                float fV = info.lightmapVecs[1][0] * vertex.x + info.lightmapVecs[1][1] * vertex.y + info.lightmapVecs[1][2] * vertex.z + info.lightmapVecs[1][3];
                if (fV < fMinV)
                    fMinV = fV;
                if (fV > fMaxV)
                    fMaxV = fV;
            }

            int nWidth = (int)(ceil(fMaxU / 16.0f) - floor(fMinU / 16.0f)) + 1;
            int nHeight = (int)(ceil(fMaxV / 16.0f) - floor(fMinV / 16.0f)) + 1;

            /* *********** end QRAD ********* */

            /* http://www.gamedev.net/community/forums/topic.asp?topic_id=538713 (last refresh: 20.02.2010) */

            float fMidPolyU = (float)(fMinU + fMaxU) / 2.0f;
            float fMidPolyV = (float)(fMinV + fMaxV) / 2.0f;
            float fMidTexU  = (float)(nWidth) / 2.0f;
            float fMidTexV  = (float)(nHeight) / 2.0f;

            for (int j = 0; j < faces[i].numEdges; ++j)
            {
                
                int iEdge = surfEdges[faces[i].firstEdge + j];
                glm::vec3 vertex;
                if (iEdge >= 0)
                    vertex = vertices[edges[iEdge].vert[0]];
                else
                    vertex = vertices[edges[-iEdge].vert[1]];

                float fU =  info.lightmapVecs[0][0] * vertex.x + 
                            info.lightmapVecs[0][1] * vertex.y + 
                            info.lightmapVecs[0][2] * vertex.z + info.lightmapVecs[0][3];
                
                float fV =  info.lightmapVecs[1][0] * vertex.x + 
                            info.lightmapVecs[1][1] * vertex.y + 
                            info.lightmapVecs[1][2] * vertex.z + info.lightmapVecs[1][3];

                float fLightMapU = fMidTexU + (fU - fMidPolyU) / 16.0f;
                float fLightMapV = fMidTexV + (fV - fMidPolyV) / 16.0f;

                pLightmapCoords[j].fS = fLightMapU / (float)nWidth;
                pLightmapCoords[j].fT = fLightMapV / (float)nHeight;
            }

            /* end http://www.gamedev.net/community/forums/topic.asp?topic_id=538713 */

            // Find unbound texture slots
            glGenTextures(1, &pnLightmapLookUp[i]);
            
            Image* pImg = new Image(3, nWidth, nHeight);
            memcpy(pImg->pData, &pLightMapData[faces[i].lightOfs], nWidth * nHeight * 3 * sizeof(unsigned char));

            AdjustTextureToPowerOfTwo(pImg);

            // Bind the texture
            glBindTexture(GL_TEXTURE_2D, pnLightmapLookUp[i]);

            // Set up Texture Filtering Parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pImg->nWidth, pImg->nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImg->pData);

            nLoadedLightmaps++;
            //printf("#%4d Loaded lightmap %2d x %2d\n", nLoadedLightmaps, nWidth, nHeight);
        
            nLoadedData += nWidth * nHeight * 3;
        }
        else
        {
            pnLightmapLookUp[i] = 0;
        }
    }
    
    printf("Loaded %d lightmaps, lightmapdatadiff: %d Bytes ", nLoadedLightmaps, nLoadedData - header.lumps[LUMP_LIGHTING].length);
    if ((nLoadedData - header.lumps[LUMP_LIGHTING].length) == 0)
        printf("OK\n");
    else
        printf("ERRORS\n");
}

void Bsp::LoadMapDetails(Camera* cam)
{
	Entity* info_player = FindEntity("info_player_counterterrorist");

	const char* origin = info_player->FindProperty("origin");
	const char* angles = info_player->FindProperty("angles");

	glm::vec3 position(GetCoordsFromString(origin));
	glm::vec3 orientation(GetCoordsFromString(angles));

	cam->SetPosition(position);
	cam->OffsetOrientation(orientation[PITCH], orientation[YAW]);

	int curLeaf = WalkBSPTree(position, 0);
	printf("Current leaf: %i\n", curLeaf);
	
	LoadBSP(0, /*curLeaf,*/ position);
	worldSize = indices.size();
	LoadBrushEntities();
	//LoadProps();
}

void Bsp::LoadBSP(int node, /*int leaf,*/ glm::vec3 pos)
{
	if (node < 0)
	{
		if (node == -1)
			return;
		LoadLeaf(~node);
		return;
	}

	float location;
	int planeIdx = nodes[node].planeIndex;
	
	if (planes[planeIdx].type == 0) // plane x
	{
		location = pos.x - planes[planeIdx].distance;
	}
	else if (planes[planeIdx].type == 1) // plane y
	{
		location = pos.y - planes[planeIdx].distance;
	}
	else if (planes[planeIdx].type == 2) // plane z
	{	
		location = pos.z - planes[planeIdx].distance;
	}
	else
	{
		location = DotProduct(planes[planeIdx].normal, pos) - planes[planeIdx].distance;
	}

	if (location > 0)
	{
		LoadBSP(nodes[node].children[1], /*leaf,*/ pos);
		LoadBSP(nodes[node].children[0], /*leaf,*/ pos);
	}
	else
	{
		LoadBSP(nodes[node].children[0], /*leaf,*/ pos);
		LoadBSP(nodes[node].children[1], /*leaf,*/ pos);
	}
}

void Bsp::LoadLeaf(int leaf)
{
	for (int x = 0; x < leafs[leaf].numLeafFaces; x++)
	{
		LoadFace(leafFaces[leafs[leaf].firstLeafFace + x]);
	}
}

void Bsp::LoadFace(int face)
{
    if (faces[face].styles[0] == 0xFF)
        return;
    
	// key indices for each face
	int vertPoint;
    int rootPoint; // if it is the first run through the first vertex is the "hub" index that all of the triangles in the plane will refer to
	int firstPoint; // the first point after the hub
	int secondPoint; // last point to create a full triangle
    
    // normal
    glm::vec3 vNormal = planes[faces[face].planeIndex].normal;
    if (faces[face].side)
        vNormal = -vNormal;

    // loop through every single edge in a face, this will end up making a triangle fan
	for (int x = 0; x < faces[face].numEdges; x++) 
	{ 
        int edgeIndex = surfEdges[faces[face].firstEdge + x];
        Edge edge = edges[abs(edgeIndex)];
        bool reverse = (edgeIndex >= 0);
        
        if (x == 0)
        {
            rootPoint = edge.vert[reverse?0:1];
            vertPoint = edge.vert[reverse?1:0];
        }
        else 
        {
            vertPoint = edge.vert[reverse?0:1];
            if(vertPoint == rootPoint)
                continue;
            else 
                firstPoint = vertPoint;
            
            vertPoint = edge.vert[reverse?1:0];
            if (vertPoint == rootPoint)
                continue;
            else 
                secondPoint = vertPoint;
            
            normals[rootPoint] = vNormal;
            normals[firstPoint] = vNormal;
            normals[secondPoint] = vNormal;

            // push back every index 
            indices.push_back(rootPoint);
            indices.push_back(firstPoint);
            indices.push_back(secondPoint);
        }   
	}
	indices.push_back(-1); // once we are done rendering our plane we put in our primitive restart index to start a new plane
}

template<typename T> void Bsp::ReadLump(std::ifstream& map, std::vector<T>& elements, unsigned int index)
{
	elements.clear();
	
	if (header.lumps[index].length == 0) // lump doesn't exist
		return;

	map.seekg(header.lumps[index].offset);

	for (int x = 0; x < header.lumps[index].length / sizeof(T); x++)
	{
		T element;
		map.read((char*)&element, sizeof(T));
		elements.push_back(element);
	}
}

int Bsp::WalkBSPTree(glm::vec3 pos, int node)
{ /* http://sourceforge.net/p/hlbsp/code/HEAD/tree/hlbsp.cpp#l897 */
	for (int x = 0; x < 2; x++) 
	{
		if (nodes[node].children[x] >= 0) // node
		{
			if (PointIsInside(pos, nodes[nodes[node].children[x]].mins, nodes[nodes[node].children[x]].maxs))
				return WalkBSPTree(pos, nodes[node].children[x]);
		}

		else if (~nodes[node].children[x] != 0) // NOT x = -x - 1, so this an index for the leaves
		{
			if (PointIsInside(pos, leafs[~(nodes[node].children[x])].mins, leafs[~(nodes[node].children[x])].maxs))
				return ~nodes[node].children[x];
		}
	}

	return -1;
}

/* I have no fucking idea what im doing seems like
 * random map pieces and not props
 * There must be something wrong. HELP ME. */
void Bsp::LoadProps() 
{
	for (int i = 0; i < propInfo.size(); i++)
	{
		propInfo[i].startIndex = indices.size() + 1;
		
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(glm::mat4(1.0f), propInfo[i].origin);
		printf("angles: %f %f %f\n", propInfo[i].angles.x, propInfo[i].angles.y, propInfo[i].angles.z);
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(m_propInfo[i].angles.z), glm::vec3(1,0,0));
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(m_propInfo[i].angles.x), glm::vec3(0,1,0));
		//modelMatrix = glm::rotate(modelMatrix, glm::radians(m_propInfo[i].angles.y), glm::vec3(0,0,1));
		
		propInfo[i].matrix = modelMatrix;
		
		for (int j = 0; j < propInfo[i].leafCount; j++)
		{
			LoadLeaf(pStaticPropLeaf[propInfo[i].firstLeaf + j].leaf);
		}
		
		propInfo[i].length = indices.size() - propInfo[i].startIndex;
		
		props[propInfo[i].name] = propInfo[i]; 
	}
}

void Bsp::RenderProps(int uniformModel, int uniformColor)
{
	for (int i = 0; i < propInfo.size(); i++)
	{
		PropInfo info = propInfo[i];		
		std::map<const char*, PropInfo>::iterator it = props.find(info.name);
		if (it == props.end())
			continue;
		
		PropInfo propInfo = it->second;
		
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(propInfo.matrix));
        
        glUniform4f(uniformColor, 1.0f, 1.0f, 1.0f, 1.0f); // white
        glDrawElements(GL_TRIANGLES, propInfo.length, GL_UNSIGNED_INT, (void*)(propInfo.startIndex * sizeof(GLuint)));
        glUniform4f(uniformColor, 0.0f, 0.8f, 0.0f, 1.0f); // green
        glDrawElements(GL_LINE_LOOP, propInfo.length, GL_UNSIGNED_INT, (void*)(propInfo.startIndex * sizeof(GLuint)));
	}
}

void Bsp::LoadBrushEntities()
{
	for (int i = 0; i < nBrushEntities; i++)
	{
		Entity* pCurEnt = ppBrushEntities[i];
		if (!pCurEnt)
			continue;
		
		const char* szModelIndex = pCurEnt->FindProperty("model");
		if (!szModelIndex)
			continue;
		
		int iModel = atoi(szModelIndex + 1);
		
		const char* szOrigin = pCurEnt->FindProperty("origin");
		if (!szOrigin)
			continue;

		ModelInfo info;
		memset(&info, 0, sizeof(ModelInfo));
		
		glm::vec3 origin = GetCoordsFromString(szOrigin);
		
		info.origin = origin;
		info.matrix = glm::translate(glm::mat4(1.0f), origin);
		
		info.startIndex = indices.size() + 1;
		
		for (int i = 0; i < models[iModel].nFaces; i++)
		{
			LoadFace(models[iModel].iFirstFace + i);
		}
		
		info.length = indices.size() - info.startIndex;
		modelInfo[szModelIndex] = info; 
	}
}

void Bsp::RenderBrushEntities(int uniformModel, int uniformColor, int useUserColorUniform)
{
	for (int i = 0; i < nBrushEntities; i++)
	{
		Entity* pCurEnt = ppBrushEntities[i];
		if (!pCurEnt)
			continue;
		
		std::map<const char*, ModelInfo>::iterator it = modelInfo.find(pCurEnt->FindProperty("model"));
		if (it == modelInfo.end())
			continue;
		
		ModelInfo info = it->second;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(info.matrix));
        
        //glUniform4f(uniformColor, 1.0f, 1.0f, 1.0f, 1.0f); // white
        //glDrawElements(GL_TRIANGLES, info.length, GL_UNSIGNED_INT, (void*)(info.startIndex * sizeof(GLuint)));
        glUniform1i(useUserColorUniform, GL_TRUE);
        glUniform4f(uniformColor, 0.75f, 0.0f, 0.75f, 1.0f); // purp
        glDrawElements(GL_LINE_LOOP, info.length, GL_UNSIGNED_INT, (void*)(info.startIndex * sizeof(GLuint)));
        glUniform1i(useUserColorUniform, GL_FALSE);
	}
}

void Bsp::RenderBrushEntityNames(Camera* cam, GLfloat color[])
{
	TextRenderer::GetInstance()->BeginText();
	
	for (int i = 0; i < nBrushEntities; i++)
	{
		Entity* pCurEnt = ppBrushEntities[i];
		if (!pCurEnt)
			continue;

		std::map<const char*, ModelInfo>::iterator it = modelInfo.find(pCurEnt->FindProperty("model"));
		if (it == modelInfo.end())
			continue;
		
		ModelInfo info = it->second;

		int screenX, screenY;
		if (cam->WorldToScreen(info.origin, screenX, screenY))
			TextRenderer::GetInstance()->RenderText(screenX, screenY, 1.0f, color, pCurEnt->FindProperty("classname"));
	}
	
	TextRenderer::GetInstance()->EndText();
}

bool Bsp::IsBrushEntity(Entity* pEnt)
{
	if (pEnt->FindProperty("model") == NULL)
		return false;

	const char* pzsClassName = pEnt->FindProperty("classname");
	if (!strcmp(pzsClassName, "func_door_rotating") ||
		!strcmp(pzsClassName, "func_door") ||
		!strcmp(pzsClassName, "func_illusionary") ||
		!strcmp(pzsClassName, "func_wall") ||
		!strcmp(pzsClassName, "func_breakable") ||
		!strcmp(pzsClassName, "func_button") ||
		!strcmp(pzsClassName, "func_rotating") ||
		!strcmp(pzsClassName, "func_brush") ||
		!strcmp(pzsClassName, "func_buyzone") ||
		!strcmp(pzsClassName, "func_bomb_target")
		)
		return true;
	else
		return false;
}

void Bsp::ParseEntities(const char* pszEntities)
{
	// Count entities
	char* pchPos = (char*)pszEntities;
	nEntities = 0;

	while (true)
	{
		pchPos = strchr(pchPos, '{');
		if (pchPos == NULL)
			break;
		else
		{
			nEntities++;
			pchPos++;
		}
	}

	// Allocate memory for the pEntities
	pEntities = new Entity[nEntities]; // new to execute ctor

	nBrushEntities = 0;
	nSpecialEntities = 0;

	// Start over
	pchPos = (char*) pszEntities;
	// Loop for each entity and parse it. count number of solid and special pEntities
	for (int i=0; i < nEntities; i++)
	{
		pchPos = strchr(pchPos, '{');
		char* pchClose = strchr(pchPos, '}');

		int nLen = pchClose - pchPos - 1;
		char* szIndividualEntity = (char*)malloc(sizeof(char) * (nLen + 1));
		strncpy(szIndividualEntity, pchPos + 1, nLen);
		szIndividualEntity[nLen] = 0;

		pEntities[i].ParseProperties(szIndividualEntity);

		free(szIndividualEntity);

		if (IsBrushEntity(&pEntities[i]))
			nBrushEntities++;
		else
			nSpecialEntities++;

		pchPos++;
	}
	printf("nBrushEntities: %i\n", nBrushEntities);
	ppBrushEntities = (Entity**)malloc(nBrushEntities * sizeof(Entity*));
	ppSpecialEntities = (Entity**)malloc(nSpecialEntities * sizeof(Entity*));

	int iBrush = 0;
	int iSpecial = 0;
	for (int i=0; i<nEntities; i++)
	{
		if (IsBrushEntity(&pEntities[i]))
		{
			ppBrushEntities[iBrush] = &pEntities[i];

			//if Entity has property "origin" apply to model struct for rendering
			const char* szOrigin;
			if ((szOrigin = pEntities[i].FindProperty("origin")) != NULL)
			{
				int iModel = atoi(pEntities[i].FindProperty("model") + 1);
				glm::vec3 origin = GetCoordsFromString(szOrigin);
				models[iModel].vOrigin = origin;
			}
			iBrush++;
		}
		else
		{
			ppSpecialEntities[iSpecial] = &pEntities[i];
			iSpecial++;
		}
	}

	// sort brush entities so that these with rendermode texture are at the back
	//qsort(ppBrushEntities, nBrushEntities, sizeof(Entity*), BrushEntityCmp);
}

Entity* Bsp::FindEntity(const char* pszNewClassName)
{
	static int i;
	static const char* pszClassName;
	if(pszNewClassName == NULL)
		i++;
	else
	{
		i = 0;
		pszClassName = pszNewClassName;
	}

	for (; i < nEntities; i++)
	{
		const char* szClass = pEntities[i].FindProperty("classname");
		if(szClass == NULL)
			continue;

		if (!strcmp(szClass, pszClassName))
			return &pEntities[i];
	}

	return NULL;
}

glm::vec3 Bsp::GetCoordsFromString(const char* str)
{
    if (!str)
    {
        printf("GetCoordsFromString: string is NULL!\n");
        return glm::vec3(0,0,0);
	}
    
    glm::vec3 pos;
	for (int i = 0; i < 3; i++, str++)
	{
		const char *begin = str;
		while(*str != ' ' && *str)
			str++;
		std::string spos(begin, str);
		pos[i] = atof(spos.c_str());
	}
	return pos;
}


bool Bsp::PointIsInside(glm::vec3 pos, short min[3], short max[3])
{
	if ((min[0] <= pos.x && pos.x <= max[0] &&
        min[1] <= pos.y && pos.y <= max[1] &&
        min[2] <= pos.z && pos.z <= max[2]) ||
       (min[0] >= pos.x && pos.x >= max[0] &&
        min[1] >= pos.y && pos.y >= max[1] &&
        min[2] >= pos.z && pos.z >= max[2]))
		return true;
	else
		return false;
}

float Bsp::DotProduct(glm::vec3 a, glm::vec3 b)
{
	return  (a.x * b.x + a.y * b.y + a.z * b.z);
}

void Bsp::CrossProduct(const glm::vec3 v1, const glm::vec3 v2, glm::vec3& cross)
{
	cross.x = v1.y * v2.z - v1.z * v2.y;
	cross.y = v1.z * v2.x - v1.x * v2.z;
	cross.z = v1.x * v2.y - v1.y * v2.x;
}