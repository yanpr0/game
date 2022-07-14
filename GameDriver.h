#ifndef GAMEDRIVER_YANPRO
#define GAMEDRIVER_YANPRO

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Engine.h"
#include "Bonus.h"
#include "Gun.h"
#include "Player.h"
#include "Projectile.h"
#include "utils.h"

namespace game
{

class GameDriver
{
public:
    GameDriver(const char* config_file)
    {
        std::map<std::string, int> params;
        std::ifstream is(config_file);
        std::string key;
        int val;
        while (is >> key >> val)
        {
            params[key] = val;
        }

        /////////////////////

        min_radius_ = params["min_radius"];

        player_hp_ = params["player_hp"];
        player_shield_ = params["player_shield"];
        player_attack_ = params["player_attack"];
        player_acceleration_ = params["player_acceleration"];
        player_max_speed_ = params["player_max_speed"];

        projectile_radius_ = params["projectile_radius"];
        projectile_speed_ = params["projectile_speed"];

        gun_attack_ = params["gun_attack"];
        gun_cooldown_ = params["gun_cooldown"] / 1000.0;

        act_duration_ = 1.0 / params["fps"];

        prespawn_duration_ = params["prespawn_duration"] / 1000.0;
        spawn_duration_ = params["spawn_duration"] / 1000.0;

        player_color_ = params["player_color"];
        projectile_color_ = params["projectile_color"];
        enemy_color_ = params["enemy_color"];
        hp_bonus_color_ = params["hp_bonus_color"];
        shield_bonus_color_ = params["shield_bonus_color"];
        gun_attack_bonus_color_ = params["gun_attack_bonus_color"];
        gun_cooldown_bonus_color_ = params["gun_cooldown_bonus_color"];

        hp_bonus_ = params["hp_bonus"];
        shield_bonus_ = params["shield_bonus"];
        gun_attack_bonus_ = params["gun_attack_bonus"];
        gun_cooldown_bonus_ = params["gun_cooldown_bonus"];

        enemy_hp_min_ = params["enemy_hp_min"];
        enemy_hp_min_prob_ = params["enemy_hp_min_prob"];
        enemy_hp_mid_ = params["enemy_hp_mid"];
        enemy_hp_mid_prob_ = params["enemy_hp_mid_prob"];
        enemy_hp_max_ = params["enemy_hp_max"];
        enemy_hp_max_prob_ = params["enemy_hp_max_prob"];

        enemy_attack_min_ = params["enemy_attack_min"];
        enemy_attack_var_ = params["enemy_attack_var"];

        enemy_acceleration_min_ = params["enemy_acceleration_min"];
        enemy_acceleration_var_ = params["enemy_acceleration_var"];

        enemy_max_speed_min_ = params["enemy_max_speed_min"];
        enemy_max_speed_var_ = params["enemy_max_speed_var"];

        bonus_prob_ = params["bonus_prob"];

        warning_radius_ = params["warning_radius"];

        /////////////////////

        objects_.reserve(1000);

        gun_ = std::make_shared<Gun>(projectile_speed_, projectile_radius_, projectile_color_, gun_attack_, gun_cooldown_);

        objects_.push_back(
            Player::create(
                0,
                1,
                SCREEN_WIDTH / 2,
                SCREEN_HEIGHT / 2,
                player_hp_,
                player_shield_,
                player_attack_,
                min_radius_,
                player_color_,
                player_acceleration_,
                player_max_speed_,
                gun_
            )
        );
    }

    void act(float dt)
    {
        if (is_key_pressed(VK_SPACE) && !gameover_)
        {
            pause_ = !pause_;
        }

        if (is_key_pressed(VK_ESCAPE))
        {
            if (pause_ || gameover_)
            {
                schedule_quit_game();
            }
            else
            {
                pause_ = true;
            }
        }

        if (!pause_)
        {
            const int d = 1000;
            if (is_key_pressed(VK_LEFT))
            {
                target_x_ -= d;
            }
            if (is_key_pressed(VK_RIGHT))
            {
                target_x_ += d;
            }
            if (is_key_pressed(VK_UP))
            {
                target_y_ -= d;
            }
            if (is_key_pressed(VK_DOWN))
            {
                target_y_ += d;
            }

            total_time_ += dt;

            act_time_ += dt;

            if (act_time_ >= act_duration_)
            {
                if (objects_[0]->is_dead())
                {
                    gameover_ = true;
                    return;
                }

                int j = 1;
                for (int i = 1; i < objects_.size(); ++i)
                {
                    if (!objects_[i]->is_dead())
                    {
                        if (j < i)
                        {
                            objects_[j] = std::move(objects_[i]);
                        }
                        ++j;
                    }
                }
                objects_.resize(j);

                for (std::size_t i = 0; i < objects_.size(); ++i)
                {
                    for (std::size_t j = i + i; j < objects_.size(); ++j)
                    {
                        auto [x1, y1] = objects_[i]->get_xy();
                        auto [x2, y2] = objects_[j]->get_xy();

                        auto dist_sqr = sqr(x2 - x1) + sqr(y2 - y1);
                        auto r1 = objects_[i]->get_radius();
                        auto r2 = objects_[j]->get_radius();

                        if (dist_sqr < sqr(r1 + r2))
                        {
                            //objects_[i]->color_=255;
                            objects_[i]->collide(*objects_[j]);
                            objects_[j]->collide(*objects_[i]);
                        }
                    }
                }

                Point player_xy = objects_[0]->get_xy();
                objects_[0]->move(act_time_, {player_xy.x + target_x_, player_xy.y + target_y_});

                for (std::size_t i = 1; i < objects_.size(); ++i)
                {
                    objects_[i]->move(act_time_, player_xy);
                }

                act_time_ = 0.0;
            }

            cooldown_time_ += dt;
            auto gun_cooldown = gun_->get_cooldown();

            if (cooldown_time_ >= gun_cooldown)
            {
                Point target = {get_cursor_x(), get_cursor_y()};

                auto [x, y] = objects_[0]->get_xy();
                auto [dx, dy] = std::static_pointer_cast<Player>(objects_[0])->get_dxdy();
                float ratio = projectile_speed_ / dist(target, Point{x, y});
                int vx = (target.x - x) * ratio;
                int vy = (target.y - y) * ratio;

                auto projectile = Projectile::create(
                    1,
                    1,
                    x,
                    y,
                    gun_->get_attack(),
                    projectile_radius_,
                    projectile_color_,
                    vx + dx,
                    vy + dy
                );

                objects_.push_back(projectile);

                std::putchar('\a'); // epic sound effects
                std::fflush(stdout);

                cooldown_time_ -= gun_cooldown;
            }

            spawn_time_ += dt;

            if (spawn_time_ >= prespawn_duration_)
            {
                if (spawn_time_ < spawn_duration_ && spawn_.empty())
                {
                    int wave_size = std::ceil(std::log(total_time_ + 1));
                    printf("%d\n", wave_size);
                    for (int i = 0; i < wave_size; ++i)
                    {
                        int test = std::rand() % 100;

                        int hp = 0;

                        if (test < enemy_hp_min_prob_)
                        {
                            hp = enemy_hp_min_;
                        }
                        else if (test < enemy_hp_min_prob_ + enemy_hp_mid_prob_)
                        {
                            hp = enemy_hp_mid_;
                        }
                        else
                        {
                            hp = enemy_hp_max_;
                        }

                        int attack = enemy_attack_min_ + std::rand() % enemy_attack_var_;
                        int acceleration = enemy_acceleration_min_ + std::rand() % enemy_acceleration_var_;
                        int max_speed = enemy_max_speed_min_ + std::rand() % enemy_max_speed_var_;

                        spawn_.push_back(
                            Player::create(
                                1,
                                1 << 1,
                                std::rand() % SCREEN_WIDTH,
                                std::rand() % SCREEN_HEIGHT,
                                hp,
                                0,
                                attack,
                                min_radius_,
                                enemy_color_,
                                acceleration,
                                max_speed
                            )
                        );
                    }

                    if (std::rand() % 100 < bonus_prob_)
                    {
                        std::shared_ptr<Object> bonus = nullptr;
                        int bonus_type_count = 4;
                        int r = std::rand() / (RAND_MAX / bonus_type_count);
                        switch (r)
                        {
                            case 0: bonus = HPBonus::create(
                                            1,
                                            1 | (1 << 1),
                                            std::rand() % SCREEN_WIDTH,
                                            std::rand() % SCREEN_HEIGHT,
                                            hp_bonus_,
                                            min_radius_,
                                            hp_bonus_color_);
                                    break;
                            case 1: bonus = ShieldBonus::create(
                                            1,
                                            1 | (1 << 1),
                                            std::rand() % SCREEN_WIDTH,
                                            std::rand() % SCREEN_HEIGHT,
                                            shield_bonus_,
                                            min_radius_,
                                            shield_bonus_color_);
                                    break;
                            case 2: bonus = GunAttackBonus::create(
                                            1,
                                            1 | (1 << 1),
                                            std::rand() % SCREEN_WIDTH,
                                            std::rand() % SCREEN_HEIGHT,
                                            gun_attack_bonus_,
                                            min_radius_,
                                            gun_attack_bonus_color_);
                                    break;
                            case 3: bonus = GunCooldownBonus::create(
                                            1,
                                            1 | (1 << 1),
                                            std::rand() % SCREEN_WIDTH,
                                            std::rand() % SCREEN_HEIGHT,
                                            gun_cooldown_bonus_,
                                            min_radius_,
                                            gun_cooldown_bonus_color_);
                                    break;
                        }
                        spawn_.push_back(std::move(bonus));
                    }
                }
                else
                {
                    objects_.insert(objects_.end(), spawn_.begin(), spawn_.end());
                    spawn_.clear();
                    spawn_time_ = 0.0;
                }
            }
        }
    }

    void draw()
    {
        memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(std::uint32_t));

        for (auto& obj : objects_)
        {
            int r = obj->get_radius();
            auto [obj_x, obj_y] = obj->get_xy();
            auto c = obj->get_color();
            for (int i = -r; i <= r; ++i)
            {
                for (int j = -r; j <= r; ++j)
                {
                    int x = obj_x + i;
                    int y = obj_y + j;
                    if (is_between(x, 0, SCREEN_WIDTH - 1) &&
                        is_between(y, 0, SCREEN_HEIGHT - 1) &&
                        sqr(i) + sqr(j) <= sqr(r))
                    {
                        buffer[y][x] = c;
                    }
                }
            }
        }

        for (auto& obj : spawn_)
        {
            int r = warning_radius_;
            auto [obj_x, obj_y] = obj->get_xy();
            for (int i = -r; i <= r; ++i)
            {
                for (int j = -r; j <= r; ++j)
                {
                    int x = obj_x + i;
                    int y = obj_y + j;
                    if (is_between(x, 0, SCREEN_WIDTH - 1) &&
                        is_between(y, 0, SCREEN_HEIGHT - 1) &&
                        sqr(i) + sqr(j) <= sqr(r))
                    {
                        buffer[y][x] = (1 << 24) - 1;
                    }
                }
            }
        }

    }

private:
    std::vector<std::shared_ptr<Object>> objects_;
    std::vector<std::shared_ptr<Object>> spawn_;
    std::shared_ptr<Gun> gun_;
    bool pause_ = false;
    bool gameover_ = false;

    int target_x_ = 0;
    int target_y_ = 0;
    float total_time_ = 0.0;
    float act_time_ = 0.0;
    float cooldown_time_ = 0.0;
    float spawn_time_ = 0.0;

    std::size_t min_radius_;

    int player_hp_;
    int player_shield_;
    int player_attack_;
    int player_acceleration_;
    int player_max_speed_;

    int projectile_radius_;
    int projectile_speed_;

    int gun_attack_;
    float gun_cooldown_;

    float act_duration_;

    float prespawn_duration_;
    float spawn_duration_;

    std::uint32_t player_color_;
    std::uint32_t projectile_color_;
    std::uint32_t enemy_color_;
    std::uint32_t hp_bonus_color_;
    std::uint32_t shield_bonus_color_;
    std::uint32_t gun_attack_bonus_color_;
    std::uint32_t gun_cooldown_bonus_color_;

    int hp_bonus_;
    int shield_bonus_;
    int gun_attack_bonus_;
    int gun_cooldown_bonus_;

    int enemy_hp_min_;
    int enemy_hp_min_prob_;
    int enemy_hp_mid_;
    int enemy_hp_mid_prob_;
    int enemy_hp_max_;
    int enemy_hp_max_prob_;

    int enemy_attack_min_;
    int enemy_attack_var_;

    int enemy_acceleration_min_;
    int enemy_acceleration_var_;

    int enemy_max_speed_min_;
    int enemy_max_speed_var_;

    int bonus_prob_;

    int warning_radius_;
};

}

#endif

