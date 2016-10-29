/*  Yet another ncurses-based snake
 *  Copyright (C) 2016  Yann Lochet
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WIN
#include <ncurses.h>
#else
#include <ncurses/ncurses.h>    //necessary for MXE
#endif

#include <time.h>

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define USAGE printf( "usage: %s [ -h | -l [fr|us] ] \n", argv[0] );

#include "list.h"
#include "snake.h"

int main(int argc, char **argv)
{
    bool isLayoutFR = false;
    
    //command line arguments parsing
    if( argc == 3 && !strcmp(argv[1],"-l") )
    {
        if(!strcmp(argv[2],"fr"))
        {
            isLayoutFR = true;
        }
        else if(!strcmp(argv[2],"us"))
        {
            isLayoutFR = false;
        }
        else
        {
            USAGE
            exit(EXIT_FAILURE);
        }
    }
    else if( argc == 2 && !strcmp(argv[1],"-h"))
    {
        USAGE
        printf("\nsnake (C) 2016 Yann Lochet\
        \nThis program comes with ABSOLUTELY NO WARRANTY.\
        \nThis is free software, and you are welcome to\
        \nredistribute it under certain conditions.\n");
        printf("\nTo play, use the WASD (US layout) or ZQSD (FR layout) keys (see -l)\
        \nTo exit, use X.\
        \nIf no arguments supplied, launch the game with US layout.\n");
        exit(EXIT_SUCCESS);
    }
    else if(argc > 1)
    {
        USAGE
        exit(EXIT_FAILURE);
    }
    
    //ncurses init
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr,1);
    curs_set(0);
    
    List *list = init(0,0);
    insert(list,0,1);

    int mapArray[10][10];
    
    TwoDArrayPointer *map = mapArray;
    
    clearMap(map);
    
    Direction direction = RIGHT;
    Direction oldDirection = RIGHT;
    
    Coord* food = createFood(list);
    
    struct timespec oldTime, currentTime;
    
    int score = 0;
    int oldScore = 0;
    
    clock_gettime(CLOCK_MONOTONIC, &oldTime);
    
    for(;;)
    {
        clock_gettime(CLOCK_MONOTONIC, &currentTime);
        direction = getInput(direction,oldDirection,isLayoutFR); //get the key event
        
        if( ((currentTime.tv_nsec - oldTime.tv_nsec + 1000000000)%1000000000) > 200000000) //200000000ns = 200ms
        {
            clock_gettime(CLOCK_MONOTONIC, &oldTime);  //diff
            clearMap(map);
            moveSnake(list,direction);
            oldDirection = direction;
            oldScore = score;
            score = hitBoxFood(list,map,food,score);	//check if the snake's head is at the same coordinates as something else
            
            if(score != oldScore)
            { food = createFood(list); }    //if the score is incremented, the snake has eaten -> must move the food
            else
            {
                delTail(list);              //else, the old tail of the snake must be deleted, otherwise he is growing
                hitBoxSelf(list,map);       //it'd also cause problem with the hitbox
            }
	    
        genMap(list,map,food);
        dispBorder();
        dispMap(list,map);
	    mvprintw(12,0," Score : %d",score);
        }
    }

    return 0;
}
