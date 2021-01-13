#pragma once
class Node
{
public:
	int k = 2; // dimentions in tree
	Node* left, * right;
	int point[];

	Node();
	Node(int pos[]);
	~Node();

	Node* NewNode(int arr[]);

	Node* insertRec(Node* root, int point[], unsigned int depth);
	Node* insert(int point[]);
	bool searchRec(Node* root, int point[], unsigned int depth);
	bool search(int point[]);
	bool arePointsSame(int point1[], int point2[]);
private:
};


