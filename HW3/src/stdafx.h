#pragma once

// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
// DX Header Files
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>

#include "Animation.h"

// 자원 안전 반환 매크로.
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

// 현재 모듈의 시작주소 얻기.
#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class DemoApp
{
public:
	DemoApp();
	~DemoApp();
	HRESULT Initialize();
	void RunMessageLoop();

private:
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources();
	void DiscardDeviceResources();
	HRESULT OnRender();
	void OnResize(UINT width, UINT height);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HRESULT LoadBitmapFromResource(ID2D1RenderTarget* pRT, IWICImagingFactory* pIWICFactory, PCWSTR resourceName, PCWSTR resourceType, __deref_out ID2D1Bitmap** ppBitmap);

private:
	HWND m_hwnd;
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRT;
	ID2D1PathGeometry* m_pPathGeometry;
	ID2D1PathGeometry* m_pObjectGeometry;
	ID2D1PathGeometry* m_pPathGeometry2;
	ID2D1PathGeometry* m_pObjectGeometry2;
	ID2D1SolidColorBrush* m_pRedBrush;
	ID2D1SolidColorBrush* m_pWhiteBrush;
	ID2D1SolidColorBrush* m_pYellowBrush;
	ID2D1SolidColorBrush* m_pGreenBrush;
	ID2D1SolidColorBrush* m_pTextBrush;

	AnimationEaseInOut<float> m_Animation;
	AnimationEaseInOut<float> m_Animation2;

	LARGE_INTEGER m_nPrevTime;
	LARGE_INTEGER m_nFrequency;

	// bitmaps
	ID2D1Bitmap* m_pFernBitmap;
	ID2D1Bitmap* m_pBitmapMask;
	// bitmap brushes
	ID2D1BitmapBrush* m_pFernBitmapBrush;
	ID2D1BitmapBrush* m_pBitmapMaskBrush;

	IWICImagingFactory* m_pWICFactory;

	//DWrite
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
};
