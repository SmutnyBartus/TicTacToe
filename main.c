#include "raylib.h"
#define OFFSET 25

#define WIDTH 350
#define HEIGHT 350

//TODO: 
// 2. AutoSolver

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

void DrawWinMsg(int xTurn, int WinLinerNr){
    if(WinLinerNr == 9){
        DrawText("DRAW", (WIDTH - MeasureText("DRAW", 100))/2, 125, 100, BLACK);
        return;
    }
    if(!xTurn){
        DrawLineEx((Vector2){WIDTH / 2 - 35, 50 - 35}, (Vector2){WIDTH / 2 + 35, 50 + 35}, 10.0, BLACK);
        DrawLineEx((Vector2){WIDTH / 2 - 35, 50 + 35}, (Vector2){WIDTH / 2 + 35, 50 - 35}, 10.0, BLACK);  
    }
    else{
        DrawRing((Vector2){WIDTH/2, 60}, 30, 35, 0, 360, 100, BLACK);
    }

    DrawText("WON", (WIDTH - MeasureText("WON", 100))/2, 125, 100, BLACK);
}

void Display(char board[3][3], int WinLinerNr, int xTurn){
    BeginDrawing();
    
    ClearBackground(WHITE);

    DrawBoard(BLACK);

    DrawChars(board, RED, BLUE);

    if(WinLinerNr != 0){
        DrawChars(board, GRAY, GRAY);
        DrawWinLine(WinLinerNr);
        //DrawWinMsg(xTurn, WinLinerNr);
    }

    EndDrawing();

    if(WinLinerNr != 0){
        WaitTime(1);
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

void InitGame(char board[3][3], int* x, int* y, int* xTurn, int* WinLineNr, int* moves){
    *x = -1;
    *y = -1;
    *xTurn = 1;
    *WinLineNr = 0;
    *moves = 0;

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

void Logic(char board[3][3], int x, int y, int* xTurn, int *WinLineNr, int* moves){
    if(x != -1 && y != -1){
        board[x][y] = *xTurn ? 'X' : 'O';
        (*xTurn) = !(*xTurn);
        (*moves)++;
    }

    *WinLineNr = Win(board);

    if(*WinLineNr == 0 && (*moves) >= 9){
        *WinLineNr = 9;
    }
}

void StartGame(){
    int x, y, xTurn, moves, WinLineNr; // WinLineNr is also a gameover variable {0 ~ still playing, 2 - 8 ~ one of players one, 9 ~ draw}
    char board[3][3];
    InitGame(board, &x, &y, &xTurn, &WinLineNr, &moves);

    while (!WindowShouldClose() && !WinLineNr) {

        Input(board, &x, &y);
        
        Logic(board, x, y, &xTurn, &WinLineNr, &moves);

        Display(board, WinLineNr, xTurn);
    }
}

void StartWindow(){
    InitWindow(WIDTH, HEIGHT, "TicTacToe");

    SetWindowPosition(3000, 0);
    ShowCursor();
    EnableCursor();
    EnableEventWaiting();

    SetTargetFPS(60);
}

int ShowMainMenu(){
    while(!WindowShouldClose()){
        ClearBackground(WHITE);
        BeginDrawing();

        //Hightlighjt hovered button
        Color colorPlay = GRAY;
        Color colorExit = GRAY;
        int y = (GetMouseY());
        int x = (GetMouseX());
        SetMouseCursor(MOUSE_CURSOR_DEFAULT); 
        if(x >= (WIDTH - MeasureText("PLAY", 50))/2 - 20 && x <= (WIDTH - MeasureText("PLAY", 50))/2 - 20 + MeasureText("PLAY", 50) + 40){
            if(y >= 75 + OFFSET - 10 && y <= 75 + OFFSET - 10 + 65){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlay = DARKGRAY;
            }
            if(y >= 175 + OFFSET - 10 && y <= 175 + OFFSET - 10 + 65){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorExit = DARKGRAY;
            }
        }


        //Draw buttons and strings inside them
        DrawRectangle((WIDTH - MeasureText("PLAY", 50))/2 - 20, 75 + OFFSET - 10, MeasureText("PLAY", 50) + 40, 65, colorPlay);
        DrawText("PLAY", (WIDTH - MeasureText("PLAY", 50))/2, 75 + OFFSET, 50, BLACK);

        DrawRectangle((WIDTH - MeasureText("PLAY", 50))/2 - 20, 175 + OFFSET - 10, MeasureText("PLAY", 50) + 40, 65, colorExit);
        DrawText("EXIT", (WIDTH - MeasureText("EXIT", 50))/2, 175 + OFFSET, 50, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            EndDrawing();
            int y = (GetMouseY());
            int x = (GetMouseX());
            if(x >= (WIDTH - MeasureText("PLAY", 50))/2 - 20 && x <= (WIDTH - MeasureText("PLAY", 50))/2 - 20 + MeasureText("PLAY", 50) + 40){
                if(y >= 75 + OFFSET - 10 && y <= 75 + OFFSET - 10 + 65)
                    return 1;
                if(y >= 175 + OFFSET - 10 && y <= 175 + OFFSET - 10 + 65)
                    return 0;
            }
        }
        EndDrawing();
    }
}

int ShowEndScreen(){
        while(!WindowShouldClose()){
        ClearBackground(WHITE);
        BeginDrawing();

        //Hightlighjt hovered button
        Color colorPlay = GRAY;
        Color colorExit = GRAY;
        int y = (GetMouseY());
        int x = (GetMouseX());
        SetMouseCursor(MOUSE_CURSOR_DEFAULT); 
        if(x >= (WIDTH - MeasureText("PLAY AGAIN", 50))/2 - 20 && x <= (WIDTH - MeasureText("PLAY AGAIN", 50))/2 - 20 + MeasureText("PLAY AGAIN", 50) + 40){
            if(y >= 75 + OFFSET - 10 && y <= 75 + OFFSET - 10 + 65){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlay = DARKGRAY;
            }
            if(y >= 175 + OFFSET - 10 && y <= 175 + OFFSET - 10 + 65){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorExit = DARKGRAY;
            }
        }


        //Draw buttons and strings inside them
        DrawRectangle((WIDTH - MeasureText("PLAY AGAIN", 50))/2 - 10, 75 + OFFSET - 10, MeasureText("PLAY AGAIN", 50) + 20, 65, colorPlay);
        DrawText("PLAY AGAIN", (WIDTH - MeasureText("PLAY AGAIN", 50))/2, 75 + OFFSET, 50, BLACK);

        DrawRectangle((WIDTH - MeasureText("PLAY AGAIN", 50))/2 - 10, 175 + OFFSET - 10, MeasureText("PLAY AGAIN", 50) + 20, 65, colorExit);
        DrawText("EXIT", (WIDTH - MeasureText("EXIT", 50))/2, 175 + OFFSET, 50, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            EndDrawing();
            int y = (GetMouseY());
            int x = (GetMouseX());
            if(x >= (WIDTH - MeasureText("PLAY AGAIN", 50))/2 - 20 && x <= (WIDTH - MeasureText("PLAY AGAIN", 50))/2 - 20 + MeasureText("PLAY AGAIN", 50) + 40){
                if(y >= 75 + OFFSET - 10 && y <= 75 + OFFSET - 10 + 65)
                    return 1;
                if(y >= 175 + OFFSET - 10 && y <= 175 + OFFSET - 10 + 65)
                    return 0;
            }
        }
        EndDrawing();
    }
}

void RunGameLoop(){
    do{
        StartGame();
    }while(ShowEndScreen());
}

int main() {   
    StartWindow(); 

    if(ShowMainMenu()){
        RunGameLoop();
    }

    CloseWindow();
    return 0;
}
