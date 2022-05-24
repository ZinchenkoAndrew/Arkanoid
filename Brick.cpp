#include "Brick.h"

Brick::Brick(int x, int y, int width, int height, uint32_t color, Bonuses bonus, int id) :
    Object(x, y, width, height, color), m_ID{ id }, m_Bonus{ bonus }, m_isPresent{ true }, m_isDashed{ false }
{

};
Brick::Brick() :Object(), m_ID{ 0 }, m_Bonus{ Bonuses::NONE }, m_isPresent{ true }, m_isDashed{ false }
{

}
void Brick::show() 
{
    if (m_isPresent)
    {
        Object::show();
    }
}
bool Brick::getStatusBrick() const
{
    return (m_isPresent);
}
void Brick::hide()
{
    m_isPresent = false;
}
void Brick::dashed()
{
    m_isDashed = true;
    hide();
}

