/* 
 * File:   Rect.h
 * Author: ankhoury
 *
 * Created on September 19, 2015, 5:53 AM
 */

#ifndef RECT_H
#define	RECT_H


typedef struct _Rect {
    _Rect() : x(0), y(0), width(0), height(0) { }
    int x, y, width, height;
} Rect;


#endif	/* RECT_H */

