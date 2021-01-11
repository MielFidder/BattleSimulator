#pragma once
class Node
{
public:
	int k = 2;
	Node* left, * right;
	int point[];

	Node();
	~Node();

	Node* NewNode(int arr[]);

	Node* insertRec(Node* root, int point[], unsigned depth);
	Node* insert(Node* root, int point[]);
	bool searchRec(Node* root, int point[], unsigned depth);
	bool search(Node* root, int point[]);
	bool arePointsSame(int point1[], int point2[]);
private:
};


