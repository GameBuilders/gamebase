#ifndef OBJECTS_H
#define OBJECTS_H

// c-style includes
#include <stdio.h>
#include <stdlib.h>

// c++ style includes
#include <string>
#include <iostream>

// local includes
#include "bullet.h"

//main
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <math.h>

class Object
{
    public:
        //basic functions
        Object();

        Object(float x, float y);

        ~Object();

        //moving functions
        void move_object();

        void change_delta(float new_delta_x, float new_delta_y);

        void change_position(float new_x, float new_y);

        //damage functions
        bool hit();//check if objects hit by bullet

        bool hit_enemy();

        //add/delete functions
        void add_object();

        void clear_object();//not implemented

        //position function
        float get_x();

        float get_y();

        //public storage
        sf::Time obj_timer;

        bool entry_flag;

        bool bullet_flag;

        int bullet_num;

        int hp;

        std::string B_type;

        float en_x;

        float en_y;

        bool fire;
    private:
        float _x;        //x pos of the bullet
        float _y;        //y pos of the bullet
        float _raduis;   //area of a object
        float _delta_x;  //speed in x direction
        float _delta_y;  //speed in y direction


};
#endif // EPNG_H
