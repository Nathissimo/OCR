#pragma once

void main_solve(int grid[9][9], int result[9][9]);
int IsBoardValid(int grid[9][9],int len, int x, int y, int value);
int Solve(int grid[9][9],int len);
void print(int grid[9][9],int len);
void grid_to_file(int grid[9][9], int len);
void load_grid(char filename[], int grid[9][9]);
