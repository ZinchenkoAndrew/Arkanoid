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
            {//���� �������� ����������, �������� ��������� ��������:
                m_setSpeedToNormal = false;
            }
    }
}


void Ball::move(const Object& obj, const Object& paddle_obj, std::vector<Brick>& bricks_obj)
{    

    bool isNoGoodKick = false;//����� �����������(������������,��� ����� ����� ��� �����)

    m_X += m_DX;
    //���� ��� ����� �� ������� ������, �� ���������� �������� �� �����������:
    //������ ����������� �� ���������������
    if (this->isOneInSecond(obj) == false)
    {        
        //���������� X �� ���������� ��������:
        m_X -= m_DX;

        //��������������� �������� �������� ����,���� ��� ���������:
        returnSpeedToNormalIfEnable();
        //������ ����������� �� ��� X:
        invertDX();
    }
    else    
    if (this->isOverlapped(paddle_obj) == true)//���� ��������� �������:
    {
        if (this->isOverlappedThisHoriz(paddle_obj))//��������� �����/������:
        {
                               
            //���� ����� ������ ������� ������ ������� �� ����,��� �� � ����,
            //������� ��� �����:
            if (dynamic_cast<const Paddle*>(&paddle_obj)->getLeftMoveLastStatus())
            {//����������� ��� ������ ����� �� �������,�� �������� overlapping:
                m_X = paddle_obj.getX() - m_Width;
                
                //������� ����� �������� �������� ���� ����� �������� �������:
                m_DX = dynamic_cast<const Paddle*>(&paddle_obj)->getDX();
                if (m_DX > 0) invertDX();
                //������ ���� ������� ������� �� �������� �������� � �����:
                m_setSpeedToNormal = true;
                if (m_DY > 0)  m_DY = - m_DY;

                //������� ������ ���:
                setm_isKickSucessfully();
            }
            else
            if (dynamic_cast<const Paddle*>(&paddle_obj)->getRightMoveLastStatus())
            {//����������� ��� ������ ������ �� �������,�� �������� overlapping:
                m_X = paddle_obj.getX() + paddle_obj.getWidth();
                
                //������� ����� �������� �������� ���� ����� �������� �������:
                m_DX = dynamic_cast<const Paddle*>(&paddle_obj)->getDX();
                if (m_DX < 0) invertDX();

                //������ ���� ������� ������� �� �������� �������� � �����:
                m_setSpeedToNormal = true;

                if (m_DY > 0)  m_DY = -m_DY;

                //������� ������ ���:
                setm_isKickSucessfully();
            }
            else
            {
                //���� ��� �������� ���, ������ ����������� �� ��� X:
                invertDX();
                isNoGoodKick = true;
            }
        }
        else
        {
            m_X -= m_DX;
            //������ ����������� �� ��� X:
            invertDX();            
        }                             
    }    
    else
    {//��������� �� ������������ � �������:
        for (Brick& brick : bricks_obj)
        {
            if (brick.getStatusBrick())
            {
                if (this->isOverlapped(brick) == true)
                {                    
                    m_X -= m_DX;

                    //��������������� �������� �������� ����,���� ��� ���������:
                    returnSpeedToNormalIfEnable();
                    //������ ����������� �� ��� X:
                    invertDX();

                    //�������� ���� ��� "���������":
                    brick.dashed();
                    break;//�� 1 ��� ����� ���� ������������ ������ � 1 ������
                }
            }
        }

    }

    m_Y += m_DY;
    //���� ��� ����� �� ������� ������, �� ���������� �������� �� ���������:
    //������ ����������� �� ���������������
    if (this->isOneInSecond(obj) == false)
    {
        m_Y -= m_DY;
        //���� ��� �������� ����,�� �� �������, �������� ��� �
        // ������������� ���� m_isLostBall:
        if (m_DY > 0)
        {
            setm_isLostBall();
            //�������� ���:
            hide();
        }
        else
        {
            //������ ����������� �� ��� Y:
            invertDY();
        }
    }
    else
    if (this->isOverlapped(paddle_obj) == true)
    {
        //���� ��� �������� ��� �����/������, �� �� �������� ���:
        if (isNoGoodKick)
        {

        }
        else
        {
            m_Y -= m_DY;
            //������ ����������� �� ��� Y:
            invertDY();

            //������� ������ ���:
            setm_isKickSucessfully();
        }
                
    }
    else
    {//��������� �� ������������ � �������:
        for (Brick& brick : bricks_obj)
        {
            if (brick.getStatusBrick())
            {
                if (this->isOverlapped(brick) == true)
                {
                    m_Y -= m_DY;
                    //������ ����������� �� ��� Y:
                    invertDY();
                    //�������� ���� ��� "���������":
                    brick.dashed();
                    break;//�� 1 ��� ����� ���� ������������ ������ � 1 ������
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

