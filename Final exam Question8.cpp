#include<iostream>
#include<fstream>
struct BinaryNode
{
  int element;
  BinaryNode *left;
  BinaryNode *right;
};

int printTreeDecreasingOrder(BinaryNode* node, ostream& out)
{
	//first we handle right sub trees (larger values)

	if(node->right != NULL) { //use this loop to move to the right-most (largest element) node in a given sub tree

		printTreeDecreasingOrder(node->right, out); 
		
	}
	out << node->element; //right is NULL; now we can print current element.
	

	//now we need to handle left sub trees (smaller values):

	if(node->left != NULL){
	
		printTreeDecreasingOrder(node->left, out);
		
	}

	

}
