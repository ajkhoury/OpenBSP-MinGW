/* 
 * File:   Color.cpp
 * Author: dude719
 * 
 * Created on May 9, 2015, 6:29 PM
 */

#include "Color.h"

float Color::black[4] = { 0, 0, 0, 1 };
float Color::red[4] = { 1, 0, 0, 1 };
float Color::transparent_green[4] = { 0, 1, 0, 0.5f };
float Color::green[4] = { 0, 1, 0, 1 };
float Color::yellow[4] = { 1, 1, 0, 1 };

Color::Color()
{
    clr[0] = 1;
    clr[1] = 1;
    clr[2] = 1;
    clr[3] = 1;
}

Color::Color(const Color& orig)
{
}

Color::Color(float r, float g, float b, float a)
{
    clr[0] = r;
    clr[1] = g;
    clr[2] = b;
    clr[3] = a;
}

Color::Color(int r, int g, int b, int a)
{
    clr[0] = r/255;
    clr[1] = g/255;
    clr[2] = b/255;
    clr[3] = a/255;
}

Color::~Color()
{
}

float* Color::Base()
{
    return &clr[0];
}

