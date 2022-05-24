#pragma once
#include <vector>
#include "Object.h"
#include "Brick.h"
#include "Paddle.h"

class Ball : public Object
{    
    int m_DX, m_DY;//приращение координат, скорость движени€ м€ча
    int m_saveDX, m_saveDY;//"нормальна€" скорость
    bool m_isPresent;//м€ч прорисовываетс€
    bool m_isLostBall;//м€ч потер€н
    bool m_isKickSucessfully;//успешное отбитие м€ча
    bool m_setSpeedToNormal;
public:
    Ball(int x, int y, int width, int height, uint32_t color, int dx = 1, int dy = 1, bool is_present = false);

    void show() override;

    void hide();

    void invertDX();

    void invertDY();

    void move(const Object& obj, const Object& paddle_obj, std::vector<Brick>& bricks_obj);

    void setSpeedToNormal(bool val);
    bool getm_setSpeedToNormal() const;
    void returnSpeedToNormalIfEnable();

    void setm_isKickSucessfully(bool val = true);
    bool getm_isKickSucessfully() const;    

    void setm_isLostBall(bool val = true);
    bool getm_isLostBall() const;
  
};