/* 
 * File:   Image.h
 * Author: dude719
 *
 * Created on September 16, 2015, 8:13 PM
 */

#ifndef IMAGE_H
#define	IMAGE_H

#include "Rect.h"

class Image
{
public:
    Image(unsigned char nChannels, unsigned short nWidth, unsigned short nHeight);
    Image(const Image& orig);
    virtual ~Image();

	unsigned char nChannels; // The channels in the image (3 = RGB, 4 = RGBA)
	unsigned short nWidth;    // The width of the image in pixels
	unsigned short nHeight;   // The height of the image in pixels
	unsigned char* pData;   // The image pixel data
};

#endif	/* IMAGE_H */

