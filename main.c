#include "raylib.h"
#include <stdio.h>
#include <limits.h>

#define OFFSET 25

#define WIDTH 350
#define HEIGHT 350

//TODO:
// 1. Refactor the code
// 2. Upgrade menu
// 3. Add dificulty levels to bots


enum Eval {
    Xwin = 10,
    Owin = -10,
    NoWinner = 0
};

enum GameState {
    InProgress,
    WinHorizontal1,
    WinHorizontal2,
    WinHorizontal3,
    WinVertical1,
    WinVertical2,
    WinVertical3,
    WinDiagTopBot,
    WinDiagBotTop,
    Draw
};

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

void DrawWinLine(enum GameState GameState){
    switch (GameState){
        case WinHorizontal1:
            DrawLineHori(50, GREEN);
            break;
        case WinHorizontal2:
            DrawLineHori(150, GREEN);
            break;
        case WinHorizontal3:
            DrawLineHori(250, GREEN);
            break;
        case WinVertical1:
            DrawLineVert(50, GREEN);
            break;
        case WinVertical2:
            DrawLineVert(150, GREEN);
            break;
        case WinVertical3:
            DrawLineVert(250, GREEN);
            break;
        case WinDiagTopBot:
            DrawLineEx((Vector2){0 + OFFSET, 0 + OFFSET}, (Vector2){300 + OFFSET, 300 + OFFSET}, 3.0, GREEN);
            break;
        case WinDiagBotTop:
            DrawLineEx((Vector2){0 + OFFSET, 300 + OFFSET}, (Vector2){300 + OFFSET, 0 + OFFSET}, 3.0, GREEN);
            break;
        default:
            break;
    }
}

void Display(char board[3][3], enum GameState GameState, int xTurn){
    BeginDrawing();
    
    ClearBackground(WHITE);

    DrawBoard(BLACK);

    DrawChars(board, RED, BLUE);

    if(GameState != InProgress){
        DrawChars(board, GRAY, GRAY);
        DrawWinLine(GameState);
    }

    EndDrawing();

    if(GameState != InProgress){
        WaitTime(1);
    }
}

int isFree(char c){
    return c == 0;
}

enum GameState CheckGameState(char board[3][3]){
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

    //Diagonal checks
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

void InitGame(char board[3][3], int* x, int* y, int* xTurn, enum GameState* GameState, int* Moves){
    *x = -1;
    *y = -1;
    *xTurn = 1;
    *GameState = InProgress;
    *Moves = 0;

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

void Logic(char board[3][3], int x, int y, int* xTurn, enum GameState* GameState, int* Moves){
    if(x != -1 && y != -1){
        board[x][y] = *xTurn ? 'X' : 'O';
        (*xTurn) = !(*xTurn);
        (*Moves)++;
    }

    *GameState = CheckGameState(board);

    if(*GameState == InProgress && (*Moves) >= 9){
        *GameState = 9;
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
        Color colorPlayBot = GRAY;
        Color colorExit = GRAY;
        int y = (GetMouseY());
        int x = (GetMouseX());
        SetMouseCursor(MOUSE_CURSOR_DEFAULT); 

        if(x >= (WIDTH - MeasureText("PLAY A BOT", 50))/2 - 20 && x <= (WIDTH - MeasureText("PLAY A BOT", 50))/2 - 20 + MeasureText("PLAY A BOT", 50) + 40){
            if(y >= 25 + OFFSET - 10 && y <= 25 + OFFSET - 10 + 65){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlay = DARKGRAY;
            }
            if(y >= 125 + OFFSET - 10 && y <= 125 + OFFSET - 10 + 65){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlayBot = DARKGRAY;
            }
            if(y >= 225 + OFFSET - 10 && y <= 225 + OFFSET - 10 + 65){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorExit = DARKGRAY;
            }
        }


        //Draw buttons and strings inside them
        DrawRectangle((WIDTH - MeasureText("PLAY A BOT", 50))/2 - 10, 25 + OFFSET - 10, MeasureText("PLAY A BOT", 50) + 20, 65, colorPlay);
        DrawText("PLAY", (WIDTH - MeasureText("PLAY", 50))/2, 25 + OFFSET, 50, BLACK);

        DrawRectangle((WIDTH - MeasureText("PLAY A BOT", 50))/2 - 10, 125 + OFFSET - 10, MeasureText("PLAY A BOT", 50) + 20, 65, colorPlayBot);
        DrawText("PLAY A BOT", (WIDTH - MeasureText("PLAY A BOT", 50))/2, 125 + OFFSET, 50, BLACK);

        DrawRectangle((WIDTH - MeasureText("PLAY A BOT", 50))/2 - 10, 225 + OFFSET - 10, MeasureText("PLAY A BOT", 50) + 20, 65, colorExit);
        DrawText("EXIT", (WIDTH - MeasureText("EXIT", 50))/2, 225 + OFFSET, 50, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            EndDrawing();
            int y = (GetMouseY());
            int x = (GetMouseX());
            if(x >= (WIDTH - MeasureText("PLAY A BOT", 50))/2 - 20 && x <= (WIDTH - MeasureText("PLAY A BOT", 50))/2 - 20 + MeasureText("PLAY A BOT", 50) + 40){
                if(y >= 25 + OFFSET - 10 && y <= 25 + OFFSET - 10 + 65){
                    return 1;
                }
                if(y >= 125 + OFFSET - 10 && y <= 125 + OFFSET - 10 + 65){
                    return 2;
                }
                if(y >= 225 + OFFSET - 10 && y <= 225 + OFFSET - 10 + 65){
                    return 0;
                }
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

enum Eval EvaluateBoard(char board[3][3], int Moves, int xTurn){
    enum GameState CurrentGameState = CheckGameState(board);
    if(CurrentGameState){
        if(!xTurn)
            return Xwin;
        else
            return Owin;
    }
    return NoWinner; // For now it always goes max depth, when it evaluates its always after all Moves if neither won its a draw
}


int MinMax(int depth, char board[3][3], int Moves, int xTurn, int* x1, int* y1){
    enum Eval score = EvaluateBoard(board, Moves, xTurn);
    if(score != NoWinner || depth == 0)
        return score;
        
    int best_move = xTurn ? INT_MIN : INT_MAX;
    int bestX = -1, bestY = -1;

    if(xTurn){
        for(int x = 0; x < 3; x++){
            for(int y = 0; y < 3; y++){
                if(isFree(board[x][y])){
                    board[x][y] = 'X';
                    int current_best = MinMax(depth - 1, board, Moves + 1, !xTurn, NULL, NULL);
                    board[x][y] = 0;
                    if(current_best > best_move){
                        best_move = current_best;
                        bestX = x;
                        bestY = y;
                    }
                }
            }
        }
    }
    else{
        for(int x = 0; x < 3; x++){
            for(int y = 0; y < 3; y++){
                if(isFree(board[x][y])){

                    board[x][y] = 'O';
                    int current_best = MinMax(depth - 1, board, Moves + 1, !xTurn, NULL, NULL);
                    board[x][y] = 0;

                    if(current_best < best_move){
                        best_move = current_best;
                        bestX = x;
                        bestY = y;
                    }
                }
            }
        }
    }
    if(x1 && y1){ // Set best moves only in the first call of the minmax
        *x1 = bestX;
        *y1 = bestY;
    }
    return best_move;
}

void BotMove(char board[3][3], int Moves, int xTurn, int* x, int *y){
    int BestX = -1;
    int BestY = -1;
    MinMax(9 - Moves, board, Moves, xTurn, &BestX, &BestY);
    *x = BestX;
    *y = BestY;
}

void StartBotGame(){
    int x, y, xTurn, Moves;
    enum GameState GameState;
    char board[3][3];

    InitGame(board, &x, &y, &xTurn, &GameState, &Moves);

    while (!WindowShouldClose() && !GameState) {
        Input(board, &x, &y);

        if(xTurn)
            BotMove(board, Moves, xTurn, &x, &y);

        Logic(board, x, y, &xTurn, &GameState, &Moves);

        Display(board, GameState, xTurn);
    }
}

void StartGame(){
    int x, y, xTurn, Moves;
    enum GameState GameState;
    char board[3][3];

    InitGame(board, &x, &y, &xTurn, &GameState, &Moves);

    while (!WindowShouldClose() && !GameState) {

        Input(board, &x, &y);
        
        Logic(board, x, y, &xTurn, &GameState, &Moves);

        Display(board, GameState, xTurn);
    }
}

void RunBotGameLoop(){
    do{
        StartBotGame();
    }while(ShowEndScreen());
}

void RunGameLoop(){
    do{
        StartGame();
    }while(ShowEndScreen());
}


int main() {   
    StartWindow(); 

    switch (ShowMainMenu()){
        case 1:
            RunGameLoop();
            break;
        case 2:
            RunBotGameLoop();
            break;
        case 0:
            break;
    }

    CloseWindow();
    return 0;
}
