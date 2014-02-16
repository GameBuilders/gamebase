#ifndef LEVEL_H
#define LEVEL_H

// local includes
#include "objects.h"
#include <algorithm>

using namespace std;

class Level
{
    public:
        //basic functions
        Level();

        ~Level();

        void add_enemy(string type, sf::Time entry_time,
                      float en_x, float en_y, float del_x, float del_y, int bullet_num, string B_type, int hp);

        void add_bullets(int bullet_num, string B_type, float x, float y, int index);

        void add_bullet_player();

        void move_stuff(sf::Time cur_time);

        //damage judging
        void damage_judging();

        std::vector<Object> enemy;
        int max_enemy_size;

        std::vector<Bullet> bullet;
        int max_bullet_size;

        std::vector<Bullet> player_bullet;

        Object player;
};
#endif // EPNG_H
