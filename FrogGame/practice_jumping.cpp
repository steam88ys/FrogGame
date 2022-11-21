#include<SFML/Graphics.hpp>
#include<ctime>
#include<vector>
#include <iostream>

using namespace sf;
using namespace std;

#define WIDTH 800        //가로
#define HEIGHT 850       //세로
#define BAR_COUNT 10    //밟는 bar 개수
static const float GRAVITY = 0.2f;    //중력

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
    Texture t1,t2;
    bool jumpFlag;
private:
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
        {   x += 4; }
        if (Keyboard::isKeyPressed(Keyboard::Left)) //왼쪽이동
        {   x -= 4; }
        
        if (x <= 0)                 //왼쪽 벽 뚫지 못하게
        {   x = 0;  }                              
        if (x >= WIDTH - imgWidth)  //오른쪽 벽 뚫지 못하게
        {   x = WIDTH - imgWidth;}  

        jumpFlag = true;
        dy += GRAVITY;
        y += static_cast<int>(dy);

        if (y > HEIGHT - imgHeight)
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

        // 화면에 쓸 내용
        Text text("GameOver", font, 100);   
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
    void Jump()
    {
        jumpFlag = false;
        dy = -11*speed;
    }
    void Speed()
    {
        speed = 0;
        Jump();
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
            p.x = rand() % WIDTH - imgWidth / 2;
            p.y = rand() % HEIGHT;
            vBar.push_back(p);
        }

        vBar[0].y = HEIGHT - 200;
    }
    ~Bar()
    {
        delete(imgBar);
    }

    void Draw(RenderWindow& window)
    {
        for (int i = 0; i < BAR_COUNT; ++i) // 통나무 화면에 띄우기
        {
            imgBar->setPosition(vBar[i].x, vBar[i].y);
            window.draw(*imgBar);
        }
    }
    bool CheckCollision(Player* pPlayer)
    {
        //null check.
        if (pPlayer == nullptr)
        {
            return false;
        }

        for (int i = 0; i < BAR_COUNT; ++i)
        {
            if (pPlayer->GetDy() > 0        // 점프가 가능한 상황
                && pPlayer->GetX() + pPlayer->GetWidth() > vBar[i].x
                && pPlayer->GetX() < vBar[i].x + imgWidth
                && pPlayer->GetY() + pPlayer->GetHeight() > vBar[i].y
                && pPlayer->GetY() + pPlayer->GetHeight() < vBar[i].y + 10)
            {
                pPlayer->Jump();
                return true;
            }
        }
        return false;
    }

    void MoveAndReset(Player* pPlayer)
    {
        static const int limit = HEIGHT / 3;
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
        if (pPlayer->GetY() >= 700) {   // 개구리가 바닥에 닿았을 때
            pPlayer->Speed();   // speed를 0으로
            pPlayer->DrawGameOver(window);
        }   // TODO: 게임 오버하면 화면 멈추게 하기
        window.display();

    }

    delete(pBar);
    delete(pPlayer);

    return 0;
}
