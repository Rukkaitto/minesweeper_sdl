#include <SDL.h>
#include <SDL_image.h> // required to load transparent texture from PNG
#include <SDL_ttf.h>   // required to use TTF fonts

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "minesweeper_sdl.h"

#define GRID_W 15
#define GRID_H 15
#define MINE_COUNT 25

#define FONT "Arial.ttf"
#define FONTSIZE 36

#define BOMB "bomb.png"
#define FLAG "flag.png"
#define REVEALED "revealed.png"
#define UNREVEALED "unrevealed.png"

#define ONE "one.png"
#define TWO "two.png"
#define THREE "three.png"
#define FOUR "four.png"
#define FIVE "five.png"
#define SIX "six.png"
#define SEVEN "seven.png"
#define EIGHT "eight.png"

/* **************************************************************** */

struct Env_t
{
    game game;
    double ratio;
    int grid_w;
    int grid_h;
    int left_x;
    int right_x;
    int top_y;
    int bottom_y;
    int tile_size;
    SDL_Rect **cells;
    SDL_Texture *bomb;
    SDL_Texture *flag;
    SDL_Texture *revealed;
    SDL_Texture *unrevealed;
    SDL_Texture *one;
    SDL_Texture *two;
    SDL_Texture *three;
    SDL_Texture *four;
    SDL_Texture *five;
    SDL_Texture *six;
    SDL_Texture *seven;
    SDL_Texture *eight;
};

/* **************************************************************** */

void usage(char *executable)
{
    fprintf(stderr, "usage: %s <width> <height> [S|N] [3|4]\n", executable);
    exit(EXIT_FAILURE);
}

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[])
{
    Env *env = malloc(sizeof(struct Env_t));

    env->game = new_game(GRID_W, GRID_H);
    set_random_mines(env->game, MINE_COUNT);

    int w = game_width(env->game);
    int h = game_height(env->game);

    //Allocates an array of SDL_Rect (great build-in structure for handling collision later)
    env->cells = malloc(w * h * sizeof(SDL_Rect));

    //Loading textures
    env->bomb = IMG_LoadTexture(ren, BOMB);
    if (!env->bomb)
        ERROR("IMG_LoadTexture: %s\n", BOMB);
    env->unrevealed = IMG_LoadTexture(ren, UNREVEALED);
    env->flag = IMG_LoadTexture(ren, FLAG);
    if (!env->flag)
        ERROR("IMG_LoadTexture: %s\n", FLAG);
    env->revealed = IMG_LoadTexture(ren, REVEALED);
    if (!env->revealed)
        ERROR("IMG_LoadTexture: %s\n", REVEALED);
    env->unrevealed = IMG_LoadTexture(ren, UNREVEALED);
    if (!env->unrevealed)
        ERROR("IMG_LoadTexture: %s\n", UNREVEALED);

    env->one = IMG_LoadTexture(ren, ONE);
    if (!env->one)
        ERROR("IMG_LoadTexture: %s\n", ONE);
    env->two = IMG_LoadTexture(ren, TWO);
    if (!env->two)
        ERROR("IMG_LoadTexture: %s\n", TWO);
    env->three = IMG_LoadTexture(ren, THREE);
    if (!env->three)
        ERROR("IMG_LoadTexture: %s\n", THREE);
    env->four = IMG_LoadTexture(ren, FOUR);
    if (!env->four)
        ERROR("IMG_LoadTexture: %s\n", FOUR);
    env->five = IMG_LoadTexture(ren, FIVE);
    if (!env->five)
        ERROR("IMG_LoadTexture: %s\n", FIVE);

    return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren, Env *env)
{
    int w = game_width(env->game);
    int h = game_height(env->game);

    int width;
    int height;

    SDL_GetWindowSize(win, &width, &height);

    /*All of the dimensions must be relative to the window's size, and putting this piece of
    code in render() executes it every frame*/
    env->ratio = (double)w / (double)h;
    //The grid's width depends on both of the window's dimensions
    env->grid_w = (width + height) / 4 - ((width + height) / 4 % w);
    //The size of a
    env->tile_size = env->grid_w / w;
    env->grid_h = env->grid_w / env->ratio;
    //The grid is positionned in the center of the screen
    env->left_x = width / 2 - env->grid_w / 2;
    env->right_x = width / 2 + env->grid_w / 2;
    env->top_y = height / 2 - env->grid_h / 2;
    env->bottom_y = height / 2 + env->grid_h / 2;

    //Cells
    //Putting every cell in an array makes displaying the cells way easier
    SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
    int cell_index = 0;
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            env->cells[cell_index] = malloc(sizeof(SDL_Rect));
            env->cells[cell_index]->h = env->tile_size;
            env->cells[cell_index]->w = env->tile_size;
            env->cells[cell_index]->x = env->left_x + x * env->tile_size;
            env->cells[cell_index]->y = env->bottom_y - y * env->tile_size - env->tile_size;

            Cell c = get_cell(env->game, x, y);
            draw_cell(env, ren, c, env->cells[cell_index], x, y);

            cell_index++;
        }
    }
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e)
{

    int h = game_height(env->game);
    int w = game_width(env->game);

    if (e->type == SDL_QUIT)
    {
        return true;
    }

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        SDL_Point mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);

        //Finds which cell the cursor is in and rotates it, depending on the button
        for (int y = h - 1; y >= 0; y--)
        {
            for (int x = 0; x < w; x++)
            {
                SDL_Rect *cell = get_rect(env, x, y);
                if (mouse.x > cell->x && mouse.x < cell->x + cell->w && mouse.y > cell->y && mouse.y < cell->y + cell->h)
                {
                    if (e->button.button == SDL_BUTTON_LEFT)
                    {
                        reveal(env->game, get_cell(env->game, x, y));
                        if (is_mine(get_cell(env->game, x, y)))
                        {
                            reveal_game(env->game);
                        }
                    }
                    if (e->button.button == SDL_BUTTON_RIGHT) {
                        if(!is_flag(get_cell(env->game, x, y))){
                            set_flag(get_cell(env->game, x, y), true);
                        } else {
                            set_flag(get_cell(env->game, x, y), false);
                        }
                    }
                }
            }
        }
    }

    return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env)
{

    free(env);
}

/* **************************************************************** */

void draw_cell(Env *env, SDL_Renderer *ren, Cell c, SDL_Rect *rect, uint x, uint y)
{
    SDL_Texture *cell_tex;
    count_neighbors(env->game, c);
    uint n_count = get_neighbor_count(c);

    if (!is_revealed(c))
    {
        if (is_flag(c))
        {
            cell_tex = env->flag;
        }
        else
        {
            cell_tex = env->unrevealed;
        }
    }
    else
    {
        if (is_mine(c))
        {
            cell_tex = env->bomb;
        }
        else
        {
            switch (n_count)
            {
            case 1:
                cell_tex = env->one;
                break;
            case 2:
                cell_tex = env->two;
                break;
            case 3:
                cell_tex = env->three;
                break;
            case 4:
                cell_tex = env->four;
                break;
            case 5:
                cell_tex = env->five;
                break;
            default:
                cell_tex = env->revealed;
            }
        }
    }
    SDL_RenderCopyEx(ren, cell_tex, NULL, rect, 0, NULL, SDL_FLIP_NONE);
}

SDL_Rect *get_rect(Env *env, int x, int y)
{
    return env->cells[x + game_width(env->game) * y];
}