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

#ifndef SNAKE_H
#define SNAKE_H

    #ifndef WIN
    #include <ncurses.h>
    #else
    #include <ncurses/ncurses.h>    //necessary for MXE
    #endif
    
    #include <time.h>
    #include <unistd.h>
    #include <stdbool.h>
    #include "list.h"
    
    typedef enum Direction Direction;
    enum Direction
    {
        UP, DOWN, LEFT, RIGHT, VOID
    };
    
    typedef int TwoDArrayPointer[10];
    
    typedef struct Coord Coord;
        struct Coord
        {
            int numberX;
            int numberY;
        };
    
    Direction getInput(Direction currentDirection, Direction oldDirection, bool isLayoutFR);
    
    void moveSnake(List *list,Direction direction);
    
    void genMap(List* list,TwoDArrayPointer map[10], Coord* food);
    
    void dispMap(List* list,TwoDArrayPointer map[10]);
    
    void clearMap(TwoDArrayPointer map[10]);
    
    void dispBorder();
    
    Coord* createFood(List* list);
    
    int hitBoxFood(List* list,TwoDArrayPointer map[10], Coord* food, int score);
    
    void hitBoxSelf(List* list,TwoDArrayPointer map[10]);
    
#endif
