#ifndef OBJECT_YANPRO
#define OBJECT_YANPRO

#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include "Gun.h"
#include "utils.h"

namespace game
{

class Object
{
public:
    virtual ~Object() = default;

    virtual void collide(Object& other) noexcept = 0;

    virtual void move(float dt, Point target) noexcept = 0;

    virtual std::size_t get_radius() const noexcept = 0;

    std::size_t get_id() const noexcept { return id_; }
    std::size_t get_team_mask() const noexcept { return team_mask_; }
    int get_x() const noexcept { return x_; }
    int get_y() const noexcept { return y_; }
    Point get_xy() const noexcept { return {x_, y_}; }
    bool is_dead() const noexcept { return is_dead_; }
    std::uint32_t get_color() const noexcept { return is_dead_ ? (1 << 24) - 1 : color_; }

    //virtual std::size_t get_hp() const noexcept { return 0; }
    //virtual std::size_t get_shield() const noexcept { return 0; }
    //virtual std::size_t get_attack() const noexcept { return 0; }

//protected:
    Object(std::size_t id, std::uint32_t team_mask, int x, int y, int hp, int shield, int attack, std::size_t min_radius, std::uint32_t color) :
        id_{id},
        team_mask_{team_mask},
        x_{x},
        y_{y},
        hp_{hp},
        shield_{shield},
        attack_{attack},
        min_radius_{min_radius},
        color_{color}
    {
    }

    std::size_t id_;
    std::uint32_t team_mask_;
    int x_ = 0;
    int y_ = 0;
    int hp_ = 0;
    int shield_ = 0;
    int attack_ = 0;
    std::size_t min_radius_ = 0;
    std::uint32_t color_ = 0;
    std::shared_ptr<Gun> gun_;
    bool is_dead_ = false;
};

}

#endif

