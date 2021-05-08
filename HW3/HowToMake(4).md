### 이미지 넣기

비트맵을 넣으려고 하는데 계속 에러가 난다. 지금 몇 시간을 쓰는 거여

<br> <br>

resource.rc에서 계속 에러나는 중.

![image-20210508213044936](C:\Users\suhg3\AppData\Roaming\Typora\typora-user-images\image-20210508213044936.png)

```c++
#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_KOR)
LANGUAGE LANG_KOREAN, SUBLANG_DEFAULT
// LANGUAGE 18, 1
#pragma code_page(949)
```

```c++
// IMAGE
//

FERN                    IMAGE                   "Fern.png"

BITMAPMASK              IMAGE                   "BitmapMask.png"
```

```c++
// PNG
//

IDB_PNG1                PNG                     "Tri.png"

IDB_PNG2                PNG                     "TriMask.png"
```

저장되어 있다고 적어두고 인식을 못하는 이유가 대체 뭐냐... 결국 이미지를 비트맵으로 변환했음.

또 에러났음. 대체 뭐가 문제냐..

<br>

**해결 방법**

1. 프로젝트 폴더에 사진 파일을 넣는다.
2. 리소스 파일을 만든다.
3. 리소스 뷰에서 리소스 추가를 누른다.
4. 사진을 넣는다.

![image-20210508221820071](C:\Users\suhg3\AppData\Roaming\Typora\typora-user-images\image-20210508221820071.png)

![image-20210508221910738](C:\Users\suhg3\AppData\Roaming\Typora\typora-user-images\image-20210508221910738.png)

![image-20210508221925786](C:\Users\suhg3\AppData\Roaming\Typora\typora-user-images\image-20210508221925786.png)

![image-20210508222002156](C:\Users\suhg3\AppData\Roaming\Typora\typora-user-images\image-20210508222002156.png)