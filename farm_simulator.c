#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define WHEAT 0
#define CORN 1
#define COW 2
#define EMPTY 3


typedef struct
{
   int corn_seed;
   int wheat_seed;
}Seeds;

typedef enum
{
    empty, //cod 0
    waiting, //cod 1
    ready, //cod 2
}PlotStatus;

typedef struct
{
    char type[21]; //literele corespunzatoare
    PlotStatus status;
}SingularPlot;

typedef struct
{
    int wheatp; //cod 0
    int cornp; //cod 1
    int cowp; //cod 2      
    int emptyp; //default; cod 3
    SingularPlot layout[MAX_ROWS][MAX_COLS];
}Terrain;

typedef struct 
{
    int wheat;
    int corn;
    int milk;
    Seeds seed;
    Terrain plots; 
    int money;
}Inventory;

Inventory initial() 
{
    Inventory inventar;

    inventar.wheat = 0; 
    inventar.corn = 0;
    inventar.milk = 0;
    inventar.money = 2;
    inventar.seed.corn_seed = 1;
    inventar.seed.wheat_seed = 1;
    
    inventar.plots.wheatp = 4;
    inventar.plots.cornp = 2;
    inventar.plots.cowp = 3;
    inventar.plots.emptyp = 3;
    
    for(int i = 0; i < 4; i++)
    {
        switch(i)
        {
            case 0:
            {
                for(int j = 0; j < inventar.plots.wheatp; j++)
                {
                    inventar.plots.layout[i][j].status = waiting;
                    strcpy(inventar.plots.layout[i][j].type, "w");
                }
            
                break;
            }
            case 1:
            {
                for(int j = 0; j < inventar.plots.cornp; j++)
                {
                    inventar.plots.layout[i][j].status = waiting;
                    strcpy(inventar.plots.layout[i][j].type, "c");
                }
                
                break;
            }
            case 2:
            {
                for(int j = 0; j < inventar.plots.cowp; j++)
                {
                    inventar.plots.layout[i][j].status = waiting;
                    strcpy(inventar.plots.layout[i][j].type, "moo");
                }
      
                break;
            }
            case 3:
            {
                for(int j = 0; j < inventar.plots.emptyp; j++)
                {
                    inventar.plots.layout[i][j].status = empty;
                    strcpy(inventar.plots.layout[i][j].type, " ");
                }
                
                break;
            }
        }
    }

    return inventar;
}

void status(const Inventory inventar) 
{
    printf("\n===== Farm Status =====\n");
    
    for(int i = 0; i < 4; i++)
    {
        switch(i)
        {
            case WHEAT:
            {
                if(inventar.plots.wheatp != 0)
                {
                    printf("Wheat:🌾 ");
                    for(int j = 0; j < inventar.plots.wheatp; j++)
                        printf("[%s] ", inventar.plots.layout[i][j].type);
                }
                
                printf("\n");
                
                break;
            }
            case CORN:
            {
                if(inventar.plots.cornp != 0)
                {
                    printf("Corn:🌽 ");
                    for(int j = 0; j < inventar.plots.cornp; j++)
                        printf("[%s] ", inventar.plots.layout[i][j].type);
                }
                
                printf("\n");
                
                break;
            }
            case COW:
            {
                if(inventar.plots.cowp != 0)
                {
                    printf("Cow:🐄 ");
                    for(int j = 0; j < inventar.plots.cowp; j++)
                        printf("[%s] ", inventar.plots.layout[i][j].type);
                }
                
                printf("\n");
                
                break;
            }
            default:
            {
                printf("Empty:🟫 ");
                for(int j = 0; j < inventar.plots.emptyp; j++)
                    printf("[%s] ", inventar.plots.layout[i][j].type);
                        
                printf("\n");        
                
                break;
            }
        }
    }
     
    printf("========================\n");
    printf("\n");
}

void water(Inventory *inventar) //choice 1
{
    int choice, which, specific, all_or_one = 0;
    
    printf("Which crop do you want to water?\n");
    
    printf("1. Wheat Plots (%d available)\n", inventar->plots.wheatp);
    printf("2. Corn Plots (%d available)\n", inventar->plots.cornp);
    printf("3. All plots (%d available)\n", inventar->plots.cornp + inventar->plots.wheatp);
    
    printf("Input:");
    scanf("%d", &choice);
    
    choice--;
    
    if(choice != 2)
    {
        printf("Do you want to water:\n");
        printf("   1. All of them.\n   2. A specific one.\n");
        printf("Input:");
        scanf("%d", &all_or_one);
    }
    
    switch(choice)
    {
        case 0:
        {
            if(all_or_one == 1)
            {
                for(int j = 0; j < inventar->plots.wheatp; j++)
                {
                    if(inventar->plots.layout[choice][j].status == waiting)
                    {
                        inventar->plots.layout[choice][j].status = ready;
                        strcpy(inventar->plots.layout[choice][j].type, "W");
                    }
                }
            }
            else
            {
                printf("Choose which wheat plot to water from 1 to %d.\n", inventar->plots.wheatp);
                printf("Input: ");
                scanf("%d", &specific);
                
                specific--;
                
                if(inventar->plots.layout[choice][specific].status == waiting)
                {
                    inventar->plots.layout[choice][specific].status = ready;
                    strcpy(inventar->plots.layout[choice][specific].type, "W");
                }
                else
                    printf("This plot has already been watered.");
            }
        
            break;
        }
        case 1:
        {
            if(all_or_one == 1)
            {
                for(int j = 0; j < inventar->plots.cornp; j++)
                {
                    if(inventar->plots.layout[choice][j].status == waiting)
                    {
                        inventar->plots.layout[choice][j].status = ready;
                        strcpy(inventar->plots.layout[choice][j].type, "C");
                    }
                }
            }
            else
            {
                printf("Choose which corn plot to water from 1 to %d.\n", inventar->plots.cornp);
                printf("Input: ");
                scanf("%d", &specific);
                
                specific--;
                
                if(inventar->plots.layout[choice][specific].status == waiting)
                {
                    inventar->plots.layout[choice][specific].status = ready;
                    strcpy(inventar->plots.layout[choice][specific].type, "C");
                }
                else
                    printf("This plot has already been watered.");
            }
            
            break;
        }
        case 2:
        {
            for(int i = 0; i < 2; i++)
            {
                if(i == 0)
                {
                    for(int j = 0; j < inventar->plots.wheatp; j++)
                    {
                        inventar->plots.layout[i][j].status = ready;
                        strcpy(inventar->plots.layout[i][j].type, "W");
                    }
                }
                if(i == 1)
                {
                    for(int j = 0; j < inventar->plots.cornp; j++)
                    {
                        inventar->plots.layout[i][j].status = ready;
                        strcpy(inventar->plots.layout[i][j].type, "C");
                    }
                }
            }
        }
        default:
        {
            printf("Invalid input!\n");
            break;
        }
    }
        
    printf("\n");
}

void feed(Inventory *inventar) //choice 2
{
    int choice, all_or_one, specific, abandon;
    
    printf("Which animal do you want to feed?\n");
    printf("  1. Cows\n");
    //printf("  2. All animals\n");
    
    printf("Input:");
    scanf("%d", &choice);
    
    choice--;
    
    if(choice != 1)
    {
        printf("Do you want to feed:\n");
        printf("   1. All of them.\n   2. A specific one.\n");
        printf("Input:");
        scanf("%d", &all_or_one);
    }
    
    switch(choice)
    {
        case 0:
        {
            if(all_or_one == 1)
            {
                printf("You need %d wheat to complete the action,you have %d available\n", inventar->plots.cowp, inventar->wheat);
                printf("   1. Feed\n");
                printf("   2. Abandon\n");
                
                printf("Input: ");
                scanf("%d", &abandon);
                
                if(inventar->wheat >= inventar->plots.cowp)
                {
                    for(int j = 0; j < inventar->plots.cowp; j++)
                    {
                        if(inventar->plots.layout[COW][j].status == waiting)
                        {
                            inventar->plots.layout[COW][j].status = ready;
                            strcpy(inventar->plots.layout[COW][j].type, "MOO");
                        }
                    }
                }
                else
                {
                    printf("Not enough resources!\n");
                        break;
                }
            }
            else
            {
                printf("Choose which cow to feed from 1 to %d.\n", inventar->plots.cowp);
                printf("Input: ");
                scanf("%d", &specific);
                
                specific--;
                
                printf("You need 1 wheat to complete the action,you have %d available\n", inventar->wheat);
                printf("   1. Feed\n");
                printf("   2. Abandon\n");
                
                printf("Input: ");
                scanf("%d", &abandon);
                
                if(abandon == 1)
                {
                    if(inventar->wheat > 1)
                    {
                        if(inventar->plots.layout[COW][specific].status == waiting)
                        {
                            inventar->plots.layout[COW][specific].status = ready;
                            strcpy(inventar->plots.layout[COW][specific].type, "MOO");
                        }
                        else
                            printf("This animal has already been fed.\n");
                    }
                    else
                    {
                        printf("Not enough resources!\n");
                        break;
                    }
                }
                else if(abandon == 2)
                {
                    break;
                }
                else
                {
                    printf("Invalid input!\n");
                    break;
                }
            }
        
            break;
        }
        default:
        {
            printf("Invalid input!\n");
            break;
        }
    }
        
    printf("\n");
}

void collect_resources(Inventory *inventar) //choice 3
{
    int choice, wheat_to_harvest = 0, corn_to_harvest = 0, cows_to_milk = 0;
    
    printf("Which resource do you want to harvest?\n");
    
    for(int j = 0; j < inventar->plots.wheatp; j++)
        if(inventar->plots.layout[WHEAT][j].status == ready)
            wheat_to_harvest++;
            
    for(int j = 0; j < inventar->plots.cornp; j++)
        if(inventar->plots.layout[CORN][j].status == ready)
            corn_to_harvest++;
            
    for(int j = 0; j < inventar->plots.cowp; j++)
        if(inventar->plots.layout[COW][j].status == ready)
            cows_to_milk++;
        
    
    printf("1. Wheat Plots: %d available\n", wheat_to_harvest);
    printf("2. Corn Plots: %d available\n", corn_to_harvest);
    printf("3. Cows: %d available\n", cows_to_milk);
    printf("4. Collect all: %d available\n", wheat_to_harvest + corn_to_harvest + cows_to_milk);
    
    printf("Input:");
    scanf("%d", &choice);
    
    choice--;
    
    switch(choice)
    {
        case 0:
        {
            int aux = inventar->plots.wheatp;
            
            for(int j = 0; j < aux; j++)
            {
                if(inventar->plots.layout[choice][j].status == ready)
                {
                    strcpy(inventar->plots.layout[choice][j].type, " ");
                    inventar->plots.layout[choice][j].status = empty;
                    (inventar->wheat) += 2;
                    (inventar->seed.wheat_seed)++;
                }
            }
        
            break;
        }
        case 1:
        {
            int aux = inventar->plots.cornp;
            
            for(int j = 0; j < aux; j++)
            {
                if(inventar->plots.layout[choice][j].status == ready)
                {
                    strcpy(inventar->plots.layout[choice][j].type, " ");
                    inventar->plots.layout[choice][j].status = empty;
                    (inventar->corn) += 2;
                    (inventar->seed.corn_seed)++;
                }
            }
            
            break;
        }
        case 2:
        {
            for(int j = 0; j < inventar->plots.cowp; j++)
            {
                if(inventar->plots.layout[choice][j].status == ready)
                {
                    strcpy(inventar->plots.layout[choice][j].type, "moo");
                    inventar->plots.layout[choice][j].status = waiting;
                    (inventar->milk)++;
                }
            }
            
            break;
        }
        case 3:
        {
            for(int i = 0; i < 3; i++)
            {
                if(i == 0)
                {
                    int aux = inventar->plots.wheatp;
                    
                    for(int j = 0; j < aux; j++)
                    {
                        if(inventar->plots.layout[i][j].status == ready)
                        {
                            strcpy(inventar->plots.layout[i][j].type, " ");
                            inventar->plots.layout[i][j].status = empty;
                            (inventar->wheat) += 2;
                            (inventar->seed.wheat_seed)++;
                        }
                    }
                }
                if(i == 1)
                {
                    int aux = inventar->plots.cornp;
            
                    for(int j = 0; j < aux; j++)
                    {
                        if(inventar->plots.layout[i][j].status == ready)
                        {
                            strcpy(inventar->plots.layout[i][j].type, " ");
                            inventar->plots.layout[i][j].status = empty;
                            (inventar->corn) += 2;
                            (inventar->seed.corn_seed)++;
                        }
                    }
                }
                if(i == 2)
                {
                    for(int j = 0; j < inventar->plots.cowp; j++)
                    {
                        if(inventar->plots.layout[choice][j].status == ready)
                        {
                            strcpy(inventar->plots.layout[choice][j].type, "moo");
                            inventar->plots.layout[choice][j].status = waiting;
                            (inventar->milk)++;
                        }
                    }
                }
            }
        }
        default:
        {
            printf("Invalid input!\n");
            break;
        }
    }
        
    printf("\n");
}

void sell(Inventory *inventar) //choice 4
{
    int choice, amount;
    
    printf("You have:\n");
    printf("1. Wheat: %d (2 coins per unit)\n", inventar->wheat);
    printf("2. Corn: %d (2 coins per unit)\n", inventar->corn);
    printf("3. Milk: %d (3 coins per unit)\n", inventar->milk);
    printf("What do you want to sell?\n");
    
    printf("Input:");
    scanf("%d", &choice);
    
    choice--;
    
    if(choice == 0 && inventar->wheat > 0) 
    {
        printf("How much wheat to sell? ");
        scanf("%d", &amount);
        if (amount <= inventar->wheat) 
        {
            inventar->money += amount * 2;
            inventar->wheat -= amount;
            printf("Sold %d wheat for %d coins!\n", amount, amount * 2);
        } 
        else 
        {
            printf("Not enough wheat!\n");
        }
    } 
    else if(choice == 1 && inventar->corn > 0) 
    {
        printf("How much corn to sell? ");
        scanf("%d", &amount);
        if (amount <= inventar->corn) 
        {
            inventar->money += amount * 2;
            inventar->corn -= amount;
            printf("Sold %d corn for %d coins!\n", amount, amount * 2);
        } 
        else 
        {
            printf("Not enough corn!\n");
        }
    } 
    else if(choice == 2 && inventar->milk > 0) 
    {
        printf("How much milk to sell? ");
        scanf("%d", &amount);
        if (amount <= inventar->milk) 
        {
            inventar->money += amount * 3;
            inventar->milk -= amount;
            printf("Sold %d milk for %d coins!\n", amount, amount * 3);
        }
        else 
        {
            printf("Not enough milk!\n");
        }
    } 
    else 
    {
        printf("Invalid choice or not enough resources!\n");
    }
    
    printf("\n");
}

void plant(Inventory *inventar) //choice 5
{
    int choice, which, how_many, enough;
    
    printf("Which type of crop do you want to plant? You have %d plots available\n", inventar->plots.wheatp + inventar->plots.cornp + inventar->plots.emptyp);
    
    printf("   1. Wheat\n");
    printf("   2. Corn\n");
    
    printf("Input:");
    scanf("%d", &choice);
    
    choice--;
    
    switch(choice)
    {
        case 0:
        {
            printf("Choose how many wheat seeds you want to plant? (%d available).\n", inventar->seed.wheat_seed);
            printf("Input: ");
            scanf("%d", &how_many);
            
            if(how_many > inventar->seed.wheat_seed || how_many > inventar->plots.wheatp + inventar->plots.emptyp)
            {
                printf("Not enough resources!\n");
                break;
            }
            
            enough = 0;
            
            for(int j = 0; j < how_many; j++)
            {
                if(inventar->plots.wheatp > enough)
                {
                    if(inventar->plots.layout[choice][j].status == empty)
                    {
                        inventar->plots.layout[choice][j].status = waiting;
                        strcpy(inventar->plots.layout[choice][j].type, "w");
                        (inventar->seed.wheat_seed)--;
                        enough++;
                    }
                }
                else
                    break;
            }
            
            while(enough < how_many)
            {
                inventar->plots.layout[choice][inventar->plots.wheatp].status = waiting;
                strcpy(inventar->plots.layout[choice][inventar->plots.wheatp].type, "w");
                (inventar->plots.wheatp)++;
                (inventar->seed.wheat_seed)--;
                (inventar->plots.emptyp)--;
                enough++;
            }
          
            break;
        }
        case 1:
        {
            printf("Choose how many corn seeds you want to plant? (%d available).\n", inventar->seed.corn_seed);
            printf("Input: ");
            scanf("%d", &how_many);
            
            if(how_many > inventar->seed.corn_seed || how_many > inventar->plots.cornp + inventar->plots.emptyp)
            {
                printf("Not enough resources!\n");
                break;
            }
            
            enough = 0;
            
            for(int j = 0; j < how_many; j++)
            {
                if(inventar->plots.cornp > enough)
                {
                    if(inventar->plots.layout[choice][j].status == empty)
                    {
                        inventar->plots.layout[choice][j].status = waiting;
                        strcpy(inventar->plots.layout[choice][j].type, "c");
                        (inventar->seed.corn_seed)--;
                        enough++;
                    }
                }
                else
                    break;
            }
            
            while(enough < how_many)
            {
                inventar->plots.layout[choice][inventar->plots.cornp].status = waiting;
                strcpy(inventar->plots.layout[choice][inventar->plots.cornp].type, "c");
                (inventar->plots.cornp)++;
                (inventar->seed.corn_seed)--;
                (inventar->plots.emptyp)--;
                enough++;
            }
          
            break;
        }
        default:
        {
            printf("Invalid input!\n");
            break;
        }
    }
        
    printf("\n");
}

void buyPlot(Inventory *inventar) //choice 6 subchoice 1 
{
    int choice;
    printf("You have %d coins. What do you want to buy?\n", inventar->money);
    printf("1. Plant plot (3 coins)\n");
    printf("2. Animal Plot (5 coins)\n");
    
    printf("Input:");
    scanf("%d", &choice);
    
    choice--;
    
    if (choice == 0 && inventar->money >= 3) 
    {
        inventar->money -= 3;
        strcpy(inventar->plots.layout[EMPTY][inventar->plots.emptyp].type, " "); 
        inventar->plots.layout[EMPTY][inventar->plots.emptyp].status = empty;
        inventar->plots.emptyp++;
        
        printf("You bought a new plot!\n");
    } 
    else if (choice == 1 && inventar->money >= 5) 
    {
        inventar->plots.cowp++;
        inventar->money -= 5;
        strcpy(inventar->plots.layout[COW][inventar->plots.cowp].type, "moo");
        inventar->plots.layout[COW][inventar->plots.cowp].status = waiting;
        
        printf("You bought a new cow plot!\n");
    } 
    else
    {
        printf("Not enough money or invalid choice!\n");
    }
    
    printf("\n");
}

void buySeeds(Inventory *inventar) //choice 6 subchoice 2 
{
    int choice, how_many;
    
    printf("What seeds do you want to buy?\n");
    printf("   1. Wheat seeds\n");
    printf("   2. Corn seeds\n");
    
    printf("Input:");
    scanf("%d", &choice);
    
    choice--;
    
    switch(choice)
    {
        case 0:
        {
            printf("How many wheat seeds do you want to buy?\n");
            printf("You have %d coins available, 1 wheat seed costs 1 coin\n", inventar->money);
            
            printf("Input:");
            scanf("%d", &how_many);
            
            if(inventar->money < how_many)
            {
                printf("Not enough resources!\n");
                break;
            }
            else
            {
                inventar->money -= how_many;
                inventar->seed.wheat_seed += how_many;
                printf("You now have %d wheat seeds.\n", inventar->seed.wheat_seed);
            }
            
            break;
        }
        case 1:
        {
            printf("How many corn seeds do you want to buy?\n");
            printf("You have %d coins available, 1 corn seed costs 1 coin\n", inventar->money);
            
            printf("Input:");
            scanf("%d", &how_many);
            
            if(inventar->money < how_many)
            {
                printf("Not enough resources!\n");
                break;
            }
            else
            {
                inventar->money -= how_many;
                inventar->seed.corn_seed += how_many;
                printf("You now have %d corn seeds.\n", inventar->seed.corn_seed);
            }
            
            break;
        }
        default:
        {
            printf("Invalid input!\n");
            break;
        }
    }
    
    printf("\n");
}

void buy(Inventory *inventar) //choice 6
{
    int choice;
    
    printf("What would you like to buy?\n");
    printf("   1. Buy plot\n");
    printf("   2. Buy seeds\n");
    
    printf("Input:");
    scanf("%d", &choice);
    
    if(choice == 1)
        buyPlot(&(*inventar));
    else if(choice == 2)
        buySeeds(&(*inventar));
    else
    {
        printf("Invalid input!\n");
        return;
    }
}

void viewInventory(const Inventory inventar) //choice 7
{
    printf("\n===== Farm Status =====\n");
    printf("Take a look at your resources:\n");
    
    printf("     Amount of wheat: %d\n", inventar.wheat);
    printf("     Amount of corn: %d\n", inventar.corn);
    printf("     Amount of milk: %d\n", inventar.milk);
    printf("     Amount of money: %d\n", inventar.money);
    printf("\n");
    
    printf("     Amount of wheat seeds: %d\n", inventar.seed.wheat_seed);
    printf("     Amount of corn seeds: %d\n", inventar.seed.corn_seed);
    printf("\n");
    
    printf("     Amount of wheat plots: %d\n", inventar.plots.wheatp);
    printf("     Amount of corn plots: %d\n", inventar.plots.cornp);
    printf("     Amount of cow plots: %d\n", inventar.plots.cowp);
    printf("     Amount of empty plots: %d\n", inventar.plots.emptyp);
    printf("========================\n");
    printf("\n");
}

void actions(Inventory *inventar, int alegere) 
{
    int which;
    
    switch(alegere)
    {
        case 1: //water
        {
            water(&(*inventar)); 
            break;
        }
        case 2: //feed
        {
            feed(&(*inventar));
            break;
        }
        case 3: //harvest/collect
        {
            collect_resources(&(*inventar));
            break;
        }
        case 4: //sell
        {
            sell(&(*inventar));
            break;
        }
        case 5: //plant
        {
            plant(&(*inventar));
            break;
        }
        case 6: //buy
        {
            buy(&(*inventar));
            break;
        }
        case 7: //view inventory: produse & seeds & number of plots
        {
            viewInventory(*inventar);
            break;
        }
        case 8: //quit game
        {
            printf("Thanks for playing!");
            exit(0);
        }
        default: //invalid
        {
            printf("Invalid input!\n");
            break;
        }
    }
}

void save()
{
    return;
}
 
Inventory loadSaveFile(FILE *save_file)
{
    return;
}

int main()
{
    int input = -1, start = 0;//start tine cont de daca e inceput sau doar continuare
    Inventory inventar;
    inventar = initial();
    
    printf("Welcome to Farm Boss!\n Let's see what we're working with:\n");
    
    do
    {    
        if(start == 0)
        {
            status(inventar);

            printf("Choose an action!\n");
            printf("1. Water the crops\n");
            printf("2. Feed the animals\n");
            printf("3. Collect resources\n");
            printf("4. Sell produce\n");
            printf("5. Plant seeds\n");
            printf("6. Buy something\n"); 
            printf("7. View inventory\n"); 
            printf("8. Quit\n");
        } 
        else
        {
            printf("\n");
            
            actions(&inventar, input);

            status(inventar);
            
            printf("What else is there to do at the farm?\n Let's get to work:\n");

            printf("Choose an action!\n");
            printf("1. Water the crops\n");
            printf("2. Feed the animals\n");
            printf("3. Collect resources\n");
            printf("4. Sell produce\n");
            printf("5. Plant seeds\n");
            printf("6. Buy something\n"); 
            printf("7. View inventory\n"); 
            printf("8. Quit\n");
        }

        printf("Input:");
        start = 1;
    }while(scanf("%d", &input) != -1);

    return 0;
}








