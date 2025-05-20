#include "raylib.h"
#include <stdio.h>

#define TILE_SIZE 64
#define GRID_ROWS 4
#define GRID_COLS 5

typedef enum GameScreen 
{ 
    LOGO, TITLE, GAMEPLAY, ENDING 
} GameScreen;

// Plot types
typedef enum 
{
    TILE_WHEAT, TILE_CORN, TILE_COW, TILE_EMPTY, TILE_NONE
} TileType;

// Plot status
typedef enum 
{
    STATUS_EMPTY, STATUS_WAITING, STATUS_READY
} PlotStatus;

typedef struct 
{
    TileType type;
    PlotStatus status;
} Plot;

// Simple game inventory
typedef struct 
{
    int wheat, corn, milk, money;
    int wheat_seed, corn_seed;
    Plot plots[GRID_ROWS][GRID_COLS];
} Inventory;

Texture2D wheatTex, cornTex, cowTex, emptyTex;
Texture2D tileBgTex, windowBgTex, texLogo, texLogoScreen; 
Texture2D milkIcon, coinIcon, seedIcon;

// Initialize farm
void InitFarm(Inventory *inv)
{
    inv->wheat = inv->corn = inv->milk = 0;
    inv->money = 10;
    inv->wheat_seed = 3;
    inv->corn_seed = 2;

    for (int r = 0; r < GRID_ROWS; r++)
     {
        for (int c = 0; c < GRID_COLS; c++) 
        {
            Plot *p = &inv->plots[r][c];
            if (r == 0) 
            { 
                p->type = TILE_WHEAT; 
                p->status = STATUS_WAITING; 
            }
            else if (r == 1 && c < 3) 
            { 
                p->type = TILE_CORN;
                p->status = STATUS_WAITING; 
            }
            else if (r == 2 && c < 2) 
            {
                p->type = TILE_COW;
                p->status = STATUS_WAITING; 
            }
            else 
            {
                p->type = TILE_EMPTY; 
                p->status = STATUS_EMPTY; 
            }
        }
    }
}

// Draw the farm grid
void DrawFarm(Inventory *inv) {
    for (int r = 0; r < GRID_ROWS; r++) 
    {
        for (int c = 0; c < GRID_COLS; c++) 
        {
            // Position of the tile
            int x = 50 + c * TILE_SIZE;
            int y = 50 + r * TILE_SIZE;
            Vector2 tilePos = { (float)x, (float)y };

            // Draw the tile background (e.g., dirt)
            float tileScale = (float)TILE_SIZE / tileBgTex.width;
            DrawTextureEx(tileBgTex, tilePos, 0.0f, tileScale, WHITE);

            // Get the crop texture based on plot type
            Plot *p = &inv->plots[r][c];
            Texture2D tex;
            switch (p->type) 
            {
                case TILE_WHEAT: 
                {
                    tex = wheatTex; 
                    break;
                }
                case TILE_CORN: 
                {
                    tex = cornTex; 
                    break;
                }
                case TILE_COW: 
                {
                    tex = cowTex; 
                    break;
                }
                case TILE_EMPTY:
                default: 
                {
                    tex = emptyTex; 
                    break;
                }
            }

            // Draw the crop on top of the tile
            float cropScale = (float)TILE_SIZE / tex.width;
            DrawTextureEx(tex, tilePos, 0.0f, cropScale, WHITE);
        }
    }
}

// Draw inventory bar
void DrawInventory(Inventory *inv) {
    int iconSize = 24;
    int x = 50, y = 10;

    // Resource icons + values
    DrawTextureEx(wheatTex, (Vector2){x, y}, 0.0f, (float)iconSize / wheatTex.width, WHITE);
    DrawText(TextFormat("%d", inv->wheat), x + 30, y + 4, 20, DARKGREEN);

    x += 80;
    DrawTextureEx(cornTex, (Vector2){x, y}, 0.0f, (float)iconSize / cornTex.width, WHITE);
    DrawText(TextFormat("%d", inv->corn), x + 30, y + 4, 20, DARKGREEN);

    x += 80;
    DrawTextureEx(milkIcon, (Vector2){x, y}, 0.0f, (float)20 / milkIcon.width, WHITE);
    DrawText(TextFormat("%d", inv->milk), x + 30, y + 4, 20, DARKGREEN);

    x += 80;
    DrawTextureEx(coinIcon, (Vector2){x, y}, 0.0f, (float)20 / coinIcon.width, WHITE);
    DrawText(TextFormat("%d", inv->money), x + 30, y + 4, 20, DARKGREEN);

    // Seed inventory
    x = 400; y = 10;
    DrawTextureEx(seedIcon, (Vector2){x, y}, 0.0f, (float)iconSize / seedIcon.width, WHITE);
    DrawText(TextFormat("wheat: %d\ncorn: %d", inv->wheat_seed, inv->corn_seed), x + 30, y + 4, 20, DARKBLUE);
    
    // Action instructions
    DrawText("[1] Water  [2] Feed  [3] Harvest  [4] Plant", 50, 350, 20, DARKGRAY);
}


int main() 
{
    const int screenWidth = 800;
    const int screenHeight = 400;

    InitWindow(screenWidth, screenHeight, "Farm Boss");

    int framesCounter = 0;          

    wheatTex = LoadTexture("assets/wheat.png");
    cornTex = LoadTexture("assets/corn.png");
    cowTex = LoadTexture("assets/cow.png");
    emptyTex = LoadTexture("assets/grass.png");
    tileBgTex = LoadTexture("assets/grass.png");
    windowBgTex = LoadTexture("assets/background_field.jpeg");
    texLogo = LoadTexture("assets/cat_logo.png");
    texLogoScreen = LoadTexture("assets/logo_background.jpeg");
    milkIcon = LoadTexture("assets/milk_icon.png");
    coinIcon = LoadTexture("assets/coin_icon.png");
    seedIcon = LoadTexture("assets/seeds.png");

    GameScreen screen = LOGO;

    SetTargetFPS(60);

    Inventory farm;
    InitFarm(&farm);

    while (!WindowShouldClose()) //ESC to end
    {
        switch(screen) 
        {
            case LOGO: 
            {
                // Update LOGO screen data here!
                
                framesCounter++;
                
                if (framesCounter > 180) 
                {
                    screen = TITLE;    // Change to TITLE screen after 3 seconds
                    framesCounter = 0;
                }
                
            } break;
            case TITLE: 
            {
                // Update TITLE screen data here!
                
                framesCounter++;
                
                // LESSON 03: Inputs management (keyboard, mouse)
                if (IsKeyPressed(KEY_ENTER)) screen = GAMEPLAY;
                
            } break;
            case GAMEPLAY:
            { 
                //gameplay logic
                
                if (IsKeyPressed(KEY_ENTER)) screen = ENDING;

            } break;
            case ENDING: 
            {
                // Update END screen data here!
                
                framesCounter++;
                
                // LESSON 03: Inputs management (keyboard, mouse)
                if (IsKeyPressed(KEY_ENTER)) screen = TITLE;

            } break;
            default: break;
        }

        // Draw full window background
        float bgScaleX = (float)GetScreenWidth() / windowBgTex.width;
        float bgScaleY = (float)GetScreenHeight() / windowBgTex.height;
        DrawTextureEx(windowBgTex, (Vector2){ 0, 0 }, 0.0f, bgScaleX, WHITE);

        DrawTextureEx(windowBgTex, (Vector2){ 0, 0 }, 0.0f, 
        (float)GetScreenWidth() / windowBgTex.width, WHITE);

        BeginDrawing();

            switch(screen) 
            {
                case LOGO:
                {
                    float scale = 0.5f; // 50% of original size

                    DrawTextureEx(texLogoScreen, (Vector2){ 0, 0 }, 0.0f, 
                    (float)GetScreenWidth() / texLogoScreen.width, WHITE);

                    DrawTextureEx(texLogo,
                    (Vector2){ screenWidth/2 - texLogo.width * scale / 2, screenHeight/2 - texLogo.height * scale / 2 },
                    0.0f, scale, WHITE);

                    break;
                } 
                case TITLE:
                {
                    DrawText("Welcome to Farm Boss!", 50, 50, 60, WHITE);

                    if ((framesCounter/30)%2 == 0) DrawText("PRESS [ENTER] to START", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] to START", 20)/2, GetScreenHeight()/2 + 60, 20, PINK);
                    break;
                } 
                case GAMEPLAY:
                {
                    // TODO: Draw GAMEPLAY screen here!
                    DrawFarm(&farm);
                    DrawInventory(&farm);

                    break;
                } 
                case ENDING:
                {
                    DrawText("Thank you for playing, boss!", 50, 50, 60, PINK);

                    if ((framesCounter/30)%2 == 0) DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 + 80, 20, GRAY);
                    break;
                } 
                default: break;
            }

        EndDrawing();
    }

    UnloadTexture(wheatTex);
    UnloadTexture(cornTex);
    UnloadTexture(cowTex);
    UnloadTexture(emptyTex);
    UnloadTexture(tileBgTex);
    UnloadTexture(windowBgTex);
    UnloadTexture(texLogo);
    UnloadTexture(texLogoScreen);
    UnloadTexture(milkIcon);
    UnloadTexture(coinIcon);
    UnloadTexture(seedIcon);
    //de dat unload la tot

    CloseWindow();
    return 0;
}
