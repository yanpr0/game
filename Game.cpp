#include "Engine.h"
#include <stdlib.h>
#include <memory.h>

#include <stdio.h>

#include <vector>
#include <utility>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  schedule_quit_game() - quit game after act()


//#include "Object.h"
//#include "Player.h"
//#include "Projectile.h"
//#include "utils.h"

//game::Player p;
//std::vector<game::Player> q;
//std::vector<game::Projectile> fire;

#include "GameDriver.h"

game::GameDriver gd("config");

// initialize game data in this function
void initialize()
{
    //Player p;
    /*p.acceleration_ = 1000;
    p.max_speed_ = 450;
    p.x_ = SCREEN_WIDTH / 2;
    p.y_ = SCREEN_HEIGHT / 2;

    q.resize(5);
    int i = 0;
    for (auto& e : q)
    {
        e.acceleration_ = 500 + i * 50;
        e.max_speed_ = 200 + i * 5;
        e.x_ = i * 100;
        e.y_ = SCREEN_HEIGHT - 1;
        i++;
    }*/
}

/*std::vector<std::pair<int, int>> a;
int x = 0;
int dx = 350;
float t = 0;
float fire_t = 0;
int tx = 0;
int ty = 0;*/

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt)
{
    gd.act(dt);
//  if (is_key_pressed(VK_ESCAPE))
//    schedule_quit_game();
//
//  if (is_key_pressed(VK_RETURN))
//    a.clear();
//
//    //auto [x, y] = p.get_xy();
//    if (is_key_pressed(VK_LEFT))
//        tx -= 10000;
//    if (is_key_pressed(VK_RIGHT))
//        tx += 10000;
//    if (is_key_pressed(VK_UP))
//        ty -= 10000;
//    if (is_key_pressed(VK_DOWN))
//        ty += 10000;
//
//    t += dt;
//    if (t > 0.017)
//    {
//        auto tg = p.get_xy();
//        for (auto& e : q)
//        {
//            e.move(t, tg);
//        }
//        auto ppp = p.get_xy();
//        p.move(t, {tx + ppp.x, ty + ppp.y});
//        tx = 0;
//        ty = 0;
//
//        for (auto& e:fire)
//            e.move(t, tg);
//
//        x += dx * t;
//        x %= SCREEN_WIDTH;
//        t = 0.0;
//    }
//
//    fire_t += dt;
//    if (fire_t >=0.1)
//    {
//        fire_t = 0;
//        fire.emplace_back();
//        fire.back().dx_ = (get_cursor_x() - p.x_) * 2;
//        fire.back().dy_ = (get_cursor_y() - p.y_) * 2;
//        fire.back().x_ = p.x_;
//        fire.back().y_ = p.y_;
//        std::putchar('\a');
//        std::fflush(stdout);
//        if (fork() == 0)
//        {
//            execl("/usr/bin/aplay", "aplay", "fire.wav", NULL);
//            exit(0);
//        }
//    }
//
//    auto [x, y] = p.get_xy();
//    if (is_key_pressed(VK_LEFT))
//        x -= 10000;
//    if (is_key_pressed(VK_RIGHT))
//        x += 10000;
//    if (is_key_pressed(VK_UP))
//        y -= 10000;
//    if (is_key_pressed(VK_DOWN))
//        y += 10000;
//
//    auto t = p.get_xy();
//    for (auto& e : q)
//    {
//        e.move(dt * 10, t);
//    }
//    p.move(dt * 10, {x, y});




//    if (is_mouse_button_pressed(0))
//    {
//      a.emplace_back(get_cursor_x(), get_cursor_y());
//    }
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw()
{
    gd.draw();
  // clear backbuffer
  /*memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

  {
    auto [x, y] = p.get_xy();
    for (int i = -5; i <= 5; ++i)
        for (int j = -5; j <= 5; ++j)
        {
            int a = std::clamp(x + i, 0, SCREEN_WIDTH - 1);
            int b = std::clamp(y + j, 0, SCREEN_HEIGHT - 1);
            buffer[b][a] = (1 << 24) - 1;
        }

    for (auto& e: q)
    {
        auto [x, y] = e.get_xy();
        for (int i = -5; i <= 5; ++i)
            for (int j = -5; j <= 5; ++j)
            {
                int a = std::clamp(x + i, 0, SCREEN_WIDTH - 1);
                int b = std::clamp(y + j, 0, SCREEN_HEIGHT - 1);
                buffer[b][a] = (1 << 16) - 1;
            }
    }

    for (auto& e: fire)
    {
        auto [x, y] = e.get_xy();
        for (int i = -2; i <= 2; ++i)
            for (int j = -2; j <= 2; ++j)
            {
                if (x > 3 && x < SCREEN_WIDTH - 3 && y > 3 && y < SCREEN_HEIGHT -3)
                {
                    int a = std::clamp(x + i, 0, SCREEN_WIDTH - 1);
                    int b = std::clamp(y + j, 0, SCREEN_HEIGHT - 1);
                    buffer[b][a] = 255 * 256 * 256 + 255 * 256;
                }
            }
    }

  }
*/


//  for (auto [x, y] : a)
//  {
//      if (0 <= x && x < SCREEN_HEIGHT - 1 && 0 <= y && y < SCREEN_HEIGHT - 1)
//      {
//        buffer[x][y] = 255;
//        buffer[x + 1][y] = 255;
//        buffer[x][y + 1] = 255;
//        buffer[x + 1][y + 1] = 255;
//
//        buffer[y][x] = 255 * 256 * 256;
//        //buffer[y + 1][x] = 255 * 256 * 256;
//        //buffer[y][x + 1] = 255 * 256 * 256;
//        //buffer[y + 1][x + 1] = 255 * 256 * 256;
//      }
//  }
//
//  for (int k = 0; k < SCREEN_HEIGHT / 10; ++k)
//  for (int i = k * 10; i < k * 10 + 10; ++i)
//  {
//    buffer[i][x + 10 * k] = 255 * 256 * 256;(1 << 24) - 1;
//  }
//
//  for (int i = 0; i < 256; ++i)
//  {
//    for (int j = 0; j < 10; ++j)
//    {
//        buffer[j + 50][i] = (i) * 256 + (255 - i) * 256 * 256;
//    }
//  }
//
//  int n=25;
//  for (int i = 0; i < n; ++i)
//    {
//        for (int j = 0; j < n; ++j)
//        {
//            buffer[i+100][j+100] = (1<<24) -1;
//        }
//    }
}

// free game data in this function
void finalize()
{
}

