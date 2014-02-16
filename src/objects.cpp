#include "objects.h"

Object::Object(){
    _x = 0;
    _y = 0;
    _delta_x = 0;
    _delta_y = 0;
    entry_flag = false;
    bullet_flag = true;
    fire = false;
}

Object::~Object(){

}

Object::Object(float x, float y){
    _x = x;
    _y = y;
    _delta_x = 0;
    _delta_y = 0;
    entry_flag = false;
    bullet_flag = true;
    fire = false;
}

void Object::move_object(){
    _x += _delta_x;
    _y += _delta_y;
}

void Object::change_delta(float new_delta_x, float new_delta_y){
    _delta_x = new_delta_x;
    _delta_y = new_delta_y;
}

void Object::change_position(float new_x, float new_y){
    _x = new_x;
    _y = new_y;
}

bool Object::hit(){
    //modifiy me later!
    return false;
}

bool Object::hit_enemy(){
    return false;
}

void Object::add_object(){

}

void Object::clear_object(){

}

float Object::get_x(){
    return _x;
}

float Object::get_y(){
    return _y;
}
