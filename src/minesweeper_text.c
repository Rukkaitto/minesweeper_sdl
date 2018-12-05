#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"

#define WIDTH 10
#define HEIGHT 10
#define MINE_COUNT 5

void display_game(game g)
{
    printf("   ");
    for(uint y = 0; y < game_width(g); y++){
        
        printf("%u ", y);
    }
    printf("\n\n");
    for (uint y = 0; y < game_height(g); y++)
    {   
        printf("%u  ", y);
        for (uint x = 0; x < game_width(g); x++)
        {

            Cell cur_cell = get_cell(g, x, y);

            if (is_revealed(cur_cell))
            {
                if (is_flag(cur_cell))
                {
                    printf("< ");
                }
                else if (is_mine(cur_cell))
                {
                    printf("* ");
                }
                else
                {
                    printf("%d ", get_neighbor_count(cur_cell));
                }
            }
            else
            {
                printf("X ");
            }
        }
        printf("\n");
    }
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

int main()
{
    game _game = new_game(WIDTH, HEIGHT);
    set_random_mines(_game, MINE_COUNT);
    count_mines(_game);
    while (!is_won(_game))
    {
        int x;
        int y;
        char sweep_or_flag;
        display_game(_game);
        printf("Sweep or Flag ? (s or f) ");
        scanf(" %c", &sweep_or_flag);
        printf("Where ? ");
        scanf("%d %d", &x, &y);

        if (x < game_width(_game) && y < game_height(_game) && (sweep_or_flag == 's' || sweep_or_flag == 'f'))
        {
            if (sweep_or_flag == 's')
            {
                if (is_mine(get_cell(_game, x, y)))
                {
                    reveal_game(_game);
                    break;
                }
                reveal(_game, get_cell(_game, x, y));
            }
            else
            {
                set_flag(get_cell(_game, x, y));
            }
        }
    }

    if (is_won(_game))
    {
        printf("You won!\n");
    }
    else
    {
        printf("You lost...\n");
    }

    display_game(_game);
    delete_game(_game);
    return EXIT_SUCCESS;
}
