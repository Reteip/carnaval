// /*
//  * rgbtohsv.c
//  *
//  * Created: 31-Jan-16 11:55:12
//  *  Author: pwith
//  */ 

// #include "rgbtohsv.h"

// void HsvToRgb(CHSV hsv, CRGB rgb)
// {
	
// 	unsigned char region, p, q, t;
// 	unsigned int h, s, v, remainder;

// 	if (hsv.s == 0)
// 	{
// 		rgb.r = hsv.v;
// 		rgb.g = hsv.v;
// 		rgb.b = hsv.v;
// 		return rgb;
// 	}

// 	// converting to 16 bit to prevent overflow
// 	h = hsv.h;
// 	s = hsv.s;
// 	v = hsv.v;

// 	region = h / 43;
// 	remainder = (h - (region * 43)) * 6;

// 	p = (v * (255 - s)) >> 8;
// 	q = (v * (255 - ((s * remainder) >> 8))) >> 8;
// 	t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

// 	switch (region)
// 	{
// 		case 0:
// 		rgb.red = v;
// 		rgb.green = t;
// 		rgb.blue = p;
// 		break;
// 		case 1:
// 		rgb.red = q;
// 		rgb.green = v;
// 		rgb.blue = p;
// 		break;
// 		case 2:
// 		rgb.red = p;
// 		rgb.green = v;
// 		rgb.blue = t;
// 		break;
// 		case 3:
// 		rgb.red = p;
// 		rgb.green = q;
// 		rgb.blue = v;
// 		break;
// 		case 4:
// 		rgb.red = t;
// 		rgb.green = p;
// 		rgb.blue = v;
// 		break;
// 		default:
// 		rgb.red = v;
// 		rgb.green = p;
// 		rgb.blue = q;
// 		break;
// 	}

// 	//return rgb;
// }

// void RgbToHsv(CRGB rgb, CHSV hsv)
// {
// 	unsigned char rgbMin, rgbMax;

// 	rgbMin = rgb.red < rgb.green ? (rgb.red < rgb.blue ? rgb.red : rgb.blue) : (rgb.green < rgb.blue ? rgb.green : rgb.blue);
// 	rgbMax = rgb.red > rgb.green ? (rgb.red > rgb.blue ? rgb.red : rgb.blue) : (rgb.green > rgb.blue ? rgb.green : rgb.blue);

// 	hsv.v = rgbMax;
// 	if (hsv.v == 0)
// 	{
// 		hsv.h = 0;
// 		hsv.s = 0;
// 		return hsv;
// 	}

// 	hsv.s = 255 * ((long)(rgbMax - rgbMin)) / hsv.v;
// 	if (hsv.s == 0)
// 	{
// 		hsv.h = 0;
// 		return hsv;
// 	}

// 	if (rgbMax == rgb.red)
// 	hsv.h = 0 + 43 * (rgb.green - rgb.blue) / (rgbMax - rgbMin);
// 	else if (rgbMax == rgb.green)
// 	hsv.h = 85 + 43 * (rgb.blue - rgb.red) / (rgbMax - rgbMin);
// 	else
// 	hsv.h = 171 + 43 * (rgb.red - rgb.green) / (rgbMax - rgbMin);

// 	//return hsv;
// }
