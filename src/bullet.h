#ifndef BULLET_H
#define BULLET_H

// c-style includes
#include <stdio.h>
#include <stdlib.h>

// c++ style includes
#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>
// local includes

/*Two knids of bullets, one shot from player, other one from enemies
*/
class Bullet
{
    public:
        //basic functions
        Bullet();

        Bullet(float x, float y);

        ~Bullet();

        //moving functions
        void move_bullet();

        void change_delta(float new_delta_x, float new_delta_y);

        void change_position(float new_x, float new_y);

        //damage functions
        bool hit();//check if main character hit by enemy bullet

        bool hit_enemy();

        //add/delete functions
        void add_bullet();

        void clear_bullet();//not implemented

        //position function
        float get_x();

        float get_y();

        float get_radius();

        bool player;
    private:
        float _x;        //x pos of the bullet
        float _y;        //y pos of the bullet
        float _radius;   //size of the bullet
        float _delta_x;  //speed in x direction
        float _delta_y;  //speed in y direction
        //sf::Texture texture;//appearance of bullet


};

#endif // EPNG_H
