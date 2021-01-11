#include "precomp.h"
#include "Node.h"



Node::Node()
{
	left = NULL;
	right = NULL;
	point[k];
}

Node::~Node()
{
}

struct Node* Node::NewNode(int arr[]) {
	Node* temp = new Node;

	for (int i = 0; i < k; i++) {
		temp->point[i] = arr[i];
	}

	temp->left = temp->right = NULL;
	return temp;
}

Node* Node::insertRec(Node* root, int point[], unsigned int depth) {
	if (root == NULL) {
		return NewNode(point);
	}

	//calculate curr dimention of comparison
	unsigned cd = depth % k;

	//decide left or right
	if (point[cd] < (root->point[cd])) {
		root->left = insertRec(root->left, point, depth + 1);
	}
	else {
		root->right = insertRec(root->right, point, depth + 1);
	}
	return root;
}

//insert new point at point in tree
Node* Node::insert(Node* root, int point[]) {
	return insertRec(root, point, 0);
}

bool Node::searchRec(Node* root, int point[], unsigned int depth)
{
	// Base cases 
	if (root == NULL)
		return false;
	if (arePointsSame(root->point, point))
		return true;

	unsigned cd = depth % k;

	// Compare point with root 
	if (point[cd] < root->point[cd]) {
		return searchRec(root->left, point, depth + 1);
	}

	return searchRec(root->right, point, depth + 1);
}

// Searches a Point in the K D tree. It mainly uses 
// searchRec() 
bool Node::search(Node* root, int point[])
{
	// Pass current depth as 0 
	return searchRec(root, point, 0);
}

bool Node::arePointsSame(int point1[], int point2[]) {
	for (int i = 0; i < k; ++i) {
		if (point1[i] != point2[i])
			return false;
	}
	return true;
}
