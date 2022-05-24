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

    //�������� ������� �� ��������� � �������������:
    bool isPointInArea(int x, int y, const Object& obj);
    //�������� ���������� ��������, ���� ��, �� ���������� true:
    bool isOverlapped(const Object& obj);

    //�������� ���������� ��������, ���� this ����������� obj �����/������,
    //�� ���������� true:
    bool isOverlappedThisHoriz(const Object& obj);
    
    //�������� �������� ��������, ���� ���� ������ � ������,���������� true:
    bool isOneInSecond(const Object& obj);

    static int getRandValue(int max);    
    static void initRand();
    
};
