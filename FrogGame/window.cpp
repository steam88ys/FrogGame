#include <windows.h>

#define BUFSIZE  256

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // �޽��� ó�� �Լ�
void MyTextPrint(HDC, LPRECT, LPSTR);           // �� ��� �Լ� (���� �� ���)

int APIENTRY WinMain(HINSTANCE hInstance,        // ���α׷��� �ν��Ͻ� �ڵ�

    HINSTANCE hPrevInstance,  // 16bit �ü������ ȣȯ�� ���ؼ� ����

    LPSTR lpCmdLine,              // ��������� �Էµ� ���α׷� �μ� 

    int nShowCmd                    // ���α׷��� ����� ���� (�ּ�ȭ, �ִ�ȭ ���)
)
{
    static char lpszClass[] = "KeyPrint";
    HWND  hwnd;
    MSG   message;
    WNDCLASS wndclass;

    // ������ Ŭ���� ����
    wndclass.cbClsExtra = 0;                                                           // ���࿵�� (���X : 0 )
    wndclass.cbWndExtra = 0;                                                          // ���࿵�� (���X : 0 )
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ����
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);              // Ŀ��
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);          // ������
    wndclass.hInstance = hInstance;                                               // ������ Ŭ���� �ڵ� ���
    wndclass.lpfnWndProc = WndProc;                                                // �޽��� ó�� �Լ� ���
    wndclass.lpszClassName = TEXT(lpszClass);                                   // Ŭ���� �̸�
    wndclass.lpszMenuName = NULL;                                                    // �޴�
    wndclass.style = CS_VREDRAW | CS_HREDRAW;               // ������ ��Ÿ�� ����

                                             // CS_VREDRAW | CS_HREDRAW : ����or�������� ���ϸ� �ٽ� �׸�

    // ������ ���

    RegisterClass(&wndclass);

    hwnd = CreateWindow(lpszClass, // �����ϰ��� �ϴ� �������� Ŭ������ �����ϴ� ���ڿ�
        lpszClass, // ������ Ÿ��Ʋ�ٿ� ��Ÿ�� ���ڿ�
        WS_OVERLAPPEDWINDOW, // ������ ���� - �Ϲ����� ������ â
        CW_USEDEFAULT, // x�� ��ġ
        CW_USEDEFAULT, // y�� ��ġ
        CW_USEDEFAULT, // �̵��� x�� �ȼ�
        CW_USEDEFAULT, // �̵��� y�� �ȼ�
        NULL, // �θ� ������ �ڵ� (MDI ��) - ���⼭�� �����Ƿ�..
        NULL, // �����쿡�� ����� �޴��� �ڵ� ����
        hInstance, // ������ �ڵ� (���α׷��� �ڵ�)
        NULL // �������� �����츦 ���� �� �� �����쿡 ������ �Ķ���͸� ������

                 // Ư���� �뵵�� ���� ���� - ���⼭�� ��� X

                // (lpvParam = CREATESTRUCT ����ü�� ����) 
    );

    // ������ ���

    ShowWindow(hwnd, nShowCmd);

    // �޽��� ����
    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message); // WM_??? �޽����� ����
        DispatchMessage(&message); // �޽���ť���� ���� �޽����� �޽��� ó�� �Լ��� ����
    }

    return (int)message.wParam; // ���ᰪ�� OS�� �˸��� �뵵
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
    HDC                 hdc;
    PAINTSTRUCT  ps;
    RECT               rect = { 100, 100, 500, 450 }; // ���� �� ���� ����


    // ���������� �޽����� �߻��� ������ �ʱ�ȭ�ǹǷ� ���� ��� �����ϱ� ���ؼ� static ���
    static TCHAR str[BUFSIZE];
    int                str_len; // ���ڿ��� ���̸� ����

    switch (message)
    {
    case WM_CHAR:  // Ű���� �Է�

        str_len = lstrlen(str);

        if ((TCHAR)wp == '\b') // �� �����̽��� ���
            memmove(str + (str_len - 1), str + str_len, sizeof(TCHAR));
        else
        {
            // WM_CHAR �޽����� �Էµ� ���ڸ� wParam���� �����Ѵ�.
            str[str_len] = (TCHAR)wp;
            str[str_len + 1] = 0;
        }

        // ������ WM_PAINT �޽����� �߻���Ŵ

        // (3��° ����: ȭ�� ����� �ɼ� [TRUE:�����o, FALSE:�����x])
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_PAINT: // ȭ�� ��ȭ �߻��� - �ٽ� �׸�

        hdc = BeginPaint(hwnd, &ps); // DC Handle ���
        MyTextPrint(hdc, &rect, str);
        EndPaint(hwnd, &ps); // DC Handle ����
        return 0;

    case WM_DESTROY: PostQuitMessage(0); return 0;  // ����



    default: return DefWindowProc(hwnd, message, wp, lp); // ���ǵ� �̿��� �޽����� ����Ʈ ó��
    }
}



void MyTextPrint(HDC hdc, LPRECT rect, LPSTR str) // �� ��� �Լ� (���� �� ���)
{
    DrawText(hdc, str, -1, rect, DT_CENTER | DT_VCENTER);
}