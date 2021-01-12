#pragma once
class Node
{
public:
	int k = 2; // dimentions in tree
	Node* left, * right;
	int point[];

	Node();
	~Node();

	Node* NewNode(int arr[]);

	Node* insertRec(Node* root, int point[], unsigned int depth);
	Node* insert(Node* root, int point[]);
	bool searchRec(Node* root, int point[], unsigned int depth);
	bool search(Node* root, int point[]);
	bool arePointsSame(int point1[], int point2[]);
private:
};
