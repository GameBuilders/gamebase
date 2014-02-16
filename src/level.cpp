#include "level.h"

const int max_width = 800;
const int max_height = 600;

float player_b_radius = 10;
sf::Clock player_clock;

Level::Level(){
    max_enemy_size = 0;
    max_bullet_size = 0;

    enemy.resize(0);
    bullet.resize(0);
    player_bullet.resize(0);

    player.change_position(400, 500);
}

Level::~Level(){

}

//String Type(enemy type):
//    "n"
//    "Boss"

//ONLY add enemy one by one!
void Level::add_enemy(string type, sf::Time entry_time,
        float en_x, float en_y, float del_x, float del_y, int bullet_num, string B_type, int hp){

    int orig_size = max_enemy_size;
    max_enemy_size += 1;
    enemy.resize(max_enemy_size);

    for (int i = orig_size; i < max_enemy_size; i++){
        enemy[i].change_position(en_x,en_y);
        enemy[i].change_delta(del_x, del_y);
        enemy[i].obj_timer = entry_time;
        enemy[i].bullet_num = bullet_num;
        enemy[i].B_type = B_type;
        enemy[i].en_x = en_x;
        enemy[i].en_y = en_y;
        enemy[i].hp = hp;
    }

    //add new bullets when enemy enters the field
}

//String B_Type(bullet_type):
//    "cir"
//    "eli"
//    "burst"
//    "bomb"
//    "dir_bomb"
//    "search"

void Level::add_bullets(int bullet_num, string B_type, float x, float y, int index){
    srand (time(NULL));

    int orig_size = max_bullet_size;
    max_bullet_size += bullet_num;
    bullet.resize(max_bullet_size);

    for (int i = orig_size; i < max_bullet_size; i++){
        bullet[i].change_position(enemy[index].get_x()+20*sin(i),enemy[index].get_y()+20*cos(i));

        //decide the pattern according to type
        if (B_type == "cir")
            bullet[i].change_delta((float)((sin(i)+0.1)/10), (float)((cos(i)+0.1)/10)); //circle
        if (B_type == "burst")
            bullet[i].change_delta(static_cast <float> ((rand()) / static_cast <float> (RAND_MAX))/5, static_cast <float> ((rand()) / static_cast <float> (RAND_MAX))/5);//random from 0 to 0.1
        if (B_type == "bomb")
            bullet[i].change_delta(0, (float)((cos(i)+0.1)/10));// directly down
        if (B_type == "dir_bomb")
            bullet[i].change_delta(0, (float)((cos(i)+1.1)/10));// directly down
        //bullet_v[i].change_delta((float)(rand()%200)/400, (float)(rand()%200)/400);   //complete random
        //bullet_v[i].change_delta((float)(sin(rand()%10+1)/50), (float)(cos(rand()%10+1)/50)); //4-block random
    }
}
void Level::add_bullet_player(){   //add bullet to player
    if ( (player_clock.getElapsedTime() >= sf::seconds(0.1f))&&(player.fire == true) ){
        //player.fire = false;
        player_clock.restart();

        int orig_size_p = player_bullet.size();
        player_bullet.resize(orig_size_p+5);
        for (unsigned int j = orig_size_p; j < player_bullet.size(); j++){
            player_bullet[j].change_position(player.get_x(),player.get_y()+0.005*j);

            player_bullet[j].change_delta(0, -0.5);// directly down
        }
    }
}

void Level::move_stuff(sf::Time cur_time){
    //first, move bullets
    for (int i = 0; i < max_bullet_size; i++){
        if ((bullet[i].get_x() > max_width+10)||(bullet[i].get_y() > max_width+10)){
            bullet.erase(bullet.begin()+i);
            max_bullet_size = bullet.size();
        }
        else
            bullet[i].move_bullet();
    }
    max_bullet_size = bullet.size();

    //Second, move player bullets
    for (unsigned int k = 0; k < player_bullet.size(); k++){
        if ((player_bullet[k].get_x() > max_width+10)||(player_bullet[k].get_y() > max_width+10)){
            player_bullet.erase(player_bullet.begin()+k);
        }
        else
            player_bullet[k].move_bullet();
    }

    //Third, enemies
    for (int j = 0; j < max_enemy_size; j++){
        if ((enemy[j].get_x() > max_width+10)||(enemy[j].get_y() > max_width+10)){
            enemy[j].entry_flag = 0;
            enemy.erase(enemy.begin()+j);
            max_enemy_size = enemy.size();
        }
        else{//still in screen range
            if (cur_time >= enemy[j].obj_timer){
                enemy[j].entry_flag = 1;
                enemy[j].move_object();
                if (enemy[j].bullet_flag == true){
                    add_bullets(enemy[j].bullet_num, enemy[j].B_type, enemy[j].en_x, enemy[j].en_y, j);
                    enemy[j].bullet_flag = false;
                }
            }
        }
    }
    max_enemy_size = enemy.size();
}

void Level::damage_judging(){
    //first, player to enemy
    for (unsigned int i = 0; i < player_bullet.size(); i++){
        for (unsigned int k = 0; k < enemy.size(); k++){
            float e_x = enemy[k].get_x();
            float e_y = enemy[k].get_y();
            float p_x = player_bullet[i].get_x();
            float p_y = player_bullet[i].get_y();
            float r = player_b_radius;
            if ( (e_x < p_x + r) && (e_x > p_x - r) && (e_y < p_y + r) && (e_x > p_x - r) ){
                enemy.erase(enemy.begin()+k);
                max_enemy_size = enemy.size();
            }
        }
    }
}
