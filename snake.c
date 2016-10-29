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

#include <time.h>
#include "snake.h"
#include "list.h"

#define SNAKE 1
#define FOOD 2
#define BLANK 0

Direction getInput(Direction currentDirection, Direction oldDirection, bool isLayoutFR)
{
    /*the oldDirection is needed because without it, the snake will be able go back (this trigger the hitbox)
    the currentDirection is needed if the user changes the direction more than one time between two moves of 
    the snake,otherwise the user still could go back by pressing the keys twice, once by a perpendicular direction,
    and a second time by the opposite to the old direction*/
  
    int input = 0;
    
    input = getch(); //getting key event
    
    Direction newDirection = VOID;
    
    char upChar;
    char leftChar;
    char downChar;
    char rightChar;
    
    if(isLayoutFR)
    {
        upChar = 'z';
        leftChar = 'q';
        downChar = 's';
        rightChar = 'd';
    }
    else
    {
        upChar = 'w';
        leftChar = 'a';
        downChar = 's';
        rightChar = 'd';
    }
    
    if( input == upChar) { newDirection =  UP;}
    else if( input == leftChar) { newDirection =  LEFT;}
    else if( input == downChar) { newDirection =  DOWN;}
    else if( input == rightChar) { newDirection =  RIGHT;}
    else if( input == 'x') { endwin(); exit(EXIT_SUCCESS); }
        
    if( (oldDirection == UP && newDirection == DOWN) || (oldDirection == DOWN && newDirection == UP) || (oldDirection == LEFT && newDirection == RIGHT) || (oldDirection == RIGHT && newDirection == LEFT) || (newDirection == VOID) )
    {
        newDirection = currentDirection;	//if the new direction is at the opposite of the old one, cancel the new move
    }
    
    //if(newDirection == VOID){newDirection = currentDirection;}
    
    return newDirection;
}

void moveSnake(List *list, Direction direction)
{
    int x,y;
    
    switch(direction) //moving snake in different direction based on the Direction
    {
        case RIGHT:
            if(list->head->numberX == 9)
                x=0;
            else
                x=list->head->numberX + 1;
            y=list->head->numberY;
            break;
        
        case LEFT:
            if(list->head->numberX == 0)
                x=9;
            else
                x=list->head->numberX - 1;
            y=list->head->numberY;
            break;
        
        case UP:
            if(list->head->numberY == 0)
                y=9;
            else
                y=list->head->numberY - 1;
            x=list->head->numberX;
            break;
        
        case DOWN:
            if(list->head->numberY == 9)
                y=0;
            else
                y=list->head->numberY + 1;
            x=list->head->numberX;
            break;
        case VOID:
            exit(EXIT_FAILURE);
    }
    
    insert(list,x,y); //create the new head of the snake based on the switch
}

void genMap(List* list,TwoDArrayPointer map[10], Coord* food)
{
    map[food->numberX][food->numberY]=FOOD;
  
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Element *actual = list->head;
    while (actual != NULL)
    {
        map[actual->numberX][actual->numberY]=SNAKE;
        actual = actual->next;
    }
    
}

void clearMap(TwoDArrayPointer map[10])
{
    int x,y;
    
    for(x=0;x<10;x++)
    {
        for(y=0;y<10;y++)
        {
            map[x][y] = BLANK;
        }
    }
}

void dispMap(List* list,TwoDArrayPointer map[10])
{
    int x,y;
    
    for(x = 0; x < 10; x++)
    {
        for(y = 0; y < 10; y++)
        {
            if(map[x][y] == SNAKE)
                mvprintw(y+1,x+1,"o"); //putting +1 because the 0,n and n,0 coordinates are taken by borders
	    
            else if(map[x][y] == FOOD)
                mvprintw(y+1,x+1,"+");
	    
            else
                mvprintw(y+1,x+1," ");
	    
            refresh();
        }
    }
}

void dispBorder()
{
    int i;
    for(i = 1;i<12;i++)
    {
        mvprintw(0,i,"-");
        mvprintw(11,i,"-");
    }
    
    for(i = 1;i<12;i++)
    {
        mvprintw(i,0,"|");
        mvprintw(i,11,"|");
    }
    
    mvprintw(0,0,"+");
    mvprintw(0,11,"+");
    mvprintw(11,0,"+");
    mvprintw(11,11,"+");
}

Coord* createFood(List* list)
{
    bool newFoodDone = false;
    Coord *food = (Coord*)malloc(sizeof(*food));
    srand(time(NULL));
    
    while(!newFoodDone)
    {
	food->numberX=(rand() % 10);
	food->numberY=(rand() % 10);
	newFoodDone = true;
      
	Element *actual = list->head;
	while (actual != NULL)
	{
	    if((food->numberX == actual->numberX) && (food->numberY == actual->numberY)) //if the coordinates are coordinates of the snake,
	    {
		newFoodDone = false; //generate coordinates again
		break; //leave the loop
	    }
	    actual = actual->next;
	}
    }
    
    return food;
}

int hitBoxFood(List* list, TwoDArrayPointer map[10], Coord* food, int score)
{
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    if ((food->numberX == list->head->numberX) && (food->numberY == list->head->numberY)) //food collision management
    {
	score++; //if the snake eats, increment the score
    }
    
    return score;
}

void hitBoxSelf(List* list, TwoDArrayPointer map[10])
{
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }
    
    Element *actual = list->head->next; //if beginning by list->head, will always be triggered
    while (actual != NULL) //snake with himself collision management
    {
        if((list->head->numberX == actual->numberX) && (list->head->numberY == actual->numberY)) //checking if 2 parts of the snake have the same coordinates
        {
            mvprintw(13,1,"Game Over");
            refresh();
            sleep(2);
            endwin();
            exit(EXIT_SUCCESS);
        }
	actual = actual->next;
    }
}
