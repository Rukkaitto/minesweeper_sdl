#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include <time.h>

game new_game(uint width, uint height)
{
    game g = (game)malloc(sizeof(struct game_s));
    g->width = width;
    g->height = height;
    g->game_over = false;
    g->win = true;
    g->cells = (Cell **)malloc(height * sizeof(Cell *));

    for (uint i = 0; i < height; i++)
        (g->cells)[i] = (Cell *)malloc(width * sizeof(Cell));

    for (uint y = 0; y < height; y++)
    {
        for (uint x = 0; x < width; x++)
        {
            Cell cell = new_cell(x, y);
            (g->cells)[y][x] = cell;
        }
    }
    return g;
}

Cell new_cell(uint x, uint y)
{
    Cell c = (Cell)malloc(sizeof(struct cell_s));
    c->x = x;
    c->y = y;
    c->neighbor_count = 0;
    c->mine = false;
    c->flag = false;
    c->revealed = false;
    return c;
}

int game_width(game g)
{
    return g->width;
}

int game_height(game g)
{
    return g->height;
}

Cell get_cell(game g, uint x, uint y)
{
    return (g->cells)[y][x];
}

void set_cell(game g, Cell c)
{
    (g->cells)[c->y][c->x] = c;
}

bool is_mine(Cell c)
{
    return c->mine;
}

void set_mine(Cell c)
{
    c->mine = true;
}

bool is_revealed(Cell c)
{
    return c->revealed;
}

void reveal(game g, Cell c)
{
    c->revealed = true;
    if (get_neighbor_count(c) == 0)
    {
        revealNeighbors(g, c);
    }
}

void reveal_game(game g)
{
    for (uint y = 0; y < g->height; y++)
    {
        for (uint x = 0; x < g->width; x++)
        {
            reveal(g, get_cell(g, x, y));
        }
    }
}

uint get_neighbor_count(Cell c)
{
    return c->neighbor_count;
}

void count_neighbors(game g, Cell c)
{

    if (is_mine(c))
    {
        c->neighbor_count = -1;
    }

    int count = 0;

    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            int x = c->x + i;
            int y = c->y + j;
            if (x > -1 && x < game_width(g) && y > -1 && y < game_height(g))
            {
                Cell neighbor = get_cell(g, x, y);
                if (is_mine(neighbor))
                {
                    count++;
                }
            }
        }
    }
    c->neighbor_count = count;
}

bool all_revealed(game g)
{
    for (uint y = 0; y < g->height; y++)
    {
        for (uint x = 0; x < g->width; x++)
        {
            if (!is_revealed(get_cell(g, x, y)))
            {
                return false;
            }
        }
    }
    return true;
}

void delete_game(game g)
{
    for (uint i = 0; i < g->height; i++)
        free((g->cells)[i]);
    free(g->cells);
    free(g);
}

void game_over(game g)
{
    g->game_over = true;
}

void set_game_state(game g, bool state)
{
    g->win = state;
}

bool get_game_state(game g)
{
    return g->win;
}

void revealNeighbors(game g, Cell c)
{
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            int x = c->x + i;
            int y = c->y + j;
            if (x > -1 && x < game_width(g) && y > -1 && y < game_height(g))
            {
                Cell neighbor = get_cell(g, x, y);
                if (!is_mine(neighbor) && !is_revealed(neighbor))
                {
                    reveal(g, neighbor);
                }
            }
        }
    }
}

bool is_flag(Cell c)
{
    return c->flag;
}

void set_flag(Cell c, bool flag)
{
    c->flag = flag;
}

bool is_won(game g)
{
    if (all_revealed(g))
    {
        for (uint y = 0; y < g->height; y++)
        {
            for (uint x = 0; x < g->width; x++)
            {
                if(is_mine(get_cell(g,x,y)) && !is_flag(get_cell(g,x,y))){
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

void set_random_mines(game g, uint mine_count)
{
    srand(time(NULL));
    for (uint i = 0; i < mine_count; i++)
    {
        Cell c = get_cell(g, rand() % game_width(g), rand() % game_height(g));
        set_mine(c);
    }
}

void count_mines(game g)
{
    for (uint y = 0; y < game_height(g); y++)
    {
        for (uint x = 0; x < game_width(g); x++)
        {
            count_neighbors(g, get_cell(g, x, y));
        }
    }
}