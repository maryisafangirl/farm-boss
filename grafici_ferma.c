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
    STATUS_WAITING, STATUS_READY, STATUS_HARVESTED
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
    bool hasWateringCan;
    bool hasSickle;
    bool hasShovel;
    Plot plots[GRID_ROWS][GRID_COLS];
} Inventory;

typedef struct 
{
    Vector2 position;
    bool isVisible;
    Rectangle bounds;
} WateringCan;

typedef struct 
{
    Vector2 position;
    bool isVisible; 
    Rectangle bounds;
} Sickle;

typedef struct 
{
    Vector2 position;
    bool isVisible; 
    Rectangle bounds;
} Shovel;

Texture2D wheatTex, cornTex, cowTex, emptyTex;
Texture2D tileBgTex, windowBgTex, texLogo, texLogoScreen; 
Texture2D milkIcon, coinIcon, seedIcon, before_waterTex;
Texture2D frogTex, charTex, wateringCanTex, sickleTex, shovelTex;

int currentMessage = 0;
float typeTimer = 0.0f;
int typedChars = 0;
bool messageComplete = false;
bool dialogueFinished = false;

// Initialize farm
void InitFarm(Inventory *inv)
{
    inv->wheat = inv->corn = inv->milk = 0;
    inv->money = 10;
    inv->wheat_seed = 3;
    inv->corn_seed = 2;
    inv->hasWateringCan = false;
    inv->hasSickle = false;

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
                p->status = STATUS_WAITING; 
            }
        }
    }
}

WateringCan wateringCan;
Sickle sickle;
Shovel shovel;

void InitWateringCan()
{
    wateringCan.position = (Vector2){ 700, 150 };  // Position it somewhere on the farm
    wateringCan.isVisible = true;
    float scale = 32.0f / wateringCanTex.width;
    int actualWidth = (int)(wateringCanTex.width * scale);
    int actualHeight = (int)(wateringCanTex.height * scale);
    wateringCan.bounds = (Rectangle){ wateringCan.position.x, wateringCan.position.y, actualWidth, actualHeight };
}

void InitSickle()
{
    sickle.position = (Vector2){ 700, 200 };  // Position it somewhere on the farm
    sickle.isVisible = true;
    float scale = 32.0f / sickleTex.width;
    int actualWidth = (int)(sickleTex.width * scale);
    int actualHeight = (int)(sickleTex.height * scale);
    sickle.bounds = (Rectangle){ sickle.position.x, sickle.position.y, actualWidth, actualHeight };
}

void InitShovel()
{
    shovel.position = (Vector2){ 700, 250 };  // Position it somewhere on the farm
    shovel.isVisible = true;
    float scale = 32.0f / shovelTex.width;
    int actualWidth = (int)(shovelTex.width * scale);
    int actualHeight = (int)(shovelTex.height * scale);
    shovel.bounds = (Rectangle){ shovel.position.x, shovel.position.y, actualWidth, actualHeight };
}

bool CheckWateringCanPickup(Player *player, WateringCan *can, Inventory *inv)
{
    if (!can->isVisible || !dialogueFinished) 
        return false;
    
    if (CheckCollisionRecs(player->bounds, can->bounds))
    {
        inv->hasWateringCan = true;
        can->isVisible = false;
        return true;
    }

    return false;
}

bool CheckSicklePickup(Player *player, Sickle *sickle, Inventory *inv)
{
    if (!sickle->isVisible || !dialogueFinished) 
        return false;
    
    if (CheckCollisionRecs(player->bounds, sickle->bounds))
    {
        inv->hasSickle = true;
        sickle->isVisible = false;
        return true;
    }

    return false;
}

bool CheckShovelPickup(Player *player, Shovel *shovel, Inventory *inv)
{
    if (!shovel->isVisible || !dialogueFinished) 
        return false;
    
    if (CheckCollisionRecs(player->bounds, shovel->bounds))
    {
        inv->hasShovel = true;
        shovel->isVisible = false;
        return true;
    }

    return false;
}

void HarvestPlot(Plot *plot, Inventory *inv)
{
    if (plot->type != TILE_EMPTY && plot->status == STATUS_READY)
    {
        plot->status = STATUS_HARVESTED;

        if(plot->type == TILE_WHEAT)
        {
            inv->wheat_seed++;
            inv->wheat++;
        }

        if(plot->type == TILE_CORN)
        {
            inv->corn_seed++;
            inv->corn++; 
        }
    }
}

void Plant(Plot *plot, Inventory *inv, TileType selectedCrop)
{
    if (plot->type == TILE_EMPTY || plot->status == STATUS_HARVESTED)
    {
        if (selectedCrop == TILE_WHEAT && inv->wheat_seed > 0)
        {
            plot->type = TILE_WHEAT;
            plot->status = STATUS_WAITING;
            inv->wheat_seed--;
        }
        else if (selectedCrop == TILE_CORN && inv->corn_seed > 0)
        {
            plot->type = TILE_CORN;
            plot->status = STATUS_WAITING;
            inv->corn_seed--;
        }
    }
}

void WaterPlot(Plot *plot)
{
    if (plot->type != TILE_EMPTY && plot->status == STATUS_WAITING)
    {
        plot->status = STATUS_READY;
    }
}

void HandleWateringInteraction(Player *player, Inventory *inv)
{
    if (!inv->hasWateringCan) 
        return;
    
    // Check which plot the player is near
    for (int r = 0; r < GRID_ROWS; r++)
    {
        for (int c = 0; c < GRID_COLS; c++)
        {
            int x = 50 + c * TILE_SIZE;
            int y = 50 + r * TILE_SIZE;
            
            Rectangle tileRect = { x, y, TILE_SIZE, TILE_SIZE };
            
            // Check if player is close to this tile
            if (CheckCollisionRecs(player->bounds, tileRect))
            {
                WaterPlot(&inv->plots[r][c]);

                Plot *p = &inv->plots[r][c];

                int x = 50 + c * TILE_SIZE;
                int y = 50 + r * TILE_SIZE;
                Vector2 tilePos = { (float)x, (float)y };

                Texture2D tex;
                switch (p->type) 
                {
                    case TILE_WHEAT: 
                    {
                        if(p->status == STATUS_READY)
                            tex = wheatTex;
                        break;
                    }
                    case TILE_CORN: 
                    {
                        if(p->status == STATUS_READY)
                            tex = cornTex;
                        break;
                    }
                    default:
                        tex = emptyTex;
                }

                float cropScale = (float)TILE_SIZE / tex.width;
                DrawTextureEx(tex, tilePos, 0.0f, cropScale, WHITE);

                return;
            }
        }  
    }
}

void HandleSickleInteraction(Player *player, Inventory *inv)
{
    if (!inv->hasSickle) 
        return;
    
    // Check which plot the player is near
    for (int r = 0; r < GRID_ROWS; r++)
    {
        for (int c = 0; c < GRID_COLS; c++)
        {
            int x = 50 + c * TILE_SIZE;
            int y = 50 + r * TILE_SIZE;
            
            Rectangle tileRect = { x, y, TILE_SIZE, TILE_SIZE };
            
            // Check if player is close to this tile
            if (CheckCollisionRecs(player->bounds, tileRect))
            {
                HarvestPlot(&inv->plots[r][c], inv);

                Plot *p = &inv->plots[r][c];

                int x = 50 + c * TILE_SIZE;
                int y = 50 + r * TILE_SIZE;
                Vector2 tilePos = { (float)x, (float)y };

                Texture2D tex;
                switch (p->type) 
                {
                    case TILE_WHEAT: 
                    {
                        if(p->status == STATUS_HARVESTED)
                            tex = emptyTex;

                        break;
                    }
                    case TILE_CORN: 
                    {
                        if(p->status == STATUS_HARVESTED)
                            tex = emptyTex;

                        break;
                    }
                    default:
                        tex = emptyTex;
                }

                float cropScale = (float)TILE_SIZE / tex.width;
                DrawTextureEx(tex, tilePos, 0.0f, cropScale, WHITE);

                return;
            }
        }  
    }
}

void HandleShovelInteraction(Player *player, Inventory *inv)
{
    if (!inv->hasShovel) 
        return;

    for (int r = 0; r < GRID_ROWS; r++)
    {
        for (int c = 0; c < GRID_COLS; c++)
        {
            int x = 50 + c * TILE_SIZE;
            int y = 50 + r * TILE_SIZE;
            Rectangle tileRect = { x, y, TILE_SIZE, TILE_SIZE };
            Vector2 tilePos = { (float)x, (float)y };

            if (CheckCollisionRecs(player->bounds, tileRect))
            {
                // Player presses 1 to plant wheat, 2 for corn
                if (IsKeyPressed(KEY_ONE) && inv->wheat_seed > 0)
                {
                    float cropScale = (float)TILE_SIZE / wheatTex.width;
                    DrawTextureEx(wheatTex, tilePos, 0.0f, cropScale, WHITE);
                    Plant(&inv->plots[r][c], inv, TILE_WHEAT);
                }

                if (IsKeyPressed(KEY_TWO) && inv->corn_seed > 0)
                {
                    float cropScale = (float)TILE_SIZE / cornTex.width;
                    DrawTextureEx(cornTex, tilePos, 0.0f, cropScale, WHITE);
                    Plant(&inv->plots[r][c], inv, TILE_CORN);
                }

                return;
            }
        }
    }
}

void FeedCowsInteraction(Player *player, Inventory *inv)
{
    if (inv->wheat < 1) return;

    for (int r = 0; r < GRID_ROWS; r++)
    {
        for (int c = 0; c < GRID_COLS; c++)
        {
            if (inv->plots[r][c].type == TILE_COW)
            {
                int x = 50 + c * TILE_SIZE;
                int y = 50 + r * TILE_SIZE;
                Rectangle tileRect = { x, y, TILE_SIZE, TILE_SIZE };

                if (CheckCollisionRecs(player->bounds, tileRect))
                {
                    inv->wheat--;
                    inv->milk++;
                    return;
                }
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
                    if(p->status == STATUS_WAITING)
                        tex = before_waterTex;
                    if(p->status == STATUS_READY)
                        tex = wheatTex; 
                    if(p->status == STATUS_HARVESTED)
                        tex = emptyTex;

                    break;
                }
                case TILE_CORN: 
                {
                    if(p->status == STATUS_WAITING)
                        tex = before_waterTex;
                    if(p->status == STATUS_READY)
                        tex = cornTex; 
                    if(p->status == STATUS_HARVESTED)
                        tex = emptyTex;

                    break;
                }
                case TILE_COW: 
                {
                    tex = cowTex; 
                    break;
                }
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

    if(inv->hasShovel)
        DrawText("[1] Plant Wheat   [2] Plant Corn", 50, 320, 20, DARKGRAY);
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

    x += 80;
    if (inv->hasWateringCan)
    {
        DrawTextureEx(wateringCanTex, (Vector2){x, y}, 0.0f, (float)iconSize / wateringCanTex.width, WHITE);

        DrawText("Can", x + 30, y + 4, 20, BLUE);
        DrawText("[W] Water Plants", 50, 350, 20, DARKGRAY);
        DrawText("[Q] Toggle Watering Can", 50, 370, 20, DARKGRAY);
    }
    else
        InitWateringCan();

    x += 80;
    if (inv->hasSickle)
    {
        DrawTextureEx(sickleTex, (Vector2){x, y}, 0.0f, (float)iconSize / sickleTex.width, WHITE);

        DrawText("Sickle", x + 30, y + 4, 20, YELLOW);
        if(inv->hasWateringCan == true)
        {
            DrawText("[C] Collect resources", 500, 350, 20, DARKGRAY);
            DrawText("[E] Toggle sickle", 500, 370, 20, DARKGRAY);
        }
        else
        {
            DrawText("[C] Collect resources", 50, 350, 20, DARKGRAY);
            DrawText("[E] Toggle sickle", 50, 370, 20, DARKGRAY);
        }
    }
    else
        InitSickle();

    x += 80;

    if (inv->hasShovel)
    {
        DrawTextureEx(shovelTex, (Vector2){x, y}, 0.0f, (float)iconSize / shovelTex.width, WHITE);

        DrawText("Shovel", x + 30, y + 4, 20, GREEN);
    }
    else
        InitShovel();

    // Seed inventory
    x += 95;
    y = 10;
    DrawTextureEx(seedIcon, (Vector2){x, y}, 0.0f, (float)iconSize / seedIcon.width, WHITE);
    DrawText(TextFormat("wheat: %d\ncorn: %d", inv->wheat_seed, inv->corn_seed), x + 30, y + 4, 20, PINK);
}

const char *frogMessages[MAX_MESSAGES] = {
    "Ribbit! Welcome to Farm Boss!",
    "Press the arrows to move.",
    "Have fun playing!"
};

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
    
    float charScale = 32.0f / charTex.width; // or whatever width you want
    float visualWidth = charTex.width * charScale;
    float visualHeight = charTex.height * charScale;

    player.size = (Vector2){ visualWidth, visualHeight };

    player.bounds = (Rectangle){ 0 };
}

void DrawChar() 
{   
    if(dialogueFinished == true)
    {
        int x = 500, y = 200;
        float scale = (float) 50 / charTex.width;

        DrawTextureEx(charTex, player.position, 0.0f, scale, WHITE);
    }
}

void DrawWateringCan(WateringCan *can)
{
    if (!can->isVisible || !dialogueFinished) 
        return;
    
    float scale = 32.0f / wateringCanTex.width; 
    DrawTextureEx(wateringCanTex, can->position, 0.0f, scale, WHITE);
}

void DrawSickle(Sickle *sickle)
{
    if (!sickle->isVisible || !dialogueFinished) 
        return;
    
    float scale = 40.0f / sickleTex.width; 
    DrawTextureEx(sickleTex, sickle->position, 0.0f, scale, WHITE);
}

void DrawShovel(Shovel *shovel)
{
    if (!shovel->isVisible || !dialogueFinished) 
        return;
    
    float scale = 40.0f / shovelTex.width; 
    DrawTextureEx(shovelTex, shovel->position, 0.0f, scale, WHITE);
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
    wateringCanTex = LoadTexture("assets/watering_can.png");
    before_waterTex = LoadTexture("assets/plant_not_ready.png");
    sickleTex = LoadTexture("assets/sickle.png");
    shovelTex = LoadTexture("assets/shovel.png");

    GameScreen screen = LOGO;

    SetTargetFPS(60);

    Inventory farm;
    InitFarm(&farm);
    InitPlayer();
    InitWateringCan();
    InitSickle();
    InitShovel();

    while (!WindowShouldClose()) //ESC to end
    {
        switch(screen) 
        {
            case LOGO: 
            {           
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
                
                player.bounds = (Rectangle){ player.position.x, player.position.y + player.size.y - 20,  player.size.x, 20};

                if (!farm.hasWateringCan)
                {
                    CheckWateringCanPickup(&player, &wateringCan, &farm);
                }

                if (!farm.hasSickle) 
                {
                    CheckSicklePickup(&player, &sickle, &farm);
                }

                if (!farm.hasShovel) 
                {
                    CheckShovelPickup(&player, &shovel, &farm);
                }

                if (IsKeyPressed(KEY_W))
                {
                    HandleWateringInteraction(&player, &farm);
                }

                if (IsKeyPressed(KEY_Q) && farm.hasWateringCan)
                {
                    farm.hasWateringCan = false;
                }

                HandleShovelInteraction(&player, &farm);

                if (IsKeyPressed(KEY_U) && farm.hasShovel)
                {
                    farm.hasShovel = false;
                }

                if (IsKeyPressed(KEY_C))
                {
                    HandleSickleInteraction(&player, &farm);
                }

                if (IsKeyPressed(KEY_E) && farm.hasSickle)
                {
                    farm.hasSickle = false;
                }

                if (IsKeyPressed(KEY_F)) // Feed cows
                {
                    FeedCowsInteraction(&player, &farm);
                }

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
                    DrawWateringCan(&wateringCan);
                    DrawSickle(&sickle);
                    DrawShovel(&shovel);

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
    UnloadTexture(wateringCanTex);
    UnloadTexture(before_waterTex);
    UnloadTexture(sickleTex);
    UnloadTexture(shovelTex);
    //de dat unload la tot

    CloseWindow();
    return 0;
}
