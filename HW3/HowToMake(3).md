#### 나는 컴퓨터랑 경쟁하는 게임을 만들 거야

일단 각지게 만들어서 돌아가는 건 잘 되네. 두 개를 만들어볼까.
<br> <br>


```c++
m_hwnd = CreateWindow( L"D2DDemoApp", L"HW3_201801567(성은지)", WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT, CW_USEDEFAULT, 1024, 512, NULL, NULL, HINST_THISCOMPONENT, this);
```

일단 윈도우 가로를 넓히고
<br> <br>


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
<br> <br>


하나 더 만들어보자.

![그림6](https://user-images.githubusercontent.com/64337152/117537486-30e87880-b03c-11eb-958b-550a1351541f.png)

<br> <br>


삼각형 추가

![그림7](https://user-images.githubusercontent.com/64337152/117537489-35149600-b03c-11eb-8429-4db4d2638ec1.gif)


오른쪽에 있는 삼각형은 사람이 SPACE BAR를 누른 만큼 움직이게 할 것이다.

**경쟁 구도!**
<br> <br>
```c++
case WM_KEYDOWN:
			{
				if (wParam == VK_ESCAPE) PostQuitMessage(0);
				else if (wParam == VK_SPACE) {
					spaceBar += 0.05;
					pDemoApp->m_Animation2.SetDuration(USER_VELOCITY - spaceBar);
				}
```

SetDuration() 안에 들어가는 변수가 작아질수록 이동속도가 빨라진다.
<br> <br>


![그림8](https://user-images.githubusercontent.com/64337152/117537492-38a81d00-b03c-11eb-982b-f89659423d72.gif)


이건 원래 속도
<br> <br>


![그림9](https://user-images.githubusercontent.com/64337152/117537494-3c3ba400-b03c-11eb-90d0-01693dff28ad.gif)


스페이스바를 연타하면 어느 정도 속도가 비슷해짐

텍스트를 넣어서 속도가 점점 빨라지고 있다는 걸 보여줘야겠다.

나중엔 UI도 넣어야지.
<br> <br>


삼각형 대신 비행기를 넣어볼까?

불투명 마스크 적용이 잘 되지 않아서 일단 패스~
