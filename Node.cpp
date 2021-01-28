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

	temp->tank = t;

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

Node* Node::closestTarget(Node* root, Tank* target, unsigned int depth)
{
	if (root == NULL) return NULL;

	Node* nextBranch = NULL;
	Node* otherBranch = NULL;

	unsigned cd = depth % 2;

	if (target->getpos()[cd] < root->tank->getpos()[cd]) {
		nextBranch = root->left;
		otherBranch = root->right;
	}
	else {
		nextBranch = root->right;
		otherBranch = root->left;
	}

	Node* temp = closestTarget(nextBranch, target, depth++);
	Node* best = closest(temp, root, target);

	long radiusSquared = distSquared(target, best->tank);

	long dist = target->getpos()[cd] - root->tank->getpos()[cd];

	if (radiusSquared >= dist * dist) {
		temp = closestTarget(otherBranch, target, depth++);
		best = closest(temp, best, target);
	}

	return best;
}

Node* Node::closestTarget(Tank target)
{
	Node* root = this;
	return closestTarget(root, &target, 0);
}

Node* Node::closest(Node* n0, Node* n1, Tank* target)
{
	if (n0 == NULL)
		return n1;
	if (n1 == NULL)
		return n0;

	long d1 = distSquared(n0->tank, target);
	long d2 = distSquared(n1->tank, target);

	if (d1 < d2)
		return n0;
	else
		n1;
}

long Node::distSquared(Tank* p0, Tank* p1)
{
	long total = 0;

	for (int i = 0; i < 2; i++) {
		int diff = std::abs(p0->getpos()[i] - p1->getpos()[i]);
		total += pow(diff, 2);
	}
	return total;
}


