#include "iGraphics.h"
#include <time.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 560
#define ENEMY_SHIP_VX 5
#define ENEMY_SHIP_VY 1
#define STAR_NUMBER 50
#define STAR_SPEED 2

#define SHIP_SPEED_Y 10
#define SHIP_SPEED_X 12
#define ENEMY_SHIP_HEALTH 80
#define PROJECTILE_SPEED 3
#define INITIAL_ASTEROID_NUMBER 7
#define MAX_ASTEROID_NUMBER 17
#define MAX_ASTEROID_SPEED 18
#define ASTEROID_HEALTH 10
#define BULLET_SPEED 8
#define MAX_ENEMY_BULLETS 150
#define ENEMY_BULLET_SPEED 3

void change();
void update_enemy_ship();

void initialize_star(int index);
void initialize_stars();
void draw_stars();
void update_stars();
void update_powerups();

void initialize_asteroid(int index);
void initialize_asteroids();
void show_asteroids();
void update_asteroids();
void show_asteroid_explosion(int index);
void show_enemy_spaceship_explosion();
void show_ship_damage();

void show_bullets();
void shoot_bullet(int mx, int my);
void update_bullets();
void update_enemy_bullets();
void shoot_enemy_bullets();

void check_collision(int j); // asteroid er shathe collision check kore
int check_enemy_spaceship_collision();
void check_spaceship_collision();

void change_timer();
void draw_timer_and_text();
void draw_enemy_spaceship();
void draw_hearts();
void draw_game_over_scene();
void draw_powerups();
void reset_game_variables();
void draw_pause_scene();
void draw_settings_scene();
void draw_high_score_page();
void update_high_scores();

int game_state = 0; // 0 for menu, 1 for game, 2 for settings, 3 for instructions, 4 for score
int pause_flag = 0;
int timer1_index = 0, timer2_index = 0;
int music_on = 1;
int music_flag = true;
int game_over_music = true;

int score = 0, final_score = 0;
char score_str[10], score_str_copy[10];
int second = 0, minute = 0, hour = 0, total_second = 0;
char sec[10], min[10], hr[10];
int total_lives = 5;
int lives = 5;
int lives_heart_x = 950 - 25 * total_lives;
int music_tick = 0, sfx_tick = 1;
int x = 50;
int y = 280;
int dx = SHIP_SPEED_X;
int dy = SHIP_SPEED_Y;

int ship_idx = 0;
int enemy_ship_idx = 0;
int asteroid_idx = 0;
int bullet_var = 0;
char enemy_health_str[10];
int enemy_bullet_var = 0;
int max_enemy_bullets = MAX_ENEMY_BULLETS;
int last_shot_time = 15;
int shield = false;
int shield_timer = 0;
int damage_var = 0;
int damage_flag = false;
int explosion_var = 0;
int bullet_interval_time = 4;

int asteroid_speed = 6;
int asteroid_number = INITIAL_ASTEROID_NUMBER;

struct EnemySpaceship
{
    int x = 1200;
    int y = 280;
    int dx = ENEMY_SHIP_VX;
    int dy = ENEMY_SHIP_VY;
    int health = ENEMY_SHIP_HEALTH;
    int explosion_flag = false;
};

EnemySpaceship enemy;

struct Star
{
    int x;
    int y;
    int size;
    int speed;
};

Star stars[STAR_NUMBER];

struct Asteroid
{
    int x;
    int y;
    int speed;
    int health = ASTEROID_HEALTH;
    int explosion_x = 0;
    int explosion_y = 0;
    int explosion_flag = 0;
    int frames = 0;
};

Asteroid asteroids[MAX_ASTEROID_NUMBER];

struct Bullet
{
    int x;
    int y;
    int active = 0;
};

Bullet bullet[100];
Bullet enemy_bullet[100];

struct Powerup
{
    int x;
    int y;
    int drop = false;
};

Powerup powerup_1, powerup_2;

struct Player_Info
{
    char name[50];
    int score;
};

Player_Info player_info[5];
char player_name[40] = {0};
char current_player_name[40] = {0};
int current_player_score = 0;
int len = 0;
int text_mode = 0;

// POPULATING ALL THE ANIMATION FRAMES
char ship[5][100];
char asteroid_explosion[6][100];
char enemy_ship[8][100];
char ship_explosion[6][100];

void populate_images()
{
    for (int i = 0; i < 5; i++)
    {
        sprintf(ship[i], "spaceship\\tile00%d.bmp", i);
    }
    for (int i = 0; i < 6; i++)
    {
        sprintf(asteroid_explosion[i], "asteroids\\tile00%d.bmp", i);
    }
    for (int i = 0; i < 8; i++)
    {
        sprintf(enemy_ship[i], "spaceship\\enemytile00%d.bmp", i);
    }
    for (int i = 0; i < 7; i++)
    {
        sprintf(ship_explosion[i], "spaceship\\explosion00%d.bmp", i);
    }
}

void iDraw()
{
    iClear();

    // state = 0 -> menu
    if (game_state == 0)
    {
        if (music_flag == true && music_on == true)
        {
            PlaySound("assets\\music\\bg_music_nge.wav", NULL, SND_LOOP | SND_ASYNC);
            music_flag = false;
        }
        iShowBMP(0, 0, "bg1\\bg002.bmp");
        iShowBMP2(380, 415, "bg1\\GAMETITLE.bmp", 0);

        // iText(435, 435, "Supernova", GLUT_BITMAP_TIMES_ROMAN_24);

        iText(482, 370, "Play");
        // iSetColor(244, 194, 194);
        // iRectangle(477, 360, 45, 30);

        // iSetColor(255, 255, 255);
        iText(468, 320, "Settings");
        // iRectangle(463, 310, 75, 30);

        iText(455, 270, "Instructions");
        // iRectangle(450, 260, 105, 30);

        iText(458, 220, "High Scores");
        // iRectangle(453, 210, 100, 30);

        iText(482, 170, "Exit");
        // iRectangle(477, 160, 45, 30);

        iText(30, 30, (char *)"Press P to play, E to exit.");
    }
    // state = 1 -> game
    else if (game_state == 1)
    {
        if (lives > 0)
        {
            if (pause_flag == true)
            {
                draw_pause_scene();
            }
            else
            {
                iShowBMP(0, 0, "bg1\\bg_level.bmp");

                draw_stars();
                show_asteroids();
                show_bullets();

                draw_powerups();

                check_spaceship_collision();
                iShowBMP2(x, y, ship[ship_idx], 0);
                if (damage_flag == true)
                {
                    show_ship_damage();
                }
                if (shield == true)
                {
                    iSetColor(255, 255, 255);
                    iCircle(x + 32, y + 32, 32);
                    iSetColor(244, 194, 194);
                    iCircle(x + 32, y + 32, 34);

                    char temp[50];
                    sprintf(temp, "%02d", 10 - shield_timer);
                    iSetColor(244, 194, 194);
                    iText(x - 10, y - 15, "Shield Timer: ", GLUT_BITMAP_HELVETICA_10);
                    iText(x + 50, y - 15, temp, GLUT_BITMAP_HELVETICA_10);
                }

                if (total_second > 10)
                {
                    draw_enemy_spaceship();
                }

                draw_timer_and_text();
            }
        }
        else
        {
            draw_game_over_scene();
        }
    }
    // state = 2 -> settings
    else if (game_state == 2)
    {
        draw_settings_scene();
        iRectangle(22, 520, 45, 22);
    }
    // state = 3 -> instructions
    else if (game_state == 3)
    {
        iShowBMP(0, 0, "bg1\\instructions_bg - Copy.bmp");
        iText(25, 527, "Back");
        iRectangle(20, 522, 43, 22);
    }
    // state = 4 -> high scores
    else if (game_state == 4)
    {
        draw_high_score_page();

        iText(25, 527, "Back");
        iRectangle(20, 522, 43, 22);
    }
}

void iMouseMove(int mx, int my)
{
    // printf("x = %d, y= %d\n",mx,my);
}

/*
        function iMouse() is called when the user presses/releases the mouse.
        (mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if (game_state == 0)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            printf("x = %d, y= %d\n", mx, my);
            if ((477 <= mx && mx <= 522) && (360 <= my && my <= 390))
            {
                game_state = 1;
            }
            if ((464 <= mx && mx <= 538) && (310 <= my && my <= 340))
            {
                game_state = 2;
            }
            if ((450 <= mx && mx <= 555) && (260 <= my && my <= 290))
            {
                game_state = 3;
            }
            if ((453 <= mx && mx <= 553) && (210 <= my && my <= 240))
            {
                game_state = 4;
            }
            if ((477 <= mx && mx <= 542) && (160 <= my && my <= 190))
            {
                exit(0);
            }
        }
    }
    if (game_state == 1)
    {
        if (lives > 0)
        {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            {
                // printf("x = %d, y= %d\n",mx,my);
                shoot_bullet(mx, my);
            }
        }
        else
        {
            if ((490 <= mx && mx <= 690) && (370 <= my && my <= 390))
            {
                text_mode = 1;
            }
            if ((455 <= mx && mx <= 560) && (265 <= my && my <= 285))
            {
                reset_game_variables();
                game_state = 1;
            }
            if ((455 <= mx && mx <= 540) && (215 <= my && my <= 235))
            {
                reset_game_variables();
                music_flag = true;
                game_state = 0;
            }
            if ((455 <= mx && mx <= 510) && (165 <= my && my <= 185))
                exit(0);
        }
        if (pause_flag == true)
        {
            if ((455 <= mx && mx <= 555) && (365 <= my && my <= 385))
            {
                if (!pause_flag)
                {
                    strcpy(score_str_copy, score_str);
                    iPauseTimer(timer1_index);
                    iPauseTimer(timer2_index);
                    pause_flag = 1;
                }
                else
                {
                    strcpy(score_str, score_str_copy);
                    iResumeTimer(timer1_index);
                    iResumeTimer(timer2_index);
                    pause_flag = 0;
                }
            }
            if ((455 <= mx && mx <= 500) && (315 <= my && my <= 335))
            {
                music_flag = true;
                game_state = 0;
            }
            if ((455 <= mx && mx <= 560) && (265 <= my && my <= 285))
            {
                reset_game_variables();
                game_state = 1;
                if (!pause_flag)
                {
                    strcpy(score_str_copy, score_str);
                    iPauseTimer(timer1_index);
                    iPauseTimer(timer2_index);
                    pause_flag = 1;
                }
                else
                {
                    strcpy(score_str, score_str_copy);
                    iResumeTimer(timer1_index);
                    iResumeTimer(timer2_index);
                    pause_flag = 0;
                }
            }
            if ((455 <= mx && mx <= 500) && (215 <= my && my <= 235))
                exit(0);
        }
    }
    else if (game_state == 2)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if ((525 <= mx && mx <= 545) && (407 <= my && my <= 430))
            {
                if (music_on)
                {
                    music_on = false;
                    PlaySound(0, 0, 0);
                }
                else
                {
                    music_on = true;
                    PlaySound("assets\\music\\bg_music_nge.wav", NULL, SND_LOOP | SND_ASYNC);
                }
            }
            if ((525 <= mx && mx <= 545) && (338 <= my && my <= 360))
            {
                sfx_tick = !sfx_tick;
            }

            if ((25 <= mx && mx <= 65) && (522 <= my && my <= 540))
            {
                game_state = 0;
            }
        }
    }
    else if (game_state == 3)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if ((25 <= mx && mx <= 65) && (522 <= my && my <= 540))
            {
                game_state = 0;
            }
        }
    }
    // high score page in game_state == 4
    else if (game_state == 4)
    {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            if ((25 <= mx && mx <= 65) && (522 <= my && my <= 540))
            {
                game_state = 0;
            }
        }
    }
}

void iKeyboard(unsigned char key)
{
    if (game_state == 0)
    {
        if (key == 'p')
            game_state = 1;
        else if (key == 'e')
            exit(0);
    }
    else if (game_state == 1)
    {
        if (lives > 0)
        {
            if (key == 'w')
            {
                if (y + SHIP_SPEED_Y + 55 < SCREEN_HEIGHT)
                {
                    y += SHIP_SPEED_Y;
                }
            }
            if (key == 'a')
            {
                if (x > SHIP_SPEED_X)
                {
                    x -= SHIP_SPEED_X;
                }
            }
            if (key == 's')
            {
                if (y - SHIP_SPEED_Y > 0)
                {
                    y -= SHIP_SPEED_Y;
                }
            }
            if (key == 'd')
            {
                if (x + SHIP_SPEED_X + 55 < SCREEN_WIDTH)
                {
                    x += SHIP_SPEED_X;
                }
            }
            if (key == ' ')
            {
                shoot_bullet(x, y);
            }
            if (key == 'p')
            {
                if (!pause_flag)
                {
                    strcpy(score_str_copy, score_str);
                    iPauseTimer(timer1_index);
                    iPauseTimer(timer2_index);
                    pause_flag = 1;
                }
                else
                {
                    strcpy(score_str, score_str_copy);
                    iResumeTimer(timer1_index);
                    iResumeTimer(timer2_index);
                    pause_flag = 0;
                }
            }
        }
        else
        {
            if (text_mode == 1)
            {
                if (key == '\r')
                {
                    text_mode = 0;
                    strcpy(current_player_name, player_name);
                    printf("%s\n", current_player_name);
                    for (int i = 0; i < len; i++)
                        player_name[i] = 0;

                    len = 0;

                    reset_game_variables();
                    music_flag = true;
                    game_state = 0;
                }
                else
                {
                    if (key == '\b')
                    {
                        if (len > 0)
                        {
                            player_name[--len] = '_';
                        }
                    }
                    else if (key == ' ')
                    {
                        player_name[len++] = '_';
                    }
                    else
                    {
                        player_name[len++] = key;
                    }
                }
            }
        }
    }
}

/*
        function iSpecialKeyboard() is called whenver user hits special keys like-
        function keys, home, end, pg up, pg down, arraows etc. you have to use
        appropriate constants to detect them. A list is:
        GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
        GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
        GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
        GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{
    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    if (key == GLUT_KEY_F1)
    {
        if (music_on)
        {
            music_on = false;
            PlaySound(0, 0, 0);
        }
        else
        {
            music_on = true;
            PlaySound("assets\\music\\bg_music_nge.wav", NULL, SND_LOOP | SND_ASYNC);
        }
    }
}

// void set_background() {
//     int sum = 0;
//     for (int i = 0; i < 25; i++) {
//         Back[i].x = sum;
//         Back[i].y = 0;
//         sum += 40;
//     }
// }

int main()
{
    srand(time(NULL));
    initialize_stars();
    initialize_asteroids();
    populate_images();
    timer1_index = iSetTimer(40, change);
    timer2_index = iSetTimer(1000, change_timer);
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, (char *)"Supernova");
    return 0;
}

void change()
{
    if (game_state == 1 && lives > 0)
    {
        if ((total_second % 20 == 0) && (total_second != 0))
        {
            powerup_1.drop = true;
            powerup_1.x = (rand() % 200) + 100;
            powerup_1.y = 650;
        }
        if ((total_second % 25 == 0) && (total_second != 0))
        {
            powerup_2.drop = true;
            powerup_2.x = (rand() % 200) + 100;
            powerup_2.y = 650;
        }

        if (enemy.explosion_flag == true)
            explosion_var++;
        if (damage_flag == true)
            damage_var++;

        ship_idx = (ship_idx + 1) % 5;
        enemy_ship_idx = (enemy_ship_idx + 1) % 8;
        update_stars();
        update_asteroids();
        update_bullets();
        update_powerups();
        if (total_second > 10)
        {
            update_enemy_ship();
        }
        if (total_second > 15)
        {
            update_enemy_bullets();
        }
    }
}

void update_enemy_ship()
{
    if (enemy.x > 875)
        enemy.x -= enemy.dx;
    if (enemy.y > 450 || enemy.y < 100)
        enemy.dy *= -1;
    if (enemy.x == 875)
        enemy.y += enemy.dy;
}

void shoot_enemy_bullets()
{
    if (enemy_bullet[enemy_bullet_var].active == true)
    {
        enemy_bullet[enemy_bullet_var].x = enemy.x - 30;
        enemy_bullet[enemy_bullet_var].y = enemy.y + 30;

        enemy_bullet_var = (enemy_bullet_var + 1) % 100;
    }
}

void initialize_star(int index)
{
    stars[index].x = rand() % SCREEN_WIDTH;
    stars[index].y = rand() % SCREEN_HEIGHT;
    stars[index].size = 1;
    stars[index].speed = STAR_SPEED;
}

void initialize_asteroid(int index)
{
    asteroids[index].explosion_flag = false;
    asteroids[index].x = rand() % SCREEN_WIDTH + 1000;
    asteroids[index].y = rand() % (SCREEN_HEIGHT - 32 * 2);
    asteroids[index].speed = (rand() % (asteroid_speed - (asteroid_speed - 3))) + (asteroid_speed - 3);
}

void initialize_stars()
{
    for (int i = 0; i < STAR_NUMBER; i++)
    {
        initialize_star(i);
    }
}

void initialize_asteroids()
{
    for (int i = 0; i < asteroid_number; i++)
    {
        initialize_asteroid(i);
    }
}

void draw_stars()
{
    for (int i = 0; i < STAR_NUMBER; i++)
    {
        iSetColor(255, 255, 255);
        iFilledCircle(stars[i].x, stars[i].y, stars[i].size);
    }
}

void show_asteroids()
{
    for (int i = 0; i < asteroid_number; i++)
    {
        check_collision(i);
        if (asteroids[i].explosion_flag == false)
        {
            if (i % 3 == 0)
                iShowBMP2(asteroids[i].x, asteroids[i].y, "asteroids\\tile000_new.bmp", 0);
            else
                iShowBMP2(asteroids[i].x, asteroids[i].y, "asteroids\\tile000.bmp", 0);
        }
        else
        {
            show_asteroid_explosion(i);
        }
    }
}

void update_stars()
{
    for (int i = 0; i < STAR_NUMBER; i++)
    {
        stars[i].x -= stars[i].speed;
        if ((stars[i].x - stars[i].size) < 0)
        {
            initialize_star(i);
            stars[i].x = SCREEN_WIDTH; // star abar left theke ashe
        }
    }
}

void update_asteroids()
{
    for (int i = 0; i < asteroid_number; i++)
    {
        if (asteroids[i].explosion_flag == false)
        {
            asteroids[i].x -= asteroids[i].speed;
            if ((asteroids[i].x) < 10)
            {
                if (sfx_tick == true)
                    PlaySound("assets\\music\\damage_sound_effect.wav", NULL, SND_ASYNC);
                lives--;
                initialize_asteroid(i);
            }
        }
    }
}

void update_powerups()
{
    if (powerup_1.drop == true)
    {
        powerup_1.y -= 3;
    }
    if (powerup_2.drop == true)
    {
        powerup_2.y -= 3;
    }
}

void show_asteroid_explosion(int index)
{
    iShowBMP2(asteroids[index].x, asteroids[index].y, asteroid_explosion[asteroids[index].frames], 0);
    asteroids[index].frames++;
    if (asteroids[index].frames > 5)
    {
        asteroids[index].frames = 0;
        initialize_asteroid(index);
    }
}

void show_enemy_spaceship_explosion()
{
    // if (explosion_var == 0) {
    //     PlaySound();
    // }
    if (explosion_var > 6)
    {
        enemy.explosion_flag = false;
        enemy.x = 2000;
        enemy.y = 280;
        enemy.health = ENEMY_SHIP_HEALTH;
        return;
    }
    iShowBMP2(enemy.x, enemy.y, ship_explosion[explosion_var], 0);
}

void show_ship_damage()
{
    if (damage_var > 6)
    {
        damage_var = 0;
        damage_flag = 0;
        return;
    }
    iShowBMP2(x, y, ship_explosion[damage_var], 0);
}

void show_bullets()
{
    for (int i = 0; i < 100; i++)
    {
        if (bullet[i].active == true)
        {
            iSetColor(255, 0, 0);
            iShowBMP2(bullet[i].x, bullet[i].y, "assets\\spaceship + powerups\\nairan weapon projectiles\\Nairan - Ray.bmp", 0);
        }
    }
    for (int i = 0; i < 100; i++)
    {
        if (enemy_bullet[i].active == true)
        {
            iSetColor(255, 165, 0);
            iFilledCircle(enemy_bullet[i].x, enemy_bullet[i].y, 3);
            iSetColor(255, 0, 0);
            iCircle(enemy_bullet[i].x, enemy_bullet[i].y, 4);

            // iShowBMP2(enemy_bullet[i].x, enemy_bullet[i].y, "spaceship\\enemy_bullet_1.bmp", 0);
        }
    }
}

void shoot_bullet(int mx, int my)
{
    if (sfx_tick == true)
        PlaySound("assets\\music\\laser_pew_sound.wav", NULL, SND_ASYNC);
    bullet[bullet_var].active = true;
    bullet[bullet_var].x = x + 60;
    bullet[bullet_var].y = y + 30;
    bullet_var = (bullet_var + 1) % 100;
}

void update_bullets()
{
    for (int i = 0; i < 100; i++)
    {
        if (bullet[i].active == true)
        {
            bullet[i].x += BULLET_SPEED;
            if (bullet[i].x > SCREEN_WIDTH)
                bullet[i].active = false;
        }
    }
    for (int i = 0; i < 100; i++)
    {
        if (enemy_bullet[i].active == true)
        {
            enemy_bullet[i].x -= ENEMY_BULLET_SPEED;
        }
        if (enemy_bullet[i].x < 0)
        {
            enemy_bullet[i].active = false;
        }
    }
}

void update_enemy_bullets()
{
    if (total_second - last_shot_time >= 4)
    {
        enemy_bullet[enemy_bullet_var].active = true;
        // enemy_bullet[(enemy_bullet_var+1) % 100].active = true;
        // enemy_bullet[(enemy_bullet_var+2) % 100].active = true;
        shoot_enemy_bullets();
        last_shot_time = total_second;
    }
}

void check_collision(int j)
{
    for (int i = 0; i < 100; i++)
    {
        if (bullet[i].active == true)
        {
            if ((bullet[i].x >= asteroids[j].x && bullet[i].x <= asteroids[j].x + 96) && (asteroids[j].y + 28 <= bullet[i].y) && (bullet[i].y <= asteroids[j].y + 64))
            {
                if (sfx_tick == true)
                    PlaySound("assets\\music\\rock_destroy_wav (mp3cut.net).wav", NULL, SND_ASYNC);
                asteroids[j].explosion_flag = true;
                bullet[i].active = false;
                score += 100;
            }
        }
    }
    if (asteroids[j].explosion_flag == false)
    {
        if (((x <= asteroids[j].x + 25) && (asteroids[j].x + 25 <= x + 55)) && ((asteroids[j].y + 24 <= y + 10 && y + 10 <= asteroids[j].y + 66) || (asteroids[j].y + 24 <= y + 55 && y + 55 <= asteroids[j].y + 66)))
        {
            if (sfx_tick == true)
                PlaySound("assets\\music\\rock_destroy_wav (mp3cut.net).wav", NULL, SND_ASYNC);
            asteroids[j].explosion_flag = true;
            show_asteroid_explosion(j);

            if (shield != true)
            {
                lives -= 2;
            }
        }
    }
}

void check_spaceship_collision()
{
    // collision with enemy bullets
    for (int i = 0; i < 100; i++)
    {
        if (enemy_bullet[i].active == true)
        {
            if ((x + 10 <= enemy_bullet[i].x && enemy_bullet[i].x <= x + 55) && (y + 10 <= enemy_bullet[i].y) && (enemy_bullet[i].y <= y + 55))
            {
                printf("damage taken! ow!");
                if (sfx_tick == true)
                    PlaySound("assets\\music\\damage_sound_effect.wav", NULL, SND_ASYNC);
                // shield off thakle damage hobe
                if (shield != true)
                {
                    damage_flag = true;
                    lives--;
                }

                enemy_bullet[i].active = false;
            }
        }
    }

    // collision with both powerups
    if (powerup_1.drop == true)
    {
        if ((y + 10 <= powerup_1.y && powerup_1.y <= y + 55) && (x + 10 <= powerup_1.x && powerup_1.x <= x + 55))
        {
            if (sfx_tick == true)
                PlaySound("assets\\music\\powerup_effect_health.wav", NULL, SND_ASYNC);
            printf("heart taken");
            lives += 2;
            if (lives > total_lives)
                lives = total_lives;
            powerup_1.drop = false;
        }
    }
    if (powerup_2.drop == true)
    {
        if ((y + 10 <= powerup_2.y && powerup_2.y <= y + 55) && (x + 10 <= powerup_2.x && powerup_2.x <= x + 55))
        {
            if (sfx_tick == true)
                PlaySound("assets\\music\\shield_powerup_wav_sound.wav", NULL, SND_ASYNC);
            printf("Shield activated!");
            shield = true;
            powerup_2.drop = false;
        }
    }
}

int check_enemy_spaceship_collision()
{
    // returns 0 for no collision, 1 for damage taken, 2 for dhongsho hoye gese
    for (int i = 0; i < 100; i++)
    {
        if (bullet[i].active == true)
        {
            if ((bullet[i].x >= enemy.x + 20) && (enemy.y + 8 <= bullet[i].y) && (bullet[i].y <= enemy.y + 60))
            {
                // collision hoise
                if (sfx_tick == true)
                    PlaySound("assets\\music\\enemy_damaged.wav", NULL, SND_ASYNC);
                bullet[i].active = false;
                enemy.health -= 10;
                if (enemy.health <= 0)
                {
                    break;
                }
            }
        }
    }
    if (enemy.health <= 0)
    {
        score += 1000;
        enemy.explosion_flag = true;
    }
    return 0;
}

void change_timer()
{
    if (game_state == 1)
    {
        if (lives > 0)
        {
            total_second++;
            score++;
            second = (total_second % 60);
            minute = (((total_second) / 60) % 60);
            hour = ((total_second / (60 * 60)));

            if ((total_second > 0) && (total_second % 60 == 0) && (asteroid_number < MAX_ASTEROID_NUMBER))
            {
                asteroid_number += 3;
            }
            if ((total_second > 0) && (total_second % 90 == 0) && (asteroid_speed < MAX_ASTEROID_SPEED))
            {
                asteroid_speed += 2;
            }
            if ((total_second > 0) && (total_second % 120 == 0) && (bullet_interval_time > 2))
            {
                bullet_interval_time--;
            }
        }
        if (shield == true)
        {
            shield_timer++;
            if (shield_timer >= 9)
            {
                shield_timer = 0;
                shield = false;
            }
        }
        if ((total_second > 0) && (total_second % 60 == 0) && (dx < 20))
        {
            dx += 2;
        }
        if ((total_second > 0) && (total_second % 60 == 0) && (dy < 20))
        {
            dx += 2;
        }
    }
}

void draw_enemy_spaceship()
{
    int flag = check_enemy_spaceship_collision(); // 0 for no collision, 1 for damage taken animation, 2 for explosion animation

    if (enemy.health > 0)
    {
        iShowBMP2(enemy.x, enemy.y, enemy_ship[enemy_ship_idx], 0);
        iSetColor(200, 0, 0);
        iRectangle(enemy.x + 5, enemy.y - 10, 60, 8);
        iFilledRectangle(enemy.x + 5, enemy.y - 10, enemy.health * (60.0 / ENEMY_SHIP_HEALTH), 8);
    }

    if (enemy.explosion_flag == true)
    {
        show_enemy_spaceship_explosion();
    }

    // if (flag == 0) {
    //     iShowBMP2(enemy.x, enemy.y, enemy_ship[enemy_ship_idx], 0);
    //     iSetColor(200, 0, 0);
    //     iRectangle(enemy.x + 5, enemy.y - 10, 60, 8);
    //     iFilledRectangle(enemy.x + 5, enemy.y - 10, enemy.health * (60.0 / ENEMY_SHIP_HEALTH), 8);
    // }
    // else if (flag == 1) {
    //     iShowBMP2(enemy.x, enemy.y, enemy_ship[enemy_ship_idx], 255);
    //     iSetColor(200, 0, 0);
    //     iRectangle(enemy.x + 5, enemy.y - 10, 60, 8);
    //     iFilledRectangle(enemy.x + 5, enemy.y - 10, enemy.health * (60.0 / ENEMY_SHIP_HEALTH), 8);
    // }
    // else if (flag == 2) {
    //     // show_enemy_spaceship_explosion();
    // }
}

void draw_timer_and_text()
{
    iSetColor(255, 255, 255);
    iText(30, 30, (char *)"Press P to pause");

    iText(30, 530, "Score: ");
    sprintf(score_str, "%06d", score);
    iText(85, 530, score_str);

    iSetColor(255, 255, 255);
    iText(180, 530, "Timer: ");
    sprintf(sec, "%02d", second);
    sprintf(min, "%02d:", minute);
    sprintf(hr, "%02d:", hour);
    iText(235, 530, hr);
    iText(260, 530, min);
    iText(285, 530, sec);

    if (lives <= 2 && (total_second % 2 == 0))
    {
        iSetColor(255, 192, 203);
        iText(620, 530, "LOW ON HEARTS!!");
    }

    draw_hearts();
}

void draw_hearts()
{
    for (int i = 0; i < total_lives; i++)
    {
        if (i < lives)
        {
            iShowBMP2(lives_heart_x + i * 25, 520, "assets\\UI\\tile0000.bmp", 0);
        }
        else
        {
            iShowBMP2(lives_heart_x + i * 25, 520, "assets\\UI\\tile0001.bmp", 0);
        }
    }
}

void draw_game_over_scene()
{
    iShowBMP(0, 0, "bg1\\gameover_bg.bmp");

    if (game_over_music == true)
    {
        if (music_on == true)
        {
            PlaySound("assets\\music\\game_over_sound.wav", NULL, SND_ASYNC);
            game_over_music = false;
        }
    }

    if (text_mode == 0)
    {
        iSetColor(128, 128, 128);
        iText(360, 375, "Enter Your Name: ");
        iRectangle(490, 370, 200, 20);
    }
    else if (text_mode == 1)
    {
        iSetColor(255, 255, 255);
        iText(360, 375, "Enter Your Name: ");
        iRectangle(490, 370, 200, 20);
    }

    iSetColor(255, 255, 255);

    if (text_mode == 1)
    {
        iText(495, 375, player_name);
    }

    iText(460, 320, "Score: ");
    iText(520, 320, score_str);
    iText(460, 270, "Play Again");
    iText(460, 220, "Main Menu");
    iText(460, 170, "Exit");

    iText(10, 10, "Click to activate the box, enter to finish.");
}

void reset_game_variables()
{
    update_high_scores();
    score = 0;
    lives = total_lives;
    second = 0, minute = 0, hour = 0, total_second = 0;
    last_shot_time = 15;

    enemy.x = 1200;
    enemy.y = 280;
    enemy.health = ENEMY_SHIP_HEALTH;

    x = 50;
    y = 280;
    dx = SHIP_SPEED_X;
    dy = SHIP_SPEED_Y;

    for (int i = 0; i < 100; i++)
    {
        enemy_bullet[i].active = false;
    }

    for (int i = 0; i < 100; i++)
    {
        bullet[i].active = false;
    }
    initialize_asteroids();

    powerup_1.drop = false;
    powerup_2.drop = false;

    asteroid_number = INITIAL_ASTEROID_NUMBER;
    asteroid_speed = 6;
    bullet_interval_time = 4;
    game_over_music = true;
}

void draw_pause_scene()
{
    iShowBMP(0, 0, "bg1\\bg_level_pause.bmp");
    // draw_stars();

    iSetColor(255, 255, 255);
    iText(30, 30, (char *)"Press P to resume");

    iText(30, 530, "Score: ");
    iText(85, 530, score_str_copy);

    draw_hearts();

    iText(460, 370, "Resume Game");
    iText(460, 320, "Menu");
    iText(460, 270, "Start Over");
    iText(460, 220, "Exit");
}

void draw_settings_scene()
{
    iShowBMP(0, 0, "bg1\\settings_bg.bmp");
    if (music_on == 1)
        iShowBMP2(526, 409, "bg1\\menu_yes.bmp", 0);
    if (sfx_tick == 1)
        iShowBMP2(525, 338, "bg1\\menu_yes.bmp", 0);

    iText(30, 525, "Back");
}

void draw_powerups()
{
    if (powerup_1.drop == true)
    {
        iShowBMP2(powerup_1.x, powerup_1.y, "assets\\UI\\tile0000.bmp", 0);
    }

    if (powerup_2.drop == true)
    {
        iShowBMP2(powerup_2.x, powerup_2.y, "assets\\UI\\power_up_2.bmp", 0);
        iSetColor(255, 255, 255);
        iCircle(powerup_2.x + 8, powerup_2.y + 8, 8);
    }
}

void update_high_scores()
{
    FILE *fp = fopen("highscores.txt", "r");

    for (int i = 0; i < 5; i++)
    {
        int a = fscanf(fp, "%39s %d", player_info[i].name, &player_info[i].score);
        if (a != 2)
        {
            strcpy(player_info[i].name, "Winter");
            player_info[i].score = 100;
        }
    }
    fclose(fp);

    fp = fopen("highscores.txt", "w");

    if (fp == NULL)
    {
        printf("Couldn't open file in update function");
    }

    current_player_score = score;

    for (int i = 0; i < 5; i++)
    {
        if (current_player_score > player_info[i].score)
        {
            for (int j = 4; j > i; j--)
            {
                player_info[j] = player_info[j - 1];
            }
            strcpy(player_info[i].name, current_player_name);
            player_info[i].score = current_player_score;
            break;
        }
    }

    // writing the updated high scores in the file
    for (int i = 0; i < 5; i++)
    {
        fprintf(fp, "%s %d\n", player_info[i].name, player_info[i].score);
    }

    fclose(fp);
}

void draw_high_score_page()
{
    iShowBMP(0, 0, "bg1\\high_score_bmp.bmp");

    FILE *fp = fopen("highscores.txt", "r");

    for (int i = 0; i < 5; i++)
    {
        int a = fscanf(fp, "%39s %d", player_info[i].name, &player_info[i].score);
        if (a != 2)
        {
            strcpy(player_info[i].name, "Winter");
            player_info[i].score = 100;
        }
    }

    fclose(fp);

    char temp[100];

    for (int i = 0; i < 5; i++)
    {
        sprintf(temp, "%02d", i + 1);
        iText(260, 425 - i * 75, temp, GLUT_BITMAP_9_BY_15);
        iText(275, 425 - i * 75, ".", GLUT_BITMAP_9_BY_15);
        iText(285, 425 - i * 75, player_info[i].name, GLUT_BITMAP_9_BY_15);
        sprintf(temp, "%06d", player_info[i].score);
        iText(285 + 350, 425 - i * 75, temp, GLUT_BITMAP_9_BY_15);
    }
}
