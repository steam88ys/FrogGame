#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <time.h>
#include <Windows.h>

#define WIDTH 520
#define HEIGHT 450

using namespace sf;

// 충돌여부 확인 (사각충돌)
bool isCollide(Sprite s1, Sprite s2);

int main(void)
{
    RenderWindow app(VideoMode(1000, 700), "FrogGame");
    app.setFramerateLimit(60);

    /*Texture t1, t2, t3, t4;
    * t1.loadFromFile("images/background.jpg");
    t2.loadFromFile("images/frog.png");

    Sprite Background(t1), Frog(t2);
    Frog.setPosition(300, 400);*/

    time_t start, end;
    double result;
    int i, j;
    int sum = 0;

    start = time(NULL);	// 시간 측정 시작
    for (i = 0; i < 100000; i++)
    {
        for (j = 0; j < 10000; j++)
        {
            sum += i * j;
        }
    }
    end = time(NULL);	// 시간 측정 끝
    result = (double)(end - start);
    printf("%f", result);	// 결과 출력

    // 윈도우 창에 시간 띄우기
    // 텍스트 관련 객체
    // TextOut(); 숫자로 변환...

    // 시간 나눠서 날짜 표시하기

    // 통나무 랜덤 위치로 생성하기

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

        // 개구리 움직이기
        Frog.move(dx, 0);
        for (int i = 0; i < n; i++) {
            if (isCollide(Frog, 통나무[i]))
            {
                dx = -dx;
                // 통나무 사라지게 하기
                통나무[i].setPosition(-300, 0);
            }
        }
		Frog.move(0, dy);
		for (int i = 0; i < n; i++) {
			if (isCollide(sBall, block[i]))
			{
				dy = -dy;
                // 통나무 사라지게 하기
				block[i].setPosition(-300, 0);
			}
		}

		// 개구리 위치(좌표)
		Vector2f b = Frog.getPosition();

		// 키보드에 따라 개구리가 움직임
		if (Keyboard::isKeyPressed(Keyboard::Right))
			Frog.move(9.0f, 0.0f);
		if (Keyboard::isKeyPressed(Keyboard::Left))
			Frog.move(-9.0f, 0.0f);

		app.clear();
		app.draw(Background);
		app.draw(Frog);
		app.display();
	}

    return 0;
}