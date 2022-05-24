#pragma once
#include "Engine.h"
#include <stdlib.h>
#include <memory>
#include <time.h>
#include <vector>

class Object
{
protected:
    int m_X, m_Y;
    int m_Width, m_Height;
    uint32_t m_Color;
public:
    Object();
    Object(int x, int y, int width, int height, uint32_t color);
    int getX() const;
    int getY() const;
    int getWidth() const;    
    int getHeight() const;
    void showXY(int x, int y, uint32_t color);
    virtual void show();

    //Проверка вершины на попадание в прямоугольник:
    bool isPointInArea(int x, int y, const Object& obj);
    //Проверка перекрытия объектов, если ДА, то возвращаем true:
    bool isOverlapped(const Object& obj);

    //Проверка перекрытия объектов, если this перекрывает obj слева/справа,
    //то возвращаем true:
    bool isOverlappedThisHoriz(const Object& obj);
    
    //Проверка вложения объектов, если один вложен в другой,возвращаем true:
    bool isOneInSecond(const Object& obj);

    static int getRandValue(int max);    
    static void initRand();
    
};
