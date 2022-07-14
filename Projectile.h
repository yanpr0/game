#ifndef PROJECTILE_YANPRO
#define PROJECTILE_YANPRO

#include <cstddef>
#include <cstdint>

#include "Object.h"
#include "utils.h"

namespace game
{

class Projectile : public Object
{
public:
    [[nodiscard]] static std::shared_ptr<Object> create(
            std::size_t id,
            std::uint32_t team_mask,
            int x,
            int y,
            int attack,
            std::size_t min_radius,
            std::uint32_t color,
            int dx,
            int dy)
    {
        auto ptr = std::make_shared<Projectile>(id, team_mask, x, y, attack, min_radius, color, dx, dy);
        return std::static_pointer_cast<Object>(ptr);
    }

    virtual void collide(Object& other) noexcept override
    {
        if ((team_mask_ & other.team_mask_) == 0)
        {
            if (other.shield_ > 0)
            {
                other.shield_ = std::max(0, other.shield_ - attack_);
            }
            else
            {
                other.hp_ = std::max(0, other.hp_ - attack_);
            }
            is_dead_ = true;
        }
    }

    virtual void move(float dt, Point target) noexcept override
    {
        x_ += dx_ * dt;
        y_ += dy_ * dt;

        if (x_ < 0 || x_ >= SCREEN_WIDTH || y_ < 0 || y_ >= SCREEN_HEIGHT)
        {
            is_dead_ = true;
        }
    }

    virtual std::size_t get_radius() const noexcept override
    {
        return min_radius_;
    }

public:
Projectile():Object(0,0,0,0,0,0,0,0,0){}
//private:
    Projectile(std::size_t id, std::uint32_t team_mask, int x, int y, int attack, std::size_t min_radius, std::uint32_t color, int dx, int dy) :
        Object(id, team_mask, x, y, 1, 0, attack, min_radius, color),
        dx_{dx},
        dy_{dy}
    {
    }

    int dx_;
    int dy_;
};

}

#endif

