#### 나는 각지게 돌아가는 걸 만들고 싶은데 어떻게 만들지?



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
<br> <br>


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
<br> <br>


```c++
D2D1_POINT_2F points[3] = {
		D2D1::Point2F(200, 0),
		D2D1::Point2F(300, 200),
		D2D1::Point2F(100, 200)
};
```

이게 점
<br> <br>


```c++
pSimplifiedSink->AddLines(points, 3);
```

이걸 이으면?
<br> <br>


![그림1](https://user-images.githubusercontent.com/64337152/117447241-34b2c700-af78-11eb-9b2a-211b75637451.png)




---

![그림3](https://user-images.githubusercontent.com/64337152/117447216-2c5a8c00-af78-11eb-93fe-876a98855d70.jpg)

내가 그리고 싶은 형태
<br> <br>
특징

- 숫자가 10씩 커짐
- i가 3, 7, 11, ... 로 바뀌는 순간에 x = (-1)* y로 바뀜
<br> <br>


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
<br> <br>


< 코드 설명 >

```
D2D1_POINT_2F movedPoint = D2D1::Point2F(x, y);
pSimplifiedSink->AddLine(movedPoint);
```
점을 하나 만들 때마다 선을 그을 거야.

<br> <br>

```
pSimplifiedSink->EndFigure(D2D1_FIGURE_END_OPEN);
```
시작점과 끝점의 연결은 필요없기 때문에 OPEN으로 변경
<br> <br>

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

<br> <br>

< 결과 >

![그림2](https://user-images.githubusercontent.com/64337152/117447264-3a101180-af78-11eb-92b1-f8c1d89cbda4.png)


빙글빙글
