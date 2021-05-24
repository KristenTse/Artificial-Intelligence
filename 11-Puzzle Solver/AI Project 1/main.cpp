#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;


// defines a node object, which contains a vector representation of the board state, as well as various other 
// variables. It also contains an assignment operator function which compares two nodes by checking if their 
// board states are the same.
struct Node {
	bool operator==(const Node& other) {
		return boardVec == other.boardVec;
	}
	int fn, hn, emptyTilePos;
	int	depth = 0;
	const Node* parent = nullptr;
	vector<int> boardVec;
};


// defines a Priority Queue, where the nodes are pointers to board states. This representation is similar to
// a linked list, with header and trailer nodes added to keep track of the beginning and end of the queue. 
// Priority is assigned based off f(n) value.
class PriorityQueue {
	struct PQNode;

public:
	PriorityQueue() : size(0) {
		header = new PQNode();
		trailer = new PQNode();
		header->next = trailer;
		trailer->prev = header;
	}

	PQNode* newNode(Node* node) {
		PQNode* temp = new PQNode();
		temp->value = node;
		return temp;
	}

	// this function adds a board state to the queue by initializing a new priority queue node, and then 
	// reassigning next and prev pointers. If the node-to-be-inserted has the same value as another node in 
	// the queue, it is added as close to the end of the queue as possible.
	void push(Node* node) { 
		PQNode* pqNode = newNode(node);
		if (size == 0) {
			size++;
			header->next = pqNode;
			pqNode->prev = header;
			trailer->prev = pqNode;
			pqNode->next = trailer;
			return;
		}
		PQNode* curr = header->next;
		while (curr->next != nullptr) {
			if (pqNode->value->fn < curr->value->fn) { break; }
			curr = curr->next;
		}
		curr->prev->next = pqNode;
		pqNode->prev = curr->prev;
		pqNode->next = curr;
		curr->prev = pqNode;
		size++;
	}

	bool isEmpty() { return size == 0; }

	// this function pops the board state node with the highest f(n) value, and returns a pointer to that node.
	Node* pop() {
		if (!isEmpty()) {
			size--;
			PQNode* toBePopped = header->next;
			Node* result = toBePopped->value;
			header->next->next->prev = header;
			header->next = header->next->next;
			delete toBePopped;
			return result;
		}
	}

private:
	struct PQNode {
		PQNode* next = nullptr;
		PQNode* prev = nullptr;
		Node* value;
	};
	PQNode* header;
	PQNode* trailer;
	int size;
};


// opens a stream after getting the file name from the user. If the file cannot be opened, an error message is
// displayed.
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


// Calculates the h(n) value of a given node. h(n) is the sum of Manhattan distances of tiles from their goal 
// positions. This is caluclated by adding the vertical and horizontal distances each tile is from their goal 
// position. Every action costs 1, so g(n) is equivalent to the depth of the given node.
int calcHn(Node* currBoard, Node& goalBoard) {
	int result = 0, currRow = 0, currCol = 0, goalRow = 0, goalCol = 0;
	for (int i = 0; i < currBoard->boardVec.size(); i++) {
		if (currBoard->boardVec[i] != goalBoard.boardVec[i]) { //if a tile is not in the goal position
			currRow = i / 4;
			currCol = i % 4;
			for (int j = 0; i < goalBoard.boardVec.size(); j++) {
				if (currBoard->boardVec[i] == goalBoard.boardVec[j]) {
					goalRow = j / 4;
					goalCol = j % 4;
					break;
				}
			}
			result += abs(currRow - goalRow);
			result += abs(currCol - goalCol);
		}
	}
	return result;
}


// Calculates the f(n) value of a given node. f(n) = h(n) + g(n), where g(n) is the cost of an action.
// All actions in this example have a cost of 1, which can be measured by the depth of the node.
int calcFn(Node* currBoard, Node& goalBoard) {
	return calcHn(currBoard, goalBoard) + currBoard->depth;
}
 

// Checks if the given board has already been seen before. The second parameter is a vector containing all the
// nodes in the tree currently.
bool isExistingState(Node* node, vector<Node*> vecNodes) {
	for (int i = 0; i < vecNodes.size(); i++) {
		if (*node == *vecNodes[i]) {
			return true;
		}
	}
	return false;
}


// updates the Node object variables
void updateVals(const Node* currNode, Node* temp, Node goalNode, vector<Node*>& seenStates) {
	temp->parent = currNode;
	temp->depth = currNode->depth;
	temp->depth++;
	temp->hn = calcHn(temp, goalNode);
	temp->fn = calcFn(temp, goalNode);
	seenStates.push_back(temp);
}


// Displays the state of the board. For debugging purposes.
void printBoard(vector<int> board) {
	for (int i = 0; i < board.size(); i++) {
		cout << board[i] << " ";
		if (i % 4 == 3) {
			cout << "\n";
		}
	}
}



int main() {
	//generate an output file
	ofstream outputFile("output.txt");

	//open the input file
	ifstream inputFile;
	openStream(inputFile);

	//initialize variables
	PriorityQueue pqueue;
	vector<Node*> seenStates; //all the nodes in the tree are store here
	Node* temp;
	Node* initNode = new Node();
	const Node* currNode = nullptr;
	Node goalNode;
	int currentDepth = 0;

	//initialize starting board by reading the first 12 numbers in the input file
	int num, counter = 0;
	while (inputFile >> num) {
		initNode->boardVec.push_back(num);
		counter++;
		if (counter == 12) { break; }
	}
	seenStates.push_back(initNode);

	//write the starting board to the output file
	for (int i = 0; i < initNode->boardVec.size(); i++) {
		outputFile << initNode->boardVec[i] << " ";
		if (i % 4 == 3) {
			outputFile << "\n";
		}
	}
	outputFile << "\n";

	//initialize goal board by reading the next 12 numbers in the input file
	counter = 0;
	while (inputFile >> num) {
		goalNode.boardVec.push_back(num);
		counter++;
		if (counter == 12) { break; }
	}

	//write the goal board to the output file
	for (int i = 0; i < goalNode.boardVec.size(); i++) {
		outputFile << goalNode.boardVec[i] << " ";
		if (i % 4 == 3) {
			outputFile << "\n";
		}
	}
	outputFile << "\n";
	
	//calculate initial f(n) and h(n) values
	initNode->hn = calcHn(initNode, goalNode);
	initNode->fn = calcFn(initNode, goalNode);

	//look for the empty tile position
	int pos;
	for (pos = 0; pos < initNode->boardVec.size(); pos++) {
		if (initNode->boardVec[pos] == 0) {
			initNode->emptyTilePos = pos;
			break;
		}
	}

	//push the first node into the priority queue
	pqueue.push(initNode);

	//continue growing the tree, while also adding nodes to the priority queue and popping when necessary.
	while (!pqueue.isEmpty()) {
		currNode = pqueue.pop(); //pop and set the current node to the node with the highest priority

		if (currNode->hn == 0) { break; } //if we've reached the goal state, break out of the loop
		pos = currNode->emptyTilePos;

		if (pos % 4 != 0) { //ensuring that we dont move the tile left if it is already in the leftmost position
			temp = new Node();
			temp->boardVec = currNode->boardVec;
			swap(temp->boardVec[pos], temp->boardVec[pos - 1]); //shift empty tile to the left
			if (!isExistingState(temp, seenStates)) {
				updateVals(currNode, temp, goalNode, seenStates);
				temp->emptyTilePos = pos - 1;
				pqueue.push(temp);
			}
		}

		if (pos % 4 != 3) { //ensuring that we dont move the tile right if it is already in the rightmost position
			temp = new Node();
			temp->boardVec = currNode->boardVec;
			swap(temp->boardVec[pos], temp->boardVec[pos + 1]); //shift empty tile to the right
			if (!isExistingState(temp, seenStates)) {
				updateVals(currNode, temp, goalNode, seenStates);
				temp->emptyTilePos = pos + 1;
				pqueue.push(temp);
			}
		}

		if (pos / 4 != 0) { //ensuring that we dont move the tile up if it is already in the upmost position
			temp = new Node();
			temp->boardVec = currNode->boardVec;
			swap(temp->boardVec[pos], temp->boardVec[pos - 4]); //shift empty tile up
			if (!isExistingState(temp, seenStates)) {
				updateVals(currNode, temp, goalNode, seenStates);
				temp->emptyTilePos = pos - 4;
				pqueue.push(temp);
			}
		}

		if (pos / 4 != 2) { //ensuring that we dont move the tile down if it is already in the downmost position
			temp = new Node();
			temp->boardVec = currNode->boardVec;
			swap(temp->boardVec[pos], temp->boardVec[pos + 4]); //shift empty tile down
			if (!isExistingState(temp, seenStates)) {
				updateVals(currNode, temp, goalNode, seenStates);
				temp->emptyTilePos = pos + 4;
				pqueue.push(temp);
			}
		}
	}
	
	//write the shallowest goal node depth to the output file.
	outputFile << currNode->depth << "\n";

	//write the number of notes generated to the output file.
	outputFile << seenStates.size() << "\n";

	//create strings representing the action sequence and f(n) values.
	string actions = "";
	string  fnVals = "";

	//determine the f(n) values and sequence of actions starting at the goal node, and traveling up to the 
	//root node
	const Node* prevNode;
	while (currNode->parent != nullptr) {
		fnVals = to_string(currNode->fn) + " " + fnVals; //append the current f(n) to the front of fnVals
		prevNode = currNode->parent;
		if (prevNode->emptyTilePos - currNode->emptyTilePos == -1) { 
			actions = "R " + actions;
		}
		else if (prevNode->emptyTilePos - currNode->emptyTilePos == 1) {
			actions = "L " + actions;
		}
		else if (prevNode->emptyTilePos - currNode->emptyTilePos == -4) {
			actions = "D " + actions;
		}
		else if (prevNode->emptyTilePos - currNode->emptyTilePos == 4) {
			actions = "U " + actions;
		}
		currNode = currNode->parent;
	}
	fnVals = to_string(currNode->fn) + " " + fnVals; //add the f(n) value of the root to the fnVals string.
	
	//write the action sequence and f(n) values to the output file
	outputFile << actions << "\n";
	outputFile << fnVals;

	//delete all the memory on the heap to prevent memory leak
	for (int i = 0; i < seenStates.size(); i++) {
		delete seenStates[i];
	}

	//close the opened files
	inputFile.close();
	outputFile.close();
}
