#include <SFML/Graphics.hpp>
#include <string>
#include <exception>

#include <time.h>
#include <Windows.h>

using namespace sf;
using namespace std;

int main(void)
{
    // main window 생성
    RenderWindow window(VideoMode(400, 300), "SFML window");

    // 쓸 폰트 생성
    Font font;
    if (!font.loadFromFile(R"(E:\CPP\FrogGame\font\DS-DIGIB.TTF)"))
        throw std::exception("font error");

    time_t start, end;
    double i, j, sum = 0;
    String result;

    start = time(NULL);	// 시간 측정 시작
    for (i = 0; i < 100000; i++)
    {
        for (j = 0; j < 10000; j++)
        {
            sum += i * j;
        }
    }
    end = time(NULL);	// 시간 측정 끝
    result = to_string(end - start);


    // 화면에 쓸 내용
    Text text(result, font, 50);   // 변수에 들어있는 내용 쓰는 법
    text.setFillColor(Color::White); //글씨 색깔

    // 윈도우 루프 시작
    while (window.isOpen())
    {
        Event event;

        // 종료 여부 체크
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        // 화면 청소
        window.clear();

        // 화면에 글씨 쓰기
        window.draw(text);

        // 띄우기
        window.display();
    }
}

/*
#include <SFML/Graphics.hpp>
#include <string>
#include <exception>

using namespace sf;

int main(void)
{
    // main window 생성
    RenderWindow window(VideoMode(400, 300), "SFML window");

    // 쓸 폰트 생성
    Font font;
    if (!font.loadFromFile(R"(E:\CPP\FrogGame\font\DS-DIGIB.TTF)"))
        throw std::exception("font error");


    // 화면에 쓸 내용
    Text text("Frog Game", font, 50);   // 변수에 들어있는 내용 쓰는 법
    text.setFillColor(Color::White); //글씨 색깔

    // 윈도우 루프 시작
    while (window.isOpen())
    {
        Event event;

        // 종료 여부 체크
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        // 화면 청소
        window.clear();

        // 화면에 글씨 쓰기
        window.draw(text);

        // 띄우기
        window.display();
    }
}
*/
