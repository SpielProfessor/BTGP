// btgp - [B]asic [T]erminal [G]raph [P]lotter v1.0, (c) MrVollbart, 2024 //
// Compile with tinyexpr.c & tinyexpr.h (Ncurses & math lib installed):
// gcc -lncurses -lm -o btgp btgp.c tinyexpr.c && ./btgp;
// TODO:
// Plotting
// add scaling options:
// - zoom
// - show floats/float support
// - better high numbers
//
// tinyexpr *best* math lib
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../lib/tinyexpr.h"
#include <locale.h>
#include <stdbool.h>

void calcPlot(char* operation, double x_lower, double x_upper, double incrementor);

int digits (int n) {
    if (n == 0) return 1;
    return floor (log10 (abs (n))) + 1;
}

int main(int argc, char* argv[]){
  //setlocale(LC_ALL, "");
  printf("\e[1;1H\e[2J");
  printf("Thanks for using:\n");
  printf("BTGP - [B]asic [T]erminal [G]raph [P]lotter by MrVollbart, (c) MrVollbart, 2024\nThis product is not under any warranty of any kind.");
  initscr();
      start_color();
    init_pair(1,COLOR_RED, COLOR_BLACK);
    init_pair(2,COLOR_CYAN, COLOR_BLACK);
  

  printw("BTGP - [B]ASIC [T]ERMINAL [G]RAPH [P]LOTTER\n");
  printw("Insert the graph to plot: (!2x UNSUPPORTED, only 2*x or x*2, !Variable only x (Case-sensitive))\n> ");
  char operation[100];
  scanw("%99s",operation);

  printw("Insert starting pos (NON-FULL NUMBERS USELESS):\n> ");

  double x_lower, x_upper;
  scanw("%lf\n", &x_lower);
  printw("Insert ending pos (NON-FULL NUMBERS USELESS):\n> ");

  scanw("%lf\n", &x_upper);
  printw("Insert incrementor (VERY MUCH UNFINISHED! IF YOU DON'T INSERT 1 BAD THINGS WILL HAPPEN!):\n> ");
  double incre;
  scanw("%lf\n", &incre);
  // CALCULATION //
    calcPlot(operation, x_lower, x_upper, incre);
}









void calcPlot(char* operation, double x_lower, double x_upper, double incre){
  bool running=true;
  int loop=0;
      start_color();
    init_pair(1,COLOR_RED, COLOR_BLACK);
    init_pair(2,COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, 100, COLOR_BLACK);
  clear();
  int w, h;
  getmaxyx(stdscr, h, w);
  double number;
  // variables; te_variable required vor tinyexpr //
  double x;
  te_variable varX[]={{"x", &x}};
  // error //
  int err;
  // compile the operation //
  te_expr *expr = te_compile(operation, varX, 1, &err);
    int CELL_WIDTH = 3;
    int MIDDLE_AXIS_MOVE = 10;

    int X_OFFSET = w/2-abs((int)(x_lower*CELL_WIDTH));
  if (expr){
    while (running){
      clear();
      int scrx=3;
      // draw shell/lines
      // X-Axis //
      for (int i=1; i<=CELL_WIDTH*(fabs(x_lower)+fabs(x_upper))+2; i++){
        // Makes the graph not leave the edge!
        int y_printoffset=MIDDLE_AXIS_MOVE;
        if (h/2+MIDDLE_AXIS_MOVE+3>h){
          y_printoffset=h/2-2;
          attroff(COLOR_PAIR(2));
        }
        if (h/2+MIDDLE_AXIS_MOVE-1<0){
          y_printoffset=-h/2+1;
          attroff(COLOR_PAIR(2));
        }
        mvprintw(h/2+y_printoffset,i+X_OFFSET,"-");
        

      }
      // Draw Index //
      // X //
      int centreX=1;
      int j=scrx;
      attron(COLOR_PAIR(3));
      for (int i=x_lower; i<=x_upper; i++){
        if (i==0){
          centreX=j;
        }

        int y_printoffset=MIDDLE_AXIS_MOVE;
        if (h/2+MIDDLE_AXIS_MOVE+3>h){
          y_printoffset=h/2-2;
          attroff(COLOR_PAIR(3));
        } else if (h/2+MIDDLE_AXIS_MOVE-1<0){
          y_printoffset=-h/2+1;
          attroff(COLOR_PAIR(3));
        }
        mvprintw(h/2+y_printoffset+1, j+X_OFFSET, "%d", i);
        mvprintw(h/2+y_printoffset, j+X_OFFSET, "^");
        j+=CELL_WIDTH;
      }
      attroff(COLOR_PAIR(3));
      // Y //
      attron(COLOR_PAIR(2));
      for (int i=1; i<=h; i+=1){
        int x_printoffset=centreX+1+X_OFFSET;
         if (centreX+1+X_OFFSET+3>w){
            x_printoffset=w-3;
          attroff(COLOR_PAIR(2));
          }
          if (centreX+1+X_OFFSET-1<0){
            x_printoffset=1;
          attroff(COLOR_PAIR(2));
          }

        mvprintw(i, x_printoffset, "%d", h/2+MIDDLE_AXIS_MOVE-i);
      } 
      attroff(COLOR_PAIR(2));
      if (centreX>1){
        // Draw lines [y] //
        for (int i=1; i<=h; i++){
          int x_printoffset=centreX+X_OFFSET;
          if (centreX+1+X_OFFSET+3>w){
            x_printoffset=w-4;
            attroff(COLOR_PAIR(2));
          }
          if (centreX+1+X_OFFSET-1<0){
            x_printoffset=0;
            attroff(COLOR_PAIR(2));
          }
          mvprintw(i, x_printoffset, "|");
        } 
       
        mvprintw(h/2+MIDDLE_AXIS_MOVE+1, centreX+X_OFFSET, "0");
        mvprintw(h/2+MIDDLE_AXIS_MOVE, centreX+X_OFFSET, "+");
      }
    

      int interceptY;
      // ToDo: Handle Null pointers (Error management with malloc)
      int elementsX=0;
      int* interceptX=(int*)malloc(sizeof(int));
      attron(COLOR_PAIR(1));
      for (x=x_lower; x<=x_upper; x+=incre){
        mvprintw(h/2+MIDDLE_AXIS_MOVE-(int)te_eval(expr), scrx+X_OFFSET, "x");
        if (scrx==centreX){
          interceptY=(int)te_eval(expr);
        }
        if (te_eval(expr) == 0){
          elementsX++;
        
          interceptX=realloc((int*)interceptX, elementsX*sizeof(int));
          interceptX[elementsX]=x;
        //printf("interception with x-axis at %f\n", x);
        }
      //printw("%g\n", te_eval(expr));
      scrx+=CELL_WIDTH;
      }
      attroff(COLOR_PAIR(1));

      char* xIntercepts=malloc(elementsX*2*sizeof(char));
      for (int i=1; i<=elementsX; i++){
        char l[10];
        sprintf(l, "%d;", interceptX[i]);
        strcat(xIntercepts, l);
      }
      mvprintw(0,1,"BTGP graph (interactive) - %s [Screen Width: %d, Screen Height: %d], Y-intercept: %d; X-intercepts: %s", 
      /*Line continuation*/ operation, w, h, interceptY, xIntercepts);
      
      noecho();
      
      int c = getch();
      switch (c){
        case 'j':
          MIDDLE_AXIS_MOVE--;
          break;
        case 'k':
          MIDDLE_AXIS_MOVE++;
          break;
        case 'q':
          running=false;
          break;
        case 'c':
          MIDDLE_AXIS_MOVE=0;
          X_OFFSET=w/2-abs((int)(x_lower*CELL_WIDTH));
          break;
        case 'h':
          X_OFFSET++;
          break;
        case 'l':
          X_OFFSET--;
          break;
        case 'x':
          X_OFFSET=w/2-abs((int)(x_lower*CELL_WIDTH));
          break;
        case 'y':
          MIDDLE_AXIS_MOVE=0;
          break;
      }
      free(interceptX);
      free(xIntercepts);
      refresh();
      loop++;
    }
      te_free(expr);
  } else {
    printf("Parse error at %d\n", err);
  }
  printw("Loops:%d\n", loop);
  endwin();
}
