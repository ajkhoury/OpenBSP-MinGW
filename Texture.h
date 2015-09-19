/* 
 * File:   Texture.h
 * Author: dude719
 *
 * Created on September 16, 2015, 11:15 PM
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

class Texture
{
public:
    Texture();
    Texture(const Texture& orig);
    virtual ~Texture();
    
private:
    int tex;        // texture object
	int sampler;    // texture sampler
    
    static const int MAXWIDTH; 
};

#endif	/* TEXTURE_H */

