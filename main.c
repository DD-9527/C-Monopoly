// Monopoly
// By: DD-9527
// Date: 2024-12-01
// Version: 1.0
// Description: This is a simple text-based game of Monopoly.
//              The game is played by two players who take turns rolling a dice and moving around the board.
//              The players can buy properties, pay rent, and collect money from the bank.
//              The game ends when one player has no money left.
//              This is a simple game and does not include all the rules of Monopoly.
// GitHub: https://github.com/DD-9527/Monopoly
// License: GNU General Public License v3.0
// Copyright (C) 2024 DD-9527

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>

#define KEY_PRESS(VK) ((GetAsyncKeyState(VK)&0x1) ? 1 : 0)
#define MAX_PLAYERS 5
#define BOARD_SIZE 30

/* -------------------------------------- */
/* ---------- Block of Windows ---------- */
/* -------------------------------------- */
typedef enum {
    Color_Black,         // 黑色
    Color_Blue,          // 蓝色
    Color_Green,         // 绿色
    Color_Cyan,          // 青色
    Color_Red,           // 红色
    Color_Magenta,       // 品红色
    Color_Brown,         // 棕色
    Color_LightGray,     // 亮灰色
    Color_DarkGray,      // 深灰色
    Color_LightBlue,     // 亮蓝色
    Color_LightGreen,    // 亮绿色
    Color_LightCyan,     // 亮青色
    Color_LightRed,      // 亮红色
    Color_LightMagenta,  // 亮品红色
    Color_Yellow,        // 黄色
    Color_White          // 白色
} Color; // Color of the text
/**
 * @brief Set the Color object
 * 
 * @param uFore Foreground color
 * @param uBack Background color
 */
void SetColor(Color uFore,Color uBack) {
	HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle,uFore+uBack*0x10);
}
/**
 * @brief Set the Pos object
 * 
 * @param x Set the x position
 * @param y Set the y position
 */
void SetPos(short x, short y){
    //获得标准输出设备的句柄
	HANDLE houtput = GetStdHandle(STD_OUTPUT_HANDLE);
	//定位光标的位置
	COORD pos = { x, y };
	SetConsoleCursorPosition(houtput, pos);
}

/* --------------------------------------- */
/* ---------- Block of GameData ---------- */
/* --------------------------------------- */
typedef struct {    // Player structure
    int position;   // Position of the player on the board
    int money;      // Money of the player
    int preCount;   // Number of properties the player has
    Color color;    // Color of the player
    bool IsDead;    // Is the player dead?
    bool IsInJail;  // Is the player in jail?
} Player; // Player structure
int playerCount = 2; // Number of players(2 to 5)
int playerIndex = 0; // Index of the current player(0 to playerCount-1)
Player players[MAX_PLAYERS] = {
	{0,5000,0,Color_Blue,false,false},
    {0,5000,0,Color_Brown,false,false},
    {0,5000,0,Color_Green,false,false},
    {0,5000,0,Color_Red,false,false},
    {0,5000,0,Color_DarkGray,false,false}
}; // Array of players

typedef enum {
    START_TILE,       	// Start Tile
    EMPTY_TILE,        	// Empty Tile
    CHANCE_TILE,       	// Chance Tile
    DESTINY_TILE,      	// Destiny Tile
    TAX_TILE,          	// Tax Tile
    ROCKET_TILE,       	// Rocket Tile
    JAIL_TILE          	// Jail Tile
} TileType; // Type of the tile
typedef struct {
    char name[9];       // Name of the tile
    int value;          // Value of the tile
    TileType type;      // Type of the tile
    int owner;          // Owner of the tile
} Tile; // Tile structure
Tile board[BOARD_SIZE] = {
    {" 起点 ",1000,START_TILE,-1},{" 中国 ",4000,EMPTY_TILE,-1},{" 韩国 ",1000,EMPTY_TILE,-1},{" 机会 ",  0,CHANCE_TILE,-1},{" 澳洲 ",1600,EMPTY_TILE,-1},
    {" 机场 ",2000,EMPTY_TILE,-1},{" 命运 ", 0,DESTINY_TILE,-1},{" 交税 ", 700,  TAX_TILE,-1},{" 埃及 ",2000,EMPTY_TILE,-1},{" 坐牢 ",    0,JAIL_TILE,-1},
    {" 南非 ",2200,EMPTY_TILE,-1},{" 公园 ",1200,EMPTY_TILE,-1},{"阿根廷",2400,EMPTY_TILE,-1},{" 命运 ", 0,DESTINY_TILE,-1},{" 法国 ",2600,EMPTY_TILE,-1},
    {" 火箭 ",  0,ROCKET_TILE,-1},{"西班牙",2200,EMPTY_TILE,-1},{" 英国 ",2400,EMPTY_TILE,-1},{" 命运 ", 0,DESTINY_TILE,-1},{"意大利",3000,EMPTY_TILE,-1},
    {" 交税 ",   700,TAX_TILE,-1},{" 酒店 ",2000,EMPTY_TILE,-1},{" 机会 ",  0,CHANCE_TILE,-1},{" 瑞士 ",3600,EMPTY_TILE,-1},{" 坐牢 ",    0,JAIL_TILE,-1},
    {"俄罗斯",3200,EMPTY_TILE,-1},{" 水库 ",1200,EMPTY_TILE,-1},{" 美国 ",2200,EMPTY_TILE,-1},{" 命运 ", 0,DESTINY_TILE,-1},{" 日本 ",1400,EMPTY_TILE,-1}
}; // Board of the game

/* ---------------------------------------- */
/* ---------- Block of Draw Menu ---------- */
/* ---------------------------------------- */
const char menu[5][50] = {
    "Start Game With 2 Players!",
    "Start Game With 3 Players!",
    "Start Game With 4 Players!",
    "Start Game With 5 Players!",
    "Setting"
};
/**
 * @brief Draws a string with a filler in the middle
 * 
 * @param x The x position of the string
 * @param y The y position of the string
 * @param startStr The start of the string
 * @param width The width of the string
 * @param filler The filler of the string
 * @param endStr The end of the string
 */
void DrawStringWithFiller(int x, int y, const char *startStr, int width, const char *filler, const char *endStr) {
    SetPos(x, y); 
    printf("%s", startStr); 
    for (int i = 0; i < width; i++) {
        printf("%s", filler); 
    }
    printf("%s", endStr);
}
/**
 * @brief Draws a menu with the given options
 * and returns the option the user selected.
 * 
 * @param x The Menu's x position
 * @param y The Menu's y position
 * @param w The Menu's width(Must be morethan the longest option)
 * @param n The number of options in the menu
 * @param menu The options in the menu
 * (Must be a 2D array of strings and less than 20 characters long)
 * @return int The option the user selected
 */
int DrawMenu(int x, int y, int w, int n,const char menu[][50]){
    // Make sure the menu is centered
    for (int i = 0; i < n; i++) w = strlen(menu[i]) > w ? strlen(menu[i]) : w;
    // Draw menubox
    DrawStringWithFiller(x, y, "╔", w + 10, "═", "╗");
	for (int i = 0; i < n; i++){                    // Middle
        DrawStringWithFiller(x, y + 2 * i + 1, "║", w + 10, " ", "║");
        if(i != n-1){
            DrawStringWithFiller(x, y + 2 * i + 2, "║     ", w, "—", "     ║");
		}
    }
    DrawStringWithFiller(x, y + 2 * n, "╚", w + 10, "═", "╝");
	//Select Menu 
    int choice = 0; // Default choice is the first option
	while(!KEY_PRESS(VK_RETURN)){
        for (int i = 0; i < n; i++) {
            SetPos(x + 1, y + i * 2 + 1);
            // Print the left arrow
            if (i == choice) {
                SetColor(Color_Yellow, Color_Cyan);
                printf(" ══> ");
            } else {
                SetColor(Color_Brown, Color_Black);
                printf("     ");
            }
            // Print the menu option
            int spaceLen = (w - strlen(menu[i])) / 2;
            for (int j = 0; j < spaceLen; j++) printf(" ");
            printf("%s", menu[i]);
            for (int j = 0; j < spaceLen + strlen(menu[i]) % 2; j++) printf(" ");
            // Print the right arrow
            if (i == choice) {
                printf(" <══ ");
            } else {
                printf("     ");
            }
        }
		//Move Arrow
        if (KEY_PRESS(VK_UP)) {
            choice = (choice + n - 1) % n; // 向上移动
        }
        if (KEY_PRESS(VK_DOWN)) {
            choice = (choice + 1) % n; // 向下移动
        }
		Sleep(10);	
	}
	return choice;
}

/* ------------------------------------------ */
/* ---------- Block of WelcomePage ---------- */
/* ------------------------------------------ */
/**
 * @brief Prints the welcome page
 * 
 */
void Welcome(void){
	//Print LOGO
	SetPos(0, 3);
	printf("\
	              $$\\      $$\\                                                   $$\\                         	 \n\
	              $$$\\    $$$ |                                                  $$ |                        		 \n\
	              $$$$\\  $$$$ | $$$$$$\\  $$$$$$$\\   $$$$$$\\   $$$$$$\\   $$$$$$\\  $$ |$$\\   $$\\               \n\
	$$$$$$\\       $$\\$$\\$$ $$ |$$  __$$\\ $$  __$$\\ $$  __$$\\ $$  __$$\\ $$  __$$\\ $$ |$$ |  $$ |      $$$$$$\\ \n\
	\\______|      $$ \\$$$  $$ |$$ /  $$ |$$ |  $$ |$$ /  $$ |$$ /  $$ |$$ /  $$ |$$ |$$ |  $$ |      \\______|	\n\
	              $$ |\\$  /$$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |  $$ |$$ |$$ |  $$ |              		\n\
	              $$ | \\_/ $$ |\\$$$$$$  |$$ |  $$ |\\$$$$$$  |$$$$$$$  |\\$$$$$$  |$$ |\\$$$$$$$ |              	\n\
	              \\__|     \\__| \\______/ \\__|  \\__| \\______/ $$  ____/  \\______/ \\__| \\____$$ |            \n\
	                                                         $$ |                    $$\\   $$ |              		\n\
	                                                         $$ |                    \\$$$$$$  |              		\n\
	                                                         \\__|                     \\______/               		\n\
$$$$$$\\ $$$$$$\\                                                                                        $$$$$$\\ $$$$$$\\  \n\
\\______|\\______|                                                                                       \\______|\\______|  \n\
$$$$$$\\ $$$$$$\\ $$$$$$\\                                                                         $$$$$$\\ $$$$$$\\ $$$$$$\\ \n\
\\______|\\______|\\______|                                                                	\\______|\\______|\\______|\n\
$$$$$$\\ $$$$$$\\                                                                                 	$$$$$$\\ $$$$$$\\ \n\
\\______|\\______|                                                                                 	\\______|\\______| \n\
$$$$$$\\                                                                                                 	$$$$$$\\ \n\
\\______|                                                                                                 	\\______| \n\
$$$$$$\\ $$$$$$\\                                                                                   	$$$$$$\\ $$$$$$\\ \n\
\\______|\\______|                                                                                   	\\______|\\______| \n\
$$$$$$\\ $$$$$$\\ $$$$$$\\                                                                  	$$$$$$\\ $$$$$$\\ $$$$$$\\ \n\
\\______|\\______|\\______|                                                                  	\\______|\\______|\\______|\
	");
    
}

/* --------------------------------------- */
/* ---------- Block of GamePage ---------- */
/* --------------------------------------- */
const char GAME[][120] = {
	{" ██╗ ██╗     ██████╗  █████╗ ███╗   ███╗███████╗    ██╗ ██╗ "},
	{"████████╗   ██╔════╝ ██╔══██╗████╗ ████║██╔════╝   ████████╗"},
	{"╚██╔═██╔╝   ██║  ███╗███████║██╔████╔██║█████╗     ╚██╔═██╔╝"},
	{"████████╗   ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝     ████████╗"},
	{"╚██╔═██╔╝   ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗   ╚██╔═██╔╝"},
	{" ╚═╝ ╚═╝     ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝    ╚═╝ ╚═╝ "}
};
const char _JAIL[][120] = {
	{"    ██╗ ██╗          ██╗ █████╗ ██╗██╗          ██╗ ██╗     "},
	{"   ████████╗         ██║██╔══██╗██║██║         ████████╗    "},
	{"   ╚██╔═██╔╝         ██║███████║██║██║         ╚██╔═██╔╝    "},
	{"   ████████╗    ██   ██║██╔══██║██║██║         ████████╗    "},
	{"   ╚██╔═██╔╝    ╚█████╔╝██║  ██║██║███████╗    ╚██╔═██╔╝    "},
	{"    ╚═╝ ╚═╝      ╚════╝ ╚═╝  ╚═╝╚═╝╚══════╝     ╚═╝ ╚═╝     "}
};
const char YOU_OUT[][120] = {
	{"██╗   ██╗ ██████╗ ██╗   ██╗      ██████╗ ██╗   ██╗████████╗"},
	{"╚██╗ ██╔╝██╔═══██╗██║   ██║     ██╔═══██╗██║   ██║╚══██╔══╝ "},
	{" ╚████╔╝ ██║   ██║██║   ██║     ██║   ██║██║   ██║   ██║    "},
	{"  ╚██╔╝  ██║   ██║██║   ██║     ██║   ██║██║   ██║   ██║    "},
	{"   ██║   ╚██████╔╝╚██████╔╝     ╚██████╔╝╚██████╔╝   ██║    "},
	{"   ╚═╝    ╚═════╝  ╚═════╝       ╚═════╝  ╚═════╝    ╚═╝   "}
};
const char _CHANCE[][120] = {
	{"       ██████╗██╗  ██╗ █████╗ ███╗   ██╗ ██████╗███████╗    "},
	{"      ██╔════╝██║  ██║██╔══██╗████╗  ██║██╔════╝██╔════╝    "},
	{"█████╗██║     ███████║███████║██╔██╗ ██║██║     █████╗█████╗"},
	{"╚════╝██║     ██╔══██║██╔══██║██║╚██╗██║██║     ██╔══╝╚════╝"},
	{"      ╚██████╗██║  ██║██║  ██║██║ ╚████║╚██████╗███████╗    "},
	{"       ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝ ╚═════╝╚══════╝    "}
};
const char _DESTINY[][120] = {
	{"   ██████╗ ███████╗███████╗████████╗██╗███╗   ██╗██╗   ██╗ "},
	{"   ██╔══██╗██╔════╝██╔════╝╚══██╔══╝██║████╗  ██║╚██╗ ██╔╝  "},
	{"   ██║  ██║█████╗  ███████╗   ██║   ██║██╔██╗ ██║ ╚████╔╝   "},
	{"   ██║  ██║██╔══╝  ╚════██║   ██║   ██║██║╚██╗██║  ╚██╔╝    "},
	{"   ██████╔╝███████╗███████║   ██║   ██║██║ ╚████║   ██║     "},
	{"   ╚═════╝ ╚══════╝╚══════╝   ╚═╝   ╚═╝╚═╝  ╚═══╝   ╚═╝    "}
};
const char _EMPTY[][120] = {
	{" ██╗ ██╗  ███████╗██████╗  █████╗  ██████╗███████╗  ██╗ ██╗ "},
	{"████████╗ ██╔════╝██╔══██╗██╔══██╗██╔════╝██╔════╝ ████████╗"},
	{"╚██╔═██╔╝ ███████╗██████╔╝███████║██║     █████╗   ╚██╔═██╔╝"},
	{"████████╗ ╚════██║██╔═══╝ ██╔══██║██║     ██╔══╝   ████████╗"},
	{"╚██╔═██╔╝ ███████║██║     ██║  ██║╚██████╗███████╗ ╚██╔═██╔╝"},
	{" ╚═╝ ╚═╝  ╚══════╝╚═╝     ╚═╝  ╚═╝ ╚═════╝╚══════╝  ╚═╝ ╚═╝ "}
};
const char _AGAIN[][120] = {
	{"    ██╗     █████╗  ██████╗  █████╗ ██╗███╗   ██╗    ██╗    "},
	{"    ██║    ██╔══██╗██╔════╝ ██╔══██╗██║████╗  ██║    ██║    "},
	{"    ██║    ███████║██║  ███╗███████║██║██╔██╗ ██║    ██║    "},
	{"    ╚═╝    ██╔══██║██║   ██║██╔══██║██║██║╚██╗██║    ╚═╝    "},
	{"    ██╗    ██║  ██║╚██████╔╝██║  ██║██║██║ ╚████║    ██╗    "},
	{"    ╚═╝    ╚═╝  ╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝    ╚═╝    "}
};
const char chance[10][50] = {
	"随机获得一个国家                           \n",
	"再摇一次色子                               \n",
	"随机获得一个属于其他玩家的国家             \n",
	"直接移动到起点                             \n",
	"免除下一次过路费                           \n",
	"下一次有玩家需要向你缴纳过路费时，过路费翻倍\n",
	"向前移动一步                               \n",
	"向前移动两步                               \n",
	"向前移动三步                               \n",
	"任选一名玩家，让他失去500元                \n"
};
const char destiny[10][50] = {
	"坐牢一回合                             \n",
	"缴纳1000元所得税                       \n",
	"缴纳2000元所得税                       \n",
	"失去一个已有的国家                     \n",
	"向后移动一步                           \n",
	"下一次你缴纳过路费时，过路费翻倍       \n",
	"向后移动一步                           \n",
	"向后移动两步                           \n",
	"向后移动三步                           \n",
	"将自己的500元交给随即一名其他玩家      \n"
};
/**
 * @brief Put message on the screen
 * 
 * @param state 
 */
void MessagePut(TileType state){
	SetPos(16,16); printf("                                                            ");
	switch (state) {
    case EMPTY_TILE:
        for(int i=0;i<6;i++){
            SetPos(16,10+i);
            printf("%s",_EMPTY[i]);
        }
        if(board[players[playerIndex].position].owner == playerIndex){
            //TODO
        }else if(board[players[playerIndex].position].owner != -1){
            SetPos(16,16); printf("--> 这块地属于玩家%c, 因此你支付了 %d元过路费!", 
            "$%@#&"[board[players[playerIndex].position].owner], 
            (int)(board[players[playerIndex].position].value * 0.2));
        }else if(players[playerIndex].money >= board[players[playerIndex].position].value){
            SetPos(16,16); printf("--> 这块地尚未有人购买, 按B/N键选择是否将其购买"); 
        } else{
            SetPos(16,16); printf("--> 买不起...");
        }
        break;
    case ROCKET_TILE:
        for(int i=0;i<6;i++){
            SetPos(16,10+i);
            printf("%s",_AGAIN[i]);
        }	
        SetPos(16,16); printf("--> 您可以再掷一次骰子!");	
        break;
    case JAIL_TILE:
        SetColor(Color_LightGray, Color_Black);
        for(int i=0;i<6;i++){
            SetPos(16,10+i);
            printf("%s",_JAIL[i]);
        }
        break;
    case START_TILE:
        SetColor(Color_Cyan, Color_Black);
        for(int i=0;i<6;i++){
            SetPos(16,10+i);
            printf("%s",GAME[i]);
        }	
        SetPos(16,16); printf("--> 按住[SPACE]开始摇骰子吧!",chance[rand()%10]);	
        break;
    case CHANCE_TILE:
        SetColor(Color_Yellow, Color_Black);
        for(int i=0;i<6;i++){
            SetPos(16,10+i);
            printf("%s",_CHANCE[i]);
        }
        SetPos(16,16); printf("--> %s",chance[rand()%10]);
        break;
    /*
    case :
        SetColor(Color_Red, Color_Black);
        for(int i=0;i<6;i++){
            SetPos(16,10+i);
            printf("%s",YOU_OUT[i]);
        }
        Sleep(500); 
        break;
    */
    case DESTINY_TILE:
        SetColor(Color_LightMagenta, Color_Black);
        for(int i=0;i<6;i++){
            SetPos(16,10+i);
            printf("%s",_DESTINY[i]);
        }
        SetPos(16,16); printf("--> %s",destiny[rand()%10]);
        break;
    default:
        //TODO
        break;
	}
	SetColor(Color_White, Color_Black);
} 
/**
 * @brief Set the block object
 * 
 * @param n the block number
 * @param dx the x offset
 * @param dy the y offset
 */
void set_block(int n,int dx,int dy){
	if(n < 10){
		SetPos(83-n*9+dx, 25+dy);
	}else if(10<=n && n<16){
		SetPos(2+dx, 21-(n-10)*4+dy);
	}else if(16<=n && n<25){
		SetPos(11+(n-16)*9+dx, 1+dy);
	}else{
		SetPos(83+dx, 5+(n-25)*4+dy);
	}
}
/**
 * @brief Draw a string with alternating fillers
 * 
 * @param x the x position
 * @param y the y position
 * @param startStr the start string
 * @param n the number of fillers
 * @param filler1 the first filler
 * @param filler2 the second filler
 * @param endStr the end string
 */
void DrawStringWithAltFillers(int x, int y, const char *startStr, int n, const char *filler1, const char *filler2, const char *endStr) {
    SetPos(x, y); 
    printf("%s", startStr); 
    for (int i = 0; i < n; i++) {
        printf("%s", filler1); 
        if (i != n - 1) {
            printf("%s", filler2); 
        }
    }
    printf("%s", endStr);
}
/**
 * @brief Draw the board
 * 
 */
void draw_board(void){
	int i,j;
	for (i = 1; i < 28; i++){
		SetPos(0, i); printf("│");
		SetPos(90, i); printf("│");
		SetPos(9, i); printf("│");
		SetPos(81, i); printf("│");
	}
    DrawStringWithAltFillers(0, 0, "╭", 10, "────────", "┬", "╮"); 
    DrawStringWithAltFillers(0, 4, "├", 10, "────────", "┴", "┤");
    DrawStringWithAltFillers(0, 24, "├", 10, "────────", "┬", "┤");
    DrawStringWithAltFillers(0, 28, "╰", 10, "────────", "┴", "╯");
	SetPos(9, 4); printf("┼");
	SetPos(9, 24); printf("┼");
	SetPos(81, 4); printf("┼");
	SetPos(81, 24); printf("┼");
	for(i=8;i<=20;i=i+4){
		SetPos(0,i); printf("├────────┤");
		SetPos(81,i); printf("├────────┤");
	}
	for(i=18;i<=72;i=i+9){
		SetPos(i,1); printf("│");
		SetPos(i,2); printf("│");
		SetPos(i,3); printf("│");
		SetPos(i,25); printf("│");
		SetPos(i,26); printf("│");
		SetPos(i,27); printf("│");
	}
	SetPos(94,0); printf("╔");
	SetPos(119,0); printf("╗");
	SetPos(94,15); printf("╚");
	SetPos(119,15);	printf("╝");
	for(j=0;j<18;j+=3){
		SetPos(95,j);
		for(i=1;i<25;i++){
			printf("═");
		}
	}
	for(i=1;i<15;i++)
	{
		SetPos(94,i);
		if(i%3==0){
			printf("╠");
		}else{
			printf("║");
		}
		SetPos(119,i);
		if(i%3==0){
			printf("╣");
		}else{
			printf("║");
		}
	}
	SetPos(43,19); printf("╔═══╗");
	SetPos(43,21); printf("╚═══╝");
	SetPos(43,20); printf("║");
	SetPos(47,20); printf("║");
	SetPos(41,20); printf("+");
	SetPos(35,19); printf("╔═══╗");
	SetPos(35,21); printf("╚═══╝");
	SetPos(35,20); printf("║");
	SetPos(39,20); printf("║");
	SetPos(49,20); printf("=");
	SetPos(51,19); printf("╔═══╗");
	SetPos(51,21); printf("╚═══╝");
	SetPos(51,20); printf("║");
	SetPos(55,20); printf("║");
	SetPos(14,8); printf("╔");
	SetPos(76,8); printf("╗");
	SetPos(14,17); printf("╚");
	SetPos(76,17); printf("╝");
	i=15;
	while(i<=75)
	{
		SetPos(i,8); printf("═");
		SetPos(i,17); printf("═");
		i++;
	}
	i=9;
	while(i<=16)
	{
		SetPos(14,i); printf("║");
		SetPos(76,i); printf("║");
		i++;
	}
	SetPos(94,28); printf("按下H来查看游戏规则"); 
}
/* ---------------------------------------- */
/* ---------- Block of HelpePage ---------- */
/* ---------------------------------------- */
/**
 * @brief Display the help page
 * 
 */
void HelpPage(){
	SetPos(0,0);
	printf("基础规则：\n\
	1.每个玩家有5000元的启动资金 \n\
	3.火箭即再投掷一次骰子 \n\
	4.当玩家第一次路过一个无主的国家时，可以将其买下；\n\
	其他玩家路过有主的国家时，需要缴纳过路费,\
价钱为此国家价格的五分之一 \n\
	5.命运和机会即随机抽一张卡，获得不同的结果 \n\
	6.若手中的钱数变为零，则宣告破产，游戏失败;\
最后一个未破产的玩家即获得游戏胜利\n");
	printf("命运卡：\n");
	for(int i=0;i<10;i++){
		printf("	%s", destiny[i]);
	}
	printf("机会卡：\n");
	for(int i=0;i<10;i++){
		printf("	%s", chance[i]);
	}
}

/* --------------------------------------- */
/* ---------- Block of Main FSM ---------- */
/* --------------------------------------- */
typedef enum {
    INIT,
    PREPARE,
    PLAY_PAGE,
	SET_PAGE,
	HELP_PAGE,
    END
}GameState;

/**
 * @brief The main function
 * Implements a Finite State Machine
 * 
 * @return int 
 */
int main(){
    GameState state = INIT;

	system("title Monopoly");  // 设置窗口标题
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);   // 获得句柄

    // 隐藏光标操作
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(hOutput, &CursorInfo); // 获取控制台光标信息
    CursorInfo.bVisible = 0; // 隐藏控制台光标
    SetConsoleCursorInfo(hOutput, &CursorInfo); // 设置控制台光标状态

    while(true){
        switch (state){
        case INIT:			//Draw base board
            /* process */
            system("CLS");
            SetColor(Color_Brown, Color_Black);
            Welcome();
            int value = DrawMenu(41, 17, 0, 5, menu);
            SetColor(Color_White, Color_Black);
            
            /* change state */
            if (value != 4){
                playerCount = value + 2;
                state = PREPARE;
            }else{
                state = SET_PAGE;
            }
            break;

        case PREPARE:			//Draw prepare page
            /* process */
			system("CLS");
            draw_board();
            
            /* change state */
            state = PLAY_PAGE;
            break;

        case PLAY_PAGE:		//Draw play page
            /* process */
            printf("Playing\n");
            /* change state */
            state = END;
            break;

        case SET_PAGE:		//Draw setting page
			system("CLS");
            /* process */
            printf("Setting\n");
            /* change state */
            state = INIT;
            break;

        case HELP_PAGE:		//Draw help page
            /* process */
			system("CLS");
            HelpPage();
            /* change state */
            state = END;
            break;
        case END:			//Draw end page
            /* process */
            printf("End\n");
            /* change state */
            state = INIT;
            break;
        
        default:
            break;
        }
    }
    return 0;
}
