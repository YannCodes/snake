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

#ifndef LISTE_H
#define LISTE_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <ncurses.h>

    typedef struct Element Element;
    struct Element
    {
        int numberX;
        int numberY;
        Element *previous;
        Element *next;
    };
    
    typedef struct List List;
    struct List
    {
        Element *head;
        Element *tail;
    };
    
    List *init(int headX, int headY);
    void insert(List *list, int newNumberX, int newNumberY);
    void delTail(List *list);
    void displayList(List *list);
    
#endif
