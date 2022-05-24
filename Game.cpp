#include "Engine.h"
#include <stdlib.h>
#include <memory>
#include <time.h>
#include <vector>

#include "Object.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"



//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, 'A', 'B')
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  clear_buffer() - set all pixels in buffer to 'black'
//  is_window_active() - returns true if window is active
//  schedule_quit_game() - quit game after act()
/*
inline void Pset(int x, int y, int color)
{
    if ((x < SCREEN_WIDTH) && (y < SCREEN_HEIGHT))
        buffer[y][x] = color;
}
*/


inline uint32_t RGBa(uint32_t red,uint32_t green,uint32_t blue,uint32_t alpha = 0xFF)
{
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}


enum class GameStates
{
    INIT_STATE,
    RUNNING_STATE,
};

class Game
{
//Задержка интерфейса:
    float m_InterfaceMoveTimer;
    const float m_InterfaceMoveTimerTrigger = static_cast<float>(0.005);//sec
    
    std::unique_ptr<Object> pScreen;//Объект Экрана:

    const uint32_t m_BackgroundColor = RGBa(58,183,173);

    //Bricks:
    const int m_BricksnHorizontal = 6;
    const int m_BricksnVertical = 6;
    const int m_nBricksTotal = m_BricksnHorizontal * m_BricksnVertical;
    
    const int m_BricksHorizontalSpace = 16;
    const int m_BricksVerticalSpace = 16;
    const int m_BricksStartX = m_BricksHorizontalSpace/2;
    const int m_BricksStartY = m_BricksVerticalSpace/2;
    const int m_BricksWidth = SCREEN_WIDTH / m_BricksnHorizontal - m_BricksHorizontalSpace;
    const int m_BricksHeight = SCREEN_HEIGHT/2 / m_BricksnVertical - m_BricksVerticalSpace;
    

    std::vector<Brick> bricks;
    

    //Balls:
    const int m_BallStartY = m_BricksStartY + (m_BricksHeight + m_BricksVerticalSpace) * m_BricksnVertical;
    const int m_BallWidth = 8;
    const int m_BallHeight = 8;
    const int m_BallHorizontalSpeedDXStart = 1;
    const int m_BallVerticalSpeedDYStart = 1;    
    const int m_BallMaxPresentInGame = 5;
    const uint32_t m_BallColor = RGBa(255,255,0);
    
    std::vector<Ball> balls;
    
    //Paddle:
    const int m_PaddleWidthStart = 64;
    int m_PaddleWidthCurrent = m_PaddleWidthStart;
    const int m_PaddleHeight = 64;
    const int m_PaddleStartX = (SCREEN_WIDTH - m_PaddleWidthStart) / 2;
    const int m_PaddleY = SCREEN_HEIGHT - m_PaddleHeight - 1-6;
    const int m_PaddleHorizontalSpeedDXStart = 5;    
    const int m_PaddleColor = RGBa(29, 203, 227);
    std::unique_ptr<Paddle> pPaddle;
    
    
    
        
    bool isBallKickedSuccessfully;//флаг успешного отбития мяча
    int m_Score;
    GameStates m_StateOfGame;
    float m_GlobalTimer;

    
public:
    void init()
    {
        Object::initRand();

        m_StateOfGame = GameStates::RUNNING_STATE;
        m_Score = 0;
        m_InterfaceMoveTimer = 0;
        m_PaddleWidthCurrent = m_PaddleWidthStart;
        m_GlobalTimer = 0;
        isBallKickedSuccessfully = false;
        

        //Добавляем блоки:
        bricks.clear();
        int brickID = 0;
        for (int i = 0; i < m_BricksnVertical; i++)
            for (int j = 0; j < m_BricksnHorizontal; j++)
            {
                bricks.push_back(Brick(m_BricksStartX + (m_BricksWidth + m_BricksHorizontalSpace) * j,
                    m_BricksStartY + (m_BricksHeight + m_BricksVerticalSpace) * i,
                    m_BricksWidth, m_BricksHeight, RGBa(0, (i << 5) + 0x3F,(j<<5)+0x3F),
                    static_cast<Bonuses>(Object::getRandValue(static_cast<int>(Bonuses::nVariants))), brickID));
                brickID++;
            }


        
        //Добавляем один мяч:
        balls.clear();
        balls.push_back(Ball(Object::getRandValue(pScreen->getWidth()),
            m_BallStartY, m_BallWidth, m_BallHeight, m_BallColor,
            m_BallHorizontalSpeedDXStart, m_BallVerticalSpeedDYStart,
            true));
    }
    Game() :m_StateOfGame{ GameStates::INIT_STATE }, m_Score{ 0 }, 
        m_InterfaceMoveTimer{ 0 }, m_PaddleWidthCurrent{ m_PaddleWidthStart },
        m_GlobalTimer{ 0 }, isBallKickedSuccessfully{false}
    {
        //Создаем объект экрана:
        pScreen = std::make_unique<Object>(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, m_BackgroundColor);
        
        //Создаем объект ракетки:
        pPaddle = std::make_unique<Paddle>(m_PaddleStartX, m_PaddleY, m_PaddleWidthCurrent, m_PaddleHeight, m_PaddleColor, m_PaddleHorizontalSpeedDXStart);                
        
    }
    
    void oneTickGame(const float &dt)
    {
        m_GlobalTimer += dt; 
        switch (m_StateOfGame)
        {
            case GameStates::INIT_STATE:
            {
                init();                
            }
            break;
            case GameStates::RUNNING_STATE:
            {
                processGame();
            }
            break;
        }        
    }
    void processGame()
    {
        moveGameObjects();
        gameLogic();
    }
    
    void moveGameObjects()
    {
        
        if ((m_GlobalTimer - m_InterfaceMoveTimer) > m_InterfaceMoveTimerTrigger)
        {
            m_InterfaceMoveTimer = m_GlobalTimer;
            
            //Двигаем ракетку:
            if (is_key_pressed(VK_LEFT))
            {
                pPaddle->moveLeft(*pScreen);
            }
            else
                if (is_key_pressed(VK_RIGHT))
                {
                    pPaddle->moveRight(*pScreen);
                }
                else pPaddle->resetMoveLast();

            isBallKickedSuccessfully = false;

            //Двигаем мяч(и):
            for (auto& ball : balls)
            {
                ball.move(*pScreen, *pPaddle, bricks);
                isBallKickedSuccessfully |= ball.getm_isKickSucessfully();
                ball.setm_isKickSucessfully(false);//сбрасываем успешно отбитый мяч
            }            
        }                
    }

    void gameLogic()
    {
        //Если есть успешно отбитые мячи,то добавим новый:
        if (isBallKickedSuccessfully && (balls.size() < m_BallMaxPresentInGame))
            //Добавляем один мяч:
            balls.push_back(Ball(Object::getRandValue(pScreen->getWidth()),
                m_BallStartY, m_BallWidth, m_BallHeight, m_BallColor,
                m_BallHorizontalSpeedDXStart, m_BallVerticalSpeedDYStart,
                true));
        isBallKickedSuccessfully = false;

        //Если есть потерянные мячи, удалим их из массива:
        for (auto it = balls.begin(); it != balls.end();)
        {
            if ((*it).getm_isLostBall())
            {
                it = balls.erase(it);
            }
            else
            {
                ++it;
            }
        }

        //Если нет мячей, то нужно уменьшить счетчик жизней и перезапустить игру с начала:
        if (balls.empty()) m_StateOfGame = GameStates::INIT_STATE;

    }

    void showGameObjects()
    {
        //Стираем экран
        //memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
        pScreen->show();

        //Рисуем блоки:
        for (auto& brick : bricks)
        {
            brick.show();
        }

        //Рисуем ракетку:
        pPaddle->show();

        //Рисуем мяч(и):
        for (auto& ball : balls)
        {
            ball.show();
        }
    }
    
};


///////////////////////////////////////////////////////////


Game game;

// initialize game data in this function
void initialize()
{
    // Инициализация консоли игры

    
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
    
void act(float dt)
{
  if (is_key_pressed(VK_ESCAPE))
    schedule_quit_game();
  game.oneTickGame(dt);
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)

void draw()
{
    game.showGameObjects();
}

// free game data in this function
void finalize()
{
    // Завершение игры и освобождение ресурсов
    
}

