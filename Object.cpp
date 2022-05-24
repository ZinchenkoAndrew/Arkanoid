#include "Object.h"

    Object::Object() :
        m_X{ 0 }, m_Y{ 0 }, m_Width{ 0 }, m_Height{ 0 }, m_Color{ 0 }
    {

    }
    Object::Object(int x, int y, int width, int height, uint32_t color) :
        m_X{ x }, m_Y{ y }, m_Width{ width }, m_Height{ height }, m_Color{ color }
    {

    }

    int Object::getX() const
    {
        return m_X;
    }

    int Object::getY() const
    {
        return m_Y;
    }

    int Object::getWidth() const
    {
        return (m_Width);
    }

    int Object::getHeight() const
    {
        return (m_Height);
    }
    void Object::showXY(int x,int y,uint32_t color)
    {
        for (int i = y; i < (y + m_Height); i++)
        {
            for (int j = x; j < (x + m_Width); j++)
            {
                if ((j < SCREEN_WIDTH) && (i < SCREEN_HEIGHT))
                    buffer[i][j] = color;
            }
        }
    }
    void Object::show()
    {
        if ((m_Width < SCREEN_WIDTH) && (m_Height < SCREEN_HEIGHT))
        {
            //shadow:
            showXY(m_X - 5, m_Y + 5, 0);
        }
        //object
        showXY(m_X, m_Y, m_Color);
    }

    //Проверка вершины на попадание в прямоугольник:
    bool Object::isPointInArea(int x, int y, const Object& obj)
    {
        int obj_m_X2 = obj.m_X + obj.m_Width - 1;
        int obj_m_Y2 = obj.m_Y + obj.m_Height - 1;

        if ((x >= obj.m_X) && (x <= obj_m_X2))
        {
            if ((y >= obj.m_Y) && (y <= obj_m_Y2))
                return true;
        }
        return false;
    }
    //Проверка перекрытия объектов, если ДА, то возвращаем true:
    bool Object::isOverlapped(const Object& obj)
    {
        int m_X2 = m_X + m_Width - 1;
        int m_Y2 = m_Y + m_Height - 1;

        int obj_m_X2 = obj.m_X + obj.m_Width - 1;
        int obj_m_Y2 = obj.m_Y + obj.m_Height - 1;
        
        //m_X,m_Y:
        if (isPointInArea(m_X, m_Y, obj))
            return true;

        //m_X2,m_Y:
        if (isPointInArea(m_X2, m_Y, obj))
            return true;

        //m_X2,m_Y2:
        if (isPointInArea(m_X2, m_Y2, obj))
            return true;

        //m_X,m_Y2:
        if (isPointInArea(m_X, m_Y2, obj))
            return true;
        return false;
    }

    bool Object::isOverlappedThisHoriz(const Object& obj)
    {
        int m_X2 = m_X + m_Width - 1;
        int m_Y2 = m_Y + m_Height - 1;


        //Если левая верхняя и правая нижняя вершины this 
        // ВНУТРИ объекта obj,значит мы врезались в него по горизонтали        
        //ВАЖНО!!!Предполагается,что по вертикали мы уже проверили-
        //объекты должны находиться на одном уровне:
        if ((isPointInArea(m_X, m_Y, obj)) || (isPointInArea(m_X2, m_Y2, obj)))
        {        
             return true;
        }
        
        return false;
    }

    //Проверка вложения объектов, если один вложен в другой,возвращаем true:
    bool Object::isOneInSecond(const Object& obj)
    {
        int m_X2 = m_X + m_Width - 1;
        int m_Y2 = m_Y + m_Height - 1;

        int obj_m_X2 = obj.m_X + obj.m_Width - 1;
        int obj_m_Y2 = obj.m_Y + obj.m_Height - 1;

        //Если obj внутри this:
        if ((obj.m_X >= m_X) && (obj_m_X2 <= m_X2))
        {
            if ((obj.m_Y >= m_Y) && (obj_m_Y2 <= m_Y2))
            {
                return true;
            }
        }
        //Если this внутри obj:
        if ((m_X >= obj.m_X) && (m_X2 <= obj_m_X2))
        {
            if ((m_Y >= obj.m_Y) && (m_Y2 <= obj_m_Y2))
            {
                return true;
            }
        }
        return false;
    }

    int Object::getRandValue(int max)
    {
        return(rand() % max);
    }
    void Object::initRand()
    {
        srand((unsigned int)time(NULL));
    }
