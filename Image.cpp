//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <iostream>
#include <cmath>
#include <tuple>
using namespace std;
#define PI 3.1415926

// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}
}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	//MyImage output;
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];

		/*cout << int(Bbuf[i]) << int(Gbuf[i]) << int(Rbuf[i]) << endl;*/
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;
}

// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;
}


// Transformation between RGB and HSV
typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;

hsv   rgb_hsv(rgb input);
rgb   hsv_rgb(hsv input);

hsv rgb_hsv(rgb input)
{
	hsv output;
	double r, g, b;
	double min, max, delta;

	r = input.r / 255.0;
	g = input.g / 255.0;
	b = input.b / 255.0;

	max = (r > g) ? (r > b ? r : b) : (g > b ? g : b);
	min = (r < g) ? (r < b ? r : b) : (g < b ? g : b);
	delta = max - min;

	// Hue calculation
	if (delta == 0) {
		output.h = 0;
	}
	else if (max == r) {
		output.h = fmod((60 * ((g - b) / delta) + 360), 360.0);
	}
	else if (max == g) {
		output.h = fmod((60 * ((b - r) / delta) + 120), 360.0);
	}
	else if (max == b) {
		output.h = fmod((60 * ((r - g) / delta) + 240), 360.0);
	}

	// Saturation computation
	if (max == 0) {
		output.s = 0;
	}
	else {
		output.s = 1 - (min / max);
	}

	// Value computation
	output.v = max;
	//cout << "H: " << output.h << ", S: " << output.s << ", V: " << output.v << endl;

	return output;
}

rgb hsv_rgb(hsv input)
{
	double s, v;
	double c, x, m;
	double r, g, b;
	rgb output;

	s = input.s;
	v = input.v;
	c = s * v;
	x = c * (1 - abs(fmod((input.h / 60.0), 2.0) - 1));
	m = v - c;

	if (input.h >= 0 && input.h < 60) {
		r = c, g = x, b = 0;
	}
	else if (input.h >= 60 && input.h < 120) {
		r = x, g = c, b = 0;
	}
	else if (input.h >= 120 && input.h < 180) {
		r = 0, g = c, b = x;
	}
	else if (input.h >= 180 && input.h < 240) {
		r = 0, g = x, b = c;
	}
	else if (input.h >= 240 && input.h < 300) {
		r = x, g = 0, b = c;
	}
	else {
		r = c, g = 0, b = x;
	}

	output.r = (r + m) * 255;
	output.g = (g + m) * 255;
	output.b = (b + m) * 255;

	return output;
}

// Here is where you would place your code to modify an image
// eg Filtering, Transformation, Cropping, etc.

// Extract targets from foreground, and return a mask which 1 in a pixel represent that's a target pixel, and 0 in a pixel represent that's a background pixel
int* MyImage::extract()
{
	hsv* origin_HSVbuf = new hsv[Width * Height];
	rgb RGBtemp;
	hsv HSVtemp;
	int* mask = new int[Height * Width]{ 1 }; 

	// Store the R,G,B of each pixel from the origin image, and transfer each pixel from RGB to HSV
	for (int i = 0; i < Height * Width; i++)
	{
		// mask initialize
		mask[i] = 1;
		RGBtemp.b = Data[3 * i];
		RGBtemp.g = Data[3 * i + 1];
		RGBtemp.r = Data[3 * i + 2];
		// Transform this pixel from RGB to HSV
		HSVtemp = rgb_hsv(RGBtemp);
		//cout << HSVtemp.h << ", " << HSVtemp.s << ", " << HSVtemp.v << endl;

		// 1 -> foreground pixel; 0 -> Boundary pixel; -1 -> green pixel;
		// If this pixel in HSV form is inside the range of "green" -> mask of this pixel set to -1
		if (HSVtemp.h >= 55 && HSVtemp.h <= 165 && HSVtemp.s >= 0.3 && HSVtemp.v >= 0.3) {
			// If the previous is NOT "green", but this pixel is -> Boundary detect!!
			if (i > 0 && mask[i - 1] == 1) {
				mask[i] = 0;
			}
			// Mark this pixel as "green"
			else
				mask[i] = -1;
		}

		// If this pixel is NOT considered as "green"
		else {
			// If the previous pixel is "green" but this pixel is not -> Boundary detect!!
			// Set this entry of mask as "grey" (0)
			if (i > 0 && mask[i - 1] == -1) {
				mask[i] = 0;
			}
		}
	}

	delete origin_HSVbuf;
	return mask;
}


// Subtraction by median of n_previous frame
int* MyImage::subtractMedian(char* input_median_frame, int h_threshold, double s_threshold, double v_threshold)
{
	int* mask = new int[Height * Width]{ 1 };
	char* average_frame = new char[640 * 480 * 3];
	double learning_rate;
	rgb rgb_present, rgb_previous;
	hsv hsv_present, hsv_previous;

	// Subtrack this frame with input_median_frame !!!
	for (int i = 0; i < Height * Width; i++) {
		// mask initialize as all foreground
		mask[i] = 1;

		rgb_present.b = Data[3 * i];
		rgb_present.g = Data[3 * i + 1];
		rgb_present.r = Data[3 * i + 2];
		hsv_present = rgb_hsv(rgb_present);

		rgb_previous.r = input_median_frame[3 * i];
		rgb_previous.g = input_median_frame[3 * i + 1];
		rgb_previous.b = input_median_frame[3 * i + 2];
		hsv_previous = rgb_hsv(rgb_previous);

		// Mark this pixel as "background", if h, s, v are all inside the threshold
		if (abs(hsv_present.h - hsv_previous.h) <= h_threshold && abs(hsv_present.s - hsv_previous.s) <= s_threshold && abs(hsv_present.v - hsv_previous.v) <= v_threshold) {
			mask[i] = -1;
		}

		// If this pixel is NOT considered as "background"
		else {
			// If the previous pixel is "background" but this pixel is not -> Boundary detect!!
			// Set this pixel of mask as "boundary" (set to 0)
			if (i > 0 && mask[i - 1] == -1) {
				mask[i] = 0;
			}
		}
	}
	cout << "Image.cpp finish subtract!" << endl;
	private_median_frame = input_median_frame;
	cout << "Image.cpp finish assign median_frame" << endl;
	return mask;
}



// Subtraction
int* MyImage::subtract(int n_previous, int h_threshold, double s_threshold, double v_threshold, int ith, double* input_average_frame, double* local_average_frame)
{
	int* mask = new int[Height * Width]{ 1 };
	double distance;
	double learning_rate;
	rgb rgb_present, rgb_previous;
	hsv hsv_present, hsv_previous;

	// First n_previous frames
	if (ith < n_previous)
		learning_rate = 1.0 / (ith + 1);
	// Frames after first n_previous frames
	else
		learning_rate = 1.0 / n_previous;
	//cout << ith << "(th of learning rate, 1-learning rate): " << learning_rate << ", " << 1 - learning_rate << endl;

	// Calculate the average frame for the first frame -> ith = 0
	if (ith == 0) {
		for (int i = 0; i < Height * Width; i++) {
			/*average_frame[3 * i] = Data[3 * i];
			average_frame[3 * i + 1] = Data[3 * i + 1];
			average_frame[3 * i + 2] = Data[3 * i + 2];*/
			rgb_present.b = Data[3 * i];
			rgb_present.g = Data[3 * i + 1];
			rgb_present.r = Data[3 * i + 2];
			hsv_present = rgb_hsv(rgb_present);
			local_average_frame[3 * i] = hsv_present.h;
			local_average_frame[3 * i + 1] = hsv_present.s;
			local_average_frame[3 * i + 2] = hsv_present.v;
			//cout << "First average frame: " << hsv_present.h << ", " << hsv_present.s << ", " << hsv_present.v << endl;
		}
	}
	else {
		for (int i = 0; i < Height * Width; i++) {
			local_average_frame[3 * i] = input_average_frame[3 * i];
			local_average_frame[3 * i + 1] = input_average_frame[3 * i + 1];
			local_average_frame[3 * i + 2] = input_average_frame[3 * i + 2];
		}
	}

	// Subtrack this frame with average_frame !!!
	for (int i = 0; i < Height * Width; i++) {
		// mask initialize as all foreground
		mask[i] = 1;

		rgb_present.b = Data[3 * i];
		rgb_present.g = Data[3 * i + 1];
		rgb_present.r = Data[3 * i + 2];
		hsv_present = rgb_hsv(rgb_present);
		hsv_previous.h = local_average_frame[3 * i];
		hsv_previous.s = local_average_frame[3 * i + 1];
		hsv_previous.v = local_average_frame[3 * i + 2];

		// Mark this pixel as "background"
		if (abs(hsv_present.h - hsv_previous.h) <= h_threshold && abs(hsv_present.s - hsv_previous.s) <= s_threshold && abs(hsv_present.v - hsv_previous.v) <= v_threshold) {
			mask[i] = -1;
		}

		// Update the average_frame
		local_average_frame[3 * i] = (1.0 - learning_rate) * hsv_previous.h + learning_rate * hsv_present.h;
		local_average_frame[3 * i + 1] = (1.0 - learning_rate) * hsv_previous.s + learning_rate * hsv_present.s;
		local_average_frame[3 * i + 2] = (1.0 - learning_rate) * hsv_previous.v + learning_rate * hsv_present.v;
	}

	private_avg_frame = local_average_frame;
	return mask;
}


bool MyImage::merge(MyImage foreground, int* mask)
{
	for (int i = 0; i < Height * Width; i++)
	{
		// This pixel is NOT a green pixel -> Set the Value of foreground_image
		if (mask[i] == 1) {
			Data[3 * i] = foreground.getImageData()[3 * i];
			Data[3 * i + 1] = foreground.getImageData()[3 * i + 1];
			Data[3 * i + 2] = foreground.getImageData()[3 * i + 2];
		}

		// This pixel is a boundary -> Set the value of this pixel as an average with several previous pixels in background_image
		else if (mask[i] == 0 && i > 1 && i < (Height * Width - 2)) {
			Data[3 * i] = (Data[3 * (i - 2)] + Data[3 * (i - 1)] + Data[3 * (i)] + Data[3 * (i + 1)] + Data[3 * (i + 2)]) / 5;
			Data[3 * i + 1] = (Data[3 * (i - 2) + 1] + Data[3 * (i - 1) + 1] + Data[3 * i + 1] + Data[3 * (i + 1) + 1] + Data[3 * (i + 2) + 1]) / 5;
			Data[3 * i + 2] = (Data[3 * (i - 2) + 2] + Data[3 * (i - 1) + 2] + Data[3 * i + 2] + Data[3 * (i + 1) + 2] + Data[3 * (i + 2) + 2]) / 5;
		}
	}

	return false;
}
