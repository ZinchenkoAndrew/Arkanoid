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
    //���������� ��������� ���������� �������
    void resetMoveLast();

    //������� ������� �����,obj - ������ ������:
    void moveLeft(const Object& obj);
    
    //������� ������� ������,obj - ������ ������:
    void moveRight(const Object& obj);

    //������ m_DX    
    int getDX() const;    
    bool getLeftMoveLastStatus() const;
    bool getRightMoveLastStatus() const;
    
};
