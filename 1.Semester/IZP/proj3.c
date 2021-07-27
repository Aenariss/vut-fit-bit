/* 
*  IZP PROJEKT 3
*  AUTHOR: VOJTECH FIALA
*  LOGIN: xfiala61
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


//Map structure
typedef struct {
  int rows;
  int cols;
  unsigned char *cells;
} Map;

//Iteration limit to stop the code from looping
#define ITERATION_LIMIT 5000

//Border values, which are also used as return values in start_border
#define LEFT 8
#define RIGHT 7
#define UP_DOWN 6

//Start_border return values
#define DOWN_RIGHT 9
#define TOP 10
#define LEFT_BOT 11
#define RIGHT_NORMAL 12
#define ANOTHER_RIGHT 13

//the value of ascii_7, to be used to check, if a cell does not contain a higher number
#define ASCII_7 55

//right or left hand
#define RIGHT_HAND -5
#define LEFT_HAND 5

//direction values
#define NORTH 80
#define SOUTH 90
#define WEST 100
#define EAST 110


//Lets make a map
Map map_ctor(int rows, int cols) {
    Map m = { .rows = rows, .cols = cols, .cells = malloc(rows*cols*sizeof(unsigned char))};
    return m;
}

//Let the cells be free
void map_dtor(Map *m) {
    free(m->cells);
}

//Give the cells of the map some actual values
int map_init(Map *m, char *file) {
    FILE *f;
    f = fopen(file, "r");
    if(f == NULL) {fprintf(stderr, "Loading error"); return 1;}
    fscanf(f, "%*[^\n]\n"); //skip the first line, we dont want it in this case
    for (int i = 1; i <= m->rows*m->cols;i++) {
        if (i == m->rows*m->cols){
             fscanf(f, "%c", &m->cells[i]); 
        }
        else {
            fscanf(f, "%c ", &m->cells[i]); 
        }
    }
    fclose(f);
    return 0;
}

//check if on a given coordinates and side of a cell is a border.
bool isborder(Map *map, int r, int c, int border) {   
    if (map->cells[(r-1)*map->cols+(c)] == '0') { //If the cell is 0, then there is no border and so on
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '1' && border == LEFT) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '1' && border == RIGHT) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '1' && border == UP_DOWN) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '2' && border == LEFT) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '2' && border == RIGHT) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '2' && border == UP_DOWN) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '3' && border == LEFT) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '3' && border == RIGHT) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '3' && border == UP_DOWN) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '4' && border == LEFT) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '4' && border == RIGHT) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '4' && border == UP_DOWN) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '5' && border == LEFT) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '5' && border == RIGHT) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '5' && border == UP_DOWN) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '6' && border == LEFT) {
        return false;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '6' && border == RIGHT) {
        return true;
    }
    if (map->cells[(r-1)*map->cols+(c)] == '6' && border == UP_DOWN) {
        return true;
    }
    if(map->cells[(r-1)*map->cols+(c)] == '7') {
        return true;
    }
    return false;
}


//Test if the map's ok
int test (char *file) {
    int rows;
    int cols;
    bool error = false;
    int count = 0;
    char tmp;
    FILE *fp;
    fp = fopen(file, "r");
    if(fp == NULL) {fprintf(stderr, "Loading error"); return 1;}
    fscanf(fp, "%d %d", &rows, &cols);
    Map m = map_ctor(rows, cols);
    map_init(&m, file);
    for (int i = 1; i <= m.rows*m.cols;i++) {
            if(m.cells[i] > ASCII_7) //map has to have values consisting only of numbers 0-7. It can't have negative values because of its data type
            error = true;
            }
    fseek(fp, 0, SEEK_SET); //move to the beginning
    fscanf(fp, "%*[^\n]\n"); //remove first line, we dont need it
    for (int i = 1; i <= m.rows; i++) { //Check the neighboring borders
        for (int k = 1; k <= m.cols; k++) {
                if(i%2 == 1) {
                    if (k+1 > m.cols) {}
                    else {
                        if(isborder(&m, i, k, RIGHT) == isborder(&m, i, k+1, LEFT)) {}
                        else {
                            error = true;
                        }
                    }
                    if (k%2 == 0) {
                        if (i+1 > m.rows) {}
                        else {
                            if (isborder(&m, i, k, UP_DOWN) == isborder(&m, i+1, k, UP_DOWN)) {}
                            else { 
                                error = true; 
                            }
                        }
                    }
                }
                else {
                    if (k+1 > m.cols) {}
                    else {
                        if(isborder(&m, i, k, RIGHT) == isborder(&m, i, k+1, LEFT)) {}
                        else {
                            error = true;
                        }
                    }
                    if(k%2 == 1) {
                        if (i+1 > m.rows) {}
                        else {
                            if (isborder(&m, i, k, UP_DOWN) == isborder(&m, i+1, k, UP_DOWN)) {}
                            else { 
                                error = true; 
                            }
                        }   
                    }
                }
    }
    }

    fseek(fp, 0, SEEK_SET); //move to the beginning
    fscanf(fp, "%*[^\n]\n"); //remove first line, we dont need it
    while (1) {
        tmp = fgetc(fp);
        count++;
        if (tmp == ' ')
            count--;
        if (tmp == '\n')
            count--;
        if(feof(fp)) {
            break;
        }
    }
    if(count-1 != rows*cols) { //check the number of values in cells
        printf("%d\n%d\n", rows*cols, count-1);
        error = true;
    }
    fclose(fp);
    if (error)  {
        map_dtor(&m);
        return 1;
    }
    else {
    map_dtor(&m);
return 0;
    }
}

int start_border(Map *map, int r, int c, int leftright) { //Return a value which will be sued to assign a starting direction
    if (r > map->rows || c > map->cols) {
        return 1;
    }
    if(leftright == RIGHT_HAND) {
                if (r == 1) {
                    if(isborder(map, r, c, UP_DOWN) == false) {
                    return LEFT;
                    }
                }
                else if (r%2 == 1 && c == 1) {
                    if(isborder(map, r, c, LEFT) == false) {
                    return RIGHT;
                    }
                }
                else if (r%2 == 0 && c == 1) {
                    if(isborder(map, r, c, LEFT) == false) {
                    return UP_DOWN;
                    }
                }

                else if ((r+c)%2 == 1 && c == map->cols && isborder(map, r, c, UP_DOWN) == true) {
                   // printf("return 6");
                    return LEFT_BOT;
                }

                else if ((c == map->cols) && ((r+c)%2 == 0) && (isborder(map, r, c, UP_DOWN) == true)) {
                   // printf("return 5");
                    return TOP;
                }

                else if (r == map->rows) {
                    if(isborder(map, r, c, UP_DOWN) == false) {
                  // printf("return 4");
                    return DOWN_RIGHT;
                    }
                }
                else if ((r+c)%2 == 0 && c == map->cols && isborder(map, r, c, RIGHT) == false) {
                    return RIGHT_NORMAL;
                }
                else if ((r+c)%2 == 1 && c == map->cols && isborder(map, r, c, RIGHT) == false) {
                    return ANOTHER_RIGHT;
                }
    }
    else if(leftright == LEFT_HAND) {
                if (r == 1) {
                    if(isborder(map, r, c, UP_DOWN) == false) {
                    return LEFT;
                    }
                }
                else if (r%2 == 1 && c == 1) {
                    if(isborder(map, r, c, LEFT) == false) {
                    return RIGHT;
                    }
                }
                else if (r%2 == 0 && c == 1) {
                    if(isborder(map, r, c, LEFT) == false) {
                    return UP_DOWN;
                    }
                }

                else if ((r+c)%2 == 1 && c == map->cols && isborder(map, r, c, UP_DOWN) == true) {
                   // printf("return 6");
                    return LEFT_BOT;
                }

                else if ((c == map->cols) && ((r+c)%2 == 0) && (isborder(map, r, c, UP_DOWN) == true)) {
                   // printf("return 5");
                    return TOP;
                }

                else if (r == map->rows) {
                    if(isborder(map, r, c, UP_DOWN) == false) {
                  // printf("return 4");
                    return DOWN_RIGHT;
                    }
                }
                else if ((r+c)%2 == 0 && c == map->cols && isborder(map, r, c, RIGHT) == false) {
                    return RIGHT_NORMAL;
                }
                else if ((r+c)%2 == 1 && c == map->cols && isborder(map, r, c, RIGHT) == false) {
                    return ANOTHER_RIGHT;
                }
    }
return 1;
}

//Function for righthand rule to assign us the direction which will be followed in the next cell
int direction_right(Map *m, int *r, int *c, int *direction) { 
    if ((*r+*c)%2 == 1) {
        if (*direction == EAST) {
            if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH; // in a context of the next triangle, aka the one i just printed a line above
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }
        }
        else if (*direction == WEST) {
                if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST; 
                    }
                }

        }
        else if (*direction == SOUTH) {
            if(isborder(m, *r+1, *c, RIGHT) == false && isborder(m, *r+1, *c, LEFT) == false && isborder(m, *r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }
                else if(isborder(m, *r+1, *c, RIGHT) == false && isborder(m, *r+1, *c, LEFT) == true && isborder(m, *r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if(isborder(m, *r+1, *c, RIGHT) == true && isborder(m, *r+1, *c, LEFT) == false && isborder(m, *r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }
                else if(isborder(m, *r+1, *c, RIGHT) == true && isborder(m, *r+1, *c, LEFT) == true && isborder(m,*r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH;
                    }
                }

        }


    }

    else {
        if (*direction == EAST) {
            if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH;
                    }
                }
                 else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                   *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }

        }

        else if (*direction == WEST) {
            if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST; 
                    }
                }
        }

        else if (*direction == NORTH) {
            if(isborder(m, *r-1, *c, RIGHT) == false && isborder(m, *r-1, *c, LEFT) == false && isborder(m, *r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                    *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if(isborder(m, *r-1, *c, RIGHT) == false && isborder(m, *r-1, *c, LEFT) == true && isborder(m, *r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                    *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;}
                }
                else if(isborder(m, *r-1, *c, RIGHT) == true && isborder(m, *r-1, *c, LEFT) == false && isborder(m, *r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                    *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;}
                }
                else if(isborder(m, *r-1, *c, RIGHT) == true && isborder(m, *r-1, *c, LEFT) == true && isborder(m,* r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                   *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH;}
                }
        }
    }
return 0;
}

//Function for lefthand rule to assign us the direction which will be followed in the next cell
int direction_left(Map *m, int *r, int *c, int *direction) {
    if ((*r+*c)%2 == 1) {
        if (*direction == EAST) {
            if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH; // in a context of the next triangle, aka the one i just printed a line above
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }
        }
        else if (*direction == WEST) {
            if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST; 
                    }
                }
        }
        else if (*direction == SOUTH) {
            if(isborder(m, *r+1, *c, RIGHT) == false && isborder(m, *r+1, *c, LEFT) == false && isborder(m, *r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if(isborder(m, *r+1, *c, RIGHT) == false && isborder(m, *r+1, *c, LEFT) == true && isborder(m, *r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if(isborder(m, *r+1, *c, RIGHT) == true && isborder(m, *r+1, *c, LEFT) == false && isborder(m, *r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }
                else if(isborder(m, *r+1, *c, RIGHT) == true && isborder(m, *r+1, *c, LEFT) == true && isborder(m,*r+1, *c, UP_DOWN) == false) {
                    if (*r!=m->rows) {
                    *r = *r+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = NORTH;
                    }
                }
        }

    }

    else {
        if (*direction == EAST) {
            if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH;
                    }
                }
                 else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == false) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == false && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                    *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;
                    }
                }
                else if (isborder(m, *r, *c+1, RIGHT) == true && isborder(m, *r, *c+1, LEFT) == false && isborder(m, *r, *c+1, UP_DOWN) == true) {
                    if (*c!=m->cols) {
                   *c = *c+1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }

        }
        else if (*direction == WEST) {
            if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == false) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == false && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST; 
                    }
                }
                else if(isborder(m, *r, *c-1, RIGHT) == false && isborder(m, *r, *c-1, LEFT) == true && isborder(m, *r, *c-1, UP_DOWN) == true) {
                    if (*c!=1) {
                    *c = *c-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST; 
                    }
                }
        }
        else if (*direction == NORTH) {
            if(isborder(m, *r-1, *c, RIGHT) == false && isborder(m, *r-1, *c, LEFT) == false && isborder(m, *r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                    *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;
                    }
                }
                else if(isborder(m, *r-1, *c, RIGHT) == false && isborder(m, *r-1, *c, LEFT) == true && isborder(m, *r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                    *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = EAST;}
                }
                else if(isborder(m, *r-1, *c, RIGHT) == true && isborder(m, *r-1, *c, LEFT) == false && isborder(m, *r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                    *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = WEST;}
                }
                else if(isborder(m, *r-1, *c, RIGHT) == true && isborder(m, *r-1, *c, LEFT) == true && isborder(m,* r-1, *c, UP_DOWN) == false) {
                    if (*r!=1) {
                   *r = *r-1;
                    printf("%d,%d\n", *r, *c);
                    *direction = SOUTH;}
                }
        }
    }
    return 0;
}

//function to go through the maze using the right hand rule
void go_right(Map *m, int r, int c) {
    int direction = 0;
    if(start_border(m, r, c, RIGHT_HAND) == UP_DOWN) { //Assign a starting direction
        printf("%d,%d\n", r, c);
        direction = EAST; //we think of a next one in line and starts with this
    }
    else if(start_border(m, r, c, RIGHT_HAND) == RIGHT) {
        printf("%d,%d\n", r, c);
        direction = NORTH;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == LEFT) {
        printf("%d,%d\n", r, c);
        direction = EAST;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == DOWN_RIGHT) {
        printf("%d,%d\n", r, c);
        direction = EAST;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == TOP) {
        printf("%d,%d\n", r, c);
        direction = WEST;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == LEFT_BOT) {
        printf("%d,%d\n", r, c);
        direction = WEST;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == RIGHT_NORMAL) {
        printf("%d,%d\n", r, c);
        direction = NORTH;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == ANOTHER_RIGHT) {
        printf("%d,%d\n", r, c);
        direction = WEST;
    }

    int q = 0;
    while (q < ITERATION_LIMIT) { //Iteration limit to stop from looping endlessly
        if (r < 0 || c < 0 || r > m->rows || c > m->cols) {
            break;
        }
        if ((r+c)%2 == 1) {

            //4
            if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == SOUTH) {
                direction = EAST;
                direction_right(m, &r, &c, &direction);
            }

            //0
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {                    
                    direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction_right(m, &r, &c, &direction);
            }

            //1
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = SOUTH;
                direction_right(m, &r, &c, &direction);
            }
            //2
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = WEST;
                direction_right(m, &r, &c, &direction);
            }
            //3
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = SOUTH;
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = SOUTH;
                direction_right(m, &r, &c, &direction);
            }

            //5
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction = EAST;
                direction_right(m, &r, &c, &direction);
            }else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == SOUTH) {
                direction = EAST;
                direction_right(m, &r, &c, &direction);
            }

            //6
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction = WEST;
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == SOUTH) {
                direction = WEST;
               direction_right(m, &r, &c, &direction);
            }




        }

        else {
            //4
            if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == NORTH) {
                direction = WEST;
                direction_right(m, &r, &c, &direction);
            }

            // 0
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction_right(m, &r, &c, &direction);
            }

            //1
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = EAST;
               direction_right(m, &r, &c, &direction);
            }

            //2
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = NORTH;
                direction_right(m, &r, &c, &direction);
            }

            //3
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = NORTH;
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = NORTH;
               direction_right(m, &r, &c, &direction);
            }

            //5
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction = EAST;
               direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == NORTH) {
                direction = EAST;
               direction_right(m, &r, &c, &direction);
            }

            //6
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction = WEST;
                direction_right(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == NORTH) {
                direction = WEST;
                direction_right(m, &r, &c, &direction);
            }


        }
        q++;
    }
}

//function to go through the maze using the left hand rule
void go_left (Map *m, int r, int c) {
    int direction = 0;
    if(start_border(m, r, c, LEFT_HAND) == UP_DOWN) {
        printf("%d,%d\n", r, c);
        direction = EAST; //we think of a next one in line and starts with this
    }
    else if(start_border(m, r, c, LEFT_HAND) == RIGHT) {
        printf("%d,%d\n", r, c);
        direction = NORTH;
    }
    else if(start_border(m, r, c, LEFT_HAND) == LEFT) {
        printf("%d,%d\n", r, c);
        direction = EAST;
    }
    else if(start_border(m, r, c, LEFT_HAND) == DOWN_RIGHT) {
        printf("%d,%d\n", r, c);
        direction = WEST;
    }
    else if(start_border(m, r, c, LEFT_HAND) == TOP) {
        printf("%d,%d\n", r, c);
        direction = WEST;
    }
    else if(start_border(m, r, c, LEFT_HAND) == LEFT_BOT) {
        printf("%d,%d\n", r, c);
        direction = WEST;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == RIGHT_NORMAL) {
        printf("%d,%d\n", r, c);
        direction = WEST;
    }
    else if(start_border(m, r, c, RIGHT_HAND) == ANOTHER_RIGHT) {
        printf("%d,%d\n", r, c);
        direction = SOUTH;
    }

    int q = 0;
    while (q < ITERATION_LIMIT) {
        if (r < 0 || c < 0 || r > m->rows || c > m->cols) {
            break;
        }
        if ((r+c)%2 == 1) {

            //4
            if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == SOUTH) {
                direction = WEST;
                direction_left(m, &r, &c, &direction);
            }

            //0
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {                    
                    direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction_left(m, &r, &c, &direction);
            }

            //1
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = EAST;
                direction_left(m, &r, &c, &direction);
            }
            //2
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = SOUTH;
                direction_left(m, &r, &c, &direction);
            }
            //3
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == SOUTH) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = SOUTH;
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = SOUTH;
                direction_left(m, &r, &c, &direction);
            }

            //5
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction = EAST;
                direction_left(m, &r, &c, &direction);
            }else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == SOUTH) {
                direction = EAST;
                direction_left(m, &r, &c, &direction);
            }

            //6
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction = WEST;
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == SOUTH) {
                direction = WEST;
               direction_left(m, &r, &c, &direction);
            }
        }

        else {
            //4
            if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == NORTH) {
                direction = EAST;
                direction_left(m, &r, &c, &direction);
            }

            // 0
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction_left(m, &r, &c, &direction);
            }

            //1
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = NORTH;
               direction_left(m, &r, &c, &direction);
            }

            //2
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = WEST;
                direction_left(m, &r, &c, &direction);
            }

            //3
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == NORTH) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == EAST) {
                direction = NORTH;
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == false && direction == WEST) {
                direction = NORTH;
               direction_left(m, &r, &c, &direction);
            }

            //5
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction = EAST;
               direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == false && isborder(m, r, c, LEFT) == true && isborder(m, r, c, UP_DOWN) == true && direction == NORTH) {
                direction = EAST;
               direction_left(m, &r, &c, &direction);
            }

            //6
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == WEST) {
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == EAST) {
                direction = WEST;
                direction_left(m, &r, &c, &direction);
            }
            else if (isborder(m, r, c, RIGHT) == true && isborder(m, r, c, LEFT) == false && isborder(m, r, c, UP_DOWN) == true && direction == NORTH) {
                direction = WEST;
                direction_left(m, &r, &c, &direction);
            }


        }
        q++;
    }
}

void print_help() {
    printf("_____________________________________________________________________________________\n");
    printf("\n\t\t\t>>> Available program arguments: <<<\n\n");
    printf("--help --> Prints this\n\n--test file.txt  --> tests if the file is a valid map\n\n");
    printf("--rpath R C file.txt --> starts going through the maze using right hand method, \nR and C specify the row and coll of entry and file specifies the maze\n\n");
    printf("--lpath R C file.txt --> starts going through the maze using left hand method, \nR and C specify the row and coll of entry and file specifies the maze\n\n");
    printf("--shortest R C file.txt --> Should find the shortest path through the maze, \nR and C specify the row and coll of entry and file specifies the maze\n");
    printf("_____________________________________________________________________________________\n");
}



int main(int argc, char *argv[]) {
    if (argc == 2) 
    {
        if (strcmp(argv[1], "--help") == 0) 
        {
          print_help();
          return 0;
        }
        else 
        {
            print_help();
            return 1;
        }
    }

    else if (argc == 3) {
        if (strcmp(argv[1], "--test") == 0) 
        {
            if(test(argv[2]) == 1)
            {
                printf("Invalid\n");
            }
            else {
                printf("Valid\n");
            }
        }
        else 
        {
            print_help();
            return 1;
        }
    }

    else if (argc == 5) {
        if ((strcmp(argv[1], "--rpath") == 0) & (isdigit(atoi(argv[2])) == 0) & (isdigit(atoi(argv[3])) == 0))
        {
            if(test(argv[4]) == 1) { printf("Invalid map\n"); return 1; }
            FILE *map_file;
            int rows;
            int cols;
            map_file = fopen(argv[4], "r");
            if(map_file == NULL) {fprintf(stderr, "Loading error"); return 1;}
            fscanf(map_file, "%d %d", &rows, &cols);
            fclose(map_file);
            Map m = map_ctor(rows, cols);
            if (m.cells == NULL) {
            fprintf(stderr, "Memory error");
            return 1;
            }
            map_init(&m, argv[4]);
            if (start_border(&m, atoi(argv[2]), atoi(argv[3]), RIGHT_HAND) == 1) {
            fprintf (stderr, "Invalid entry point\n");
            map_dtor(&m);
            return 1;
            }
            else {
                go_right(&m, atoi(argv[2]), atoi(argv[3]));
            }
           map_dtor(&m);
           return 0; 

        }
        else if ((strcmp(argv[1], "--lpath") == 0) & (isdigit(atoi(argv[2])) == 0) & (isdigit(atoi(argv[3])) == 0))
        {
           if(test(argv[4]) == 1) { printf("Invalid map\n"); return 1; }
            FILE *map_file;
            int rows;
            int cols;
            map_file = fopen(argv[4], "r");
            if(map_file == NULL) {fprintf(stderr, "Loading error"); return 1;}
            fscanf(map_file, "%d %d", &rows, &cols);
            fclose(map_file);
            Map m = map_ctor(rows, cols);
            if (m.cells == NULL) {
            fprintf(stderr, "Memory error");
            return 1;
            }
            map_init(&m, argv[4]);
            if ((start_border(&m, atoi(argv[2]), atoi(argv[3]), LEFT_HAND) == 1) || atoi(argv[2]) > rows || atoi(argv[3]) > cols) {
            fprintf (stderr, "Invalid entry point\n");
            map_dtor(&m);
            return 1;
            }
            else {
                go_left(&m, atoi(argv[2]), atoi(argv[3]));
            }
           map_dtor(&m);
           return 0; 
        }
        else if ((strcmp(argv[1], "--shortest") == 0) & (isdigit(atoi(argv[2])) == 0) & (isdigit(atoi(argv[3])) == 0))
        {
            printf("I haven't programmed that path yet\n");
        }
        else 
        {
            fprintf(stderr, "Invalid arguments\n");
            print_help();
            return 1;
        }
        }
    else 
    {
        fprintf(stderr, "Invalid number of arguments!\n");
        print_help();
        return 1;
    }


    return 0;
}