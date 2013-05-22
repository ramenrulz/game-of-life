#ifndef GOL_H_
#define GOL_H_

typedef enum {
  DEAD,
  ALIVE
} status;

typedef enum {
  RED_RED = 1,
  YELLOW_YELLOW,
  GREEN_GREEN,
  BLACK_BLACK
} color_pair;


// prototypes
int *init_universe(int max_cols,int max_rows); 
int isAlive(int *universe, int x, int y, int max_cols, int max_rows);
int num_neighbors(int *universe, int x, int y, int max_cols, int max_rows);
int *update_universe(int *universe, int max_cols, int max_rows);
int init_curses(void);

#endif /* GOL_H_ */
