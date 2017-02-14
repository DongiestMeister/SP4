#include "AStar.h"
#include <iostream>

using std::cout;
using std::endl;

#define MIN_COST (float) 1.0		// Minimum Permissible Cost	For Heuristics	
#define ALPHA    (float) 0.5		// Scaling Factor For Heuristics

// Declare Structure Representing Neighborhood Of A Node
const struct { int x, y; } succ[4] = { { 0, -1 }, { 0, 1 }, { 1, 0 }, { -1, 0 } };

Node::Node() :parent(nullptr), f(0.0), g(0.0), h(0.0) {} // Initialised Values


AStar::AStar(int sx, int sy, int gx, int gy, TileMap *map)
{
	start = new Node;									// Create Node Objects & Allocate Memory
	goal = new Node;
	start->x = sx; start->y = sy; goal->x = gx; goal->y = gy;		// Set Start and Goal x-y Values
	cout << "Initialised start...";
	start->h = Compute_h(start);
	start->f = start->g + start->h;
	cout << "Initialised goal....";
	cout << endl;
	this->grid = map->theScreenMap;
	this->ROWS = map->numTilesHeight;
	this->COLS = map->numTilesWidth;
}

// Get Best ( Minimum f ) Node From Open List
Node* AStar::GetBest()
{
	int best = 0;										// Initialise Best (Minimum f) Index=0 For Start Of Comparison
	for (int i = 1; i < (int)openList.size(); i++) {
		if (openList[i]->f < openList[best]->f) {		// Comparing ..
			best = i;									// Get Index With Lower 'f' Value
		}
	}
	return openList[best];								// return the best node
}

// Get Neighborhood Nodes ( By Index ) Of Current Node
Node* AStar::GetSuccessor(Node *current, int i)
{
	Node *n = NULL;										// Null Initialisation
	int x = current->x + succ[i].x;						// Get x,y Position By Index
	int y = current->y + succ[i].y;
	if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
	{
		if (grid[y][x] == 0) {								// If Grid Element Contains Empty Space
			n = new Node;									// Create A Node Object
			n->x = x;										// Initialise To x-y Value Of Successor
			n->y = y;
		}
	}

	return n;											// Return Successor Node
}

// Calculate Start To Current Node 'n' Cost
// Formula Below Reflects Scalability According To Cost Behavior
// ALPHA = Scaling Factor 
float AStar::Compute_g(Node* n){
	float tempG;
	if (n->parent != NULL) tempG = n->parent->g;
	else tempG = n->g;
	return (float)(1.0 + ALPHA * (tempG - 1.0));
}

// Calculate Cost From Start 'n' to Goal Node Using 'Manhattan' Distance Formula
float AStar::Compute_h(Node* n)
{
	return  (float)(MIN_COST*(abs((float)n->x - (float)goal->x) + abs((float)n->y - (float)goal->y)));
}

// Check Whether Node 'n' Is In List
bool AStar::InList(vector <Node*> list, Node *n)
{
	for (int i = 0; i<(int)list.size(); i++)
	{
		if ((list[i]->x == n->x) && (list[i]->y == n->y))
		{
			return  true;			// If Found In List
		}
	}
	return false;					// If Not In List
}

// Get From Open List Item As Specified In 'succ' Successor
Node* AStar::getFromOpenList(Node* succ)
{
	Node *n;
	for (int i = 0; i<(int)openList.size(); i++)	// Loop Through List		
	{
		if ((openList[i]->x == succ->x) && (openList[i]->y == succ->y))	// Have A Match ...
		{
			n = openList[i];													// Pick The Node
			break;
		}
	}
	return n;
}
// Get From Close List Item As Sspecified In 'succ' Successor
Node* AStar::getFromCloseList(Node* succ)
{
	Node *n;
	for (int i = 0; i<(int)closeList.size(); i++)  // Loop Through List
	{
		if ((closeList[i]->x == succ->x) && (closeList[i]->y == succ->y)) // Have A Match ...
		{
			n = closeList[i];											   // Pick The Node
			break;
		}
	}
	return n;
}

// Search For Best Path ( Minimum Cost )
bool AStar::Search()
{
	Node *temp;
	cout << "Searching....\n\n";
	AddOpenList(start);									// Add Start Node To Open List
	while ((int)openList.size() != 0)					// Checking If Open List Is Empty
	{
		Node *n = GetBest();							// Get Best Node With Minimum 'f' Value
		//cin.get();									// UNCOMMENT TO SEE INTERMEDIATE RESULTS		
		if ((n->x == goal->x) && (n->y == goal->y))			// If Current Node 'n' Matches Goal Node in x,y Values
		{												// ie: Reached Goal
			ShowPath(n);								// Show Path In Text Mode & Return True(Found For Search)

			Node* temp = goal;
			vector<Node*> tempList;
			while (temp != nullptr)
			{
				tempList.push_back(temp);
				temp = temp->parent;
			}

			for (int i = tempList.size() - 1; i >= 0; --i)
			{
				bestPath.push_back(tempList[i]);
			}
			

			return true;
		}
		else											// Goal Not Reached Yet 
		{
			RemoveOpenList(n);							// Remove Node From Open List Since It Has Been Examined
			AddCloseList(n);							// Add To Examined (Close) List
			Node *successor;							// Define Successor Node
			// Define Temporary Node
			for (int i = 0; i < 4; i++) {				// Loop Through 4 Neighborhood Nodes By Their Index
				successor = GetSuccessor(n, i);			// Get Successor Node By Index
				if (successor != NULL)						// If A Valid Space For A Successor Node
				{
					successor->parent = n;						// Link Back To Parent/Current Node
					successor->g = Compute_g(successor);		// Calculate 'g' Cost Value
					successor->h = Compute_h(successor);		// Calculate 'h' Cost Value
					successor->f = successor->g + successor->h;   // Calculate 'f' Cost Value
					cout << "Getting successor ....";
					if (InList(openList, successor))						// If Proposed Succesor Exists In Open List..
					{
						temp = getFromOpenList(successor);				// Get The Node Concerned
						if (temp->f > successor->f)						// Check If Successor's 'f' Value Is Lower
						{
							RemoveOpenList(temp);						// If lower, Remove Existing Node From Open List
							AddOpenList(successor);						// Add Successor To Open List Instead
						}
					}
					else
					if (InList(closeList, successor))						// If Proposed Succesor Exists In Close List..
					{
						temp = getFromCloseList(successor);				// Get The Node Concerned
						if (temp->f > successor->f)						// Check If Successor's 'f' Value Is Lower
						{
							RemoveCloseList(temp);						// Remove From Close List
							AddCloseList(successor);					// Add Successor To Close List
						}
					}
					else												// if Not In Either Open Or Close List ie : New ...
					{
						AddOpenList(successor);						// Add Successor To Open List
					}
				}
			}
		}
	}
	return false;					// Return False(Not Found)

}

// Show In Text Mode The Grid Map & The Best Path Worked Backwards From Goal Node
// 'walker' Is A Node Traversing Pointer
void AStar::ShowPath(Node *walker)
{
	cout << "\nBEST PATH SOLUTION";
	grid[goal->y][goal->x] = 'G';		// Mark "Goal" Node On Grid Array
	walker = walker->parent;				// Get Node On Best Path Linked To Goal 
	while (walker->parent != NULL)		// If Start Point IS Not NULL
	{
		grid[walker->y][walker->x] = '*';	// Grid Map Node Is Marked As Best Path Node
		walker = walker->parent;		// Go To Next Link To The Path
	}
	grid[start->y][start->x] = 'S';		// Mark "Start" Node On Grid Array

	cout << "\n";
	for (int y = 0; y<ROWS; y++)			// Loop Through 2D Array To Show Map & Path
	{
		for (int x = 0; x<COLS; x++)
		{
			cout << (char)grid[y][x];
		}
		cout << "\n";
	}
	cout << "\n";
}

// Add To Open List, Show Its Content
void AStar::AddOpenList(Node *n) {
	openList.push_back(n);	// Add To Open List
	//cout << "Add to open list...\nOpen list content :\n";
	ShowList(openList);		// Show List Content
	cout << endl;
}
// Add To Close List, Show Its Content
void AStar::AddCloseList(Node *n){
	closeList.push_back(n);	// Add To Close List
	//cout << "Add to close list...\nClose list content :\n";
	ShowList(closeList);	// Show List Content
	cout << endl;
}

// Remove Node From Open List After It Is Examined For Minimum 'f' Value
void AStar::RemoveOpenList(Node *n) {
	for (int i = 0; i<(int)openList.size(); i++)	// Go Through Open List Items
	{
		if ((openList[i]->x == n->x) && (openList[i]->y == n->y)) // If Found The Item 
		{
			//cout << "Removing from open list.. ";
			openList.erase(openList.begin() + i, openList.begin() + i + 1);	// Erase Item From List	
		}
	}
	//cout << "Open List content :\n";									// Show Updated Open List Content
	ShowList(openList);
	//cout << endl;
}

// Remove Node From Close List After It Is Examined For Minimum 'f' Value
void AStar::RemoveCloseList(Node *n) {
	for (int i = 0; i<(int)closeList.size(); i++)	// Go Through Close List Items
	{
		if ((closeList[i]->x == n->x) && (closeList[i]->y == n->y)) // If Found The Item 
		{
			//cout << "Removing from close list.. ";
			closeList.erase(closeList.begin() + i, closeList.begin() + i + 1);	// Erase Item From List	
		}
	}
	//cout << "Close List content :\n";
	ShowList(closeList);												// Show Updated Close List Content
	//cout << endl;
}

// Show Open or Close  List Content
void AStar::ShowList(vector <Node*> list)
{
	//for (int i = 0; i<(int)list.size(); i++)
	//{
	//}
}