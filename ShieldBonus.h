#include "Bonus.h"

namespace game
{

class ShieldBonus : public Bonus
{
public:
    virtual void collide(Object& other) const noexcept override
    {
        if (other.id_ == 0)
        {
            other.shield_ += extra_shield_;
        }
    }

private:
    std::size_t extra_shield_;
};

} // namespace game

