#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Externals 
unsigned int level;
int input;
// Screen Size
int row, col;

// Key maps
#define QUIT 113
#define ENTER 10

int main(int argc, char *argv[])
{
  /* Argument check */
  if (argc != 2)
    {
      printf("usage: sodatris level_num\n\tlevel_num: numbers between 1 and 10\n");
      return 1;
    }

  /* Default level value 0 */
  level = atoi(argv[1]);

  if (level > 10)
    {
      printf("\nlevel_num must be between 1 and 10\n");
      return 1;
    }
  
  /* Screen Initialization */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0); // hide cursor
  getmaxyx(stdscr, row, col);
  init(row, col);
  refresh();

  /* Wait for user command */
  input = getch();
  while (input != QUIT && input != ENTER)
    {
	mvprintw(row-1, col-10, "%i", input);
	input = getch();
    }

  /* Start TODO: Print points out to console */
  unsigned long final_points = 0;
  if (input == ENTER) {
    clear();
    refresh();
    final_points = startgame(level);
  }
  
  /* Quit */
  endwin();
  printf("Final points: %lu\n", final_points);
  return 0;
}

/* Level determines the speed of the game.
   level 1: 1 second user input window
   level 2: 0.9 secs
   .
   .
   level 9: 0.2 secs
   level 10: 0.1 secs
*/
unsigned long startgame (unsigned int level)
{
  /* keep new level for dynamic level adjustment */
  unsigned int current_level = level;
  unsigned long points = 0.0;
  /* Initialize speed */
  float curr_speed = get_speed(level);

  /* Load game screen */

  WINDOW *gw;
  unsigned int block_size_y = row / 20;
  unsigned int block_size_x = (col / 3) / 10;
  
  int width =  block_size_x * 10;
  int height = block_size_y * 20;
  int startx = col / 3;
  int starty = (row - height)/2;

  // show game window
  gw = newwin(height, width, starty, startx);
  box(gw, 0, 0);
  wrefresh(gw);
  
  /* Generate random block */
  int b1x = 0, b1y = 0;
  char *line = malloc(sizeof(char) * (block_size_x + 1));
  for (int i = 0; i < block_size_x; ++i)
    {
      line[i] = '.';
    }
        
  for (int i = 0; i < block_size_y; ++i)
    {
      mvwprintw(gw, (b1y += i), b1x, "%s", line);
    }

  wrefresh(gw);
  
  /* User input window */
  input = getch();
  while (input != QUIT && input != ENTER)
    {
	mvprintw(row-1, col-10, "%i", input);

	// move block
	wclear(gw);
	box(gw, 0, 0);

	for (int i = 0; i < block_size_y; ++i)
	  {
	    mvwprintw(gw, (b1y += i), b1x, "%s", line);
	  }
	wrefresh(gw);
	  
	input = getch();
    }
  /* Move block */

  /* Bring block down */
  
  /* Game over! Show results */
  free(line);
  delwin(gw);
  return points;
}

/* startgame Helper functions */
float get_speed(unsigned int level)
{
  return (1.0 - (float) (level / 10)) + 0.1;
}

// starting screen
void init(int row, int col)
{
  char hello_mesg[] = "Welcome to Tetris Soda";
  char start_mesg[] = "Press ENTER to start!(q to quit)";

  /* initial centered y position */
  int center_y = row / 2;

  /* Intro text */
  attron(A_BOLD);
  mvprintw(center_y, (col-strlen(hello_mesg))/2, "%s", hello_mesg);
  mvprintw(center_y + 1, col/2-1, "%i", level);
  attroff(A_BOLD);
  attron(A_BLINK);
  mvprintw(center_y + 2, (col-strlen(start_mesg))/2, "%s", start_mesg);
  attroff(A_BLINK);
  
  /* Bottom text */
  mvprintw(row -1, 0, "Made by Samantha\n", row, col);
  /* mvprintw(row-1, 0, "This screen has %d rows and %d columns\n", row, col); */
}
