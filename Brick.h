#pragma once
#include "Object.h"

enum class Bonuses
{
    NONE,
    EXTRA_WIDE,
    MANY_BALLS,
    DEMOLUTION,
    EXTRA_LIFE,
    nVariants
};
class Brick :public Object
{
    int m_ID;
    Bonuses m_Bonus;
    bool m_isPresent;
    bool m_isDashed;
public:
    Brick(int x, int y, int width, int height, uint32_t color, Bonuses bonus, int id);
    Brick();
    void show() override;
    
    bool getStatusBrick() const;    
    void hide();    
    void dashed();    
};
