#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <time.h>
#include <unistd.h>
#include "main.h"

int *init_universe(int max_cols,int max_rows) 
{
  int i,k;
  srand(time(NULL));
  int *universe = malloc(max_cols*max_rows*sizeof(int));

  for (i = 0; i < max_cols; i++) {
    for (k = 0; k < max_rows; k++) {
      universe[max_rows*i + k] = rand() % 2;
      //mvaddch(k,i,'*');
    }
  }

  return universe;
}

int isAlive(int *universe, int x, int y, int max_cols, int max_rows)
{
  if(universe[max_rows*x + y] == ALIVE) {
    return 1;
  }
  
  return 0;
}

int num_neighbors(int *universe, int x, int y, int max_cols, int max_rows) 
{
  int n = 0;
  
  if(y < max_rows) {
    //bottom 
    n += isAlive(universe,x,y+1,max_cols,max_rows);
  }
  if(y > 0) {
    //top
    n += isAlive(universe,x,y-1,max_cols,max_rows);
  }
  if(x < max_cols) {
    //right +
    n += isAlive(universe,x+1,y,max_cols,max_rows);
    if(y < max_rows) {
      //bottom right
      n += isAlive(universe,x+1,y+1,max_cols,max_rows);
    }
    if(y > 0) {
      //top right
      n += isAlive(universe,x+1,y-1,max_cols,max_rows);
    }
  }
  if(x > 0) {
    //left
    n += isAlive(universe,x-1,y,max_cols,max_rows);
    if(y < max_rows) {
      //bottom left
      n += isAlive(universe,x-1,y+1,max_cols,max_rows);
    }
    if(y > 0) {
      //bottom right
      n += isAlive(universe,x-1,y-1,max_cols,max_rows);
    }
  }
  
  return n;
}

int *update_universe(int *universe, int max_cols, int max_rows) 
{
  int i,k;
  int neighbors;
  int *new_universe = malloc(max_cols*max_rows*sizeof(int));

  for (i = 0; i < max_cols; i++) {
    for (k = 0; k < max_rows; k++) {
      neighbors = num_neighbors(universe,i,k,max_cols,max_rows);
      if(universe[max_rows*i + k] == ALIVE){      
        if(neighbors < 2) {
          //under-population dies
          attron(COLOR_PAIR(YELLOW_YELLOW));
          mvaddch(k,i,' ');
          attroff(COLOR_PAIR(YELLOW_YELLOW));
          
          new_universe[max_rows*i + k] = DEAD; 
        }else if( neighbors > 3) {
          //overcrowding dies
          attron(COLOR_PAIR(RED_RED));
          mvaddch(k,i,' ');
          attroff(COLOR_PAIR(RED_RED));
          
          new_universe[max_rows*i + k] = DEAD; 
        }else {
          //lives on
          attron(COLOR_PAIR(GREEN_GREEN));
          mvaddch(k,i,' ');
          attroff(COLOR_PAIR(GREEN_GREEN));
          
          new_universe[max_rows*i + k] = ALIVE; 
        }
      }else if(universe[max_rows*i + k] == DEAD){
        if(neighbors == 3){
          //spawned
          attron(COLOR_PAIR(BLACK_BLACK));
          mvaddch(k,i,' ');
          attroff(COLOR_PAIR(BLACK_BLACK));

          new_universe[max_rows*i + k] = ALIVE; 
        }else {
          attron(COLOR_PAIR(BLACK_BLACK));
          mvaddch(k,i,' ');
          attroff(COLOR_PAIR(BLACK_BLACK));
        }
      }
    }
  }
  return new_universe;
}

int init_curses(void)
{
  initscr(); raw();
  keypad(stdscr,TRUE);  // enable keypad
  noecho();             // don't echo() while we do getch
  curs_set(0);          // set cursor invisible
  nodelay(stdscr,TRUE); // getch is non blocking

  if(has_colors() == FALSE){
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }

  start_color();
  init_pair(RED_RED,COLOR_RED,COLOR_RED);
  init_pair(YELLOW_YELLOW,COLOR_YELLOW,COLOR_YELLOW);
  init_pair(GREEN_GREEN,COLOR_GREEN,COLOR_GREEN);
  init_pair(BLACK_BLACK,COLOR_BLACK,COLOR_BLACK);

  return 0;
}

int main(int argc, const char *argv[])
{
  int ch = 0;
  init_curses();
  int *universe = init_universe(COLS,LINES);
  
  while((ch = getch()) != 'q'){
    universe = update_universe(universe,COLS,LINES);
    usleep(300000);
  }

  endwin(); 
  return 0;
}
