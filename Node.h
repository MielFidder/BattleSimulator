#pragma once
#include "precomp.h"

class Node
{
public:
	int k = 2; // dimentions in tree
	Node* left, * right;
	Tank* tank;

	Node();
	Node(Tank* t);
	~Node();

	Node* NewNode(Tank* t);

	Node* insertRec(Tank* t, unsigned int depth);
	Node* insert(Tank* t);
	bool searchRec(Node* root, Tank* t, unsigned int depth);
	bool search(Tank* t);
	bool arePointsSame(Tank* t1, Tank* t2);
	Tank& closestTarget(Tank* t, unsigned int depth);
private:
};


