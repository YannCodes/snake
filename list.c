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

#include "list.h"

List *init(int headX, int headY)
{
    List *list = (List*)malloc(sizeof(*list));			//create the list
    Element *element = (Element*)malloc(sizeof(*element));	//create the element
    if (list == NULL || element == NULL)	//if there is a problem, exit
    {
        exit(EXIT_FAILURE);
    }
    element->numberX = headX;
    element->numberY = headY;
    element->previous = NULL;
    element->next = NULL;
    list->head = element;
    list->tail = element;
    return list;
}

void insert(List *list, int newNumberX, int newNumberY)
{
    Element *newElement = (Element*)malloc(sizeof(*newElement));  //allocate the new element

    if (newElement == NULL)
    {
        exit(EXIT_FAILURE);
    }

    newElement->numberX = newNumberX;
    newElement->numberY = newNumberY;
    newElement->previous = NULL;

    if(list->head != NULL)	//if there is already a head
    {
        list->head->previous = newElement;	//the old head previous becomes the new head
        newElement->next = list->head;		//the new head next becomes the old head
    }
    else
    {
        newElement->next = NULL; //if it is the 1st element, the next is NULL
    }
    list->head = newElement;	//adding the new element as head
}

void delTail(List *list)
{
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }

    if (list->head != NULL)
    {
        Element *actual = list->head;
        while (actual->next != NULL)
        {
            actual = actual->next; 			//this way, we are getting the tail
        }
        Element *toDelete = actual;
        
	if(toDelete->previous != NULL)			//only toDelete only null if the tail is the head (only one element)
	{
            /*Element *newTail = toDelete->previous;
            newTail->next = NULL;*/
	    toDelete->previous->next = NULL;
        }
        else
	{
            list->head = NULL;				//if the tail is the head, we delete it and the list becomes NULL
        }
        free(toDelete);
    }
}

void displayList(List *list)
{
    if (list == NULL)
    {
        exit(EXIT_FAILURE);
    }
    Element *actual = list->head;
    int i = 0;
    while (actual != NULL)
    {
        printf("%dX%d -> ", actual->numberX, actual->numberY);
        actual = actual->next;
        i++;
    }
    printf("NULL\n");
}

