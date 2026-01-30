#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX_STORES 3
#define MAX_RIDERS 3
#define MAX_ITEMS 3
#define MAX_DISTANCE 7.0
#define MIN_ORDER 100
#define SPEED 2.0     // minutes per km

// ---------- STRUCTURES ----------

struct Item {
    char name[20];
    int stock;
    float price;
};

struct Store {
    int id;
    float x, y;
    struct Item items[MAX_ITEMS];
};

struct Rider {
    int id;
    float x, y;
    int available;
};

// ---------- GLOBAL STATS ----------
float totalRevenue = 0;
float totalProfit = 0;
int totalOrders = 0;


// ---------- FUNCTIONS ----------

float distance(float x1, float y1, float x2, float y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

float deliveryCost(float d) {
    return 25 + (10 * d);
}

void showItems(struct Store s) {
    printf("\nAvailable items at Store %d\n", s.id);
    for(int i=0;i<MAX_ITEMS;i++){
        printf("%d. %-6s | Price: Rs %.2f | Stock: %d\n",
               i+1, s.items[i].name, s.items[i].price, s.items[i].stock);
    }
}

// ---------- MAIN PROGRAM ----------

int main() {

    struct Store stores[MAX_STORES] = {
        {1, 2, 3, {{"Milk",10,30},{"Bread",8,25},{"Eggs",6,60}}},
        {2, 7, 2, {{"Milk",5,28},{"Bread",4,24},{"Eggs",10,58}}},
        {3, 5, 8, {{"Milk",7,31},{"Bread",6,26},{"Eggs",5,62}}}
    };

    struct Rider riders[MAX_RIDERS] = {
        {1,3,3,1},
        {2,6,5,1},
        {3,8,7,1}
    };

    int choice;

    while(1){

        printf("\n===== PepperTap 2.0 =====\n");
        printf("1. Place Order\n");
        printf("2. View Summary\n");
        printf("3. Exit\n");
        printf("Choose: ");
        scanf("%d",&choice);

        if(choice==3) break;

        // ---------- SUMMARY ----------
        if(choice==2){
            printf("\n---- DAILY SUMMARY ----\n");
            printf("Total Orders : %d\n", totalOrders);
            printf("Revenue      : Rs %.2f\n", totalRevenue);
            printf("Profit       : Rs %.2f\n", totalProfit);
            printf("-----------------------\n");
            continue;
        }

        // ---------- PLACE ORDER ----------

        float cx, cy;
        printf("Enter your location (x y): ");
        scanf("%f %f",&cx,&cy);

        // Find nearest store
        float minDist=999;
        int storeIndex=-1;

        for(int i=0;i<MAX_STORES;i++){
            float d = distance(cx,cy,stores[i].x,stores[i].y);
            if(d < minDist){
                minDist=d;
                storeIndex=i;
            }
        }

        if(minDist > MAX_DISTANCE){
            printf("No nearby store. Order rejected.\n");
            continue;
        }

        showItems(stores[storeIndex]);

        int itemChoice, qty;
        printf("Select item number: ");
        scanf("%d",&itemChoice);

        printf("Enter quantity: ");
        scanf("%d",&qty);

        itemChoice--;

        if(itemChoice < 0 || itemChoice >= MAX_ITEMS){
            printf("Invalid choice.\n");
            continue;
        }

        if(stores[storeIndex].items[itemChoice].stock < qty){
            printf("Insufficient stock.\n");
            continue;
        }

        // Find nearest rider
        minDist=999;
        int riderIndex=-1;

        for(int i=0;i<MAX_RIDERS;i++){
            if(riders[i].available){
                float d = distance(stores[storeIndex].x,stores[storeIndex].y,
                                   riders[i].x,riders[i].y);
                if(d < minDist){
                    minDist=d;
                    riderIndex=i;
                }
            }
        }

        if(riderIndex==-1){
            printf("No rider available.\n");
            continue;
        }

        float orderValue = qty * stores[storeIndex].items[itemChoice].price;
        float cost = deliveryCost(minDist);
        float profit = orderValue - cost;
        int eta = (int)(minDist * SPEED);

        // Business rules
        if(orderValue < MIN_ORDER || profit <= 0){
            printf("Order rejected: Not profitable.\n");
            continue;
        }

        // Update system
        stores[storeIndex].items[itemChoice].stock -= qty;
        riders[riderIndex].available = 0;

        totalRevenue += orderValue;
        totalProfit += profit;
        totalOrders++;

        // ---------- CONFIRMATION ----------
        printf("\n----- ORDER CONFIRMED -----\n");
        printf("Store     : %d\n", stores[storeIndex].id);
        printf("Item      : %s\n", stores[storeIndex].items[itemChoice].name);
        printf("Qty       : %d\n", qty);
        printf("Rider     : %d\n", riders[riderIndex].id);
        printf("ETA       : %d minutes\n", eta);
        printf("Order Rs  : %.2f\n", orderValue);
        printf("Cost Rs   : %.2f\n", cost);
        printf("Profit Rs : %.2f\n", profit);
        printf("---------------------------\n");

        // Rider becomes free again (simulate delivery complete)
        riders[riderIndex].available = 1;
    }

    printf("\nThank you for using PepperTap 2.0\n");
    return 0;
}