#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <time.h>
#include <Windows.h>
#include <iostream>

#define WIDTH 800
#define HEIGHT 850

using namespace sf;

// �浹���� Ȯ�� (�簢�浹)
bool isCollide(Sprite s1, Sprite s2);

int main(void)
{
	// ������ â ����
	RenderWindow app(VideoMode(WIDTH, HEIGHT), "FrogGame");
	app.setFramerateLimit(60);

	Texture bg1, bg2, t2, t3, t4;
	bg1.loadFromFile("images/background.jpg");	// ���ȭ�� �̹���1
	bg2.loadFromFile("images/background.jpg");	// ���ȭ�� �̹���2
	t2.loadFromFile("images/frog.png");			// ������ �̹���
	t3.loadFromFile("images/wood.png");			// ���� �̹���
	t4.loadFromFile("images/fly.png");			// ��ֹ� �̹���

	Sprite Background(bg1), Frog(t2), Wood(t3), Fly(t4);
	Frog.setPosition(300, 400);
	Wood.setPosition(300, 400);
	Fly.setPosition(300, 400);

	// �볪�� ���� ��ġ�� �����ϱ� getPosition radom

	float dx = 5.0f, dy = 6.0f;	// (�ӵ� ���� + ���� ����)

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

		// ������ ��ġ(��ǥ)
		Vector2f b = Frog.getPosition();	// x, y��ǥ �� �޾Ƴ�		setPosition

		// Ű���忡 ���� �������� ������
		if (Keyboard::isKeyPressed(Keyboard::Right))
			Frog.move(9.0f, 0.0f);
		if (Keyboard::isKeyPressed(Keyboard::Left))
			Frog.move(-9.0f, 0.0f);
		if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Space))
		{
			Frog.move(0.0f, -9.0f);
			// õõ�� �������� �ϰ� ���� (������ �������� �ؾ���) -> ������ ���ڸ�
			Sleep(500);
			while (b.y > 0)	// ������ �������� �ʾ��� ���� �ݺ�
			{
				Frog.move(0.0f, 18.0f);		// ������ ������
				Sleep(500);
			}
		}

		if (b.x <= 0)    //���� �� ���� ���ϰ�
		{
			Frog.move(9.0f, 0.0f);
		}
		if (b.x >= WIDTH - 100)    //������ �� ���� ���ϰ�
		{
			Frog.move(-9.0f, 0.0f);
		}

		// ��� �� �Ʒ��� ��ũ��	����: https://oneday0012.tistory.com/16


		// �ؿ� �볪���� ���� �� ����
		// �ؿ� �ƹ��͵� ������ ���� ȭ�� ����


		app.clear();
		app.draw(Background);
		app.draw(Frog);
		//app.draw(Wood);
		//app.draw(Fly);
		app.display();
	}

	return 0;
}