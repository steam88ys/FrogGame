#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <time.h>
#include <Windows.h>

#define WIDTH 520
#define HEIGHT 450

using namespace sf;

// �浹���� Ȯ�� (�簢�浹)
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

    start = time(NULL);	// �ð� ���� ����
    for (i = 0; i < 100000; i++)
    {
        for (j = 0; j < 10000; j++)
        {
            sum += i * j;
        }
    }
    end = time(NULL);	// �ð� ���� ��
    result = (double)(end - start);
    printf("%f", result);	// ��� ���

    // ������ â�� �ð� ����
    // �ؽ�Ʈ ���� ��ü
    // TextOut(); ���ڷ� ��ȯ...

    // �ð� ������ ��¥ ǥ���ϱ�

    // �볪�� ���� ��ġ�� �����ϱ�

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
				app.close();
		}

        // ������ �����̱�
        Frog.move(dx, 0);
        for (int i = 0; i < n; i++) {
            if (isCollide(Frog, �볪��[i]))
            {
                dx = -dx;
                // �볪�� ������� �ϱ�
                �볪��[i].setPosition(-300, 0);
            }
        }
		Frog.move(0, dy);
		for (int i = 0; i < n; i++) {
			if (isCollide(sBall, block[i]))
			{
				dy = -dy;
                // �볪�� ������� �ϱ�
				block[i].setPosition(-300, 0);
			}
		}

		// ������ ��ġ(��ǥ)
		Vector2f b = Frog.getPosition();

		// Ű���忡 ���� �������� ������
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