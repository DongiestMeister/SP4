#ifndef ASTAR_H
#define ASTAR_H
#include "../TileMap.h"

#include <vector>
#include "Vector2.h"
using std::vector;

class Node
{
public:
	Node();
	Node *parent;	// Link To Previous Node On Best Path
	int x, y;		// x,y Position Of Grid
	float f, g, h;	// Cost Elements : g = Cost From Start To Current Node
	//                 h = Cost From Current To Goal Node
	//				   f = Cost From Start To Goal (g+h) Node
};


class AStar
{
public:
	vector<vector<int>> grid;

	int ROWS;
	int COLS;

	Node* goal;								// Goal node
	Node* start;							// Start node
	vector <Node *> openList;				// Container For Open List ( nodes[list] To Examine When Searching)
	vector <Node *> closeList;				// Container For Close List (Nodes Already Examined)
	AStar(int sx, int sy, int gx, int gy,TileMap *map);		// Constructor For Initialising Start And Goal x,y Nodes
	void AddOpenList(Node *n);				// Add Node To Open list
	void AddCloseList(Node *n);				// Add Node To Close List
	void RemoveOpenList(Node *n);			// Remove Node From Open List
	void RemoveCloseList(Node *n);			// Remove Node From Close List
	void ShowList(vector <Node*> list);		// Show List Either Open Or Close
	Node* GetBest();						// Get Best Node (Minimum f) In Open List
	Node* GetSuccessor(Node *current, int i);// Get Neighborhood Nodes( Empty Space) Of Current Node Given A Neighborhood Index 
	float Compute_g(Node* n);				// Calculate 'g' Cost
	float Compute_h(Node* n);				// Calculate 'h' Cost
	bool InList(vector <Node*> list, Node *n); // Check If Node 'n' Is In List
	bool Search();							// Search For Best Path
	void ShowPath(Node *walker);			// Show In Text Mode The Map And Best Path (walker=navigating node)
	Node* AStar::getFromOpenList(Node* succ); // Get From List Item Same As 'succ' Successor
	Node* AStar::getFromCloseList(Node* succ);

	vector<Vector2> bestPath;
};

#endif