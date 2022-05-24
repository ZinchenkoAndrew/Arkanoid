#pragma once
#include "Object.h"


class Paddle : public Object
{
    int m_DX;
    bool isLeftMoveLast;
    bool isRightMoveLast;
public:
    Paddle();    
    Paddle(int x, int y, int width, int height, uint32_t color, int dx = 1);        
    //сбрасываем последнюю активность ракетки
    void resetMoveLast();

    //Двигаем ракетку влево,obj - объект экрана:
    void moveLeft(const Object& obj);
    
    //Двигаем ракетку вправо,obj - объект экрана:
    void moveRight(const Object& obj);

    //геттер m_DX    
    int getDX() const;    
    bool getLeftMoveLastStatus() const;
    bool getRightMoveLastStatus() const;
    
};
