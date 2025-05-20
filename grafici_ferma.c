#include "raylib.h"
#include <stdio.h>

#define TILE_SIZE 64
#define GRID_ROWS 4
#define GRID_COLS 5

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
Texture2D tileBgTex;      
Texture2D windowBgTex;    

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
    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) {
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
            switch (p->type) {
                case TILE_WHEAT: tex = wheatTex; break;
                case TILE_CORN: tex = cornTex; break;
                case TILE_COW: tex = cowTex; break;
                case TILE_EMPTY:
                default: tex = emptyTex; break;
            }

            // Draw the crop on top of the tile
            float cropScale = (float)TILE_SIZE / tex.width;
            DrawTextureEx(tex, tilePos, 0.0f, cropScale, WHITE);
        }
    }
}

// Draw inventory bar
void DrawHUD(Inventory *inv) 
{
    DrawText(TextFormat("🌾: %d 🌽: %d 🥛: %d 💰: %d", inv->wheat, inv->corn, inv->milk, inv->money), 50, 10, 20, DARKGREEN);
    DrawText(TextFormat("Seeds 🌾: %d 🌽: %d", inv->wheat_seed, inv->corn_seed), 400, 10, 20, DARKBLUE);
    DrawText("[1] Water  [2] Feed  [3] Harvest  [4] Plant", 50, 350, 20, DARKGRAY);
}


int main() 
{
    InitWindow(800, 400, "Farm Boss - Raylib Base");

    wheatTex = LoadTexture("assets/wheat.png");
    cornTex = LoadTexture("assets/corn.png");
    cowTex = LoadTexture("assets/cow.png");
    emptyTex = LoadTexture("assets/dirt.png");
    tileBgTex = LoadTexture("assets/grass.png");
    windowBgTex = LoadTexture("assets/window_bg.png");


    SetTargetFPS(60);

    Inventory farm;
    InitFarm(&farm);

    while (!WindowShouldClose()) 
    {
        // Draw full window background
        float bgScaleX = (float)GetScreenWidth() / windowBgTex.width;
        float bgScaleY = (float)GetScreenHeight() / windowBgTex.height;
        DrawTextureEx(windowBgTex, (Vector2){ 0, 0 }, 0.0f, bgScaleX, WHITE);

        DrawTextureEx(windowBgTex, (Vector2){ 0, 0 }, 0.0f, 
        (float)GetScreenWidth() / windowBgTex.width, WHITE);

        BeginDrawing();

        DrawFarm(&farm);
        DrawHUD(&farm);

        EndDrawing();
    }

    UnloadTexture(wheatTex);
    UnloadTexture(cornTex);
    UnloadTexture(cowTex);
    UnloadTexture(emptyTex);
    UnloadTexture(tileBgTex);
    UnloadTexture(windowBgTex);

    CloseWindow();
    return 0;
}
