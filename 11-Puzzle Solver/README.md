# Instructions
**Project Description**<br>
<p>Implement the A* algorithm with graph search for solving the 11-puzzle problem as described below. Use sum of Manhattan distances of tiles from their goal positions as heuristic function.</p><br>

**11-Puzzle Problem**<br>
<p>On a 3 x 4 board there are 11 tiles numbered from 1 to 11 and a blank position. A tile can slide into the blank position if it is horizontally or vertically adjacent to the blank position. Given a start board configuration and a goal board configuration, find a move sequence with a minimum number of moves to reach the goal configuration from the start configuration. (Note: the 3 x 4 board has 3 rows and 4 columns.)</p><br>

**File Format**<br>
<p>Your program will read in the initial and goal states from a text file that contains 7 lines as follows:
- Lines 1 to 3 contain the tile pattern for the initial state
- Line 4 is a blank line.
- Lines 5 to 7 contain the tile pattern for the goal state

Integer 0 represents the blank position and integers 1 to 11 represent tile numbers. Your program will produce an output text file that contains 12 lines as follows:
- Lines 1 to 3 contain the tile pattern for the initial state as given in the input file
- Lines 5 to 7 contain the tile pattern for the goal state as given in the input file
- Lines 4 and 8 are blank lines.
- Line 9 is the depth level of the shallowest goal node as found by your search algorithm (assume the root node is at level 0).
- Line 10 is the total number of nodes N generated in your tree (including the root node.) 
- Line 11 contains the solution that you have found. 
  - The solution is a sequence of actions (from root node to goal node) separated by blanks. 
  - Each A is a character from the set {L, R, U, D}, representing the left, right, up and down movements of the blank position.
  - There should be d number of A values
- Line 12 contains the *f(n)* values of the nodes along the solution path from the root node to the goal node, separated by blanks. 
  - There should be *d*+1 number of *f* values
</p>

# Example Input Format
n n n n<br>
n n n n<br>
n n n n<br>

m m m m<br>
m m m m<br>
m m m m<br>

# Example Output Format
n n n n<br>
n n n n<br>
n n n n<br>

m m m m<br>
m m m m<br>
m m m m<br>

d<br>
N<br>
A A A A A A A ……………<br>
f f f f f f f f ……………