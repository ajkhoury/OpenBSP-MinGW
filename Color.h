/* 
 * File:   Color.h
 * Author: dude719
 *
 * Created on May 9, 2015, 6:29 PM
 */

#ifndef COLOR_H
#define	COLOR_H

class Color
{
public:
    Color();
    Color(const Color& orig);  
    Color(float r, float g, float b, float a);
    Color(int r, int g, int b, int a);
    virtual ~Color();
    
    float* Base();
    
    static float white[4];
    static float black[4];
	static float red[4];
	static float transparent_green[4];
	static float green[4];
	static float yellow[4];
    
private:
    float clr[4];
};

#endif	/* COLOR_H */

