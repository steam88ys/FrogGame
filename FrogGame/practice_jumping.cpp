#include<SFML/Graphics.hpp>
#include<ctime>
#include<vector>
#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace sf;
using namespace std;

#define WIDTH 800       //가로
#define HEIGHT 850      //세로
#define BAR_COUNT 10    //발판 개수
static const float GRAVITY = 0.2f;    //중력
int score = 0;

//플래이어 클래스
class Player
{
private:
    int x;
    int y;
    int imgWidth;
    int imgHeight;
    float speed = 1;    // 속도
    float dy;
    Sprite* imgJump;
    Sprite* imgReady;
    Texture t1, t2;
    bool jumpFlag;
    const Sprite& GetImg()
    {
        if (jumpFlag)
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

        if (Keyboard::isKeyPressed(Keyboard::Right)) //오른쪽이동
        {
            x += (4 * speed);
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) //왼쪽이동
        {
            x -= (4 * speed);
        }

        if (x <= 0)                 //왼쪽 벽 뚫지 못하게
        {
            x = 0;
        }
        if (x >= WIDTH - imgWidth)  //오른쪽 벽 뚫지 못하게
        {
            x = WIDTH - imgWidth;
        }

        jumpFlag = true;
        dy += GRAVITY;
        y += static_cast<int>(dy);

        if (y > HEIGHT - imgHeight) // 창 높이를 넘어가지 않게
        {
            jumpFlag = false;
            dy = -10;
        }

    }

    
    void Draw(RenderWindow& window)
    {
        window.draw(GetImg());
    }


    void DrawGameOver(RenderWindow& window)
    {

        // 화면에 게임 오버 폰트 띄우기
        Font font;
        if (!font.loadFromFile(R"(E:\CPP\FrogGame\font\DS-DIGIB.TTF)"))
            throw std::exception("font error");

        String str = to_string(score);
        Text text("\n\n      GameOver\n      Score: "+str, font, 120);
        text.setFillColor(Color::Red); //글씨 색깔

        // 화면에 글씨 쓰기
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
    void Jump()         // 점프하기
    {
        score += 10;    // 같은 발판 받았을 때 체크 해야 됨
        jumpFlag = false;
        dy = -11;
    }
    void StopJump()     // 점프 멈추기
    {
        speed = 0; 
        jumpFlag = false;
        dy = -11*speed;
    }
};



//점프 bar 클래스
class Bar
{
private:
    struct Pos
    {
        int x;
        int y;
    };
    vector<Pos> vBar;
    Sprite* imgBar;
    Texture t;
    int imgWidth;

public:
    Bar()
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        t.loadFromFile("images/wood.png");
        imgBar = new Sprite(t);

        imgWidth = imgBar->getTexture()->getSize().x;

        for (int i = 0; i < BAR_COUNT; ++i) // 통나무 위치 랜덤 생성
        {
            Pos p;
            p.x = rand() % WIDTH - imgWidth / 2;    // 통나무 가로 겹치지 않는 랜덤 위치
            p.y = rand() % HEIGHT;                  // 통나무 높이 랜덤 위치
            vBar.push_back(p);
        }

        vBar[0].y = HEIGHT - 200;   // 발판 내려감
    }
    ~Bar()  // 발판 소멸
    {
        delete(imgBar);
    }


    void Draw(RenderWindow& window) // 발판 그리기
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
        {                                   // 개구리가 점프하는 경우
            if (pPlayer->GetDy() > 0        // 개구리 발판에 닿을 때
                && pPlayer->GetX() + pPlayer->GetWidth() > vBar[i].x    // 개구리의 x좌표 + 개구리의 넓이 > 발판 이미지 왼쪽 좌표
                && pPlayer->GetX() < vBar[i].x + imgWidth   // 개구리의 x좌표 + 개구리의 넓이 < 발판 이미지 오른쪽 좌표
                && pPlayer->GetY() + pPlayer->GetHeight() > vBar[i].y   // 개구리의 y좌표 + 현재 높이 > 발판 아래쪽 y좌표
                && pPlayer->GetY() + pPlayer->GetHeight() < vBar[i].y + 10) // 개구리의 y좌표 + 현재 높이 < 발판 위쪽 y좌표
            {
                pPlayer->Jump();
                return true;
            }
        }
        return false;
    }

    void MoveAndReset(Player* pPlayer)
    {
        static const int limit = HEIGHT / 3;    // limit : 화면 높이의 3분의 1
        if (pPlayer->GetY() < limit)
        {
            for (int i = 0; i < BAR_COUNT; ++i)
            {
                pPlayer->SetY(limit);
                vBar[i].y -= static_cast<int>(pPlayer->GetDy());
                if (vBar[i].y > HEIGHT + 10)    // 통나무 움직이기
                {
                    vBar[i].y = rand() % HEIGHT / 4 + 100;
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

    PlaySound(TEXT("Calimba-E_s-Jammy-Jams.wav"), NULL, SND_ASYNC | SND_LOOP);    // 반복 재생

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

        if (pPlayer->GetY() > 730) {   // 개구리가 바닥에 닿았을 때
            pPlayer->StopJump();   // speed를 0으로
            pPlayer->DrawGameOver(window);
        }
        window.display();

    }

    delete(pBar);
    delete(pPlayer);

    return 0;
}
