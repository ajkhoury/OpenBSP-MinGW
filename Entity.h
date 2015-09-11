/* 
 * File:   Entity.h
 * Author: ankhoury
 *
 * Created on April 19, 2015, 6:20 AM
 */

#ifndef ENTITY_H
#define	ENTITY_H

// Structure which defines a property of an entity
typedef struct _ENTITYPROPERTY
{
	char* pszName;  // String containing the name of the property
	char* pszValue; // String containing the value of the property
} ENTITYPROPERTY;

class Entity
{
public:
	Entity();
	~Entity();

	void ParseProperties(const char* pszProperties); // Parses an entity of the bsp file entity lump into ENTITYPROPERTY data structurs
	const char* FindProperty(const char* pszName);   // Returns the value of a property by the given name

private:
	ENTITYPROPERTY* properties; // Pointer to array of properties
	int nProperties;            // The number of properties
};

#endif	/* ENTITY_H */

