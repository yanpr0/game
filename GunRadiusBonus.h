#include "GunBonus.h"

namespace game
{

class GunRadiusBonus : public GunBonus
{
public:
    virtual void collide(Object& other) const noexcept override
    {
        if (other.id_ == 0)
        {
            other.gun_->change_radius(percent_);
        }
    }

private:
    int percent_;
};

} // namespace game

