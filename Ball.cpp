#include "Ball.h"
#include <cmath>

Ball::Ball(int x, int y, int width, int height, uint32_t color, int dx, int dy, bool is_present) :
    Object(x, y, width, height, color), m_DX{ dx },
    m_DY{ dy }, m_saveDX{ dx }, m_saveDY{ dy }, 
    m_isPresent{ is_present }, m_isLostBall{ false }, 
    m_isKickSucessfully{false}, m_setSpeedToNormal{ false }
{

}
void Ball::show()
{
    if (m_isPresent)
    {
        Object::show();
    }
}
void Ball::hide()
{
    m_isPresent = false;
}

void Ball::invertDX()
{
    m_DX = -m_DX;    
}
void Ball::invertDY()
{
    m_DY = -m_DY;    
}

void Ball::returnSpeedToNormalIfEnable()
{
    if (m_setSpeedToNormal)
    {
        int m_DX_sign = m_DX > 0 ? 1 : -1;
        int m_DX_mod = std::abs(m_DX);

        int m_DY_sign = m_DY > 0 ? 1 : -1;
        int m_DY_mod = std::abs(m_DY);

        if (m_DX_mod != m_saveDX)
        {
            if (m_DX_mod < m_saveDX) m_DX_mod++;
            else m_DX_mod--;
            m_DX = m_DX_mod * m_DX_sign;
        }
        else
            if (m_DY_mod != m_saveDY)
            {
                if (m_DY_mod < m_saveDY) m_DY_mod++;
                else m_DY_mod--;
                m_DY = m_DY_mod * m_DY_sign;
            }
            else
            {//Если скорость нормальная, отключим коррекцию скорости:
                m_setSpeedToNormal = false;
            }
    }
}


void Ball::move(const Object& obj, const Object& paddle_obj, std::vector<Brick>& bricks_obj)
{    

    bool isNoGoodKick = false;//будем оптимистами(предполагаем,что игрок отбил мяч сбоку)

    m_X += m_DX;
    //Если мяч вышел за пределы экрана, то аннулируем движение по горизонтали:
    //меняем направление на противоположное
    if (this->isOneInSecond(obj) == false)
    {        
        //Возвращаем X на предыдущее значение:
        m_X -= m_DX;

        //Восстанавливаем скорость движения мяча,если это разрешено:
        returnSpeedToNormalIfEnable();
        //Меняем направление по оси X:
        invertDX();
    }
    else    
    if (this->isOverlapped(paddle_obj) == true)//Если коснулись ракетки:
    {
        if (this->isOverlappedThisHoriz(paddle_obj))//Коснулись слева/справа:
        {
                               
            //Если игрок ударил боковой частью ракетки по мячу,так уж и быть,
            //отобьем мяч ВВЕРХ:
            if (dynamic_cast<const Paddle*>(&paddle_obj)->getLeftMoveLastStatus())
            {//Располагаем мяч всегда слева от ракетки,не допуская overlapping:
                m_X = paddle_obj.getX() - m_Width;
                
                //Вначале удара ракеткой скорость мяча равна скорости ракетки:
                m_DX = dynamic_cast<const Paddle*>(&paddle_obj)->getDX();
                if (m_DX > 0) invertDX();
                //Однако даем задание вернуть со временем скорость к норме:
                m_setSpeedToNormal = true;
                if (m_DY > 0)  m_DY = - m_DY;

                //Успешно отбили мяч:
                setm_isKickSucessfully();
            }
            else
            if (dynamic_cast<const Paddle*>(&paddle_obj)->getRightMoveLastStatus())
            {//Располагаем мяч всегда справа от ракетки,не допуская overlapping:
                m_X = paddle_obj.getX() + paddle_obj.getWidth();
                
                //Вначале удара ракеткой скорость мяча равна скорости ракетки:
                m_DX = dynamic_cast<const Paddle*>(&paddle_obj)->getDX();
                if (m_DX < 0) invertDX();

                //Однако даем задание вернуть со временем скорость к норме:
                m_setSpeedToNormal = true;

                if (m_DY > 0)  m_DY = -m_DY;

                //Успешно отбили мяч:
                setm_isKickSucessfully();
            }
            else
            {
                //Если мяч коснулся САМ, меняем направление по оси X:
                invertDX();
                isNoGoodKick = true;
            }
        }
        else
        {
            m_X -= m_DX;
            //Меняем направление по оси X:
            invertDX();            
        }                             
    }    
    else
    {//Проверяем на столкновение с блоками:
        for (Brick& brick : bricks_obj)
        {
            if (brick.getStatusBrick())
            {
                if (this->isOverlapped(brick) == true)
                {                    
                    m_X -= m_DX;

                    //Восстанавливаем скорость движения мяча,если это разрешено:
                    returnSpeedToNormalIfEnable();
                    //Меняем направление по оси X:
                    invertDX();

                    //помечаем блок как "ударенный":
                    brick.dashed();
                    break;//за 1 раз может быть столкновение только с 1 блоком
                }
            }
        }

    }

    m_Y += m_DY;
    //Если мяч вышел за пределы экрана, то аннулируем движение по вертикали:
    //меняем направление на противоположное
    if (this->isOneInSecond(obj) == false)
    {
        m_Y -= m_DY;
        //Если мяч двигался вниз,то он потерян, скрываем мяч и
        // устанавливаем флаг m_isLostBall:
        if (m_DY > 0)
        {
            setm_isLostBall();
            //Скрываем мяч:
            hide();
        }
        else
        {
            //Меняем направление по оси Y:
            invertDY();
        }
    }
    else
    if (this->isOverlapped(paddle_obj) == true)
    {
        //Если мяч ударился САМ слева/справа, то НЕ отбиваем его:
        if (isNoGoodKick)
        {

        }
        else
        {
            m_Y -= m_DY;
            //Меняем направление по оси Y:
            invertDY();

            //Успешно отбили мяч:
            setm_isKickSucessfully();
        }
                
    }
    else
    {//Проверяем на столкновение с блоками:
        for (Brick& brick : bricks_obj)
        {
            if (brick.getStatusBrick())
            {
                if (this->isOverlapped(brick) == true)
                {
                    m_Y -= m_DY;
                    //Меняем направление по оси Y:
                    invertDY();
                    //помечаем блок как "ударенный":
                    brick.dashed();
                    break;//за 1 раз может быть столкновение только с 1 блоком
                }
            }
        }
    }
}


bool Ball::getm_setSpeedToNormal() const
{
    return m_setSpeedToNormal;
}

void Ball::setSpeedToNormal(bool val)
{
    m_setSpeedToNormal = val;
}

void Ball::setm_isKickSucessfully(bool val)
{    
    m_isKickSucessfully = val;
}
bool Ball::getm_isKickSucessfully() const
{
    return m_isKickSucessfully;
}

void Ball::setm_isLostBall(bool val)
{
    m_isLostBall = val;
}
bool Ball::getm_isLostBall() const
{
    return m_isLostBall;
}

