#include <stdio.h>

typedef struct
{
   int corn_seed;
   int wheat_seed;
}Seeds;

typedef struct
{
    int wheatp; //cod 0
    int cornp; //cod 1
    int cowp; //cod 2
    int empty; //default
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
    inventar.money = 1;
    inventar.seed.corn_seed = 1;
    inventar.seed.wheat_seed = 1;
    
    inventar.plots.wheatp = 4;
    inventar.plots.cornp = 2;
    inventar.plots.cowp = 3;
    inventar.plots.empty = 3;
    
    return inventar;
}

void status(const Inventory inventar) 
{
    int total_plots = inventar.plots.wheatp + inventar.plots.cornp + inventar.plots.cowp;
    
    printf("\n===== Farm Status =====\n");
    
    for(int i = 0; i < 4; i++)
    {
        switch(i)
        {
            case 0:
            {
                if(inventar.plots.wheatp != 0)
                {
                    for(int j = 0; j < inventar.plots.wheatp; j++)
                        printf("[W] ");
                }
                
                printf("\n");
                
                break;
            }
            case 1:
            {
                if(inventar.plots.cornp != 0)
                {
                    for(int j = 0; j < inventar.plots.cornp; j++)
                        printf("[C] ");
                }
                
                printf("\n");
                
                break;
            }
            case 2:
            {
                if(inventar.plots.cowp != 0)
                {
                    for(int j = 0; j < inventar.plots.cowp; j++)
                        printf("[MOO] ");
                }
                
                printf("\n");
                
                break;
            }
            default:
            {
                 for(int j = 0; j < inventar.plots.empty; j++)
                        printf("[ ] ");
                        
                printf("\n");        
                
                break;
            }
        }
    }
     
    printf("========================\n");
}

void water(Inventory *inventar) 
{
    int choice;
    
    printf("Which crop do you want to water?\n");
    
    printf("1. Wheat Plots (%d available)\n", inventar->plots.wheatp);
    printf("2. Corn Plots (%d available)\n", inventar->plots.cornp);
    scanf("%d", &choice);
    
    if (choice == 0 && inventar->plots.wheatp > 0) 
        printf("You watered all wheat crops!\n");
    else if (choice == 1 && inventar->plots.cornp > 0) 
        printf("You watered all corn crops!\n");
    else 
        printf("Invalid choice or no crops available!\n");
}

void sell(Inventory *inventar) 
{
    int choice, amount;
    
    printf("You have:\n");
    printf("1. Wheat: %d (2 coins per unit)\n", inventar->wheat);
    printf("2. Corn: %d (2 coins per unit)\n", inventar->corn);
    printf("3. Milk: %d (3 coins per unit)\n", inventar->milk);
    printf("What do you want to sell? ");
    scanf("%d", &choice);
    
    if (choice == 0 && inventar->wheat > 0) 
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
    else if (choice == 1 && inventar->corn > 0) 
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
    else if (choice == 2 && inventar->milk > 0) 
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
}

void buyPlot(Inventory *inventar) 
{
    int choice;
    printf("You have %d coins. What do you want to buy?\n", inventar->money);
    printf("1. Wheat Plot (3 coins)\n");
    printf("2. Corn Plot (3 coins)\n");
    printf("3. Animal Plot (5 coins)\n");
    scanf("%d", &choice);
    
    if (choice == 0 && inventar->money >= 5) 
    {
        inventar->plots.wheatp++;
        inventar->money -= 5;
        printf("You bought a new Wheat Plot!\n");
    } 
    else if (choice == 1 && inventar->money >= 5) 
    {
        inventar->plots.cornp++;
        inventar->money -= 5;
        printf("You bought a new Corn Plot!\n");
    } 
    else if (choice == 2 && inventar->money >= 8) 
    {
        inventar->plots.cowp++;
        inventar->money -= 8;
        printf("You bought a new Animal Plot!\n");
    } 
    else
    {
        printf("Not enough money or invalid choice!\n");
    }
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
            break;
        }
        case 3: //sell
        {
            sell(&(*inventar));
            break;
        }
        case 4: //buy, ar trb sa poti sa cumperi si mancare/seeds
        {
            buyPlot(&(*inventar));
            break;
        }
        default:
        {
            printf("Invalid input!");
            break;
        }
    }
}

int main()
{
    int input, start = 0;//start tine cont de daca e inceput sau doar continuare
    Inventory inventar;
    inventar = initial();
    
    printf("Welcome to Farm Boss!\n Let's see what we're working with:\n");
    status(inventar);
    
    printf("Choose an action!\n");
    printf("1. Water the crops\n");
    printf("2. Feed the animals\n");
    printf("3. Sell produce\n");
    printf("4. Buy something\n");
    printf("Input:");
    
    while(scanf("%d", &input) != -1) //tot scrisu tre sa apara inainte de scanf;poate do while?
    {
        start = 1;
        
        if(start != 0)
        {
            printf("What else is there to do at the farm?\n Let's geet to work:\n");
    
            printf("Choose an action!\n");
            printf("1. Water the crops\n");
            printf("2. Feed the animals\n");
            printf("3. Sell produce\n");
            printf("4. Buy something\n");
            printf("Input:");
        }
        actions(&inventar, input);
        status(inventar);
    }

    return 0;
}




