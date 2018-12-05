#include <stdbool.h>

typedef unsigned int uint;

struct cell_s{
    uint x;
    uint y;
    bool mine;
    bool flag;
    bool revealed;
    int neighbor_count;
};

typedef struct cell_s *Cell;

struct game_s{
    uint width;
    uint height;
    bool game_over;
    bool win;
    Cell **cells;
};

typedef struct game_s *game;

game new_game(uint width, uint height);

Cell new_cell(uint x, uint y);

int game_width(game g);

int game_height(game g);

Cell get_cell(game g, uint x, uint y);

void set_cell(game g, Cell c);

bool is_mine(Cell c);

void set_mine(Cell c);

bool is_revealed(Cell c);

void reveal(game g, Cell c);

void reveal_game(game g);

uint get_neighbor_count(Cell c);

void count_neighbors(game g, Cell c);

bool all_revealed(game g);

void game_over(game g);

void set_game_state(game g, bool state);

bool get_game_state(game g);

void delete_game(game g);

void revealNeighbors(game g, Cell c);

bool is_flag(Cell c);

void set_flag(Cell c);

bool is_won(game g);
