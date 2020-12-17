#include <iostream>
#include <fstream>

using namespace std;

#define BLACK 0
#define RED 1

class RBT;
class NODE {
	NODE() {};
	NODE(int value) :key(value) {};

private:
	
	NODE *parent = NULL, *left = NULL, *right = NULL;
	int key = -1;
	bool color = RED;	// balck is 0, red is 1
	friend class RBT;
};

class RBT {
public:
	RBT() { 
		nil = new NODE;
		nil->color = BLACK;
		root = nil;
		root->parent = nil;
	};
	~RBT() { delete nil; };
	
	void insertNode(int key);
	void deleteNode(int key);
	NODE* search(int key);
private:
	NODE *root, *nil;
	void L_rotate(NODE *cur);
	void R_rotate(NODE *cur);
	void insert_adjust(NODE *cur);
	void delete_adjust(NODE *cur);
	NODE* successor(NODE *cur);
	NODE* mostLeft(NODE *cur);

};

int main()
{
	int num;
	
	


	return 0;
}

void RBT::L_rotate(NODE *cur)
{		
	/*--------------------------------
		  [before]			 [after]		
		   parent			  parent
			 |					|
			cur					y
		   /   \			  /   \ 
		  A     y			cur    C
			   / \ 		   /   \ 
			  B	  C		  A		B
	----------------------------------*/
	NODE *y = cur->right;				
										
	cur->right = y->left;				
	if (y->left != nil)					
		y->left->parent = cur;			
										
	y->parent = cur->parent;			
	if (cur->parent = nil)
		root = y;
	else if (cur == cur->parent->left)	// 若原本cur是left child
		cur->parent->left = y;			
	else                                // 若原本cur是right child
		cur->parent->right = y;

	y->left = cur;
	cur->parent = y;
}

void RBT::R_rotate(NODE *cur)
{	
	/*---------------------------------
		  [before]			 [after]		
		   parent			  parent
			 |					|
			cur					y
		   /   \			  /   \ 
		  y     C			 A    cur
	   / \ 					 /   \ 
	  A	B					B	  C
	----------------------------------*/
	NODE *y = cur->left;				
										
	cur->left = y->right;			
	if (y->right != nil)				
		y->right->parent = cur;			
										
	y->parent = cur->parent;			
	if (cur->parent = nil)
		root = y;
	else if (cur == cur->parent->right)	// 若原本cur是left child
		cur->parent->left = y;
	else                                // 若原本cur是right child
		cur->parent->right = y;

	y->left = cur;
	cur->parent = y;
}

void RBT::insertNode(int key)
{
	NODE *cur = root, *parent = nil;
	NODE *newNode = new NODE(key);

	while (cur != nil) {
		parent = nil;
		if (key < cur->key)
			cur = cur->left;
		else
			cur = cur->right;
	}

	newNode->parent = parent;

	if (parent == nil)
		root = newNode;
	else if (key < parent->key)
		parent->left = newNode;
	else
		parent->right = newNode;

	newNode->left = newNode->right = nil;

	insert_adjust(newNode);
}

void RBT::insert_adjust(NODE *cur)
{
	while (cur->parent->color == RED) {
		if (cur->parent==cur->parent->parent->left){
			NODE *uncle = cur->parent->parent->right;
			if (uncle->color == RED) {
				cur->parent->color = BLACK;			// make parent's color black;
				uncle->color = BLACK;				// make uncle's color black
				cur->parent->parent->color = RED;	// make grandparent's color red.
				cur = cur->parent->parent;
			}
			else {
				if (cur == cur->parent->parent->right) {
					cur = cur->parent;
					L_rotate(cur);
				}
				cur->parent->color = BLACK;
				cur->parent->parent->color = RED;
				R_rotate(cur->parent->parent);
			}
		}
		else {
			NODE *uncle = cur->parent->parent->left;
			if (uncle->color == RED) {
				cur->parent->color = BLACK;			// make parent's color black;
				uncle->color = BLACK;				// make uncle's color black
				cur->parent->parent->color = RED;	// make grandparent's color red.
				cur = cur->parent->parent;
			}
			else {
				if (cur == cur->parent->parent->left) {
					cur = cur->parent;
					R_rotate(cur);
				}
				cur->parent->color = BLACK;
				cur->parent->parent->color = RED;
				L_rotate(cur->parent->parent);
			}
		}
	}
	root->color = BLACK;
}

void RBT::deleteNode(int key)
{
	NODE *dNode = search(key);

	NODE *y, *child;

	bool isFull = dNode->left == nil || dNode->right == nil;
	y = isFull ? dNode : successor(dNode);

	child = y->left == nil ? y->right : y->left;

	if (y->parent == nil)
		root = child;
	else if (y == y->parent->left)
		y->parent->left = child;
	else
		y->parent->right = child;

	if (y != dNode) 
		dNode->key = y->key;
	
	if (y->color == BLACK)
		delete_adjust(child);
}

void RBT::delete_adjust(NODE *cur)
{
	while (cur != root && cur->color == BLACK) {
		bool isLeft = cur == cur->parent->left;

		NODE *sibling = isLeft ? cur->parent->right : cur->parent->left;

		if (sibling->color == RED) {
			sibling->color = BLACK;
			cur->parent->color = RED;
			isLeft ? L_rotate(cur->parent) : R_rotate(cur->parent);
			sibling = isLeft ? cur->parent->right : cur->parent->left;
		}
		// both sibilings are black
		if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
			sibling->color = RED;
			cur = cur->parent;
		}
		// only one of the sibilings is black
		else {
			if (isLeft) {
				if (sibling->right->color == BLACK) {
					sibling->left->color = BLACK;
					sibling->color = RED;
					R_rotate(sibling);
					sibling = cur->parent->right;
				}

				sibling->color = cur->parent->color;
				cur->parent->color = BLACK;
				sibling->right->color = BLACK;
				L_rotate(cur->parent);
				cur = root;
			}
			else {
				if (sibling->left->color == BLACK) {
					sibling->right->color = BLACK;
					sibling->color = RED;
					L_rotate(sibling);
					sibling = cur->parent->left;
				}

				sibling->color = cur->parent->color;
				cur->parent->color = BLACK;
				sibling->left->color = BLACK;
				R_rotate(cur->parent);
				cur = root;
			}
		}
	}
	cur->color = BLACK;
}

NODE* RBT::successor(NODE *cur)
{
	if (cur->right != nil)
		return mostLeft(cur->right);

	NODE *newNode = cur->parent;

	while (newNode != nil && cur == newNode->right) {
		cur = newNode;
		newNode = newNode->parent;
	}

	return newNode;
}

NODE* RBT::mostLeft(NODE *cur)
{
	while (cur->left != nil)
		cur = cur->left;

	return cur;
}

NODE* RBT::search(int key)
{
	NODE *cur = root;

	while (cur != nil && key != cur->key)
		cur = key < cur->key ? cur->left : cur->right;

	return cur;
}