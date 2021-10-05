//*****************************************************************************
//
// Image.h : Defines the class operations on images
//
// Author - Parag Havaldar
// Main Image class structure 
//
//*****************************************************************************

#ifndef IMAGE_DISPLAY
#define IMAGE_DISPLAY

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "afxwin.h"

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <tuple>

// Class structure of Image 
// Use to encapsulate an RGB image
class MyImage 
{

private:
	int		Width;					// Width of Image
	int		Height;					// Height of Image
	char	ImagePath[_MAX_PATH];	// Image location
	char*	Data;					// RGB data of the image
	double* private_avg_frame;			// The average frame that next pixel should compare to
	char* private_median_frame;

public:
	// Constructor
	MyImage();
	// Copy Constructor
	MyImage::MyImage( MyImage *otherImage);
	// Destructor
	~MyImage();

	// operator overload
	MyImage & operator= (const MyImage & otherImage);

	// Reader & Writer functions
	void	setWidth( const int w)  { Width = w; }; 
	void	setHeight(const int h) { Height = h; }; 
	void	setImageData( const char *img ) { Data = (char *)img; };
	void	setImagePath( const char *path) { strcpy(ImagePath, path); }
	int		getWidth() { return Width; };
	int		getHeight() { return Height; };
	char*	getImageData() { return Data; };
	char*	getImagePath() { return ImagePath; }
	double* getPrivate_AvgFrameData() { return private_avg_frame; }
	char* getPrivate_MedianFrameData() { return private_median_frame; }

	// Input Output operations
	bool	ReadImage();
	bool	WriteImage();

	// mode 1 -> Extract from green screen
	// mode 0 -> Subtract from moving background
	int*	extract();
	int*	subtract(int n_previous, int h_threshold, double s_threshold, double v_threshold, int ith, double* average_frame, double* local_average_frame);
	int*	subtractMedian(char* n_previous_frame, int h_threshold, double s_threshold, double v_threshold);
	bool	merge(MyImage foreground, int* mask);
};

#endif //IMAGE_DISPLAY