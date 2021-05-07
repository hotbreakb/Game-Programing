## 노란 삼각형이 왜 빨간선을 따라갈까?



```c++
HRESULT DemoApp::OnRender()
{
	HRESULT hr;

	hr = CreateDeviceResources();
	if (SUCCEEDED(hr))
	{
		D2D1_POINT_2F point;
		D2D1_POINT_2F tangent;

		D2D1_MATRIX_3X2_F triangleMatrix;
		D2D1_SIZE_F rtSize = m_pRT->GetSize();
		float minWidthHeightScale = min(rtSize.width, rtSize.height) / 512;

		D2D1::Matrix3x2F scale = D2D1::Matrix3x2F::Scale(minWidthHeightScale, minWidthHeightScale);

		D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(rtSize.width / 2, rtSize.height / 2);

		// 그리기를 준비함.
		m_pRT->BeginDraw();

		// 변환을 항등행렬로 리셋함.
		m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());

		// 렌더타겟을 클리어함.
		m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		// 이동 동선 기하 경로가 화면 중심에 그려지도록 함.
		m_pRT->SetTransform(scale * translation);

		// 이동 동선을 붉은색으로 그림.
		m_pRT->DrawGeometry(m_pPathGeometry, m_pRedBrush);

		static float anim_time = 0.0f;

		float length = m_Animation.GetValue(anim_time);

		// 현재 시간에 해당하는 기하 길이에 일치하는 이동 동선 상의 지점을 얻음.
		m_pPathGeometry->ComputePointAtLength(length, NULL, &point, &tangent);

		// 삼각형의 방향을 조절하여 이동 동선을 따라가는 방향이 되도록 함.
		triangleMatrix = D2D1::Matrix3x2F(
			tangent.x, tangent.y,
			-tangent.y, tangent.x,
			point.x, point.y);

		m_pRT->SetTransform(triangleMatrix * scale * translation);

		// 삼각형을 노란색으로 그림.
		m_pRT->FillGeometry(m_pObjectGeometry, m_pYellowBrush);

		// 그리기 연산들을 제출함.
		hr = m_pRT->EndDraw();
```



여기서 일부를 떼어서 보자.
<br> <br>


```
m_pPathGeometry->ComputePointAtLength(length, NULL, &point, &tangent);
```

length = m_Animation.GetValue(0.0);

​	→ 바로 갈 수 있는 길로 가자!

​	m_pPathGeometry 이게 뭘까?

<br> <br>

```
m_pRT->DrawGeometry(m_pPathGeometry, m_pRedBrush);
```

아, 빨간 선이네. 근데 빨간 선에 저 동그란 게 어떻게 저장되어 있지?

<br> <br>

이 함수에 보니까 이게(m_pPathGeometry ) 있네.

```c++
HRESULT DemoApp::CreateDeviceIndependentResources()
{
	HRESULT hr;
	ID2D1GeometrySink* pSink = NULL;

	// D2D 팩토리를 생성함.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	// 나선형 모양의 경로 기하를 생성함.
	if (SUCCEEDED(hr))
	{
		hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pPathGeometry->Open(&pSink);
	}
	if (SUCCEEDED(hr))
	{
		D2D1_POINT_2F currentLocation = { 0, 0 };

		pSink->BeginFigure(currentLocation, D2D1_FIGURE_BEGIN_FILLED);

		D2D1_POINT_2F locDelta = { 2, 2 };
		float radius = 3;

		for (UINT i = 0; i < 30; ++i)
		{
			currentLocation.x += radius * locDelta.x;
			currentLocation.y += radius * locDelta.y;

			pSink->AddArc(
				D2D1::ArcSegment(
					currentLocation,
					D2D1::SizeF(2 * radius, 2 * radius), // radiusx/y size
					0.0f, // rotation angle
					D2D1_SWEEP_DIRECTION_CLOCKWISE,
					D2D1_ARC_SIZE_SMALL
				)
			);

			locDelta = D2D1::Point2F(-locDelta.y, locDelta.x);

			radius += 3;
		}

		pSink->EndFigure(D2D1_FIGURE_END_OPEN);

		hr = pSink->Close();
	}

	SAFE_RELEASE(pSink);
```


<br> <br>
```
hr = m_pD2DFactory->CreatePathGeometry(&m_pPathGeometry);
```

오, 만들었어!

<br> <br>

```
hr = m_pPathGeometry->Open(&pSink);
```

수정을 하네.

<br> <br>

```
		D2D1_POINT_2F currentLocation = { 0, 0 };

		pSink->BeginFigure(currentLocation, D2D1_FIGURE_BEGIN_FILLED);

		D2D1_POINT_2F locDelta = { 2, 2 };
		float radius = 3;

		for (UINT i = 0; i < 30; ++i)
		{
			currentLocation.x += radius * locDelta.x;
			currentLocation.y += radius * locDelta.y;

			pSink->AddArc(
				D2D1::ArcSegment(
					currentLocation,
					D2D1::SizeF(2 * radius, 2 * radius), // radiusx/y size
					0.0f, // rotation angle
					D2D1_SWEEP_DIRECTION_CLOCKWISE,
					D2D1_ARC_SIZE_SMALL
				)
			);

			locDelta = D2D1::Point2F(-locDelta.y, locDelta.x);

			radius += 3;
		}
```

이게 동그랗게 그리는 거 같은데?
<br> <br>


```
			pSink->AddArc(
				D2D1::ArcSegment(
					currentLocation,
					D2D1::SizeF(2 * radius, 2 * radius),
					0.0f,
					D2D1_SWEEP_DIRECTION_CLOCKWISE,
					D2D1_ARC_SIZE_SMALL
				)
			);
```
**ArcSegment()** 가 두 점 사이에 타원을 그리는 함수야. 그래서 빨간선이 동그란 거지.

currentLocation을 기준으로 D2D1::SizeF(2 * radius, 2 * radius)를 반지름으로 그리는 거지.

<br> <br>

0.0f: 값을 바꿔봤는데 바뀌는 게 없어..

D2D1_SWEEP_DIRECTION_CLOCKWISE : 시계 방향

D2D1_ARC_SIZE_SMALL: An arc's sweep should be 180 degrees or less. (호의 sweep이 뭐야?)



