/* 
 * File:   Utils.h
 * Author: dude719
 *
 * Created on April 19, 2015, 7:40 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <windows.h>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

namespace Utils
{
	static void CreateConsole(const char* name)
	{
		HANDLE lStdHandle = 0;
		int hConHandle = 0;
		FILE *fp = 0;
		AllocConsole();
		lStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		hConHandle = _open_osfhandle(PtrToUlong(lStdHandle), _O_TEXT);
		SetConsoleTitle(name);
		//SetConsoleTextAttribute(lStdHandle, FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_RED);
		fp = _fdopen(hConHandle, "w");
		*stdout = *fp;
		setvbuf(stdout, NULL, _IONBF, 0);
	}

    static float RemapFloat(float x, float oMin, float oMax, float nMin, float nMax )
	{
		//check reversed input range
		bool reverseInput = false;
		float oldMin = oMin < oMax ? oMin : oMax;
		float oldMax = oMin < oMax ? oMax : oMin;
		if (oldMin == oMin)
			reverseInput = true;

		//check reversed output range
		bool reverseOutput = false;  
		float newMin = nMin < nMax ? nMin : nMax;
		float newMax = nMin < nMax ? nMax : nMin;
		if (newMin == nMin)
			reverseOutput = true;

		float portion = (x-oldMin)*(newMax-newMin)/(oldMax-oldMin);
		if (reverseInput)
			portion = (oldMax-x)*(newMax-newMin)/(oldMax-oldMin);

		float result = portion + newMin;
		if (reverseOutput)
			result = newMax - portion;

		return result; 
	}
    
}

#endif	/* UTILS_H */

