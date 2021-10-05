//*****************************************************************************
//
// Main.cpp : Defines the entry point for the application.
// Creates a white RGB image with a black line and displays it.
// Two images are displayed on the screen.
// Left Pane: Input Image, Right Pane: Modified Image
//
// Author - Parag Havaldar
// Code used by students as a starter code to display and modify images
//
//*****************************************************************************


// Include class files
#include "Image.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <tuple>
#include <algorithm>
#include "windows.h"
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
MyImage			foreground_Image;				// The foreground Image
MyImage			Image;							// The outpit Image
HINSTANCE		hInst;							// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// Create a array of class Image, which is the final output images array (24 fps, 20 seconds long)
MyImage images_array[480];

// Second Per Frame (spf) = Each frame show 1/fps seconds
long long		spf = 1 / 24.0;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// Main entry point for a windows application
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	char commandline[200];
	char foreground_fname[100];
	char background_fname[100];
	char mode[10];

	string fore_fname, back_fname;
	int m;

	// Create a separate console window to display output to stdout
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	// Parse the input command
	if (lpCmdLine[0] == 0) {
		wprintf(L"No command line arguement.");
		return -1;
	}
	int cnt=0;
	while (lpCmdLine[cnt]!= ' '&& lpCmdLine[cnt] !=0) {
		cnt++;
	}
	
	strcpy(commandline, lpCmdLine);
	printf("The whole command: %s\n", commandline);

	// parse the foreGroudVideo
	strncpy(foreground_fname, commandline, cnt);
	foreground_fname[cnt] = '\0';
	fore_fname = string(foreground_fname);
	printf("The foreGroudVideo was: %s\n", foreground_fname);

	//parse the backGroundVideo
	int cnt1 = cnt+1;
	while (lpCmdLine[cnt1] != ' ') {
		cnt1++;
	}
	strncpy(background_fname, commandline+cnt+1, cnt1-cnt-1);
	background_fname[cnt1 - cnt - 1] = '\0';
	back_fname = string(background_fname);
	printf("The backGroundVideo was: %s\n", background_fname);

	//parse the mode parameter: Mode value
	int totalLen = strlen(lpCmdLine);
	strncpy(mode, commandline+cnt1+1, totalLen-(cnt1+1));
	mode[totalLen - (cnt1 + 1)] = '\0';
	m = atoi(mode);
	printf("The mode parameter was: %d\n", m);

	// Set up the images
	int w = 640;
	int h = 480;
	Image.setWidth(w);
	Image.setHeight(h);
	foreground_Image.setWidth(w);
	foreground_Image.setHeight(h);

	int n_previous = 24;
	int h_threshold = 30;
	double s_threshold = 0.2;
	double v_threshold = 0.2;
	string fore_rgb;
	string back_rgb;
	string number;
	int* mask = new int[h * w]{ 1 };
	double* average_frame = new double[640 * 480 * 3];
	double* local_average_frame = new double[640 * 480 * 3];

	for (int i = 0; i < 480; i++) {
		number = string(4 - to_string(i).length(), '0') + to_string(i);
		fore_rgb = fore_fname + "." + number + ".rgb";
		back_rgb = back_fname + "." + number + ".rgb";
		strcpy(foreground_fname, fore_rgb.c_str());
		strcpy(background_fname, back_rgb.c_str());
		cout << i << endl;
		
		foreground_Image.setImagePath(foreground_fname);
		foreground_Image.ReadImage();

		// Mode 1 -> green screen
		if (m == 1) {
			mask = foreground_Image.extract();
		}

		// Mode 0 -> Subtraction
		else if (m == 0) {
			// Mean method
			mask = foreground_Image.subtract(n_previous, h_threshold, s_threshold, v_threshold, i, average_frame, local_average_frame);
			average_frame = foreground_Image.getPrivate_AvgFrameData();
		}


		// Merge the mask of this foreground_image with background_image
		Image.setImagePath(background_fname);
		Image.ReadImage();
		Image.merge(foreground_Image, mask);
		images_array[i] = Image; 
	}
	

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_IMAGE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_IMAGE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_IMAGE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_IMAGE;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
// TO DO: part useful to render video frames, may place your own code here in this function
// You are free to change the following code in any way in order to display the video

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	RECT rt;
	GetClientRect(hWnd, &rt);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case ID_MODIFY_IMAGE:
				   InvalidateRect(hWnd, &rt, false); 
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			{
				hdc = BeginPaint(hWnd, &ps);
				// TO DO: Add any drawing code here...
				char text[1000];
				strcpy(text, "The original image is shown as follows. \n");
				DrawText(hdc, text, strlen(text), &rt, DT_LEFT);
				strcpy(text, "\nUpdate program with your code to modify input image. \n");
				DrawText(hdc, text, strlen(text), &rt, DT_LEFT);

				// Print out the video of 480 frames
				BITMAPINFO bmi;
				CBitmap bitmap;
				memset(&bmi, 0, sizeof(bmi));
				bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
				bmi.bmiHeader.biPlanes = 1;
				bmi.bmiHeader.biBitCount = 24;
				bmi.bmiHeader.biCompression = BI_RGB;
				bmi.bmiHeader.biWidth = 640;
				bmi.bmiHeader.biHeight = -480;  // Use negative height.  DIB is top-down.
				bmi.bmiHeader.biSizeImage = 640 * 480;

				// Width = 640, Height = 480
				for (int i = 0; i < 480; i = i + 1) {
					SetDIBitsToDevice(hdc,
						100, 100, 640, 480,
						0, 0, 0, 480,
						images_array[i].getImageData(), &bmi, DIB_RGB_COLORS);

					// Sleep 41.7 millseconds for each loop 
					// = Display each frame for 1/24 seconds 
					Sleep(30);
				}

				EndPaint(hWnd, &ps);
			}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}




// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


