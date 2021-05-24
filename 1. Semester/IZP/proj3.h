/**
* \author   Vojtech Fiala ... xfiala61
* \version  1.0
* \date     22.12.2019
*/

#ifndef PROJ3_H
#define PROJ3_H

/**
* \brief A structure to store the number of rows and cols and the cell values of a map.
*/
typedef struct {
    int rows;   /**< rows is used to store the number of rows in the map. */
    int cols;   /**< cols is used to store the number of cols in the map. */
    unsigned char *cells;   /**< cells is used to store the value of cells in the map. */
} Map;

/**
* \brief A function used to define the bit values of the map cell borders.
*
* \param BLEFT describes the left border, which has the value of the lowest bit.
* \param BRIGHT describes the right border, which has the value of the second lowest bit.
* \param BTOP and BBOTTOM describe the value of top and bottom borders, depending of the cell orientation, and
* have the value of the third lowest bit.
*/

enum borders { BLEFT=0x1, BRIGHT=0x2, BTOP=0x4, BBOTTOM=0x4 };
/**
* \brief A function to deallocate the cells of the map.
*
* A function used to free the allocated map cells from memory.
* \pre The map has been allocated into memory.
* \post The map has been deallocated from the memory.
* \param map A pointer to the map structure that is going to be deallocated.
*/

void free_map(Map *map);
/**
* \brief A function to load the map into memory.
*
* A function to load the number of rows and cols of the map and
* to allocate memory to the map which will be used in the program.
* \pre The map does not yet exist.
* \post The map has been allocated into memory and rows and cols have been given values.
* \param filename A pointer to the file from which the map will be loaded.
* \param map A pointer to the map structure, into which the values will be loaded.
* \return A value of zero will be returned if the loading was a success and a value of one
* will be returned if the loading ended in error.
*/

int load_map(const char *filename, Map *map);
/**
* \brief A function to tell us if there is a border on a given cell of the map.
*
* A function which will take the coordinates given to it and the border of which existence we want to know
* and tell us, if the border on those coordinates exists or not. This information will be used to go through the map.
* \param map A pointer to the map structure, from which we will get the value of the cell and from the value of the cell, we will know if there are some borders or not.
* \param r A row of the cell in which the border will be proved to exist or not.
* \param c A col of the cell in which the border will be proved to exist or not.
* \param border A side of the border which's existence we want to find out.
* \return "True" will be returned if the border exists, "false" if it does not.
*/

bool isborder(Map *map, int r, int c, int border);
/**
* \brief A function to tell us if the border has bottom border.
*
* A function which will calculate if the cell is oriented upward or downward, depending
* on the r+c values. If (r+c)%2 == 1, the cell is an upward triangle, else it points downward.
* \param r A row of the cell which's orientation we want to find out.
* \param c A col of the cell which's orientation we want to find out.
* \return "True" if the cell has a bottom border, "False" if it has a top border.
*/

bool hasbottom(int r, int c);
/**
* \brief A function to give us the border we want to follow first.
*
* A function which will give us the border at the specified entrance coordinates that we will follow.
* Depending if we are going through the maze using the right of left hand method, the followed border will change.
* \param map A pointer to the map structure, which will be used to check if the given coordinates are correct.
* \param r A row of the cell where we want to enter.
* \param  A col of the cell where we want to enter.
* \param leftright A value to tell the program which method to use.
* \return The border we want to follow after entering the maze.
*/
int start_border(Map *map, int r, int c, int leftright);

/**
* \brief A function to check if the map is valid.
*
* A function which will check if the cell values in the map are valid.
* For example, the map must contain only numbers, no letters can be present.
* Neighboring borders must have matching border values. For example, a cell with a bottom border
* needs to have a cell under it with a top border. Otherwise, the map is not valid.
* \param map A pointer to the map structure, which's cells will be tested.
* \return Return 0 if the map is valid, 1 if it is not.
*/
int check_map(Map *map);

/**
* \brief A function to load the map and check if the map is valid.
*
* A function that will load the map from a file into memory and check if the cell values of that map
* are valid.
* For example, the map must contain only numbers, no letters can be present.
* Neighboring borders must have matching border values. For example, A cell with a bottom border
* needs to have a cell under it with a top border. Otherwise, the map is not valid.
* \param filename A pointer to the file from which the map will be loaded.
* \param map A pointer to the map structure, which's cells will be tested.
* \return Return 0 if the map was loaded successfully and is valid, 1 if it is not or the loading ended in error.
*/
int load_and_check_map(const char *filename, Map *map);

/**
* \brief A function to check if we are still in the map.
*
* This function will check if we are still within the map. If we have left
* the map, it will tell us. This will be used to check if the map printing should stop.
* \param map A pointer to the map structure, which's cells locations we want check.
* \param r A row of the cell of which we want to find out if it's located outside the map.
* \param c A col of the cell of which we want to find out if it's located outside the map.
* \return Return "True" if the given values are out of the map. "False" if we are still inside.
*/
bool is_out(Map *map, int r, int c);

/**
* \brief A function to print the path to exit.
*
* This function will print all the way from the entrance to the exit. Depending on the
* leftright value, it will use either a left or right hand method to go through the map of the maze.
* The program will stop after it has left the maze.
* \param map A pointer to the map structure, which's cells locations we want check.
* \param r A row of the cell from which we will enter.
* \param c A col of the cell from which we will enter.
* \param leftright A value to tell the program if it should use the right or left hand method.
*/
void print_path(Map *map, int r, int c, int leftright);

#endif
