#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "solver.h"

void main_solve(int grid[9][9], int result[9][9])
{ 
	//deep copy
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			result[i][j] = grid[i][j];
	}
	
    Solve(result,9);
    grid_to_file(result,9);
}

/* SolveRec : solve sudoku
 * param : array[9][9] -> grid , int -> len of a side of the grid
 * return type : int
 */
 
 
 
int Solve(int grid[9][9], int len)
{
    int x;
    int y;
    	
	int i = 0;
    int j = 0;

    while (grid[i][j] != 0)
    {
        if (j == 8)
        {
            i++;
            j = 0;
        }
        else
            j++;
        
        
        if (i > 8)
            return 1;
        
    }
    
    x = i;
    y = j;

    for (int value = 1; value < len+1; value++)
    {
        if (IsBoardValid(grid,len,x,y,value))
        {
         	grid[x][y] = value;
            
		    if (Solve(grid,len))
	        {
	        	return 1;
	        }
	        
	        grid[x][y] = 0;
        }
    }

    return 0;
}



/* IsBoardValid : check if the value can be insert in grid[x][y]
 * param : array[9][9] -> grid , int -> len of a side of the grid,
 	   int -> actual coordinate x in the grid, int -> ...... y .....,
 	   int -> value to insert in grid[x][y]
 * return type : int
 */

int IsBoardValid(int grid[9][9], int len, int x, int y, int value)
{
    int SquareX = x / 3 * 3;
    int SquareY = y / 3 * 3;

    for (int n = 0; n < len; n++)
    {
        if (grid[x][n] == value)
            return 0;
            
        if (grid[n][y] == value)
            return 0;
            
        if (grid[SquareX + (n % 3)][SquareY + (n / 3)] == value)
            return 0;
    }
    
    return 1;
}



/* print : save the grid into a file .result
 * param : char[] filename -> name of the file, array[9][9] -> grid,
 		   int -> len of a side of the grid
 * return type : void
 */

void grid_to_file(int grid[9][9], int len)
{
	char newfilename[] = "temp_files/grid.result";

	FILE *f;
    f = fopen(newfilename, "w");
    
    if (f == NULL)
    {
    	errx(1, "Error creating file %s", newfilename);
    }
   
	for (int i = 0; i < len; i++)
	{
	
		for (int j = 0; j < len; j++)
		{
			if (j == 2 || j  == 5)
				fprintf(f,"%d ",grid[i][j]);
			else
				fprintf(f,"%d",grid[i][j]);
		}
		
		fprintf(f,"\n");
		if (i == 2 || i == 5)
			fprintf(f,"\n");
	}
	
	fclose(f);
}



/* print : load the grid of a file and convert to mutidimentiona array
 * param : char[] filename -> name of the file, array[9][9] -> grid,
 		   int -> len of a side of the grid
 * return type : void
 */
 
void load_grid(char filename[], int grid[9][9])
{
    FILE *f;
    
    f = fopen(filename, "r");
    
    if (f == NULL)
    {
    	errx(1, "Error loading file %s ", filename);
    }
    
    int i = 0;
    int j = 0;
    int backslash = 0;
    
    char c;
    
    while ((c = fgetc(f)) != EOF)
    {
        if (c != ' ')
        {
            if (c == '\n')
            {
            	if (backslash != 3 && backslash != 7)
            	{
		            j = 0;
		            i++;
		        }
		        backslash++;
            }
            else if (c == '.')
            {
                grid[i][j] = 0;
                j++;
            }
            else
            {
                grid[i][j] = c - '0';
                j++;
            }
        }
    }
    
    fclose(f);
}



/* print : print the grid
 * param : array[9][9] -> grid
 * return type : void
 */

void print(int grid[9][9],int len)
{
    for (int x = 0; x < len; x++)
    {
       printf("\n");
       if (x != 0 && x % 3 == 0)
           printf("\n");
           
       for (int y = 0; y < len; y++)
       {
       	   if (y != 0 && y % 3 == 0)
       	       printf(" ");
           printf(" %i ",grid[x][y]);
       }
    }
    
    printf("\n");
}
