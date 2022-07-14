#include "GunBonus.h"

namespace game
{

class GunAttackBonus : public GunBonus
{
public:
    virtual void collide(Object& other) const noexcept override
    {
        if (other.id_ == 0)
        {
            other.gun_->change_attack(percent_);
        }
    }

private:
    int percent_;
};

} // namespace game

