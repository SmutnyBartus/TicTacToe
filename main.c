#include "raylib.h"
#define OFFSET 25

#define WIDTH 350
#define HEIGHT 350

//TODO: 
// 3. Add Menu and win interface
// 3. AutoSolver

void DrawLineVert(int x, Color color){
    DrawLineEx((Vector2){x + OFFSET, 0 + OFFSET}, (Vector2){x + OFFSET, 300 + OFFSET}, 3.0, color);
}

void DrawLineHori(int y, Color color){
    DrawLineEx((Vector2){OFFSET, y + OFFSET}, (Vector2){300 + OFFSET, y + OFFSET}, 3.0, color);
}

void DrawBoard(Color color){
    ClearBackground(RAYWHITE);
    DrawLineVert(100, color);
    DrawLineVert(200, color);
    DrawLineHori(100, color);
    DrawLineHori(200, color);
}

void DrawChars(char board[3][3], Color x, Color y){
    for(int i = 0; i < 3; i++){
        for(int j = 0 ; j < 3; j++){
            if(board[i][j] == 'O')
                DrawRing((Vector2){i*100 + OFFSET + 50, j*100 + OFFSET + 50}, 40, 44, 0, 360, 100, x);
            if(board[i][j] == 'X'){
                DrawLineEx((Vector2){i * 100 + OFFSET + 10, j * 100 + OFFSET + 10}, (Vector2){90 + i * 100 + OFFSET, 90 + j * 100 + OFFSET}, 3.0, y);
                DrawLineEx((Vector2){i * 100 + OFFSET + 10, 90 + j * 100 + OFFSET}, (Vector2){90 + i * 100 + OFFSET, j * 100 + OFFSET + 10}, 3.0, y);
            }
        }
    }
}

void DrawWinLine(int WinLinerNr){
    switch (WinLinerNr){
        case 1:
            DrawLineHori(50, GREEN);
            break;
        case 2:
            DrawLineHori(150, GREEN);
            break;
        case 3:
            DrawLineHori(250, GREEN);
            break;
        case 4:
            DrawLineVert(50, GREEN);
            break;
        case 5:
            DrawLineVert(150, GREEN);
            break;
        case 6:
            DrawLineVert(250, GREEN);
            break;
        case 7:
            DrawLineEx((Vector2){0 + OFFSET, 0 + OFFSET}, (Vector2){300 + OFFSET, 300 + OFFSET}, 3.0, GREEN);
            break;
        case 8:
            DrawLineEx((Vector2){0 + OFFSET, 300 + OFFSET}, (Vector2){300 + OFFSET, 0 + OFFSET}, 3.0, GREEN);
            break;
        default:
            break;
    }
}

void DrawWinMsg(int xTurn){
    if(!xTurn){
        DrawLineEx((Vector2){WIDTH / 2 - 34, 100 - 34}, (Vector2){WIDTH / 2 + 34, 100 + 34}, 7.0, BLACK);
        DrawLineEx((Vector2){WIDTH / 2 - 34, 100 + 34}, (Vector2){WIDTH / 2 + 34, 100 - 34}, 7.0, BLACK);  
    }
    else{
        DrawRing((Vector2){WIDTH/2, 100}, 27, 34, 0, 360, 100, BLACK);
    }

    DrawText("WYGRYWA", (WIDTH - MeasureText("WYGRYWA", 65))/2, 200, 65, BLACK);
}

void Display(char board[3][3], int WinLinerNr, int xTurn){
    BeginDrawing();
    
    ClearBackground(RAYWHITE);

    DrawBoard(BLACK);

    DrawChars(board, RED, BLUE);

    if(WinLinerNr != 0){
        DrawChars(board, GRAY, GRAY);
        DrawWinLine(WinLinerNr);
        DrawWinMsg(xTurn);
    }

    EndDrawing();

    if(WinLinerNr != 0){
        WaitTime(2.0);
    }
}

int isFree(char c){
    return c == 0;
}

int Win(char board[3][3]){
    for(int i = 0; i < 3; i++){
        //Horizontal check
        if(board[0][i] != 0
        && board[0][i] == board[1][i]
        && board[1][i] == board[2][i])
            return i + 1;
        //Vertical check
        if(board[i][0] != 0
        && board[i][0] == board[i][1]
        && board[i][1] == board[i][2])
            return i + 4;
    }

    //Diagonal check
    if(board[0][0] == board[1][1]
    && board[1][1] == board[2][2]
    && board[1][1] != 0)
        return 7;
    
    if(board[0][2] == board[1][1]
    && board[2][0] == board[1][1]
    && board[1][1] != 0)
        return 8;

    return 0;
}

void InitGame(char board[3][3], int* x, int* y, int* xTurn, int* WinLineNr){
    InitWindow(WIDTH, HEIGHT, "TicTacToe");

    SetWindowPosition(3000, 0);
    ShowCursor();
    EnableCursor();
    EnableEventWaiting();

    SetTargetFPS(60);

    *x = -1;
    *y = -1;
    *xTurn = 1;
    *WinLineNr = 0;

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++)
            board[i][j] = 0;
    }
    
}

void Input(char board[3][3], int* x, int* y){
    *x = -1;
    *y = -1;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *x = (GetMouseX() - OFFSET) / 100;
        *y = (GetMouseY() - OFFSET) / 100;
        
        if(((*x) < 0 || (*x) > 2 || (*y) < 0 || (*y) > 2) || (!isFree(board[*x][*y]))){
            // Field is either taken or out of bounds
            *x = -1;
            *y = -1;
            return;
        }
    }
}

void Logic(char board[3][3], int x, int y, int* xTurn, int *WinLineNr){
    static int moves = 0;
    if(x != -1 && y != -1){
        board[x][y] = *xTurn ? 'X' : 'O';
        (*xTurn) = !(*xTurn);
        moves++;
    }

    *WinLineNr = Win(board);

    if(*WinLineNr == 0 && moves >= 9){
        *WinLineNr = 9;
    }
}

int main() {    
    int x, y, xTurn;
    int WinLineNr; // Also a gameover variable
    char board[3][3];
    InitGame(board, &x, &y, &xTurn, &WinLineNr);

    while (!WindowShouldClose() && !WinLineNr) {

        Input(board, &x, &y);
        
        Logic(board, x, y, &xTurn, &WinLineNr);

        Display(board, WinLineNr, xTurn);
    }

    CloseWindow();
    return 0;
}
