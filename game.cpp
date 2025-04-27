#include "raylib.h"
#include <iostream>
using namespace std;
typedef enum GameScreen { LOGO = 0, TITLE,MAINMENU,THEME, GAMEPLAY, ENDING } GameScreen;

Color theme = BLACK;
Color theme_in = RAYWHITE;
Color text_color = PINK;
Color text_colorin = BLACK;

int x = 750;
int y = 750;
int inputarr[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void zero(int centerx, int centery);
void cross(int centerx, int centery);
void game();
bool checkWin(int player);
void takeinput(int mousex, int mousey, int player);
void computerMove();
int computerno();
void checkarr();
void DelayFrames(int frames);
void drawxo();
void sound();
int main() {
    InitWindow(y, x, "TIC_TAC_TOE");
    SetTargetFPS(60);
    GameScreen currentScreen = LOGO;
    int framescounter = 0;
    int player = 1; // 1 for player 1 (cross), 2 for computer (circle)
    bool gameOver = false;
    int winner = 0; 
    int mousex;
    int mousey ;
    int i = 0;
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(theme_in);

        switch (currentScreen) {
        case LOGO:
            framescounter++; // Count frames

            // Wait for 2 seconds (120 frames) before jumping to TITLE screen
            if (framescounter > 120) {
                currentScreen = TITLE;
            }
            break;

        case TITLE:
            DrawText("TIC-TAC-TOE", 250, 200, 50, text_color);
            DrawText("CREATED BY- ADESH GUPTA", 250, 300, 30, text_color);
            // Press enter to change to GAMEPLAY screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                currentScreen = MAINMENU;
                framescounter = 0;
            }
            break;
        case MAINMENU:
            
            mousex = GetMouseX();
            mousey = GetMouseY();
            DrawRectangle(250, 200,100,30,theme_in);
            DrawText("PLAY", 260, 210, 20, text_colorin);

            DrawRectangle(250, 300, 110,30, theme_in);
            DrawText("THEME", 260, 310, 20, text_colorin);
            if (mousex > 250 && mousex < 250 + 100 && mousey > 200 && mousey < 200 + 30) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                   
                    currentScreen = GAMEPLAY;
                }
            }
            if (mousex > 250 && mousex < 250 + 110 && mousey > 300 && mousey < 300 + 30) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentScreen = THEME;
                }
            }
            break;
        case THEME:
            DrawRectangle(250, 200, 100, 30, theme_in);
            DrawText("LIGHT(DEFAULT)", 260, 210, 20, text_colorin);

            DrawRectangle(250, 300, 110, 30, theme_in);
            DrawText("DARK", 260, 310, 20, text_colorin);
            if (mousex > 250 && mousex < 250 + 100 && mousey > 200 && mousey < 200 + 30) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    theme = BLACK;
                    text_color = PINK;
                    theme_in = RAYWHITE;
                    text_colorin = BLACK;
                    currentScreen = GAMEPLAY;
                }
            }
            if (mousex > 250 && mousex < 250 + 110 && mousey > 300 && mousey < 300 + 30) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    theme = RAYWHITE;
                    text_color = DARKGRAY;
                    theme_in = BLACK;
                    text_colorin = PINK;
                    currentScreen = GAMEPLAY;
                }
            }
            break;
        case GAMEPLAY:
            game();

            if (!gameOver && player == 1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                 mousex = GetMouseX();
                 mousey = GetMouseY();
                takeinput(mousex, mousey, player);
                if (checkWin(player)) {
                    gameOver = true;
                    winner = player;
                    
                }
                else {
                    player = 2; // Switch to computer's turn
                }
            }

            if (!gameOver && player == 2) {
                
                computerMove();
                if (checkWin(player)) {
                    gameOver = true;
                    winner = player;
                }
                else {
                    player = 1; // Switch to human player's turn
                }
            }

            if (gameOver) {
                i++;
                if (i == 1) {
                    
                DrawText((winner == 1 ? "Player (X) wins!" : "Computer (O) wins!"), 200, 150, 50, text_color);
                if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {

                    currentScreen = ENDING;
                }
                
            }
            }
            break;

        case ENDING:
            DrawText((winner == 1 ? "Player (X) wins!" : "Computer (O) wins!"), 250, 200, 50, text_color);

            // Press enter to return to TITLE screen
            if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) {
                currentScreen = TITLE;
                gameOver = false;
                winner = 0;
                memset(inputarr, 0, sizeof(inputarr)); // Reset the game board
            }
            break;

        default:
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void game() {
    int posx = x / 4;
    int posy = y / 4;
    float a = (90.0 - 12.5);
    float gamehi = 600;
    int blockhig = ((gamehi - 25) / 3) + 5;
    int mousex = GetMouseX(), mousey = GetMouseY();
    DrawText("TIC-TAC-TOE", (((posx - a) * 2.75) + 3), ((posy - a) - 50), 20, text_color);
    DrawRectangle(posx - 90, posy - 90, gamehi, gamehi, theme_in);
    DrawRectangle(posx - a, posy - a, gamehi - 25, gamehi - 25, theme);

    // Draw the grid
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int blockx = posx - a + i * (blockhig + 5);
            int blocky = posy - a + j * (blockhig + 5);
            DrawRectangle(blockx, blocky, blockhig, blockhig, theme_in);
            if (inputarr[i + j * 3] == 1) {
                cross(blockx + blockhig / 2, blocky + blockhig / 2);
            }
            else if (inputarr[i + j * 3] == 2) {
                /*DelayFrames(60);*/
                zero(blockx + blockhig / 2, blocky + blockhig / 2);
            }
        }
    }
}

void zero(int centerx, int centery) {
    DrawCircle(centerx, centery, 60, RED);
    DrawCircle(centerx, centery, 50, theme_in);
}

void cross(int centerx, int centery) {
    for (int i = 50; i <= 65; i++) {


        DrawLine(centerx - i, centery - i, centerx + i, centery + i, RED);
        DrawLine(centerx + i, centery - i, centerx - i, centery + i, RED);
    }
}

bool checkWin(int player) {
    // Check rows and columns
    
    for (int i = 0; i < 3; i++) {
        if ((inputarr[i * 3] == player && inputarr[i * 3 + 1] == player && inputarr[i * 3 + 2] == player) ||
            (inputarr[i] == player && inputarr[i + 3] == player && inputarr[i + 6] == player)) {
            return true;
        }

      
    }

    // Check diagonals
    if ((inputarr[0] == player && inputarr[4] == player && inputarr[8] == player) ||
        (inputarr[2] == player && inputarr[4] == player && inputarr[6] == player)) {
        return true;
    }

    return false;
}

void takeinput(int mousex, int mousey, int player) {
    int posx = x / 4;
    int posy = y / 4;
    float a = (90.0 - 12.5);
    float gamehi = 600;
    int blockhig = ((gamehi - 25) / 3) + 5;
    sound();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int blockx = posx - a + i * (blockhig + 5);
            int blocky = posy - a + j * (blockhig + 5);
            if (mousex > blockx && mousex < blockx + blockhig &&
                mousey > blocky && mousey < blocky + blockhig) {
                if (inputarr[i + j * 3] == 0) {
                    inputarr[i + j * 3] = player;
                }
            }
        }
    }
}

void computerMove() {
    while (true) {
        
        int index = computerno();
        if (inputarr[index] == 0) {
            inputarr[index] = 2;
            break;
        }
    }
}

int computerno() {
    return GetRandomValue(0, 8);
}

void checkarr() {
    int indexck = computerno();
    for (int i = 0; i <= indexck; i++) {
        if (inputarr[indexck] == 0) {
            inputarr[indexck] = 1;
            break;
        }
    }
}

void drawxo() {
    for (int i = 0; i < 9; i++) {
        if (inputarr[i] == 1) {
            int mousex = GetMouseX();
            int mousey = GetMouseY();
            cross(mousex, mousey);
        }
        else if (inputarr[i] == 2) {
            for (int j = 200; j <= 600; j = j + 200) {
                for (int k = 200; k <= 200; k = k + 200) {
                    zero(j, k);
                }
            }
        }
    }
}
void DelayFrames(int frames) {
    for (int i = 0; i < frames; ++i) {
        BeginDrawing();
        
        // Optionally draw something here if you want a visual during the delay
        EndDrawing();
    }
}
void sound() {
    InitAudioDevice();

    if (IsAudioDeviceReady()) {
        Sound click = LoadSound("XO.wav");

        if (IsSoundValid(click)) {
            PlaySound(click);
            UnloadSound(click); // Remember to unload sound after use
        }
        else {
            cout << "Failed to load sound!" << endl;
        }
    }
    else {
        cout << "Audio device not ready!" << endl;
    }
        CloseAudioDevice();
}