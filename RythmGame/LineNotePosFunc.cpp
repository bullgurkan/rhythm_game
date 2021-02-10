#include "LineNotePosFunc.hpp"

LineNotePosFunc::LineNotePosFunc(int speed) : NotePosFunc(speed)
{}

sf::Vector2f LineNotePosFunc::GetPosition(sf::Vector2f endPos, int timeLeftUntilHit, Skin& skin)
{
    return (float)timeLeftUntilHit / 1000 * speed * endPos / skin.getMiddleRadius();
}
