#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


// Defines a Cell object. The sudoku board consists of these cells.
struct Cell {
	int value, col, row;
	int numDomains, degree;
};


// opens a stream after getting the input file name from the user. If the file cannot be opened, an error 
// message is displayed.
void openStream(ifstream& ifs) {
	do {
		string fileName;
		cout << "Enter the name of the input file: ";
		cin >> fileName;
		ifs.open(fileName);
		if (!ifs) {
			cerr << "Could not open the file." << endl;
			ifs.clear();
		}
	} while (!ifs.is_open());
}


// Checks if the given number can be legally assigned to the given cell. Returns true if possible,
// false if otherwise. This function also keeps track of the degree (number of unassigned neighbors).
// However it is only accurate when degree has been reset to 0 and num == 0. This feature is only used 
// in findNextLocation().
bool canAssign(Cell(&board)[9][9], const int num, const int row, const int col) {
	int i, j;
	//check row and col neighbors
	for (i = 0; i < 9; i++) {
		if (board[i][col].value == num) { board[row][col].degree++; return false; }
		if (board[row][i].value == num) { board[row][col].degree++; return false; }
	}
	//check square neighbors
	int square_row = row - (row % 3);
	int square_col = col - (col % 3);
	for (i = square_row; i < square_row + 3; i++) {
		for (j = square_col; j < square_col + 3; j++) {
			if (board[i][j].value == num) {
				board[row][col].degree++;
				return false;
			}
		}
	}
	//check hyper square neighbors
	if (row != 0 && row != 4 && row != 8 && col != 0 && col != 4 && col != 8) {
		if (0 < row && row < 4) { square_row = 1; }
		else if (4 < row && row < 8) { square_row = 5; }
		if (0 < col && col < 4) { square_col = 1; }
		else if (4 < col && col < 8) { square_col = 5; }
		for (i = square_row; i < square_row + 3; i++) {
			for (j = square_col; j < square_col + 3; j++) {
				if (board[i][j].value == num) {
					board[row][col].degree++;
					return false;
				}
			}
		}
	}
	return true;
}


// Returns an array of all the possible domain values for the given cell. Also updates the number
// of legal domain values left for the given cell.
bool* getDomains(Cell(&board)[9][9], const int row, const int col) {
	int counter = 9;
	bool domains[9] = { true, true, true, true, true, true, true, true, true };
	for (int i = 1; i <= 9; i++) {
		if (!canAssign(board, i, row, col)) {
			domains[i - 1] = false;
			counter--;
		}
	}
	board[row][col].numDomains = counter;
	return domains;
}


// Finds the next variable based on MRV and Degree heuristics. Returns true if a new cell can be chosen,
// false if otherwise. If false, the puzzle should be solved.
bool findNextLocation(Cell(&board)[9][9], int& row, int& col) {
	vector<Cell*> MRV;
	Cell* minCell = nullptr;
	int numUnassigned = 0;

	// MRV Heuristic
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) { //iterate thorugh the whole board
			if (board[i][j].value == 0) { //found an unassigned cell
				numUnassigned++;
				getDomains(board, row, col);
				if (minCell == nullptr || board[i][j].numDomains == minCell->numDomains) {
					minCell = &board[i][j];
					MRV.push_back(minCell);
				}
				else if (board[i][j].numDomains < minCell->numDomains) { //found a better cell
					while (!MRV.empty()) { MRV.pop_back(); }
					minCell = &board[i][j];
					MRV.push_back(minCell);
				}
			}
		}
	}
	if (numUnassigned == 0) { return false; }

	// Degree Heuristic
	minCell = MRV[0];
	for (size_t i = 0; i < MRV.size(); i++) { //of the remaining cells, chose the one with less
		MRV[i]->degree = 0;					  //unassigned neighbors
		canAssign(board, 0, MRV[i]->row, MRV[i]->col);
		if (MRV[i]->degree > minCell->degree) { minCell = MRV[i]; }
	}
	row = minCell->row;
	col = minCell->col;
	return true;
}


// Performs backtracking. The puzzle is solved once no new unassigned cells are found. While the puzzle
// is unsolved, recursive calls are made on the various domain values of the chosen cell.
bool backtracking(Cell(&board)[9][9]) {
	int row, col;
	if (findNextLocation(board, row, col) == false) { //sodoku is solved
		return true;
	}
	for (int i = 1; i <= 9; i++) {
		if (canAssign(board, i, row, col)) {
			board[row][col].value = i;
			if (backtracking(board)) { //successfully changed value
				return true;
			}
			board[row][col].value = 0; //failed, so make the cell unassigned again
		}
	}
	return false;
}


// Performs forward checking on the cells with assigned values. If a cell has a 0 legal domain values
// left, no solution exists and false is returned. Otherwise true is returned.
bool forwardChecking(Cell(&board)[9][9]) {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (board[i][j].value != 0) {
				getDomains(board, i, j);
				if (board[i][j].numDomains == 0) {
					return false;
				}
			}
		}
	}
	return true;
}


int main() {
	//generate an output file
	ofstream outputFile("output.txt");

	//open the input file
	ifstream inputFile;
	openStream(inputFile);

	//initialize board as a 2D array of Cells
	Cell board[9][9];
	int num, i = 0, j = 0;
	while (inputFile >> num) {
		board[i][j].value = num;
		board[i][j].row = i;
		board[i][j].col = j;
		j++;
		if (j == 9) { j = 0; i++; }
	}

	//start by forward checking
	forwardChecking(board);

	//begin backtracking
	if (backtracking(board)) {
		//write the solution to the output file
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				outputFile << board[i][j].value << " ";
				if (j == 8) {
					outputFile << "\n";
				}
			}
		}
	}
	else {
		outputFile << "Couldn't find a solution";
	}

	//close the opened files
	inputFile.close();
	outputFile.close();
}