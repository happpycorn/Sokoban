#include "config.h"

GameState game_state;

signed main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("請提供地圖檔案，例如： ./sokoban map.txt\n");
        return 1;
    }

    const char *filename = argv[1];
    if (!mapRead(filename, &game_state)) return 1;

    renderMap(game_state);

    while (1) {

        printf("輸入 W/A/S/D 移動：");
        char input;
        scanf(" %c", &input);
        
        playerMove(input, &game_state);

        renderMap(game_state);
        
        if (gameStateCheck(game_state)) break;
    }

    printf("You win!\n");

    return 0;
}

int mapRead(const char *filename, GameState *game_state) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("無法開啟檔案：%s\n", filename);
        return 0;
    }

    if (fscanf(fp, "%d %d", &game_state->rows, &game_state->cols) != 2) {
        printf("地圖檔案格式錯誤：無法讀取行列數。\n");
        fclose(fp);
        return 0;
    }

    // 讀掉第一行剩餘的字元（避免 fgets 讀到換行）
    fgetc(fp);

    for (int i = 0; i < game_state->rows; i++) {

        if (!fgets(game_state->map[i], MAX_COLS, fp)) {
            printf("地圖資料不足（第 %d 行讀取失敗）。\n", i + 1);
            fclose(fp);
            return 0;
        }

        for (int j = 0; game_state->map[i][j]; j++) {
            if (game_state->map[i][j] == '\n')
                game_state->map[i][j] = '\0';
            
            if (game_state->map[i][j] == CH_PLAYER) {
                game_state->player.x = j;
                game_state->player.y = i;
            }

            game_state->origin_map[i][j] = game_state->map[i][j];
        }
    }

    fclose(fp);
    return 1;
}

void renderMap(GameState game_state) {
    // printf("Rows: %d, Cols: %d\n", game_state.rows, game_state.cols);

    // 清螢幕
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    for (int i = 0; i < game_state.rows; i++)
        printf("%s\n", game_state.map[i]);
}

void playerMove(char input, GameState *game_state) {
    int dx = 0, dy = 0;

    switch (input) {
        case 'w': case 'W': dy = -1; break;
        case 's': case 'S': dy =  1; break;
        case 'a': case 'A': dx = -1; break;
        case 'd': case 'D': dx =  1; break;
        default: return; // 非移動鍵直接返回
    }

    int x = game_state->player.x;
    int y = game_state->player.y;
    int nx = x + dx;
    int ny = y + dy;

    char target = game_state->map[ny][nx];

    if (target == CH_EMPTY || target == CH_POINT) {
        game_state->map[y][x] = (game_state->origin_map[y][x] == CH_POINT) ? CH_POINT : CH_EMPTY;
        game_state->map[ny][nx] = CH_PLAYER;
        game_state->player.x = nx;
        game_state->player.y = ny;
    } 
    else if (target == CH_BOX) {
        int bx = nx + dx;
        int by = ny + dy;
        char boxTarget = game_state->map[by][bx];

        if (boxTarget == CH_EMPTY || boxTarget == CH_POINT) {
            game_state->map[by][bx] = CH_BOX;
            game_state->map[ny][nx] = CH_PLAYER;
            game_state->map[y][x] = (game_state->origin_map[y][x] == CH_POINT) ? CH_POINT : CH_EMPTY;
            game_state->player.x = nx;
            game_state->player.y = ny;
        }
    }
}

int gameStateCheck(GameState game_state) {
    for (int i = 0; i < game_state.rows; i++)
        for (int j = 0; game_state.map[i][j]; j++)
            if (game_state.origin_map[i][j] == CH_POINT && game_state.map[i][j] != CH_BOX)
                return 0;
    return 1;
}