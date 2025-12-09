#include "config.h"

GameState game_state;

signed main(int argc, char *argv[]) {

    const char *filename = argc < 2 ? "Map/0_test_map.txt" : argv[1];
    if (!readMap(filename, &game_state)) return 1;

    game_state.score = 0;

    renderMap(game_state);

    while (1) {

        playerMove(&game_state);

        renderMap(game_state);
        
        if (gameStateCheck(game_state)) break;
    }

    printf("You win!\n");

    return 0;
}

/*
    read map from txt
    input  : game_state, filename
    output : succes or not
    side effect : Fill the setting file into gamestate.
*/
int readMap(const char *filename, GameState *game_state) {
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

/*
    render map
    input  : game_state
    output : not thing
    side effect : Print map on the screen.
*/
void renderMap(GameState game_state) {

    // Clear screen
   
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("Spend: %d\n", game_state.score);

    for (int i = 0; i < game_state.rows; i++)
        printf("%s\n", game_state.map[i]);
}

/*
    player move
    input  : game_state, user's input
    output : not thing
    side effect : change the user and box's position, if it's allow.
*/
void playerMove(GameState *game_state) {

    printf("輸入 W/A/S/D 移動：");
    char input;
    scanf(" %c", &input);

    int dx = 0, dy = 0;

    switch (input) {
        case 'w': case 'W': dy = -1; break;
        case 's': case 'S': dy =  1; break;
        case 'a': case 'A': dx = -1; break;
        case 'd': case 'D': dx =  1; break;
        default: return; // Not allow input
    }

    int x = game_state->player.x;
    int y = game_state->player.y;
    int nx = x + dx;
    int ny = y + dy;

    // Boundary check
    if (nx < 0 || nx >= game_state->cols || ny < 0 || ny >= game_state->rows) {
        printf("撞牆啦！按 Enter 繼續...");
        getchar(); getchar();
        return;
    }

    char target = game_state->map[ny][nx];

    if (target == CH_EMPTY || target == CH_POINT) {
        // 移動玩家
        game_state->map[y][x] = (game_state->origin_map[y][x] == CH_POINT) ? CH_POINT : CH_EMPTY;
        game_state->map[ny][nx] = CH_PLAYER;
        game_state->player.x = nx;
        game_state->player.y = ny;
        game_state->score++;
    } else if (target == CH_BOX) {
        int bx = nx + dx;
        int by = ny + dy;

        // 箱子邊界檢查
        if (bx < 0 || bx >= game_state->cols || by < 0 || by >= game_state->rows) {
            printf("箱子撞到邊界！按 Enter 繼續...");
            getchar(); getchar();
            return;
        }

        char boxTarget = game_state->map[by][bx];
        if (boxTarget == CH_EMPTY || boxTarget == CH_POINT) {
            // 推箱子成功
            game_state->map[by][bx] = CH_BOX;
            game_state->map[ny][nx] = CH_PLAYER;
            game_state->map[y][x] = (game_state->origin_map[y][x] == CH_POINT) ? CH_POINT : CH_EMPTY;
            game_state->player.x = nx;
            game_state->player.y = ny;
            game_state->score++;
        } else {
            // 箱子前方被阻擋
            printf("箱子前方被阻擋！按 Enter 繼續...");
            getchar(); getchar();
        }
    }
    else if (target == CH_WALL) {
        printf("撞牆啦！按 Enter 繼續...");
        getchar(); getchar();
    }
}
/*
    game state check
    input  : game_state
    output : bool -> win or not
*/
int gameStateCheck(GameState game_state) {
    for (int i = 0; i < game_state.rows; i++) {
        for (int j = 0; j < game_state.cols; j++) {
            if (game_state.origin_map[i][j] == CH_POINT && game_state.map[i][j] != CH_BOX)
                return 0;
        }
    }
    return 1;
}
