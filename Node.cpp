#include "precomp.h"
#include "Node.h"

Node::Node()
{
	left = NULL;
	right = NULL;
}

Node::Node(Tank* t) {
	left = NULL;
	right = NULL;
	tank = t;

	insert(t);
}

Node::~Node()
{
}

Node* Node::NewNode(Tank* t) {
	Node* temp = new Node;

	//for (int i = 0; i < k; i++) {
		temp->tank = t;
	//}

	temp->left = temp->right = NULL;
	return temp;
}

Node* Node::insertRec(Tank* t, unsigned int depth) {
	if (this == NULL) {
		return NewNode(t);
	}

	//calculate curr dimention of comparison
	unsigned cd = depth % k;

	//decide left or right
	if (t->get_position()[cd] < (this->tank->get_position()[cd])) {
		if (!this->left)
			return this->left = NewNode(t);
		this->left->insertRec(t, depth + 1);
	}
	else {
		if (!this->right)
			return this->right = NewNode(t);
		this->right->insertRec(t, depth + 1);
	}
	return this;
}

//insert new point at point in tree
Node* Node::insert(Tank* t) {
	return insertRec(t, 0);
}

bool Node::searchRec(Node* root, Tank* t, unsigned int depth)
{
	// Base cases 
	if (this == NULL)
		return false;
	if (arePointsSame(this->tank, t))
		return true;

	unsigned cd = depth % k;

	// Compare point with this 
	if (t->get_position()[cd] < (this->tank->get_position()[cd])) {
		return searchRec(root->left, t, depth + 1);
	}

	return searchRec(root->right, t, depth + 1);
}

// Searches a Point in the K D tree. It mainly uses 
// searchRec() 
bool Node::search(Tank* t)
{
	// Pass current depth as 0 
	return searchRec(this, t, 0);
}

bool Node::arePointsSame(Tank* t1, Tank* t2) {
	for (int i = 0; i < k; ++i) {
		if (&t1 != &t2)
			return false;
	}
	return true;
}

Tank& Node::closestTarget(Tank* t, unsigned int depth)
{
	Tank* closestTank = NULL;
	float minDist = INFINITY;

	//calculate curr dimention of comparison
	unsigned cd = depth % k;

	float tposxy;
	float nposxy;
	if (cd == 0) {
		tposxy = tank->getpos().x;
		nposxy = this->right->tank->getpos().x;
	}
	else {
		tposxy = tank->getpos().y;
	}
	if (tposxy) {

	}
	return *closestTank;
}
