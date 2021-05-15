#pragma once

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER              // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS      // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE           // Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600    // Change this to the appropriate value to target other versions of IE.
#endif

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers

#define TOP_BOX_WIDTH 50
#define DOWN_BOX_WIDTH 200
// #define BOX_HEIGHT 100

#define MAX_COUNT 8

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <time.h>
#include <string>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <vector>
#include <iterator>   /* vector */
#include <algorithm>  /* sort */


/* Macros */


template<class Interface>
inline void
SafeRelease(
	Interface** ppInterfaceToRelease
)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#endif

/* DemoApp */

class DemoApp
{
public:
	DemoApp();
	~DemoApp();

	// Register the window class and call methods for instantiating drawing resources
	HRESULT Initialize();

	// Process and dispatch messages
	void RunMessageLoop();

private:

	// Initialize device-independent resources
	HRESULT CreateDeviceIndependentResources();

	// Initialize device-dependent resources
	HRESULT CreateDeviceResources();

	HRESULT CreateGridPatternBrush(
		ID2D1RenderTarget* pRenderTarget,
		ID2D1BitmapBrush** ppBitmapBrush
	);

	// Release device-dependent resources
	void DiscardDeviceResources();

	// Draw content
	HRESULT OnRender();

	// Resize the render target
	void OnResize(
		UINT width,
		UINT height
	);

	void GetRate();

	// Translate
	void DrawBox();

	// The windows procedure
	static LRESULT CALLBACK WndProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);

private:
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1StrokeStyle* m_pStrokeStyleDash;

	ID2D1SolidColorBrush* m_pOriginalShapeBrush[MAX_COUNT];
	ID2D1SolidColorBrush* m_pFillBrush;
	ID2D1SolidColorBrush* m_pTopBrush;
	ID2D1SolidColorBrush* m_pTextBrush;
	ID2D1SolidColorBrush* m_pTextBrush2;
	ID2D1BitmapBrush* m_pGridPatternBitmapBrush;

	//DWrite
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
};