#include "level.h"

const int max_width = 800;

float player_b_radius = 3;
float enemy_b_radius = 5;
float player_damage = 2.5;
sf::Time player_attack_interval = sf::seconds(0.15f);

sf::Clock player_clock;

Level::Level(){
    max_enemy_size = 0;
    max_bullet_size = 0;

    enemy.resize(0);
    bullet.resize(0);
    player_bullet.resize(0);

    player.change_position(400, 500);
    game_over = false;
    super_gm = false;
}

Level::~Level(){

}

//String Type(enemy type):
//    "n"
//    "Boss"

//ONLY add enemies one by one!
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
    if ( (player_clock.getElapsedTime() >= player_attack_interval)&&(player.fire == true) ){
        //player.fire = false;
        player_clock.restart();

        player_bullet.resize(player_bullet.size() + 1);

        player_bullet.back().change_position(player.get_x()+37/4,player.get_y()+0.005);

        player_bullet.back().change_delta(0, -0.5); // directly up
    }
}

void Level::move_stuff(sf::Time cur_time){
    //first, move bullets
    for (int i = 0; i < max_bullet_size; i++){
        if (bullet[i].get_x() < -30 || bullet[i].get_x() > max_width+10 || std::abs(bullet[i].get_y() - player.get_y()) > 600) {
            std::swap(bullet[i], bullet.back());
            bullet.pop_back();
            max_bullet_size = bullet.size();
        }
        else
            bullet[i].move_bullet();
    }
    max_bullet_size = bullet.size();

    //Second, move player bullets
    for (unsigned int j = 0; j < player_bullet.size(); j++){
        if (player_bullet[j].get_x() < -30 || player_bullet[j].get_x() > max_width+10 || std::abs(player_bullet[j].get_y() - player.get_y()) > 600) {
            std::swap(player_bullet[j], player_bullet.back());
            player_bullet.pop_back();
        }
        else
            player_bullet[j].move_bullet();
    }

    //Third, enemies
    for (int k = 0; k < max_enemy_size; k++){
        if (enemy[k].get_x() < -30 || enemy[k].get_x() > max_width+10 || enemy[k].get_y() - player.get_y() > 1800) {
            enemy[k].entry_flag = 0;
            std::swap(enemy[k], enemy.back());
            enemy.pop_back();
            max_enemy_size = enemy.size();
        }
        else { //still in screen range
            if (cur_time >= enemy[k].obj_timer){
                enemy[k].entry_flag = 1;
                enemy[k].move_object();
                if (enemy[k].bullet_flag == true){
                    add_bullets(enemy[k].bullet_num, enemy[k].B_type, enemy[k].en_x, enemy[k].en_y, k);
                    enemy[k].bullet_flag = false;
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
            if ( (e_x < p_x + r) && (e_x > p_x - r) && (e_y < p_y + r) && (e_y > p_y - r) ){
                enemy[k].hp = enemy[k].hp - player_damage;
                if (enemy[k].hit_enemy() == true){
                    std::swap(enemy[k], enemy.back());
                    enemy.pop_back();
                    max_enemy_size--;
                }
            }
        }
    }

    //Second, enemy to player
    for (unsigned int l = 0; l < bullet.size(); l++){
            float pl_x = player.get_x();
            float pl_y = player.get_y();
            float eb_x = bullet[l].get_x();
            float eb_y = bullet[l].get_y();
            float rb = enemy_b_radius;
            if ( (pl_x < eb_x + rb) && (pl_x > eb_x - rb) && (pl_y < eb_y + rb) && (pl_y > eb_y - rb) ){
                game_over = true;
            }
    }
}
