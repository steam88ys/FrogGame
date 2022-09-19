#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;
// https://maincodes.tistory.com/49

// Text 설정 함수
int textPrint(Text& textMsg, Font& font, int size, float x, float y, const Color& color, const Color& outColor, string p)
{
	textMsg.setFont(font);
	textMsg.setCharacterSize(size);
	textMsg.setPosition(x, y);
	textMsg.setFillColor(color);
	textMsg.setOutlineColor(outColor);
	textMsg.setOutlineThickness(1.f);
	textMsg.setString(p);

	return 0;
}

int main()
{
	Text text1, text2, text3;
	Uint8 r = 0, g = 0, b = 0;
	string msgStr = "!Frog Game!";
	int x = 0, y = 0;

	Clock clock;
	float interval = 0;

	cout << "프로그램이 시작되었습니다." << endl;

	RenderWindow app(VideoMode(504, 504), "https://maincodes.tistory.com/");
	app.setFramerateLimit(60);

	Font font;
	font.loadFromFile("resources/DS-DIGIB.ttf");
	// 폰트 저장 위치 모르겠음

	textPrint(text1, font, 30, 0, 0,
		Color::Yellow, Color::White,
		msgStr);

	textPrint(text2, font, 100, 0, 0,
		Color::White, Color::White,
		msgStr);

	textPrint(text3, font, 150, 0, 0,
		Color::Blue, Color::White,
		msgStr);

	while (app.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();

		interval += time;

		Event event;

		while (app.pollEvent(event))
		{
			if (event.type == Event::EventType::Closed)
			{
				app.close();
				cout << "프로그램이 종료되었습니다." << endl;
			}
		}
	}
}