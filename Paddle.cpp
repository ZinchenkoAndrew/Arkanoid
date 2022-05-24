#include "Paddle.h"

Paddle::Paddle() : Object(), m_DX{ 1 }, isLeftMoveLast{false}, isRightMoveLast{ false }
{

};
Paddle::Paddle(int x, int y, int width, int height, uint32_t color, int dx) :
    Object(x, y, width, height, color), m_DX{ dx }, isLeftMoveLast{ false }, isRightMoveLast{ false }
{

}

//сбрасываем последнюю активность ракетки
void Paddle::resetMoveLast()
{
    isLeftMoveLast = isRightMoveLast = false;
}

//Двигаем ракетку влево,obj - объект экрана:
void Paddle::moveLeft(const Object& obj)
{
    isLeftMoveLast = true;
    m_X -= m_DX;
    //Если ракетка вышла за пределы экрана, то аннулируем сдвиг влево:
    if (this->isOneInSecond(obj) == false)
    {
        m_X = 0;
    }    
}

//Двигаем ракетку вправо,obj - объект экрана:
void Paddle::moveRight(const Object& obj)
{
    isRightMoveLast = true;
    m_X += m_DX;
    //Если ракетка вышла за пределы экрана, то аннулируем сдвиг вправо:
    if (this->isOneInSecond(obj) == false)
    {
        m_X -= m_DX;
    }    
}



int Paddle::getDX() const
{
    return m_DX;
}



bool Paddle::getLeftMoveLastStatus() const
{
    return isLeftMoveLast;
}

bool Paddle::getRightMoveLastStatus() const
{
    return isRightMoveLast;
}
