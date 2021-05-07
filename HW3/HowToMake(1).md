#### 나는 사각형으로 돌아가는 원을 만들고 싶은데 어떻게 만들지?



```c++
if (SUCCEEDED(hr))
	{
		hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry1);

		if (SUCCEEDED(hr))
		{
			// 경로 기하에 조각들을 기록하기 위해서 기하 싱크를 얻음.
			hr = m_pPathGeometry1->Open(&pSimplifiedSink);

			if (SUCCEEDED(hr))
			{
				pSimplifiedSink->BeginFigure(
					D2D1::Point2F(0, 0),
					D2D1_FIGURE_BEGIN_FILLED
					);

				D2D1_POINT_2F points[3] = {
					D2D1::Point2F(200, 0),
					D2D1::Point2F(300, 200),
					D2D1::Point2F(100, 200)
					};

				pSimplifiedSink->AddLines(points, 3);

				pSimplifiedSink->EndFigure(D2D1_FIGURE_END_CLOSED);

				hr = pSimplifiedSink->Close();
			}
		}
		SAFE_RELEASE(pSimplifiedSink);
	}
```

이걸 쓰면 사다리꼴이 그려지니까 이걸 활용해보자.



```c++
HRESULT DemoApp::OnRender()
{
	HRESULT hr = CreateDeviceResources();

	if (SUCCEEDED(hr))
	{
		m_pRenderTarget->BeginDraw();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

		m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = m_pRenderTarget->GetSize();

		m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(500, 200));
		m_pRenderTarget->DrawGeometry(m_pPathGeometry1, m_pBlackBrush, 5);

		hr = m_pRenderTarget->EndDraw();
	}
```


```c++
m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(500, 200));
```

여기가 시작점



```c++
D2D1_POINT_2F points[3] = {
					D2D1::Point2F(200, 0),
					D2D1::Point2F(300, 200),
					D2D1::Point2F(100, 200)
					};
```

이게 점



```c++
pSimplifiedSink->AddLines(points, 3);
```

이걸 이으면?



![그림1](C:\Users\suhg3\OneDrive\바탕 화면\그림1.png)



---

![그림3](C:\Users\suhg3\OneDrive\바탕 화면\그림3.jpg)

내가 그리고 싶은 형태

특징

- 숫자가 10씩 커짐
- i가 3, 7, 11, ... 로 바뀌는 순간에 x = (-1)* y로 바뀜



< 코드 >

```c++
				int x(0), y(0);
				int base(10);
				bool isX = true;
				for (int i = 1; i < 20; i++) {
					
					D2D1_POINT_2F movedPoint = D2D1::Point2F(x, y);
					pSimplifiedSink->AddLine(movedPoint);
					if (isX) {
						x = base * i;
						if ((i-3)%4 == 0) x = (-1) * x;
						isX = false;
					}
					else {
						y = x;
						isX = true;
					}
				}

				pSimplifiedSink->EndFigure(D2D1_FIGURE_END_OPEN);
```



< 코드 설명 >

```
D2D1_POINT_2F movedPoint = D2D1::Point2F(x, y);
pSimplifiedSink->AddLine(movedPoint);
```

점을 하나 만들 때마다 선을 그을 거야.



```c++
					if (isX) {
						x = base * i;
						if ((i-3)%4 == 0) x = (-1) * x;
						isX = false;
					}
					else {
						y = x;
						isX = true;
					}
```

isX:  x차례

x 차례면 base * i (= 10, 20, 30, ...)을 해줘. 만약 x가 음수로 바뀌는 시점에는 (-1)을 곱해.

반대로 y 차례면 x의 값을 그대로 갖다 써.



< 결과 >

![그림2](C:\Users\suhg3\OneDrive\바탕 화면\그림2.png)

빙글빙글