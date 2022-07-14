#ifndef BONUS_YANPRO
#define BONUS_YANPRO

#include <cstddef>
#include <cstdint>

#include "Object.h"
#include "utils.h"

namespace game
{

class Bonus : public Object
{
public:
    virtual void move(float dt, Point target) noexcept override
    {
    }

    virtual std::size_t get_radius() const noexcept override
    {
        return min_radius_;
    }

protected:
    Bonus(std::size_t id, std::uint32_t team_mask, int x, int y, int add_hp, std::size_t min_radius, std::uint32_t color) :
        Object(id, team_mask, x, y, 1, 0, add_hp, min_radius, color)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class HPBonus : public Bonus
{
public:
    [[nodiscard]] static std::shared_ptr<Object> create(
            std::size_t id,
            std::uint32_t team_mask,
            int x,
            int y,
            int add_hp,
            std::size_t min_radius,
            std::uint32_t color)
    {
        auto ptr = std::shared_ptr<HPBonus>(new HPBonus(id, team_mask, x, y, add_hp, min_radius, color));
        return std::static_pointer_cast<Object>(ptr);
    }

    virtual void collide(Object& other) noexcept override
    {
        if (other.id_ == 0)
        {
            other.hp_ += attack_;
            is_dead_ = true;
        }
    }

private:
    HPBonus(std::size_t id, std::uint32_t team_mask, int x, int y, int add_hp, std::size_t min_radius, std::uint32_t color) :
        Bonus(id, team_mask, x, y, add_hp, min_radius, color)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class ShieldBonus : public Bonus
{
public:
    [[nodiscard]] static std::shared_ptr<Object> create(
            std::size_t id,
            std::uint32_t team_mask,
            int x,
            int y,
            int add_shield,
            std::size_t min_radius,
            std::uint32_t color)
    {
        auto ptr = std::shared_ptr<ShieldBonus>(new ShieldBonus(id, team_mask, x, y, add_shield, min_radius, color));
        return std::static_pointer_cast<Object>(ptr);
    }

    virtual void collide(Object& other) noexcept override
    {
        if (other.id_ == 0)
        {
            other.shield_ += attack_;
            is_dead_ = true;
        }
    }

private:
    ShieldBonus(std::size_t id, std::uint32_t team_mask, int x, int y, int add_shield, std::size_t min_radius, std::uint32_t color) :
        Bonus(id, team_mask, x, y, add_shield, min_radius, color)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class GunBonus : public Bonus
{
protected:
    GunBonus(std::size_t id, std::uint32_t team_mask, int x, int y, int add_shield, std::size_t min_radius, std::uint32_t color) :
        Bonus(id, team_mask, x, y, add_shield, min_radius, color)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class GunAttackBonus : public GunBonus
{
public:
    [[nodiscard]] static std::shared_ptr<Object> create(
            std::size_t id,
            std::uint32_t team_mask,
            int x,
            int y,
            int percent,
            std::size_t min_radius,
            std::uint32_t color)
    {
        auto ptr = std::shared_ptr<GunAttackBonus>(new GunAttackBonus(id, team_mask, x, y, percent, min_radius, color));
        return std::static_pointer_cast<Object>(ptr);
    }

    virtual void collide(Object& other) noexcept override
    {
        if (other.id_ == 0)
        {
            other.gun_->change_attack(attack_);
            is_dead_ = true;
        }
    }

private:
    GunAttackBonus(std::size_t id, std::uint32_t team_mask, int x, int y, int percent, std::size_t min_radius, std::uint32_t color) :
        GunBonus(id, team_mask, x, y, percent, min_radius, color)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class GunRadiusBonus : public GunBonus
{
public:
    [[nodiscard]] static std::shared_ptr<Object> create(
            std::size_t id,
            std::uint32_t team_mask,
            int x,
            int y,
            int percent,
            std::size_t min_radius,
            std::uint32_t color)
    {
        auto ptr = std::shared_ptr<GunRadiusBonus>(new GunRadiusBonus(id, team_mask, x, y, percent, min_radius, color));
        return std::static_pointer_cast<Object>(ptr);
    }

    virtual void collide(Object& other) noexcept override
    {
        if (other.id_ == 0)
        {
            other.gun_->change_radius(attack_);
            is_dead_ = true;
        }
    }

private:
    GunRadiusBonus(std::size_t id, std::uint32_t team_mask, int x, int y, int percent, std::size_t min_radius, std::uint32_t color) :
        GunBonus(id, team_mask, x, y, percent, min_radius, color)
    {
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class GunCooldownBonus : public GunBonus
{
public:
    [[nodiscard]] static std::shared_ptr<Object> create(
            std::size_t id,
            std::uint32_t team_mask,
            int x,
            int y,
            int percent,
            std::size_t min_radius,
            std::uint32_t color)
    {
        auto ptr = std::shared_ptr<GunCooldownBonus>(new GunCooldownBonus(id, team_mask, x, y, percent, min_radius, color));
        return std::static_pointer_cast<Object>(ptr);
    }

    virtual void collide(Object& other) noexcept override
    {
        if (other.id_ == 0)
        {
            other.gun_->change_cooldown(attack_);
            is_dead_ = true;
        }
    }

private:
    GunCooldownBonus(std::size_t id, std::uint32_t team_mask, int x, int y, int percent, std::size_t min_radius, std::uint32_t color) :
        GunBonus(id, team_mask, x, y, percent, min_radius, color)
    {
    }
};

}

#endif

