#include <windows.h>

#define BUFSIZE  256

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // 메시지 처리 함수
void MyTextPrint(HDC, LPRECT, LPSTR);           // 글 출력 함수 (여러 줄 출력)

int APIENTRY WinMain(HINSTANCE hInstance,        // 프로그램의 인스턴스 핸들

    HINSTANCE hPrevInstance,  // 16bit 운영체제와의 호환을 위해서 존재

    LPSTR lpCmdLine,              // 명령행으로 입력된 프로그램 인수 

    int nShowCmd                    // 프로그램이 실행될 상태 (최소화, 최대화 등등)
)
{
    static char lpszClass[] = "KeyPrint";
    HWND  hwnd;
    MSG   message;
    WNDCLASS wndclass;

    // 윈도우 클래스 세팅
    wndclass.cbClsExtra = 0;                                                           // 예약영역 (사용X : 0 )
    wndclass.cbWndExtra = 0;                                                          // 예약영역 (사용X : 0 )
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 배경색
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);              // 커서
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // 아이콘
    wndclass.hInstance = hInstance;                                               // 윈도우 클래스 핸들 등록
    wndclass.lpfnWndProc = WndProc;                                                // 메시지 처리 함수 등록
    wndclass.lpszClassName = TEXT(lpszClass);                                   // 클래스 이름
    wndclass.lpszMenuName = NULL;                                                    // 메뉴
    wndclass.style = CS_VREDRAW | CS_HREDRAW;               // 윈도우 스타일 지정

                                             // CS_VREDRAW | CS_HREDRAW : 수직or수평으로 변하면 다시 그림

    // 윈도우 등록

    RegisterClass(&wndclass);

    hwnd = CreateWindow(lpszClass, // 생성하고자 하는 윈도우의 클래스를 지정하는 문자열
        lpszClass, // 윈도우 타이틀바에 나타날 문자열
        WS_OVERLAPPEDWINDOW, // 윈도우 형태 - 일반적인 윈도우 창
        CW_USEDEFAULT, // x축 위치
        CW_USEDEFAULT, // y축 위치
        CW_USEDEFAULT, // 이동할 x축 픽셀
        CW_USEDEFAULT, // 이동할 y축 픽셀
        NULL, // 부모 윈도우 핸들 (MDI 등) - 여기서는 없으므로..
        NULL, // 윈도우에서 사용할 메뉴의 핸들 지정
        hInstance, // 윈도우 핸들 (프로그램의 핸들)
        NULL // 여러개의 윈도우를 만들 때 각 윈도우에 고유의 파라미터를 전달할

                 // 특수한 용도로 쓰는 인자 - 여기서는 사용 X

                // (lpvParam = CREATESTRUCT 구조체의 번지) 
    );

    // 윈도우 출력

    ShowWindow(hwnd, nShowCmd);

    // 메시지 루프
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message); // WM_??? 메시지를 생성
        DispatchMessage(&message); // 메시지큐에서 받은 메시지를 메시지 처리 함수로 전달
    }

    return (int)message.wParam; // 종료값을 OS에 알리는 용도
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
    HDC                 hdc;
    PAINTSTRUCT  ps;
    RECT               rect = { 100, 100, 500, 450 }; // 글을 쓸 공간 지정


    // 지역변수는 메시지가 발생할 때마다 초기화되므로 값을 계속 유지하기 위해서 static 사용
    static TCHAR str[BUFSIZE];
    int                str_len; // 문자열의 길이를 저장

    switch (message)
    {
    case WM_CHAR:  // 키보드 입력

        str_len = lstrlen(str);

        if ((TCHAR)wp == '\b') // 백 스페이스일 경우
            memmove(str + (str_len - 1), str + str_len, sizeof(TCHAR));
        else
        {
            // WM_CHAR 메시지는 입력된 문자를 wParam으로 전달한다.
            str[str_len] = (TCHAR)wp;
            str[str_len + 1] = 0;
        }

        // 강제로 WM_PAINT 메시지를 발생시킴

        // (3번째 인자: 화면 지우기 옵션 [TRUE:지우기o, FALSE:지우기x])
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_PAINT: // 화면 변화 발생시 - 다시 그림

        hdc = BeginPaint(hwnd, &ps); // DC Handle 얻기
        MyTextPrint(hdc, &rect, str);
        EndPaint(hwnd, &ps); // DC Handle 해제
        return 0;

    case WM_DESTROY: PostQuitMessage(0); return 0;  // 종료



    default: return DefWindowProc(hwnd, message, wp, lp); // 정의된 이외의 메시지는 디폴트 처리
    }
}



void MyTextPrint(HDC hdc, LPRECT rect, LPSTR str) // 글 출력 함수 (여러 줄 출력)
{
    DrawText(hdc, str, -1, rect, DT_CENTER | DT_VCENTER);
}