#include "Bonus.h"

namespace game
{

class HPBonus : public Bonus
{
public:
    virtual void collide(Object& other) const noexcept override
    {
        if (other.id_ == 0)
        {
            other.hp_ += add_hp_;
        }
    }

private:
    std::size_t add_hp_;
};

} // namespace game

