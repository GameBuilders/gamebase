#include "bullet.h"

Bullet::Bullet(){
    _x = 0;
    _y = 0;
    _delta_x = 0;
    _delta_y = 0;
    player = false;
    _radius = 3;
}

Bullet::~Bullet(){

}

Bullet::Bullet(float x, float y){
    _x = x;
    _y = y;
    _delta_x = 0;
    _delta_y = 0;
    player = false;
    _radius = 3;
}

void Bullet::move_bullet(){
    _x += _delta_x;
    _y += _delta_y;
}

void Bullet::change_delta(float new_delta_x, float new_delta_y){
    _delta_x = new_delta_x;
    _delta_y = new_delta_y;
}

void Bullet::change_position(float new_x, float new_y){
    _x = new_x;
    _y = new_y;
}

bool Bullet::hit(){
    //modifiy me later!
    return false;
}

bool Bullet::hit_enemy(){
    return false;
}

void Bullet::add_bullet(){

}

void Bullet::clear_bullet(){

}

float Bullet::get_x(){
    return _x;
}

float Bullet::get_y(){
    return _y;
}

float Bullet::get_radius(){
   return _radius;
}
