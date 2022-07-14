#ifndef GUN_YANPRO
#define GUN_YANPRO

#include <cstddef>
#include <cstdint>
#include <memory>

#include "utils.h"
//#include "Object.h"
//#include "Player.h"
//#include "Projectile.h"

namespace game
{

/*class Object;
class Player;
class Projectile;
*/
class Gun
{
public:
    Gun(std::size_t projectile_speed, int projectile_radius, std::uint32_t projectile_color, std::size_t attack, float cooldown) :
        projectile_speed_{projectile_speed},
        projectile_radius_{projectile_radius},
        projectile_color_{projectile_color},
        attack_{attack},
        cooldown_{cooldown}
    {
    }

    /*std::shared_ptr<Object> shoot(Point target)
    {
        auto [x, y] = owner_.lock()->get_xy();
        auto [dx, dy] = owner_.lock()->get_dxdy();
        float ratio = projectile_speed_ / dist(target, Point{x, y});
        int vx = (target.x - x) * ratio;
        int vy = (target.y - y) * ratio;

        return Projectile::create(
            1,
            1,
            x,
            y,
            attack_,
            projectile_radius_,
            projectile_color_,
            vx + dx,
            vy + dy
        );
    }

    void bind_to_owner(std::shared_ptr<Player> owner)
    {
        owner_ = owner->weak_from_this();
    }*/

    void change_attack(int percent) noexcept
    {
        attack_ = attack_ * (100 + percent) / 100;
    }

    void change_radius(int percent) noexcept
    {
        radius_ = radius_ * (100 + percent) / 100;
    }

    void change_cooldown(int percent) noexcept
    {
        cooldown_ = cooldown_ * (100 + percent) / 100;
    }

    float get_cooldown() const noexcept
    {
        return cooldown_;
    }

    int get_attack() const noexcept
    {
        return attack_;
    }

private:
    //std::weak_ptr<Player> owner_;

    std::size_t projectile_speed_;
    int projectile_radius_;
    std::uint32_t projectile_color_;
    std::size_t attack_;
    std::size_t radius_;
    float cooldown_;
};

}

#endif

