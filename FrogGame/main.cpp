#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 850

using namespace sf;

// 충돌여부 확인 (사각충돌)
bool isCollide(Sprite s1, Sprite s2);

int main(void)
{
	// 윈도우 창 생성
	RenderWindow app(VideoMode(WIDTH, HEIGHT), "FrogGame");
	app.setFramerateLimit(60);

	Texture t1, t2, t3, t4;
	t1.loadFromFile("images/background.jpg");	// 배경화면 이미지
	t2.loadFromFile("images/frog.png");			// 개구리 이미지
	t3.loadFromFile("images/wood.png");			// 발판 이미지
	t4.loadFromFile("images/fly.png");			// 장애물 이미지

	Sprite Background(t1), Frog(t2), Wood(t3), Fly(t4);
	Frog.setPosition(300, 400);

	// 통나무 랜덤 위치로 생성하기 getPosition radom
	Wood.setPosition( (rand()*(300-0)+300), (rand() * (400 - 0) + 400));
	// 0~100		0~300


	float dx = 5.0f, dy = 6.0f;	// (속도 조절 + 각도 조절)

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		// 개구리 위치(좌표)
		Vector2f b = Frog.getPosition();	// x, y좌표 값 받아냄		setPosition

		// 키보드에 따라 개구리가 움직임
		if (Keyboard::isKeyPressed(Keyboard::Right))
			Frog.move(9.0f, 0.0f);
		if (Keyboard::isKeyPressed(Keyboard::Left))
			Frog.move(-9.0f, 0.0f);

		float gravity= (-18);	// 중력
		float acc = 1;	// 가속도
		
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space))
		{

			while (b.y > 0)
			{
				gravity += (acc*=3);
				Frog.move(0.0f, gravity);
			}

			// 천천히 내려오게 하고 싶음 (끝까지 내려오게 해야함) -> 지금은 제자리

			// 내려가는 값이 계속 변해야함 (중력, 떨어지는 정도를 변수로 처리)
			// 값 변하는거 출력하면서 보기
		}

		if (b.x <= 0)    //왼쪽 벽 뚫지 못하게
		{
			Frog.move(9.0f, 0.0f);
		}
		if (b.x >= WIDTH - 100)    //오른쪽 벽 뚫지 못하게
		{
			Frog.move(-9.0f, 0.0f);
		}

		// 배경 위 아래로 스크롤	참고: https://oneday0012.tistory.com/16


		// 밑에 통나무를 만날 때 까지
		// 밑에 아무것도 없으면 엔딩 화면 띄우기


		app.clear();
		app.draw(Background);
		app.draw(Frog);
		app.draw(Wood);
		//app.draw(Fly);
		app.display();
	}

	return 0;
}
