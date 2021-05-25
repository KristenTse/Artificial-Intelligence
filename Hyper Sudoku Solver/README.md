# Instructions
**Project Description**<br>
<p>Design and implement a program to solve Hyper Sudoku puzzles. Hyper Sudoku differs from the classic Sudoku in that four overlapping regions are defined in addition to the regular regions, as described below. The rules of the game are:

- The game board consists of 9 × 9 cells divided into 3 × 3 non-overlapping regions. Four additional overlapping regions are defined. The game board therefore contains 9 non-overlapping regions and 4 overlapping regions, with each region containing 3 × 3 cells. Some of the cells already have numbers (1 to 9) assigned to them initially.
- The goal is to find assignments (1 to 9) for the empty cells so that every row, column, nonoverlapping region and overlapping region contains all the digits from 1 to 9. Each of the 9 digits, therefore, can only appear once in every row, column, non-overlapping region and overlapping region.

As a first step in your program, apply Forward Checking to cells that already have a number assigned to them and reduce the domain of their neighbors. If any cell has an empty domain after applying Forward Checking, then the puzzle does not have a solution and the program can stop and exit. Here, you run the Forward Checking algorithm before you run the Backtracking Algorithm for CSPs. Next, use the Backtracking Algorithm to solve for a solution. Implement the function SELECT-UNASSIGNED-VARIABLE by using the minimum remaining value heuristic and then the degree heuristic. If there are more than one variables left after applying the two heuristics, you can arbitrarily choose a variable to work on next. You do not have to implement the least constraining value heuristic in the ORDER-DOMAIN-VALUES function; instead, simply order the domain values in increasing order, from 1 to 9. You do not have to implement the INFERENCE function inside the Backtracking Algorithm. *(Remember: two or more variables are neighbors if they share a common constraint.)*</p><br>

**File Format**<br>
<p>Your program will read in the initial game board configuration from an input text file and produce an output text file that contains the solution. The input file contains 9 rows (or lines) of integers. Each row contains 9 integers ranging from 0 to 9, separated by blank spaces. Digits 1-9 represent the cell values and 0’s represent blank cells.</p>

# Example Input
3 0 0 0 0 0 2 0 0<br>
0 0 2 5 0 0 0 0 0<br>
4 7 0 0 0 2 0 6 0<br>
0 0 0 0 0 1 0 5 9<br>
0 9 0 0 0 0 0 4 0<br>
0 1 0 0 0 0 0 0 0<br>
1 0 0 9 0 0 5 7 0<br>
0 3 0 0 0 8 0 0 0<br>
0 6 0 0 0 0 0 3 0<br>


# Example Output
3 5 1 7 9 6 2 8 4<br>
6 8 2 5 3 4 7 9 1<br>
4 7 9 1 8 2 3 6 5<br>
7 4 6 3 2 1 8 5 9<br>
2 9 3 8 6 5 1 4 7<br>
8 1 5 4 7 9 6 2 3<br>
1 2 8 9 4 3 5 7 6<br>
9 3 7 6 5 8 4 1 2<br>
5 6 4 2 1 7 9 3 8<br>