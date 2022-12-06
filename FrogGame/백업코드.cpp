#include<SFML/Graphics.hpp>
#include<ctime>
#include<vector>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace sf;
using namespace std;


#define WIDTH 800       //����
#define HEIGHT 850      //����
#define BAR_COUNT 8    //���� ����

int score = 0;  // ���� ���ھ�
int level = 1;  // ���� ����
float GRAVITY = 0.2f;    //�߷�


//�÷��̾� Ŭ����
class Player
{
private:
    int x;
    int y;
    int imgWidth;
    int imgHeight;
    float speed = 1;    // �ӵ�
    float dy;
    Sprite* imgJump;
    Sprite* imgReady;
    Texture t1, t2;
    bool jumpFlag;
    const Sprite& GetImg()
    {
        if (jumpFlag)      // true�� �� ����, false�� �� ���� �غ� �̹���
        {
            return *imgJump;
        }
        else
        {
            return *imgReady;
        }
    }

public:
    Player() : dy(0), jumpFlag(true)
    {
        x = static_cast<int>(WIDTH / 2);
        y = static_cast<int>(HEIGHT / 2);

        t1.loadFromFile("images/frog2.png");
        t2.loadFromFile("images/frog.png");

        imgJump = new Sprite(t1);
        imgReady = new Sprite(t2);

        imgWidth = static_cast<int>(imgReady->getTexture()->getSize().x);
        imgHeight = static_cast<int>(imgReady->getTexture()->getSize().y);
    }
    ~Player()
    {
        delete(imgJump);
        delete(imgReady);
    }

    void SetPosition()
    {
        imgReady->setPosition(x, y);
        imgJump->setPosition(x, y);
    }

    void Move()
    {

        if (Keyboard::isKeyPressed(Keyboard::Right)) //�������̵�
        {
            x += (4 * speed);
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) //�����̵�
        {
            x -= (4 * speed);
        }

        if (x <= 0)                 //���� �� ���� ���ϰ�
        {
            x = 0;
        }
        if (x >= WIDTH - imgWidth)  //������ �� ���� ���ϰ�
        {
            x = WIDTH - imgWidth;
        }

        jumpFlag = true;
        dy += GRAVITY;
        y += static_cast<int>(dy);

        if (y > HEIGHT - imgHeight) // â ���̸� �Ѿ�� �ʰ�
        {
            jumpFlag = false;
            dy = -10;
        }

    }


    void Draw(RenderWindow& window)
    {
        /* ���� ��� */
        Font font;
        if (!font.loadFromFile(R"(E:\CPP\FrogGame\font\DS-DIGIB.TTF)"))
            throw std::exception("font error");

        String str1 = to_string(score);
        String str2 = to_string(level);
        Text text("\n  Score: " + str1 + "\n  Level: " + str2, font, 30);
        text.setFillColor(Color::White);

        window.draw(text);

        window.draw(GetImg());
    }


    void DrawGameOver(RenderWindow& window)
    {

        /* ���ӿ��� �޽��� ��� */
        Font font;
        if (!font.loadFromFile(R"(E:\CPP\FrogGame\font\DS-DIGIB.TTF)"))
            throw std::exception("font error");

        Text text("\n\n   GameOver", font, 140);
        text.setFillColor(Color::Red);

        window.draw(text);

    }

    float GetDy() const
    {
        return dy;
    }
    int GetY() const
    {
        return y;
    }
    int GetX() const
    {
        return x;
    }
    int GetWidth() const
    {
        return imgWidth;
    }
    int GetHeight() const
    {
        return imgHeight;
    }
    void SetY(int _y)
    {
        y = _y;
    }
    void Jump()         // �����ϱ�
    {
        jumpFlag = false;
        dy = -11;
        PlaySound(TEXT("jumping_sound.wav"), NULL, SND_ASYNC);
    }
    void LevelUp()     // ������
    {
        //speed = 1.3;
        dy -= 0.5f;
    }
    void StopJump()     // ���� ���߱�
    {
        speed = 0;
        dy = -11 * speed;
    }
};



//���� bar Ŭ����
class Bar
{
private:
    struct Pos
    {
        int x;
        int y;
        int pspeed = 0;
        int count = 0;  // ���� Ƚ�� üũ
    };

    vector<Pos> vBar;
    Sprite* imgBar;
    Texture t;
    int imgWidth;
    int imgHeight;

public:
    Bar()
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        t.loadFromFile("images/wood.png");
        imgBar = new Sprite(t);

        imgWidth = imgBar->getTexture()->getSize().x;
        imgHeight = imgBar->getTexture()->getSize().y;

        for (int i = 0; i < BAR_COUNT; ++i) // ���� ��ġ ���� ����
        {
            Pos p;
            p.x = rand() % WIDTH - imgWidth / 2;    // ���� ���� ��ġ�� �ʴ� ���� ��ġ
            p.y = rand() % HEIGHT;                  // ���� ���� ���� ��ġ
            vBar.push_back(p);
        }

        vBar[0].y = HEIGHT - 200;   // �� �� ���� y��ǥ ����
    }
    ~Bar()  // ���� �Ҹ�
    {
        delete(imgBar);
    }


    void Draw(RenderWindow& window) // ���� �׸���
    {
        for (int i = 0; i < BAR_COUNT; ++i)
        {
            imgBar->setPosition(vBar[i].x, vBar[i].y);
            window.draw(*imgBar);
        }
    }


    bool CheckCollision(Player* pPlayer)
    {
        //null check
        if (pPlayer == nullptr)
        {
            return false;
        }

        for (int i = 0; i < BAR_COUNT; ++i)
        {                                   // �������� �����ϴ� ���
            if (pPlayer->GetDy() > 0        // ������ ���ǿ� ���� ��
                && pPlayer->GetX() + pPlayer->GetWidth() > vBar[i].x    // �������� x��ǥ + �������� ���� > ���� �̹��� ���� ��ǥ
                && pPlayer->GetX() < vBar[i].x + imgWidth   // �������� x��ǥ + �������� ���� < ���� �̹��� ������ ��ǥ
                && pPlayer->GetY() + pPlayer->GetHeight() > vBar[i].y   // �������� y��ǥ + ���� ���� > ���� �Ʒ��� y��ǥ
                && pPlayer->GetY() + pPlayer->GetHeight() < vBar[i].y + imgHeight) // �������� y��ǥ + ���� ���� < ���� ���� y��ǥ + 10
            {
                pPlayer->Jump();

                if (score == 20)
                {
                    pPlayer->LevelUp();
                }

                if (score == 40)
                {
                    pPlayer->LevelUp();
                }

                // ���� ȿ����
                vBar[i].count++;
                if (vBar[i].count == 1) {   // ���� ���� ����� ��
                    score += 10;
                }

                // ���ھ� ���
                if (score != 0 && score % 100 == 0)
                {
                    level++;
                }

                return true;
            }
        }
        return false;
    }

    void MoveAndReset(Player* pPlayer)
    {
        static const int limit = HEIGHT / 3;    // limit : ȭ�� ������ 3���� 1
        if (pPlayer->GetY() < limit)    // �������� ���� ��ġ�� limit���� ���� ��
        {
            for (int i = 0; i < BAR_COUNT; ++i)
            {
                pPlayer->SetY(limit);   // �������� limit ��ġ�� �ǵ���
                vBar[i].y -= static_cast<int>(pPlayer->GetDy());
                if (vBar[i].y > HEIGHT + 10)    //������ y��ǥ�� �� ȭ�� ���� ���� �ö� ��
                {
                    vBar[i].y = rand() % HEIGHT / 4 + 100;  // �ּ� ���� 100 
                    vBar[i].x = rand() % WIDTH;
                }
            }
        }
    }

};



int main(void)
{

    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Frog Game");
    window.setFramerateLimit(60);

    //setting
    Player* pPlayer = new Player();
    Bar* pBar = new Bar();

    Texture bg;
    bg.loadFromFile("images/background.jpg");
    Sprite Background(bg);

    while (window.isOpen())
    {
        Event e;
        if (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
            {
                window.close();
            }
        }

        //logic
        pPlayer->Move();
        pBar->MoveAndReset(pPlayer);
        pBar->CheckCollision(pPlayer);
        pPlayer->SetPosition();

        //draw
        window.clear(Color::White);
        window.draw(Background);
        pPlayer->Draw(window);
        pBar->Draw(window);

        if (pPlayer->GetY() > 720) {   // �������� �ٴڿ� ����� ��
            pPlayer->StopJump();   // speed�� 0����
            pPlayer->DrawGameOver(window);
        }
        window.display();

    }

    delete(pBar);
    delete(pPlayer);

    return 0;
}
