/* 
 * File:   Entity.cpp
 * Author: dude719
 * 
 * Created on April 19, 2015, 6:20 AM
 */

#include "Entity.h"

#include <stdlib.h>
#include <string.h>

Entity::Entity()
{
    nProperties = 0;
}

Entity::~Entity()
{
    for(int i=0;i<nProperties;i++)
    {
        free(properties[i].pszName);
        free(properties[i].pszValue);
    }
    free(properties);
}

void Entity::ParseProperties(const char* pszProperties)
{
	char* pchPos = (char*)pszProperties;
	nProperties = 0;
	// Count num of properties
	while(true)
	{
		pchPos = strchr(pchPos, '"');
		if(pchPos == NULL)
			break;
		else
		{
			nProperties++;
			pchPos++;
		}
	}

	// There are 4 quotes per line, so this gives the num of lines/entity properties
	nProperties /= 4;

	properties = (ENTITYPROPERTY*)malloc(nProperties * sizeof(ENTITYPROPERTY));

	// Start over
	pchPos = (char*)pszProperties;
	char* pchClose;

	// Run for each line
	for(int i=0; i < nProperties; ++i)
	{
		// Find opening of name
		pchPos = strchr(pchPos, '"');
		// Advance past the opening "
		pchPos++;
		// Find the closing "
		pchClose = strchr(pchPos, '"');

		int nLen = pchClose - pchPos;
		properties[i].pszName = (char*)malloc((nLen + 1) * sizeof(char));
		strncpy(properties[i].pszName, pchPos, nLen);
		properties[i].pszName[nLen] = 0;

		// Move past the closing "
		pchPos = pchClose + 1;
		// Find opening of value
		pchPos = strchr(pchPos, '"');
		// Advance past the opening "
		pchPos++;
		// Find the closing "
		pchClose = strchr(pchPos, '"');

		nLen = pchClose - pchPos;
		properties[i].pszValue = (char*)malloc((nLen + 1) * sizeof(char));
		strncpy(properties[i].pszValue, pchPos, nLen);
		properties[i].pszValue[nLen] = 0;

		// Move past the closing "
		pchPos = pchClose + 1;
	}
}

// This will need to be modified later when there are more than one of the same type
const char* Entity::FindProperty(const char* pszName)
{
	for(int i=0; i < nProperties; i++)
	{
		if(!strcmp(properties[i].pszName, pszName))
		{
			return properties[i].pszValue;
		}
	}
	return NULL;
}

