#include "raylib.h"
#define OFFSET 25

void DrawLineVert(int x){
    DrawLineEx((Vector2){x + OFFSET, 0 + OFFSET}, (Vector2){x + OFFSET, 300 + OFFSET}, 3.0, BLACK);
}

void DrawLineHori(int y){
    DrawLineEx((Vector2){OFFSET, y + OFFSET}, (Vector2){300 + OFFSET, y + OFFSET}, 3.0, BLACK);
}

void DrawMap(){
    ClearBackground(RAYWHITE);
    DrawLineVert(100);
    DrawLineVert(200);
    DrawLineHori(100);
    DrawLineHori(200);
}

void DrawChars(char map[3][3]){
    for(int i = 0; i < 3; i++){
        for(int j = 0 ; j < 3; j++){
            if(map[i][j] == 'O')
                DrawRing((Vector2){i*100 + OFFSET + 50, j*100 + OFFSET + 50}, 40, 44, 0, 360, 100, BLUE);
            if(map[i][j] == 'X'){
                DrawLineEx((Vector2){i * 100 + OFFSET + 10, j * 100 + OFFSET + 10}, (Vector2){90 + i * 100 + OFFSET, 90 + j * 100 + OFFSET}, 3.0, RED);
                DrawLineEx((Vector2){i * 100 + OFFSET + 10, 90 + j * 100 + OFFSET}, (Vector2){90 + i * 100 + OFFSET, j * 100 + OFFSET + 10}, 3.0, RED);
            }
        }
    }
}

void Draw(char map[3][3]){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawMap();
    DrawChars(map);
    EndDrawing();
}

int isFree(char c){
    return c == 0;
}

bool win(char map[3][3]){
    for(int i = 0; i < 3; i++){
        //Vertical check
        if(map[i][0] != 0
        && map[i][0] == map[i][1]
        && map[i][1] == map[i][2])
            return true;

        //Horizontal check
        if(map[0][i] != 0
        && map[0][i] == map[1][i]
        && map[1][i] == map[2][i])
            return true;
    }

    //Diagonals
    if(map[0][0] == map[1][1]
    && map[1][1] == map[2][2]
    && map[1][1] != 0)
        return true;
    
    if(map[0][2] == map[1][1]
    && map[2][0] == map[1][1]
    && map[1][1] != 0)
        return true;

    return false;
}

int main() {
    InitWindow(350, 350, "TicTacToe");

    SetWindowPosition(3000, 0);
    ShowCursor();
    EnableCursor();

    SetTargetFPS(60);
    
    char map[3][3] = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    int x = -1, y = -1;
    int xTurn = 1;

    while (!WindowShouldClose()) {

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            x = (GetMouseX() - OFFSET) / 100;
            y = (GetMouseY() - OFFSET) / 100;

            if(x >= 0 && x < 3 && y >= 0 && y < 3){
                if(isFree(map[x][y])){
                    map[x][y] = xTurn ? 'X' : 'O';
                    xTurn = !xTurn;
                }
            }
        }

        Draw(map);

        if(win(map)){
            BeginDrawing();

            char winner[1];
            winner[0] = xTurn ? 'O' : 'X';

            DrawText(winner, 350/2 - 35, 20, 70, BLACK); 
            DrawText("Wygrales", 20, 100, 70, BLACK); 
            EndDrawing();
            WaitTime(3.0);
            break;
        }    
    }

    CloseWindow();
    return 0;
}
