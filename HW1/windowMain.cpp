#include "stdafx.h"
#include "student.h"

#define MAX_COUNT 8
#define STUDENT_NUM 10

using namespace std;

string name[STUDENT_NUM] = { "성은지", "이지연","이서현","조유진","허가현","김민주","유현아", "이동은", "소원임","박미나" };

vector<student> vStudent;

HWND MainWindowHandle = 0;

bool checkName[STUDENT_NUM] = { false, };

// 표시될 상자의 위치
int recx = 450; int recy = 400;
// 사용자가 클릭한 위치
int xpos = 0; int ypos = 0;

/* --------------------------------------------------------------------------- */

// (1) 진입점 함수
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	if (!InitWindowsApp(hInstance, nShowCmd))
	{
		::MessageBox(0, L"Init - Failed", L"Error", MB_OK);
		return 0;
	}
	return Run();
}

// (2) 응용 프로그램의 윈도우를 생성하는 함수
bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"Hello";
	if (!::RegisterClass(&wc)) {
		::MessageBox(0, L"RegisterClass - Failed", 0, 0);
		return false;
	}
	MainWindowHandle = ::CreateWindow(
		L"Hello", L"Hello", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, instanceHandle, 0);
	if (MainWindowHandle == 0) {
		::MessageBox(0, L"CreateWindow - Failed", 0, 0);
		return false;
	}
	::ShowWindow(MainWindowHandle, show);
	::UpdateWindow(MainWindowHandle);
	return true;
}

// (3) 창이 생성된 이후에 창으로 오는 메시지를 받아서 이를 윈도우 프로시저로 보내는 함수
int Run()
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	while (::GetMessage(&msg, 0, 0, 0)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}

// GID로 사각형 그리는 함수 (이 프로젝트에서는 사용되지 않음)
/* LRESULT OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;

	BeginPaint(hWnd, &ps);

	RECT rc; GetClientRect(hWnd, &rc);

	FillRect(ps.hdc, &rc, GetSysColorBrush(COLOR_HIGHLIGHT));

	HPEN hMyRedPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HGDIOBJ hOldPen = SelectObject(ps.hdc, hMyRedPen);

	Rectangle(ps.hdc, rc.left + 100, rc.top + 100, rc.right - 100, rc.bottom - 100);

	SelectObject(ps.hdc, hOldPen);
	DeleteObject(hMyRedPen);
	EndPaint(hWnd, &ps);
	return 0;
} */

// Win32 프로그램에서 printf()와 같은 역할을 하는 함수
void TRACE(LPCTSTR lpszFormat, ...) {
	TCHAR lpszBuffer[1024]; // 0x160
	va_list fmtList;
	va_start(fmtList, lpszFormat);
	_vstprintf(lpszBuffer, sizeof(lpszBuffer), lpszFormat, fmtList);
	va_end(fmtList);
	::OutputDebugString(lpszBuffer);
}

/* --------------------------------------------------------------------------- */

// string to wstring for TextOut
// TextOut에서는 wstring으로 변환한 후에 LPCWSTR로 다시 바꿔서 사용
wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

// 삭제할 영역을 흰색으로 칠하는 함수
LRESULT drawRectangleWhite(HWND windowHandle, HDC hdc) {
	COLORREF color = RGB(255, 255, 255);

	HBRUSH myBrush = CreateSolidBrush(color);
	HPEN myPen = CreatePen(PS_SOLID, 1, color);
	HGDIOBJ oldBrush = SelectObject(hdc, myBrush);
	HGDIOBJ oldPen = (HBRUSH)SelectObject(hdc, myPen);

	Rectangle(hdc, recx, recy, recx + 200, recy + 50);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(myBrush);
	DeleteObject(myPen);

	return 0;
}

// 학생을 한 명씩 추가할 때마다 직사각형을 그리는 함수
LRESULT drawRectangle(HWND windowHandle, HDC hdc)
{
	COLORREF color = NULL;

	int colorNum = 0;

	for (vector<student>::iterator iterDraw = vStudent.begin(); iterDraw != vStudent.end(); iterDraw++) {

		/* 학생의 이름과 성적 */
		wstring tmpName = s2ws(iterDraw->name);
		wstring tmpScore = s2ws(to_string(iterDraw->score));

		LPCWSTR name = tmpName.c_str();
		LPCWSTR score = tmpScore.c_str();

		/* 직사각형에 표시될 색 */
		switch (colorNum) {

		case 0:
			color = RGB(255, 87, 127);
			break;
		case 1:
			color = RGB(255, 136, 75);
			break;
		case 2:
			color = RGB(255, 199, 100);
			break;
		case 3:
			color = RGB(205, 255, 252);
			break;
		case 4:
			color = RGB(74, 71, 163);
			break;
		case 5:
			color = RGB(74, 71, 0);
			break;
		case 6:
			color = RGB(112, 159, 176);
			break;
		case 7:
			color = RGB(167, 197, 235);
			break;
		default:
			break;
		}

		/* 배경과 테두리 색을 일치시킴 */
		HBRUSH myBrush = CreateSolidBrush(color);
		HPEN myPen = CreatePen(PS_SOLID, 1, color);
		HGDIOBJ oldBrush = SelectObject(hdc, myBrush);
		HGDIOBJ oldPen = (HBRUSH)SelectObject(hdc, myPen);

		/* 직사각형의 위치 */
		Rectangle(hdc, recx, recy, recx + 200, recy + 50);

		/* 직사각형 안에 이름과 점수 표기 */
		TextOut(hdc, recx + 20, recy + 10, name, sizeof(name) - sizeof(' '));
		TextOut(hdc, recx + 20, recy + 30, score, to_string(iterDraw->score).length());

		/* 다음에 나타나는 직사각형의 위치 */
		recy -= 50;

		SelectObject(hdc, oldBrush);
		SelectObject(hdc, oldPen);
		DeleteObject(myBrush);
		DeleteObject(myPen);

		colorNum++;
	}
	return 0;
}

// 학생들의 점수가 낮은 순서대로 정렬
bool cmp(const student& p1, const student& p2) {
	if (p1.score < p2.score) return true;
	else return false;
}

// (4) 윈도우 프로시저 함수
LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg) {

		/* 왼쪽 마우스 클릭 */
	case WM_LBUTTONDOWN:
		/* 클릭된 위치 저장 */
		xpos = GET_X_LPARAM(lParam);
		ypos = GET_Y_LPARAM(lParam);

		/* 저장된 데이터가 없을 때, 새로운 학생 데이터 생성 */
		if (vStudent.size() == 0) {
			int nameIndex = 0;

			student tmpStudent;
			srand((unsigned)time(NULL));
			nameIndex = rand() % STUDENT_NUM;

			// student[]에 저장된 이름 중 하나
			tmpStudent.name = name[nameIndex];
			// 점수의 범위는 50~99점
			tmpStudent.score = rand() % 50 + 50;

			// 이 이름이 사용되었음을 표기
			checkName[nameIndex] = true;
			// 벡터에 추가
			vStudent.push_back(tmpStudent);

			InvalidateRect(windowHandle, NULL, FALSE);
			::MessageBox(0, L"You made a box.", L"Making", MB_OK);
		}
		/* 저장된 데이터가 있을 때 */
		else {
			// 맨 위에 있는 박스 클릭 시 삭제
			if (((recx <= xpos) && (xpos <= recx + 200) && (recy + 50 <= ypos) && (ypos <= recy + 100)) && (recy <= 600)) {
				RECT temp;

				temp.left = recx;
				temp.top = recy + 50;
				temp.right = recx + 200;
				temp.bottom = recy + 100;

				recy += 50;

				// 벡터에서 점수가 제일 높은 학생 삭제
				vStudent.erase(vStudent.end() - 1);
				// 더 이상 이름이 쓰이지 않으므로 false로 표기
				checkName[vStudent.size()] = false;

				InvalidateRect(windowHandle, &temp, TRUE);

				// 삭제된 영역을 흰색으로 표시
				hdc = BeginPaint(windowHandle, &ps);
				drawRectangleWhite(windowHandle, hdc);
				EndPaint(windowHandle, &ps);
				::MessageBox(0, L"Delete Box", L"Delete", MB_OK);
			}
			// 이 외 영역 클릭 시 추가
			else {
				/* 벡터의 최대 사이즈는 8이므로 더 이상 추가 불가 */
				if (vStudent.size() == 8) {
					::MessageBox(0, L"Size of the struct is 8. So you can't make it.", L"Out of Size", MB_OK);
					return 0;
				}
				/* 추가 */
				else {
					student tmpStudent;
					srand((unsigned)time(NULL));

					int nameIndex = 0;

					// 이름 중복성 검사
					while (true) {
						nameIndex = rand() % STUDENT_NUM;
						if (!checkName[nameIndex]) break;
					}
					checkName[nameIndex] = true;

					tmpStudent.name = name[nameIndex];


					// 점수는 50~99점 사이
					tmpStudent.score = rand() % 50 + 50;

					vStudent.push_back(tmpStudent);;

					// y좌표 초기화하여 처음부터 그릴 수 있도록 설정
					recy = 400;

					InvalidateRect(windowHandle, NULL, TRUE);

					::MessageBox(0, L"You made a box.", L"Make Box", MB_OK);
				}
			}
		}
		return 0;
		/* 화면에 그리기 */
	case WM_PAINT:
		hdc = BeginPaint(windowHandle, &ps);
		stable_sort(vStudent.begin(), vStudent.end(), cmp);
		drawRectangle(windowHandle, hdc);
		EndPaint(windowHandle, &ps);
		break;
	case WM_RBUTTONDOWN:
		::MessageBox(0, L"You should click the Left Button.", L"Error", MB_ICONERROR);
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(MainWindowHandle);
		else ::MessageBox(0, L"If you want to exit, please press the ESC.", L"Error", MB_ICONERROR);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(windowHandle, msg, wParam, lParam);
}