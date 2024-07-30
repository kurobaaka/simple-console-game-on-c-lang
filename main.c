#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MONSTERS 10
#define MAX_ITEMS 10
#define MAX_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 256

typedef struct {
    char name[MAX_NAME_LENGTH];
    int health;
    int attack;
    int experience;
} Monster;

typedef struct {
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    int value; // Для зелья: 20 - восстановление здоровья, 5 - увеличение атаки
} Item;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int health;
    int attack;
    int level;
    int experience;
    Item inventory[MAX_ITEMS];
    int itemCount;
} Player;

void initializeMonsters(Monster monsters[]) {
    strcpy(monsters[0].name, "Goblin");
    monsters[0].health = 30;
    monsters[0].attack = 5;
    monsters[0].experience = 10;

    strcpy(monsters[1].name, "Orc");
    monsters[1].health = 50;
    monsters[1].attack = 10;
    monsters[1].experience = 20;

    strcpy(monsters[2].name, "Troll");
    monsters[2].health = 80;
    monsters[2].attack = 15;
    monsters[2].experience = 30;

    strcpy(monsters[3].name, "Dragon");
    monsters[3].health = 120;
    monsters[3].attack = 20;
    monsters[3].experience = 50;

    // Добавьте больше монстров по желанию
}

void initializePlayer(Player *player) {
    strcpy(player->name, "Hero");
    player->health = 100;
    player->attack = 10;
    player->level = 1;
    player->experience = 0;
    player->itemCount = 0;
}

void printStatus(Player *player) {
    printf("Player: %s\n", player->name);
    printf("Level: %d\n", player->level);
    printf("Health: %d\n", player->health);
    printf("Attack: %d\n", player->attack);
    printf("Experience: %d\n", player->experience);
    printf("Items: %d\n", player->itemCount);
    for (int i = 0; i < player->itemCount; i++) {
        printf(" - %s: %s\n", player->inventory[i].name, player->inventory[i].description);
    }
}

void levelUp(Player *player) {
    player->level++;
    player->health += 20; // Увеличиваем здоровье
    player->attack += 5;  // Увеличиваем атаку
    printf("Congratulations! You leveled up to level %d!\n", player->level);
}

void fight(Player *player, Monster *monster) {
    printf("You encountered a %s!\n", monster->name);
    while (player->health > 0 && monster->health > 0) {
        printf("You attack the %s for %d damage.\n", monster->name, player->attack);
        monster->health -= player->attack;
        if (monster->health > 0) {
            printf("The %s attacks you for %d damage.\n", monster->name, monster->attack);
            player->health -= monster->attack;
        }
    }
    if (player->health <= 0) {
        printf("You have been defeated by the %s!\n", monster->name);
    } else {
        printf("You have defeated the %s!\n", monster->name);
        player->experience += monster->experience;
        printf("You gained %d experience!\n", monster->experience);
        if (player->experience >= player->level * 50) { // 50 опыта для повышения уровня
            levelUp(player);
        }
    }
}

void findItem(Player *player) {
    if (player->itemCount < MAX_ITEMS) {
        Item newItem;
        int itemType = rand() % 2; // 0 - зелье здоровья, 1 - зелье атаки
        if (itemType == 0) {
            sprintf(newItem.name, "Health Potion");
            strcpy(newItem.description, "Restores 20 health.");
            newItem.value = 20;
        } else {
            sprintf(newItem.name, "Attack Potion");
            strcpy(newItem.description, "Increases attack by 5.");
            newItem.value = 5;
        }
        player->inventory[player->itemCount++] = newItem;
        printf("You found a %s!\n", newItem.name);
    } else {
        printf("Your inventory is full!\n");
    }
}

void useItem(Player *player, int itemIndex) {
    if (itemIndex < 0 || itemIndex >= player->itemCount) {
        printf("Invalid item index!\n");
        return;
    }
    Item *item = &player->inventory[itemIndex];
    if (strcmp(item->name, "Health Potion") == 0) {
        player->health += item->value;
        printf("You used a %s and restored %d health!\n", item->name, item->value);
    } else if (strcmp(item->name, "Attack Potion") == 0) {
        player->attack += item->value;
        printf("You used a %s and increased your attack by %d!\n", item->name, item->value);
    }
    // Удаляем использованный предмет
    for (int i = itemIndex; i < player->itemCount - 1; i++) {
        player->inventory[i] = player->inventory[i + 1];
    }
    player->itemCount--;
}

void explore(Player *player, Monster monsters[], int monsterCount) {
    int action;
    printf("You are exploring the dungeon...\n");
    while (player->health > 0) {
        printf("Choose an action:\n1. Fight a monster\n2. Find an item\n3. Check status\n4. Use an item\n5. Exit\n");
        scanf("%d", &action);
        switch (action) {
            case 1: {
                int monsterIndex = rand() % monsterCount;
                fight(player, &monsters[monsterIndex]);
                break;
            }
            case 2:
                findItem(player);
                break;
            case 3:
                printStatus(player);
                break;
            case 4: {
                int itemIndex;
                printf("Enter item index to use (0 to %d): ", player->itemCount - 1);
                scanf("%d", &itemIndex);
                useItem(player, itemIndex);
                break;
            }
            case 5:
                printf("Exiting the dungeon...\n");
                return;
            default:
                printf("Invalid action. Try again.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    Player player;
    Monster monsters[MAX_MONSTERS];

    initializePlayer(&player);
    initializeMonsters(monsters);
    explore(&player, monsters, 4);

    return 0;
}
