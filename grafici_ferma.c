#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define TILE_SIZE 64
#define GRID_ROWS 4
#define GRID_COLS 5
#define MAX_MESSAGES 3

const int screenWidth = 800;
const int screenHeight = 400;


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

typedef struct Player
{
    Vector2 position;
    Vector2 speed;
    Vector2 size;
    Rectangle bounds;
} Player;

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
Texture2D frogTex, charTex;

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
void DrawFarm(Inventory *inv) 
{
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
void DrawInventory(Inventory *inv) 
{
    int iconSize = 24;
    int x = 50, y = 10;

    // Resource icons + values
    DrawTextureEx(wheatTex, (Vector2){x, y}, 0.0f, (float)iconSize / wheatTex.width, WHITE);
    DrawText(TextFormat("%d", inv->wheat), x + 30, y + 4, 20, DARKGREEN);

    x += 80;
    DrawTextureEx(cornTex, (Vector2){x, y}, 0.0f, (float)iconSize / cornTex.width, WHITE);
    DrawText(TextFormat("%d", inv->corn), x + 30, y + 4, 20, DARKGREEN);

    x += 80;
    DrawTextureEx(milkIcon, (Vector2){x, y}, 0.0f, (float) 35 / milkIcon.width, WHITE);
    DrawText(TextFormat("%d", inv->milk), x + 30, y + 4, 20, DARKGREEN);

    x += 80;
    DrawTextureEx(coinIcon, (Vector2){x, y}, 0.0f, (float)20 / coinIcon.width, WHITE);
    DrawText(TextFormat("%d", inv->money), x + 30, y + 4, 20, DARKGREEN);

    // Seed inventory
    x = 400; y = 10;
    DrawTextureEx(seedIcon, (Vector2){x, y}, 0.0f, (float)iconSize / seedIcon.width, WHITE);
    DrawText(TextFormat("wheat: %d\ncorn: %d", inv->wheat_seed, inv->corn_seed), x + 30, y + 4, 20, PINK);
    
    // Action instructions
    DrawText("[1] Water  [2] Feed  [3] Harvest  [4] Plant", 50, 350, 20, DARKGRAY);
}

const char *frogMessages[MAX_MESSAGES] = {
    "Ribbit! Welcome to Farm Boss!",
    "Press the arrows to move.",
    "Have fun playing!"
};

int currentMessage = 0;
float typeTimer = 0.0f;
int typedChars = 0;
bool messageComplete = false;
bool dialogueFinished = false;

void UpdateFrogDialogue()
{
    float delta = GetFrameTime();

    // Typewriter animation
    if (!messageComplete) 
    {
        typeTimer += delta;

        if (typeTimer >= 0.05f) 
        { // Adjust speed here
            typedChars++;
            typeTimer = 0.0f;

            if (typedChars >= strlen(frogMessages[currentMessage])) 
            {
                messageComplete = true;
            }
        }
    }

    // Proceed to next message when user presses E
    if (messageComplete && IsKeyPressed(KEY_E)) 
    {
        currentMessage++;

        if (currentMessage >= MAX_MESSAGES)
        {
            dialogueFinished = true;
            return;
        }

        typedChars = 0;
        typeTimer = 0.0f;
        messageComplete = false;
    }
}

void DrawFrogDialogue() 
{
    if(dialogueFinished == true)
        return;

    int x = 500, y = 200;
    float scale = (float)150 / frogTex.width;

    DrawTextureEx(frogTex, (Vector2){x, y}, 0.0f, scale, WHITE);

    // Draw speech bubble
    DrawRectangle(x - 20, 170, 300, 25, WHITE);

    char buffer[256];
    strncpy(buffer, frogMessages[currentMessage], typedChars);
    buffer[typedChars] = '\0';

    DrawText(buffer, x - 20, 170, 20, PINK);

    if (messageComplete)
        DrawText("Press [E] to continue", x - 20, 195, 10, GRAY);
}

Player player;

void InitPlayer()
{
    player.position = (Vector2){ 500, 200};
    player.speed = (Vector2){ 8.0f, 0.0f };
    player.size = (Vector2){ 50, 24 };
    player.bounds = (Rectangle){ 0 };
}

void DrawChar() 
{   
    if(dialogueFinished == true)
    {
        int x = 500, y = 200;
        float scale = (float)50 / charTex.width;

        DrawTextureEx(charTex, player.position, 0.0f, scale, WHITE);
    }
}

int main() 
{
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
    frogTex = LoadTexture("assets/frog.png");
    charTex = LoadTexture("assets/character.png");

    InitPlayer();

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

               break; 
            } 
            case TITLE: 
            {
                // Update TITLE screen data here!
                
                framesCounter++;
                
                // LESSON 03: Inputs management (keyboard, mouse)
                if (IsKeyPressed(KEY_ENTER)) 
                    screen = GAMEPLAY;

                break;
            } 
            case GAMEPLAY:
            { 
                //gameplay logic
                if (IsKeyDown(KEY_LEFT))
                    player.position.x -= player.speed.x;

                if (IsKeyDown(KEY_RIGHT)) 
                    player.position.x += player.speed.x;

                if (IsKeyDown(KEY_UP)) 
                    player.position.y -= player.speed.x;

                if (IsKeyDown(KEY_DOWN)) 
                    player.position.y += player.speed.x;
                    
                if ((player.position.x) <= 0) 
                    player.position.x = 0;

                if ((player.position.x + player.size.x) >= screenWidth) 
                    player.position.x = screenWidth - player.size.x;
                    
                player.bounds = (Rectangle){ player.position.x, player.position.y, player.size.x, player.size.y };
                
                if (IsKeyPressed(KEY_ENTER)) 
                    screen = ENDING;

                break;
            } 
            case ENDING: 
            {
                // Update END screen data here!
                
                framesCounter++;
                
                // LESSON 03: Inputs management (keyboard, mouse)
                if (IsKeyPressed(KEY_ENTER)) 
                    screen = TITLE;

                break;
            } 
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
                    
                    UpdateFrogDialogue();
                    DrawFrogDialogue();

                    DrawChar();

                    break;
                } 
                case ENDING:
                {
                    DrawText("Thank you for playing, boss!", 50, 50, 50, PINK);

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
    UnloadTexture(frogTex);
    UnloadTexture(charTex); 
    //de dat unload la tot

    CloseWindow();
    return 0;
}
