#pragma once
#include "precomp.h"

class Node
{
public:
	const int k = 2; // dimentions in tree
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
	Node* closestTarget(Node* root, Tank* target, unsigned int depth);
	Node* closestTarget(Tank target);
	Node* closest(Node* n0, Node* n1, Tank* target);
	static long distSquared(Tank* p0, Tank* p1);
private:
};


