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

#include <ncurses.h>
#include <time.h>
#include "list.h"
#include "snake.h"

int main(int argc, char **argv)
{
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
        direction = getInput(list,direction,oldDirection); //get the key event
        
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
