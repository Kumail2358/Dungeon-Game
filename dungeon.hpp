#include<iostream>
#include<string>
#include<SDL.h>
#include<SDL_ttf.h>
// #include "block.hpp"
const int xJump=50, yJump=50;
const int xGrid = 10, yGrid = 10;

using namespace std;



void makeMove(string direction);
void createDungeon();
void update(SDL_Renderer* gRenderer, SDL_Texture* assets, string direction="");
enum {ROCK, CORONA, BURGER, WELL, FIRE, GOLD};

struct Block{
    int shape;
    int life;
    int health;
    SDL_Rect src, mover;
};

struct Player{
    int row, col;
    int life, health;
    SDL_Rect src, mover;
};