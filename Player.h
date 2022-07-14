#ifndef PLAYER_YANPRO
#define PLAYER_YANPRO

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "Object.h"
#include "utils.h"

namespace game
{

class Player :
    public Object,
    public std::enable_shared_from_this<Player>
{
public:
    [[nodiscard]] static std::shared_ptr<Object> create(
            std::size_t id,
            std::uint32_t team_mask,
            int x,
            int y,
            int hp,
            int shield,
            int attack,
            std::size_t min_radius,
            std::uint32_t color,
            int acceleration,
            int max_speed,
            std::shared_ptr<Gun> gun = nullptr)
    {
        auto ptr = std::make_shared<Player>(id, team_mask, x, y, hp, shield, attack, min_radius, color, acceleration, max_speed);
        if (gun)
        {
            //gun->bind_to_owner(ptr);
            ptr->gun_ = std::move(gun);
        }

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
                if (other.hp_ == 0)
                {
                    other.is_dead_ = true;
                }
            }
        }
    }

    virtual void move(float dt, Point target) noexcept override
    {
        float ax = 0.0;
        float ay = 0.0;

        if (target != get_xy())
        {
            float a_over_d = acceleration_ / dist({x_, y_}, target);

            ax = (target.x - x_) * a_over_d;
            ay = (target.y - y_) * a_over_d;
        }

        float p = dt * dt / 2;

        x_ += dx_ * dt + ax * p;
        y_ += dy_ * dt + ay * p;

        dx_ += ax * dt;
        dy_ += ay * dt;

        int v = std::hypot(dx_, dy_);
        if (v > max_speed_)
        {
            dx_ = dx_ * max_speed_ / v;
            dy_ = dy_ * max_speed_ / v;
        }

        int r = get_radius();

        x_ = std::clamp(x_, r, SCREEN_WIDTH - r - 1);
        y_ = std::clamp(y_, r, SCREEN_HEIGHT - r - 1);
    }

    virtual std::size_t get_radius() const noexcept override
    {
        return std::max<std::size_t>(min_radius_, std::sqrt(hp_));
    }

    std::pair<int, int> get_dxdy() const noexcept
    {
        return {dx_, dy_};
    }

public:
Player():Object(0,0,0,0,0,0,0,0,0){}
//private:
    Player(std::size_t id, std::uint32_t team_mask, int x, int y, int hp, int shield, int attack, std::size_t min_radius, std::uint32_t color, int acceleration, int max_speed) :
        Object(id, team_mask, x, y, hp, shield, attack, min_radius, color),
        acceleration_{acceleration},
        max_speed_{max_speed}
    {
        int r = get_radius();
        x_ = std::clamp(x_, r, SCREEN_WIDTH - r - 1);
        y_ = std::clamp(y_, r, SCREEN_HEIGHT - r - 1);

    }

    int dx_ = 0;
    int dy_ = 0;
    int acceleration_;
    int max_speed_;
};

}

#endif

