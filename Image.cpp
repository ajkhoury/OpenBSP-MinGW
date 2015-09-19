/* 
 * File:   Image.cpp
 * Author: dude719
 * 
 * Created on September 16, 2015, 8:13 PM
 */

#include "Image.h"

#include <cstdio>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

Image::Image(unsigned char nChannels, unsigned short nWidth, unsigned short nHeight)
{
    if(nChannels != 3 && nChannels != 4)
    {
       printf("Channels not 3 or 4!\n");
       return;
    }
    
    this->nChannels = nChannels;
    this->nWidth = nWidth;
    this->nHeight = nHeight;
    
    this->pData = (unsigned char*)malloc(nWidth * nHeight * nChannels * sizeof(unsigned char));
    if(this->pData == NULL)
    {
        printf("Image class memory allocation failed!\n");
        return;
    }
    
    memset(this->pData, 0, this->nWidth * this->nHeight * this->nChannels * sizeof(unsigned char));
}

Image::Image(const Image& orig)
{
    nChannels = orig.nChannels;
    nWidth = orig.nWidth;
    nHeight = orig.nHeight;
    memcpy(pData, orig.pData, nWidth * nHeight * nChannels * sizeof(unsigned char));  
}

Image::~Image()
{
    free(pData);
    pData = NULL;
}

