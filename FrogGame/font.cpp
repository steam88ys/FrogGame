#include <SFML/Graphics.hpp>
#include <string>
#include <exception>

using namespace sf;

int main(void)
{
    // main window ����
    RenderWindow window(VideoMode(400, 300), "SFML window");

    // �� ��Ʈ ����
    Font font;
    if (!font.loadFromFile(R"(E:\CPP\FrogGame\font\DS-DIGIB.TTF)"))
        throw std::exception("font error");


    // ȭ�鿡 �� ����
    Text text("Frog Game", font, 50);   // ������ ����ִ� ���� ���� ��
    text.setFillColor(Color::White); //�۾� ����

    // ������ ���� ����
    while (window.isOpen())
    {
        Event event;

        // ���� ���� üũ
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        // ȭ�� û��
        window.clear();

        // ȭ�鿡 �۾� ����
        window.draw(text);

        // ����
        window.display();
    }
}
