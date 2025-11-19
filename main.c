#include "raylib.h"
#include <stdio.h>
#include <limits.h>

#define OFFSET 25

#define WIDTH 350
#define HEIGHT 350

#define LINE_WIDTH 3.0
#define PADDING 10

#define TILE_WIDTH 100
#define TILE_HEIGHT 100

#define TEXT_SIZE 50

//TODO:
// 1. Add dificulty levels to bots

enum Choice{
    EXIT,
    PLAY,
    MENU,
    PLAYBOT
};

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

void DrawLineVertical(float col, Color color){
    col*=TILE_HEIGHT;
    DrawLineEx((Vector2){col + OFFSET, OFFSET}, (Vector2){col + OFFSET, 3 * TILE_HEIGHT + OFFSET}, LINE_WIDTH, color);
}

void DrawLineHorizontal(float row, Color color){
    row*=TILE_WIDTH;
    DrawLineEx((Vector2){OFFSET, row + OFFSET}, (Vector2){3 * TILE_WIDTH + OFFSET, row + OFFSET}, LINE_WIDTH, color);
}

void DrawX(int row, int col, Color color){
    row *= TILE_WIDTH;
    col *= TILE_HEIGHT;
    DrawLineEx((Vector2){row + OFFSET + PADDING, col + OFFSET + PADDING}, (Vector2){row + OFFSET + TILE_WIDTH - PADDING, col + OFFSET + TILE_HEIGHT - PADDING}, LINE_WIDTH, color);
    DrawLineEx((Vector2){row + OFFSET + PADDING, col + OFFSET + TILE_HEIGHT - PADDING}, (Vector2){row + OFFSET + TILE_WIDTH - PADDING, col + OFFSET + PADDING}, LINE_WIDTH, color);
}

void DrawO(int row, int col, Color Color){
    row *= TILE_WIDTH;
    col *= TILE_HEIGHT;
    DrawRing((Vector2){row + OFFSET + TILE_WIDTH / 2, col + OFFSET + TILE_HEIGHT / 2}, TILE_WIDTH / 2 - PADDING, TILE_WIDTH / 2 - PADDING + LINE_WIDTH, 0, 360, 100, Color);
}

void DrawBoard(Color color){
    ClearBackground(WHITE);
    DrawLineVertical(1, color);
    DrawLineVertical(2, color);
    DrawLineHorizontal(1, color);
    DrawLineHorizontal(2, color);
}

void DrawChars(char board[3][3], Color colorX, Color colorO){
    for(int x = 0; x < 3; x++){
        for(int y = 0 ; y < 3; y++){
            if(board[x][y] == 'O')
                DrawO(x, y, colorO);
            if(board[x][y] == 'X')
                DrawX(x, y, colorX);
        }
    }
}

void DrawWinLine(enum GameState GameState){
    switch (GameState){
        case WinHorizontal1:
            DrawLineHorizontal(0.5, GREEN);
            break;
        case WinHorizontal2:
            DrawLineHorizontal(1.5, GREEN);
            break;
        case WinHorizontal3:
            DrawLineHorizontal(2.5, GREEN);
            break;
        case WinVertical1:
            DrawLineVertical(0.5, GREEN);
            break;
        case WinVertical2:
            DrawLineVertical(1.5, GREEN);
            break;
        case WinVertical3:
            DrawLineVertical(2.5, GREEN);
            break;
        case WinDiagTopBot:
            DrawLineEx((Vector2){OFFSET, OFFSET}, (Vector2){3 * TILE_WIDTH + OFFSET, 3 * TILE_HEIGHT + OFFSET}, LINE_WIDTH, GREEN);
            break;
        case WinDiagBotTop:
            DrawLineEx((Vector2){OFFSET, 3 * TILE_HEIGHT + OFFSET}, (Vector2){3 * TILE_WIDTH + OFFSET, OFFSET}, LINE_WIDTH, GREEN);
            break;
        default:
            break;
    }
}

void Display(char board[3][3], enum GameState GameState, int xTurn){
    BeginDrawing();
    
    ClearBackground(WHITE);

    DrawBoard(BLACK);

    DrawChars(board, BLUE, RED);

    if(GameState != InProgress){
        DrawChars(board, GRAY, GRAY);
        DrawWinLine(GameState);
    }

    EndDrawing();

    if(GameState != InProgress){
        WaitTime(1);
    }
}

enum Choice ShowMainMenu(){
    Color colorPlay = GRAY;
    Color colorPlayBot = GRAY;
    Color colorExit = GRAY;
    int x = -1;
    int y = -1;


    //Calculate Button and Text
    int MaxTextWidth = MeasureText("PLAY A BOT", TEXT_SIZE);
    int ButtonWidth = MaxTextWidth + 2 * PADDING;
    int ButtonHeight = TEXT_SIZE + 2 * PADDING;

    int ButtonX = (WIDTH - MaxTextWidth)/2 - PADDING;

    int ButtonPlayY = (HEIGHT - 2 * OFFSET) / 3 - (TEXT_SIZE + 2 * PADDING);
    int ButtonBotY = 2 * ((HEIGHT - 2 * OFFSET) / 3) - (TEXT_SIZE + 2 * PADDING);
    int ButtonExitY = HEIGHT - 2 * OFFSET - (TEXT_SIZE + 2 * PADDING);

    while(!WindowShouldClose()){
        ClearBackground(WHITE);
        BeginDrawing();

        colorPlay = GRAY;
        colorPlayBot = GRAY;
        colorExit = GRAY;

        y = (GetMouseY());
        x = (GetMouseX());
        SetMouseCursor(MOUSE_CURSOR_DEFAULT); 

        //Hightlight hovered button
        if(x >= ButtonX && x <= ButtonX + ButtonWidth){
            if(y >= ButtonPlayY && y <= ButtonPlayY + ButtonHeight){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlay = DARKGRAY;
            }
            if(y >= ButtonBotY && y <= ButtonBotY + ButtonHeight){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlayBot = DARKGRAY;
            }
            if(y >= ButtonExitY && y <= ButtonExitY + ButtonHeight){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorExit = DARKGRAY;
            }
        }

        //Draw buttons and strings inside them
        DrawRectangle(ButtonX, ButtonPlayY, ButtonWidth, ButtonHeight, colorPlay);
        DrawText("PLAY", ButtonX + (ButtonWidth  - MeasureText("PLAY", TEXT_SIZE)) / 2, ButtonPlayY + (ButtonHeight - TEXT_SIZE) / 2, TEXT_SIZE, BLACK);
        

        DrawRectangle(ButtonX, ButtonBotY, ButtonWidth, ButtonHeight, colorPlayBot);
        DrawText("PLAY A BOT", ButtonX + (ButtonWidth  - MeasureText("PLAY A BOT", TEXT_SIZE)) / 2, ButtonBotY + (ButtonHeight - TEXT_SIZE) / 2, TEXT_SIZE, BLACK);


        DrawRectangle(ButtonX, ButtonExitY, ButtonWidth, ButtonHeight, colorExit);
        DrawText("EXIT", ButtonX + (ButtonWidth  - MeasureText("EXIT", TEXT_SIZE)) / 2, ButtonExitY + (ButtonHeight - TEXT_SIZE) / 2, TEXT_SIZE, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            EndDrawing();
            if(x >= ButtonX && x <= ButtonX + ButtonWidth){
                if(y >= ButtonPlayY && y <= ButtonPlayY + ButtonHeight){
                    return PLAY;
                }
                if(y >= ButtonBotY && y <= ButtonBotY + ButtonHeight){
                    return PLAYBOT;
                }
                if(y >= ButtonExitY && y <= ButtonExitY + ButtonHeight){
                    return EXIT;
                }
            }
        }
        EndDrawing();
    }
}

enum Choice ShowEndScreen(){
    Color colorPlay = GRAY;
    Color colorPlayBot = GRAY;
    Color colorExit = GRAY;
    int x = -1;
    int y = -1;


    //Calculate Button and Text
    int MaxTextWidth = MeasureText("PLAY AGAIN", TEXT_SIZE);
    int ButtonWidth = MaxTextWidth + 2 * PADDING;
    int ButtonHeight = TEXT_SIZE + 2 * PADDING;

    int ButtonX = (WIDTH - MaxTextWidth)/2 - PADDING;

    int ButtonPlayY = (HEIGHT - 2 * OFFSET) / 3 - (TEXT_SIZE + 2 * PADDING);
    int ButtonMenuY = 2 * ((HEIGHT - 2 * OFFSET) / 3) - (TEXT_SIZE + 2 * PADDING);
    int ButtonExitY = HEIGHT - 2 * OFFSET - (TEXT_SIZE + 2 * PADDING);

    while(!WindowShouldClose()){
        ClearBackground(WHITE);
        BeginDrawing();

        colorPlay = GRAY;
        colorPlayBot = GRAY;
        colorExit = GRAY;

        y = (GetMouseY());
        x = (GetMouseX());
        SetMouseCursor(MOUSE_CURSOR_DEFAULT); 

        //Hightlight hovered button
        if(x >= ButtonX && x <= ButtonX + ButtonWidth){
            if(y >= ButtonPlayY && y <= ButtonPlayY + ButtonHeight){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlay = DARKGRAY;
            }
            if(y >= ButtonMenuY && y <= ButtonMenuY + ButtonHeight){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorPlayBot = DARKGRAY;
            }
            if(y >= ButtonExitY && y <= ButtonExitY + ButtonHeight){
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                colorExit = DARKGRAY;
            }
        }

        //Draw buttons and strings inside them
        DrawRectangle(ButtonX, ButtonPlayY, ButtonWidth, ButtonHeight, colorPlay);
        DrawText("PLAY AGAIN", ButtonX + (ButtonWidth  - MeasureText("PLAY AGAIN", TEXT_SIZE)) / 2, ButtonPlayY + (ButtonHeight - TEXT_SIZE) / 2, TEXT_SIZE, BLACK);
        

        DrawRectangle(ButtonX, ButtonMenuY, ButtonWidth, ButtonHeight, colorPlayBot);
        DrawText("MENU", ButtonX + (ButtonWidth  - MeasureText("MENU", TEXT_SIZE)) / 2, ButtonMenuY + (ButtonHeight - TEXT_SIZE) / 2, TEXT_SIZE, BLACK);


        DrawRectangle(ButtonX, ButtonExitY, ButtonWidth, ButtonHeight, colorExit);
        DrawText("EXIT", ButtonX + (ButtonWidth  - MeasureText("EXIT", TEXT_SIZE)) / 2, ButtonExitY + (ButtonHeight - TEXT_SIZE) / 2, TEXT_SIZE, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            EndDrawing();
            if(x >= ButtonX && x <= ButtonX + ButtonWidth){
                if(y >= ButtonPlayY && y <= ButtonPlayY + ButtonHeight){
                    return PLAY;
                }
                if(y >= ButtonMenuY && y <= ButtonMenuY + ButtonHeight){
                    return MENU; // FOR NOW BOTH MENU AND EXIT GO TO MENU
                }
                if(y >= ButtonExitY && y <= ButtonExitY + ButtonHeight){
                    return EXIT;
                }
            }
        }
        EndDrawing();
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
    if(x == -1 || y == -1){
        return;
    }

    board[x][y] = *xTurn ? 'X' : 'O';
    (*xTurn) = !(*xTurn);
    (*Moves)++;

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

enum Eval EvaluateBoard(char board[3][3], int Moves, int xTurn){
    enum GameState CurrentGameState = CheckGameState(board);
    if(CurrentGameState){
        if(!xTurn)
            return Xwin;
        else
            return Owin;
    }
    return NoWinner; // for now draw is as good as a game in progress
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
    if(x1 && y1){ // Set best moves, only in the first call of the minmax
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

enum Choice RunBotGameLoop(){
    enum Choice choice;

    do{
        StartBotGame();
        choice = ShowEndScreen();
    }while(choice == PLAY);

    return choice;
}

enum Choice RunGameLoop(){
    enum Choice choice;

    do{
        StartGame();
        choice = ShowEndScreen();
    }while(choice == PLAY);

    return choice;
}


int main() {   
    StartWindow(); 

    while (!WindowShouldClose())
    {
        enum Choice choice = ShowMainMenu();
        switch(choice){
            case 0:
                CloseWindow();
                return 0;
            case 1:
                if(RunGameLoop() == EXIT){
                    CloseWindow();
                    return 0;
                }
                break;
            case 2:
                if(RunBotGameLoop() == EXIT){
                    CloseWindow();
                    return 0;
                }
                break;
        }
    }

    CloseWindow();
    return 0;
}
