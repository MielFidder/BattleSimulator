#include "precomp.h"
#include "Node.h"

Node::Node()
{
	left = NULL;
	right = NULL;
	point[k];
}

Node::Node(int pos[]) {
	left = NULL;
	right = NULL;
	point[k];

	insert(pos);
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
	if (this == NULL) {
		return NewNode(point);
	}

	//calculate curr dimention of comparison
	unsigned cd = depth % k;

	//decide left or right
	if (point[cd] < (this->point[cd])) {
		this->left = insertRec(root->left, point, depth + 1);
	}
	else {
		this->right = insertRec(root->right, point, depth + 1);
	}
	return this;
}

//insert new point at point in tree
Node* Node::insert(int point[]) {
	return insertRec(this, point, 0);
}

bool Node::searchRec(Node* root, int point[], unsigned int depth)
{
	// Base cases 
	if (this == NULL)
		return false;
	if (arePointsSame(this->point, point))
		return true;

	unsigned cd = depth % k;

	// Compare point with this 
	if (point[cd] < this->point[cd]) {
		return searchRec(root->left, point, depth + 1);
	}

	return searchRec(root->right, point, depth + 1);
}

// Searches a Point in the K D tree. It mainly uses 
// searchRec() 
bool Node::search(int point[])
{
	// Pass current depth as 0 
	return searchRec(this, point, 0);
}

bool Node::arePointsSame(int point1[], int point2[]) {
	for (int i = 0; i < k; ++i) {
		if (point1[i] != point2[i])
			return false;
	}
	return true;
}
