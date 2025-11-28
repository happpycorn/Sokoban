#pragma once

#include <stdio.h>
#include <stdlib.h>

#define MAX_ROWS 20
#define MAX_COLS 20

#define CH_WALL  '#'
#define CH_POINT '.'
#define CH_PLAYER '$'
#define CH_BOX    '@'
#define CH_EMPTY ' '

typedef struct {
    int x;
    int y;
} Player;

typedef struct {
    int rows, cols;
    char map[MAX_ROWS][MAX_COLS];
    char origin_map[MAX_ROWS][MAX_COLS];
    Player player;
} GameState;

int mapRead(const char *filename, GameState *game_state);
void renderMap(GameState game_state);
void playerMove(char input, GameState *game_state);
int gameStateCheck(GameState game_state);