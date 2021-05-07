#### 나는 컴퓨터랑 경쟁하는 게임을 만들 거야

일단 각지게 만들어서 돌아가는 건 잘 되네. 두 개를 만들어볼까.



```c++
m_hwnd = CreateWindow( L"D2DDemoApp", L"HW3_201801567(성은지)", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 512, NULL, NULL, HINST_THISCOMPONENT, this);
```

일단 윈도우 가로를 넓히고



```c++
HRESULT DemoApp::OnRender()
{
	...
        
    D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(rtSize.width / 4, rtSize.height / 2);
    
    ...

	// 이동 동선 기하 경로가 화면 중심에서 비교적 왼쪽에 그려지도록 함.
	m_pRT->SetTransform(scale * translation);
```

원래 있던 도형을 왼쪽으로 옮기자.



하나 더 만들어보자.

![그림6](C:\Users\suhg3\OneDrive\바탕 화면\그림6.png)



삼각형 추가

![그림7](C:\Users\suhg3\OneDrive\바탕 화면\그림7.gif)



오른쪽에 있는 삼각형은 사람이 SPACE BAR를 누른 만큼 움직이게 할 것이다.

**경쟁 구도!**



