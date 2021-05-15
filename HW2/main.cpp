#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")

#include "stdafx.h"
#include "student.h"

BOOL MakeBox = false;
BOOL MoveBox = false;
BOOL DeleteBox = false;

/* 현재 마우스 위치 */
D2D_POINT_2F current_p;

/* 회전 각도 */
FLOAT Angle = 0.0f;

/* 윈도우 크기 */
D2D1_SIZE_F rtSize;
UINT winWidth;
UINT winHeight;

/* Direct2d 가로, 세로 비율 */
float width_rate;
float height_rate;

/* 가로, 세로 길이 제어 변수 */
float move_rate = 0;

/* 학생 이름 */
std::string name[10] = { "성은지", "이지연","이서현","조유진","허가현","김민주","유현아", "이동은", "소원임","박미나" };

student tmpStudent;
vector<student> vStudent;
vector<student>::iterator iter;

bool checkName[STUDENT_NUM] = { false, };

/* ------------------------------------ main ----------------------------------------- */

int WINAPI WinMain(
	HINSTANCE /* hInstance */,
	HINSTANCE /* hPrevInstance */,
	LPSTR /* lpCmdLine */,
	int /* nCmdShow */
)
{
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);
	if (SUCCEEDED(CoInitialize(NULL)))
	{
		{
			DemoApp app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}
		CoUninitialize();
	}

	return 0;
}

DemoApp::DemoApp() :
	m_hwnd(NULL),
	m_pD2DFactory(NULL),
	m_pRenderTarget(NULL),
	m_pStrokeStyleDash(NULL),
	m_pGridPatternBitmapBrush(NULL),
	m_pTopBrush(NULL),
	m_pFillBrush(NULL),
	m_pTextBrush(NULL),
	m_pTextBrush2(NULL),
	m_pDWriteFactory(NULL),
	m_pTextFormat(NULL)
{

}

DemoApp::~DemoApp()
{
	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pStrokeStyleDash);
	SafeRelease(&m_pGridPatternBitmapBrush);

	SafeRelease(&m_pFillBrush);
	SafeRelease(&m_pTextBrush);
	SafeRelease(&m_pTextBrush2);
	SafeRelease(&m_pDWriteFactory);
	SafeRelease(&m_pTextFormat);
	for (int i = 0; i < MAX_COUNT; i++)
		SafeRelease(&m_pOriginalShapeBrush[i]);
}

HRESULT DemoApp::Initialize()
{
	HRESULT hr;

	hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr))
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = DemoApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.lpszClassName = L"HW2_201801567(성은지)";

		RegisterClassEx(&wcex);

		m_hwnd = CreateWindow(
			L"HW2_201801567(성은지)",
			L"HW2_201801567(성은지)",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			1920,
			1080,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);

		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	srand((unsigned int)time(NULL));

	return hr;
}

HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory
	);

	if (SUCCEEDED(hr))
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);
	}

	if (SUCCEEDED(hr))
	{
		hr = m_pDWriteFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			13.0f,
			L"ko-kr",
			&m_pTextFormat
		);
	}

	if (SUCCEEDED(hr))
	{
		float dashes[] = { 10.0f, 2.0f };
		hr = m_pD2DFactory->CreateStrokeStyle(
			D2D1::StrokeStyleProperties(
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_CAP_STYLE_FLAT,
				D2D1_LINE_JOIN_MITER,
				10.0f,
				D2D1_DASH_STYLE_CUSTOM,
				0.0f),
			dashes,
			ARRAYSIZE(dashes),
			&m_pStrokeStyleDash
		);
	}

	return hr;
}

HRESULT DemoApp::CreateDeviceResources()
{
	HRESULT hr = S_OK;
	srand((unsigned int)time(NULL));

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);

		hr = m_pD2DFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&m_pRenderTarget
		);

		/* 학생 직사각형에 들어갈 8개의 색상 */
		FLOAT colorArr[3];

		for (int i = 0; i < MAX_COUNT; i++) {													// (1) 헷갈린 부분

			if (i == 0) {
				colorArr[0] = 255; colorArr[1] = 87; colorArr[2] = 127;
			}
			else if (i == 1) {
				colorArr[0] = 255; colorArr[1] = 136; colorArr[2] = 75;
			}
			else if (i == 2) {
				colorArr[0] = 255; colorArr[1] = 199; colorArr[2] = 100;
			}
			else if (i == 3) {
				colorArr[0] = 205; colorArr[1] = 255; colorArr[2] = 252;
			}
			else if (i == 4) {
				colorArr[0] = 74; colorArr[1] = 71; colorArr[2] = 163;
			}
			else if (i == 5) {
				colorArr[0] = 74; colorArr[1] = 71; colorArr[2] = 0;
			}
			else if (i == 6) {
				colorArr[0] = 112; colorArr[1] = 159; colorArr[2] = 176;
			}
			else if (i == 7) {
				colorArr[0] = 167; colorArr[1] = 197; colorArr[2] = 235;
			}
			else
				break;

			/* 색의 범위는 0~1 */
			for (int j = 0; j < 3; j++)
				colorArr[j] = (FLOAT)(colorArr[j] / 255);

			if (SUCCEEDED(hr))
			{
				// Create a brush for outlining the pre-transform shape.
				hr = m_pRenderTarget->CreateSolidColorBrush(
					D2D1::ColorF(colorArr[0], colorArr[1], colorArr[2], 1),
					&m_pOriginalShapeBrush[i]
				);
			}
		}

		/* 상단에 있는 직사각형의 색상은 회색 */
		if (SUCCEEDED(hr))
		{
			// Create a brush to fill the area of Top Box
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Gray),
				&m_pTopBrush
			);
			m_pTopBrush->SetOpacity(0.2f);
		}

		/* 하단에 있는 직사각형의 색상은 파란색 */
		if (SUCCEEDED(hr))
		{
			// Create a brush to fill the area of an element.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Blue),
				&m_pFillBrush
			);
		}

		if (SUCCEEDED(hr))
		{
			// Create a solid color brush for writing text.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Black),
				&m_pTextBrush
			);
		}
		if (SUCCEEDED(hr))
		{
			// Create a solid color brush for writing text.
			hr = m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&m_pTextBrush2
			);
		}

		if (SUCCEEDED(hr))
		{
			hr = CreateGridPatternBrush(m_pRenderTarget, &m_pGridPatternBitmapBrush);
		}
	}

	return hr;
}

HRESULT DemoApp::CreateGridPatternBrush(
	ID2D1RenderTarget* pRenderTarget,
	ID2D1BitmapBrush** ppBitmapBrush
)
{
	HRESULT hr = S_OK;

	ID2D1BitmapRenderTarget* pCompatibleRenderTarget = NULL;
	hr = pRenderTarget->CreateCompatibleRenderTarget(
		D2D1::SizeF(10.0f, 10.0f),
		&pCompatibleRenderTarget
	);

	if (SUCCEEDED(hr))
	{
		ID2D1SolidColorBrush* pGridBrush = NULL;
		hr = pCompatibleRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(0.93f, 0.94f, 0.96f, 1.0f),
			&pGridBrush
		);

		if (SUCCEEDED(hr))
		{
			pCompatibleRenderTarget->BeginDraw();
			pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 10.0f, 1.0f), pGridBrush);
			pCompatibleRenderTarget->FillRectangle(D2D1::RectF(0.0f, 0.0f, 1.0f, 10.0f), pGridBrush);
			hr = pCompatibleRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				// Discard all of the domain resources on this error.
				DiscardDeviceResources();
			}
			if (SUCCEEDED(hr))
			{
				// Retrieve the bitmap from the render target.
				ID2D1Bitmap* pGridBitmap = NULL;
				hr = pCompatibleRenderTarget->GetBitmap(&pGridBitmap);

				if (SUCCEEDED(hr))
				{
					// Create the bitmap brush.
					hr = m_pRenderTarget->CreateBitmapBrush(
						pGridBitmap,
						D2D1::BitmapBrushProperties(
							D2D1_EXTEND_MODE_WRAP,
							D2D1_EXTEND_MODE_WRAP),
						ppBitmapBrush
					);

					pGridBitmap->Release();
				}
			}

			pGridBrush->Release();
		}

		pCompatibleRenderTarget->Release();
	}

	return hr;
}

void DemoApp::DiscardDeviceResources()
{
	SafeRelease(&m_pRenderTarget);
	SafeRelease(&m_pFillBrush);
	SafeRelease(&m_pTextBrush);
	SafeRelease(&m_pGridPatternBitmapBrush);
	for (int i = 0; i < MAX_COUNT; i++)
		SafeRelease(&m_pOriginalShapeBrush[i]);
}

void DemoApp::RunMessageLoop()
{
	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT DemoApp::OnRender()
{
	HRESULT hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		// Get ready to draw.
		m_pRenderTarget->BeginDraw();

		// Reset to the identity transform.
		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		// Clear the render target contents.
		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		m_pRenderTarget->FillRectangle(
			D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height),
			m_pGridPatternBitmapBrush
		);

		m_pFillBrush->SetOpacity(0.2f);

		/* 상자 만들기 함수 호출 */
		DrawBox();

		m_pRenderTarget->EndDraw();
	}

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}


/* --------------------------------------- Draw ------------------------------------ */

/* Direct2d Window 비율 구하기 */
void DemoApp::GetRate() {
	RECT temp;
	GetClientRect(m_hwnd, &temp);

	rtSize = m_pRenderTarget->GetSize(); //윈도우 사이즈

	width_rate = rtSize.width / (temp.right - temp.left);
	height_rate = rtSize.height / (temp.bottom - temp.top);
}

void DemoApp::DrawBox()
{
	GetRate();

	/* 왼쪽 상단 정보 */
	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	WCHAR szText[100];
	swprintf_s(szText, L"마우스x:%f\n마우스y:%f\n회전각:%f\n크기조정:%f\n", current_p.x, current_p.y, Angle, move_rate);

	m_pRenderTarget->DrawText(
		szText,
		wcslen(szText),
		m_pTextFormat,
		D2D1::RectF(10.0f, 10.0f, 150.0f, 150.0f),
		m_pTextBrush
	);


	// 상단 회색 박스
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		rtSize.width / 2 - TOP_BOX_WIDTH * width_rate, 10 * height_rate,
		rtSize.width / 2 + TOP_BOX_WIDTH * width_rate, 10 * height_rate + TOP_BOX_WIDTH * width_rate);
	m_pRenderTarget->FillRectangle(&rectangle1, m_pTopBrush);

	D2D1_RECT_F rectangle[MAX_COUNT + 1];

	int size = vStudent.size();

	// 하단 파란색 박스
	if ((vStudent.size() != MAX_COUNT) && (!DeleteBox)) {
		rectangle[size + 1] = D2D1::RectF(
			rtSize.width / 2 - TOP_BOX_WIDTH * width_rate * 2, rtSize.height - ((size + 1) * TOP_BOX_WIDTH * width_rate),
			rtSize.width / 2 + TOP_BOX_WIDTH * width_rate * 2, rtSize.height - (size * TOP_BOX_WIDTH * width_rate));
		m_pRenderTarget->FillRectangle(&rectangle[size + 1], m_pFillBrush);
	}

	iter = vStudent.begin();

	/* vStudent에 있는 데이터 그리기 */
	for (int i = 0; i < size; i++) {

		rectangle[i] = D2D1::RectF(
			rtSize.width / 2 - TOP_BOX_WIDTH * width_rate * 2, rtSize.height - ((i)*TOP_BOX_WIDTH * width_rate),
			rtSize.width / 2 + TOP_BOX_WIDTH * width_rate * 2, rtSize.height - ((i + 1) * TOP_BOX_WIDTH * width_rate));

		WCHAR tempText[200];
		const char* multiByte = iter->name.c_str();
		TCHAR UniCode[15];
		memset(UniCode, 0, sizeof(UniCode));
		MultiByteToWideChar(CP_ACP, MB_COMPOSITE, multiByte, -1, UniCode, 15);

		// 상자 내부에 쓰여지는 글자 생성
		swprintf_s(tempText, L"%s  %d\n", UniCode, iter->score);
		iter++;																				// (2) 헷갈린 부분

		// 상자 그리기
		m_pRenderTarget->FillRectangle(&rectangle[i], m_pOriginalShapeBrush[i]);

		m_pRenderTarget->DrawText(
			tempText,
			wcslen(tempText),
			m_pTextFormat,
			D2D1::RectF(rtSize.width / 2 - TOP_BOX_WIDTH * width_rate * 2, rtSize.height - ((i)*TOP_BOX_WIDTH * width_rate),
				rtSize.width / 2 + TOP_BOX_WIDTH * width_rate * 2, rtSize.height - ((i + 1) * TOP_BOX_WIDTH * width_rate)),
			m_pTextBrush2
		);
	}

	if (!MoveBox) return;

	float xpos = current_p.x * width_rate;
	float ypos = current_p.y * height_rate;

	D2D1_RECT_F temprectangle = D2D1::RectF(
		current_p.x - ((current_p.y / 960) + 0.9583) * (TOP_BOX_WIDTH * width_rate), current_p.y + 10 * height_rate,
		current_p.x + ((current_p.y / 960) + 0.9583) * (TOP_BOX_WIDTH * width_rate), current_p.y + 10 * height_rate + TOP_BOX_WIDTH * width_rate);

	Angle = -0.5f * ypos;

	m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(Angle, current_p));
	m_pRenderTarget->FillRectangle(&temprectangle, m_pOriginalShapeBrush[size]);

	WCHAR movingText[200];

	const char* movingByte = tmpStudent.name.c_str();
	TCHAR movingCode[15];
	memset(movingCode, 0, sizeof(movingCode));
	MultiByteToWideChar(CP_ACP, MB_COMPOSITE, movingByte, -1, movingCode, 15);

	swprintf_s(movingText, L"%s  %d\n", movingCode, tmpStudent.score);

	m_pRenderTarget->DrawText(
		movingText,
		wcslen(movingText),
		m_pTextFormat,
		temprectangle,
		m_pTextBrush2
	);
}

void DemoApp::OnResize(UINT width, UINT height) {
	if (m_pRenderTarget)
		m_pRenderTarget->Resize(D2D1::SizeU(width, height));
}


LRESULT CALLBACK DemoApp::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		DemoApp* pDemoApp = (DemoApp*)pcs->lpCreateParams;

		::SetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA,
			PtrToUlong(pDemoApp)
		);

		return 1;
	}

	DemoApp* pDemoApp = reinterpret_cast<DemoApp*>(static_cast<LONG_PTR>(
		::GetWindowLongPtrW(
			hwnd,
			GWLP_USERDATA
		)));

	if (pDemoApp)
	{
		switch (message)
		{
		case WM_SIZE:
		{
			winWidth = LOWORD(lParam);
			winHeight = HIWORD(lParam);
			pDemoApp->OnResize(winWidth, winHeight);
		}
		return 0;

		case WM_DISPLAYCHANGE:
		{
			InvalidateRect(hwnd, NULL, FALSE);
		}
		return 0;

		case WM_LBUTTONDOWN:
		{
			current_p.x = LOWORD(lParam);
			current_p.y = HIWORD(lParam);


			int size = vStudent.size();

			float xpos = current_p.x * width_rate;
			float ypos = current_p.y * height_rate;

			if (!vStudent.empty()) {
				/* 맨 위에 있는 상자를 클릭했을 때 */
				if (xpos >= (rtSize.width / 2 - DOWN_BOX_WIDTH * width_rate) &&
					xpos <= rtSize.width / 2 + DOWN_BOX_WIDTH * width_rate &&
					ypos <= rtSize.height - (size - 1) * TOP_BOX_WIDTH * width_rate &&
					ypos >= rtSize.height - size * TOP_BOX_WIDTH * width_rate) {


					iter = vStudent.end() - 1;

					tmpStudent.name = iter->name;
					tmpStudent.score = iter->score;

					MoveBox = true; DeleteBox = true;
				}
			}

			/* 데이터 삽입 */
			if (xpos >= (rtSize.width / 2 - TOP_BOX_WIDTH * width_rate) &&
				xpos <= rtSize.width / 2 + TOP_BOX_WIDTH * width_rate &&
				ypos >= 10 * height_rate &&
				ypos <= 50 * height_rate) {

				if (vStudent.size() == MAX_COUNT) {
					::MessageBox(0, L"Size of the struct is 8. So you can't make it.", L"Out of Size", MB_OK);
					return 0;
				}
				else {
					int nameIndex = 0;

					// 이름 중복성 검사
					while (true) {
						nameIndex = rand() % STUDENT_NUM;
						if (!checkName[nameIndex]) break;
					}
					checkName[nameIndex] = true;

					tmpStudent.name = name[nameIndex];
					tmpStudent.score = rand() % 50 + 50;

					MoveBox = true; MakeBox = true;
				}
			}
		}
		break;

		case WM_MOUSEMOVE: {

			current_p.x = LOWORD(lParam);
			current_p.y = HIWORD(lParam);

			int size = vStudent.size();

			float xpos = current_p.x * width_rate;
			float ypos = current_p.y * height_rate;

			if (!MoveBox) return 0;

			if (MakeBox) {
				if (xpos >= (rtSize.width / 2 - DOWN_BOX_WIDTH * width_rate) &&
					xpos <= rtSize.width / 2 + DOWN_BOX_WIDTH * width_rate &&
					ypos <= rtSize.height - size * TOP_BOX_WIDTH * width_rate &&
					ypos >= rtSize.height - (size + 1) * TOP_BOX_WIDTH * width_rate) {

					vStudent.push_back(tmpStudent);
					stable_sort(vStudent.begin(), vStudent.end(), cmp);

					winHeight -= 100;

					MakeBox = false; MoveBox = false;
				}
			}

			else if (DeleteBox) {
				if (xpos >= (rtSize.width / 2 - TOP_BOX_WIDTH * width_rate) &&
					xpos <= rtSize.width / 2 + TOP_BOX_WIDTH * width_rate &&
					ypos >= 10 * height_rate &&
					ypos <= 50 * height_rate) {

					vStudent.pop_back();
					checkName[vStudent.size()] = false;

					winHeight += 100;

					MoveBox = false; DeleteBox = false;
				}
			}

			UINT tempHeight = winHeight;
			if (DeleteBox)
				tempHeight += 100;
			move_rate = TOP_BOX_WIDTH / 10 / ((tempHeight - 50) * height_rate) * ypos;

			InvalidateRect(hwnd, NULL, FALSE);
			break;
		}

		case WM_LBUTTONUP:
		{
			MakeBox = false;
			MoveBox = false;
			DeleteBox = false;
			Angle = 0;
			move_rate = 0;
			InvalidateRect(hwnd, NULL, FALSE);
		}
		break;

		case WM_RBUTTONUP:
		{
			::MessageBox(0, L"Please Click the Left Button :)", L"Error", MB_OK);
			return 0;
		}

		case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE) PostQuitMessage(0);
			else ::MessageBox(0, L"If you want to exit, please press the ESC.", L"Error", MB_ICONERROR);
			break;
		}

		case WM_PAINT:
		{
			pDemoApp->OnRender();
			ValidateRect(hwnd, NULL);
		}
		return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 1;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}