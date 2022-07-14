#include "GunBonus.h"

namespace game
{

class GunCooldownBonus : public GunBonus
{
public:
    virtual void collide(Object& other) const noexcept override
    {
        if (other.id_ == 0)
        {
            other.gun_->change_cooldown(percent_);
        }
    }

private:
    int percent_;
};

} // namespace game

