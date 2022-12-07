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
#define BAR_COUNT 8    //발판 개수

int score = 0;  // 게임 스코어
int level = 1;  // 게임 레벨
float GRAVITY = 0.2f;    //중력

clock_t start, finish;  // 측정할 시간 시작, 끝
double duration;    // 측정한 시간


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
        //점프 시간이 0초 이상일 때
        if (jumpFlag && duration<=0)      // true일 때 점프, false일 때 점프 준비 이미지
        {
            return *imgJump;
        }
        else
        {
            return *imgReady;
            // 지연시키는 코드
            // 점프 상태가 a초 이상일 때
            // 바닥에 돌아오면 a를 0으로 초기화
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
        /* 점수 출력 */
        Font font;
        if (!font.loadFromFile(R"(E:\CPP\FrogGame\font\DS-DIGIB.TTF)"))
            throw std::exception("font error");

        String str1 = to_string(score);
        String str2 = to_string(level);
        Text text("\n  Score: " + str1+"\n  Level: "+str2, font, 30);
        text.setFillColor(Color::White);

        window.draw(text);

        window.draw(GetImg());
    }


    void DrawGameOver(RenderWindow& window)
    {

        /* 게임오버 메시지 출력 */
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
    void Jump()         // 점프하기
    {
        dy = -11*speed;
        PlaySound(TEXT("jumping_sound.wav"), NULL, SND_ASYNC);
    }
    void LevelUp()     // 레벨업 (움직임 빨라지고, 점프 후 더 빨리 떨어짐)
    {
        speed += 0.06f;
        GRAVITY += 0.02f;
    }
    void StopJump()     // 점프 멈추기
    {
        speed = 0; 
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
        int pspeed = 0;
        int count = 0;  // 밟은 횟수 체크
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

        for (int i = 0; i < BAR_COUNT; ++i) // 발판 위치 랜덤 생성
        {
            Pos p;
            p.x = rand() % WIDTH - imgWidth / 2;    // 발판 가로 겹치지 않는 랜덤 위치
            p.y = rand() % HEIGHT;                  // 발판 높이 랜덤 위치
            vBar.push_back(p);
        }

        vBar[0].y = HEIGHT - 400;   // 처음 발판 y좌표 설정
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

    // 충돌 체크 후 점프 (착지, 스코어, 레벨 기능)
    void CheckCollision(Player* pPlayer)
    {

        for (int i = 0; i < BAR_COUNT; ++i)
        {                                   // 개구리가 점프하는 경우
            if (pPlayer->GetDy() > 0        // 개구리 발판에 닿을 때
                && pPlayer->GetX() + pPlayer->GetWidth() > vBar[i].x    // 개구리의 x좌표 + 개구리의 넓이 > 발판 이미지 왼쪽 좌표
                && pPlayer->GetX() < vBar[i].x + imgWidth   // 개구리의 x좌표 + 개구리의 넓이 < 발판 이미지 오른쪽 좌표
                && pPlayer->GetY() + pPlayer->GetHeight() > vBar[i].y   // 개구리의 y좌표 + 현재 높이 > 발판 아래쪽 y좌표
                && pPlayer->GetY() + pPlayer->GetHeight() < vBar[i].y + imgHeight) // 개구리의 y좌표 + 현재 높이 < 발판 위쪽 y좌표 + 10
            {
                
                // 점프 시간 측정
                start = clock();
                pPlayer->Jump();
                finish = clock();
                duration = (double)(finish - start) / CLOCKS_PER_SEC;
           

                vBar[i].count++;
                if (vBar[i].count == 1) {   // 같은 발판 밟았을 때
                  score += 10;

                  // 스코어 계산
                  if (score != 0 && score % 50 == 0)
                  {
                      level++;
                      pPlayer->LevelUp();
                  }
                }
                
            }
        }
    }

    // 개구리가 점프 가능한 범위 지정
    void MoveAndReset(Player* pPlayer)
    {
        static const int limit = HEIGHT / 3;    // limit : 화면 높이의 3분의 1
        if (pPlayer->GetY() < limit)    // 개구리의 현재 위치가 limit보다 작을 때
        {
            for (int i = 0; i < BAR_COUNT; ++i)
            {
                pPlayer->SetY(limit);   // 개구리를 limit 위치로 되돌림
                vBar[i].y -= static_cast<int>(pPlayer->GetDy());
                if (vBar[i].y > HEIGHT + 10)    //발판의 y좌표가 한 화면 보다 높이 올라갈 때
                {
                    vBar[i].y = rand() % HEIGHT / 4 + 100;  // 최소 단차 100 
                    vBar[i].x = rand() % WIDTH;
                }
            }
        }
    }

};



int main(void)
{

    system("mode con cols=80 lines=42");
    
    printf("\n\n\n\n                            Gravity Frog Game");
    printf("\n\n\n\n                            엔터키를 입력하세요.");

    char ch;
    ch = getchar();

    if (ch == 10)
    {
        printf("\n\n\n\n\n\n                           __   __   _____     __");
        printf("\n                          |  | |  | |_   _|   /  |\n");
        printf("\n                          |  |_|  |   | |    |   |\n");
        printf("\n                          |   _   |   | |    |   |\n");
        printf("\n                          |  | |  |   | |    |__/\n");
        printf("\n                          |  | |  |  _| |_    __ \n");
        printf("\n                          |__| |__| |_____|  |__|\n");
    }  

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

        if (pPlayer->GetY() > 720) {   // 개구리가 바닥에 닿았을 때
            pPlayer->StopJump();   // speed를 0으로
            pPlayer->DrawGameOver(window);
        }
        window.display();

    }

    delete(pBar);
    delete(pPlayer);

    return 0;

}
