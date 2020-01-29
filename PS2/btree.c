#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// A single node in the binary tree
typedef struct btree_node{
	int data;
	struct btree_node * parent;
	struct btree_node * leftleaf;
	struct btree_node * rightleaf;
} btree_node;


/// Stores a binary, sorted tree of integers, in which each element may only occur once
typedef struct btree{
	
	btree_node* root;
	int number_of_nodes;

}btree;





/// Creates and returns a new binary tree
/// must be deallocated by btree_destroy
btree* btree_create(){


	btree_node *tmp = (btree_node*) malloc(sizeof(btree_node));	
	tmp->leftleaf = NULL;
	tmp->rightleaf = NULL;
	tmp->parent = NULL;
	


	btree* tmp1 = (btree*) malloc(sizeof(btree));	
	tmp1->root = tmp;
	
	tmp1->number_of_nodes = 0;

	return tmp1;
	
}


//deallocates every single node
void btree_node_destroy(btree_node* t){

	if(t->leftleaf != NULL){
		btree_node_destroy(t->leftleaf);
	}
	if(t->rightleaf != NULL){
		btree_node_destroy(t->rightleaf);
	}
	free(t);
	
}

/// Destroys and deallocates all memory for the given tree 't'
void btree_destroy(btree* t){
	btree_node_destroy(t->root);
	free(t);
}



//insert nodes
int btree_insert_node(btree_node* t, int d){
	
	if(t->data == d){
		return 1;
	}
	else if(d < t->data && t->leftleaf != NULL){
		return btree_insert_node(t->leftleaf, d);

	}
	else if(d > t->data && t->rightleaf != NULL){
		return btree_insert_node(t->rightleaf, d);	
	}
	else{
		btree_node* tmp = (btree_node*) malloc(sizeof(btree_node));	
		tmp->leftleaf = NULL;
		tmp->rightleaf = NULL;
		tmp->data = d;
		tmp->parent = t;
		if(d < t->data){
			t->leftleaf = tmp;
		}		
		else{
			t->rightleaf = tmp;
		}

		return 0;
	}
	return 1;
}

/// Inserts the given number 'd' into tree 't'
/// if the number is already in 't', no changes are made
void btree_insert(btree* t, int d){

	if(t->number_of_nodes == 0){
		t->root->data = d;	
		t->number_of_nodes++;
	}
	else{	
		int i = btree_insert_node(t->root, d);
		if (i == 0){
			t->number_of_nodes++;
		}
	}
}




int btree_contains_node(const btree_node* t, int d){
	

	if(t->data == d){
		return 0;
	}
	else if(d < t->data && t->leftleaf != NULL){
		return btree_contains_node(t->leftleaf, d);
	}
	else if(d > t->data && t-> rightleaf != NULL){
		return btree_contains_node(t->rightleaf, d);
	}
	
	else{
		return 1;
	}


}



/// Returns true if the given tree 't' contains 'd', false otherwise
bool btree_contains(const btree* t, int d){

	int i = btree_contains_node(t->root, d);

	if(i == 0){
		return true;
	}

	return false;	
}



//returns node from the very far left side
int btree_node_minimum(const btree_node* t){

	if(t->leftleaf != NULL){
		return btree_node_minimum(t->leftleaf);
	}
	
	return t->data;
	
}

//returns node from the very far right side
int btree_node_maximum(const btree_node* t){

	
	if(t->rightleaf != NULL){
		return btree_node_maximum(t->rightleaf);
	}
	return t->data;

}

/// Returns the smallest number in tree 't'
int btree_minimum(const btree* t){

	return btree_node_minimum(t->root);
	
}




/// Returns the largest number in tree 't'
int btree_maximum(const btree* t){

	return btree_node_maximum(t->root);

}


//remove node 
void btree_node_remove(btree_node* t, int d){


	if(d < t->data){
		btree_node_remove(t->leftleaf, d);
	}
	else if(d > t->data){
		btree_node_remove(t->rightleaf, d);
	}
	
	else{
		if(t->leftleaf == NULL && t->rightleaf != NULL){
			t = t->rightleaf;
		}
		else if(t->rightleaf == NULL && t-> leftleaf != NULL){
			t = t->leftleaf; 
		}
		else if(t->rightleaf != NULL && t-> leftleaf != NULL){
			int tmp = btree_node_minimum(t->rightleaf);
			btree_node_remove(t, tmp);
			t->data = tmp; 

		}
		else{
			if(d < t->parent->data){
				t->parent->leftleaf = NULL;
				free(t);
			}
			else{
				
				t->parent->rightleaf = NULL; 
				free(t);
			}
		}
		
	}
}




/// Removes the given number 'd' from tree 't' 
void btree_remove(btree* t, int d){

	if(btree_contains(t, d) == true){
		btree_node_remove(t->root, d);
		t->number_of_nodes--;
	}
}






/// Returns the number of elements of the given btree 't'
/// NOTE: should complete in O(1) time
size_t btree_size(const btree* t){

	return t->number_of_nodes;

}


//search and print every single node from the far left side to the far right side
char* btree_node_print(btree_node* t){
	
	char *x = NULL;
	char tmp[10];
	sprintf(tmp, "%d", t->data);
	char* leftstrg = "";
	char* rightstrg = "";
	if(t->leftleaf != NULL){
		leftstrg = btree_node_print(t->leftleaf);
	}
	if(t->rightleaf != NULL){
		rightstrg = btree_node_print(t->rightleaf);
	}

	x = realloc(x, strlen(leftstrg) + strlen(rightstrg) + strlen(tmp) + 2);
	sprintf(x, "(%s %d %s)",leftstrg, t->data, rightstrg);
	return x;
	

}

/// Prints the given btree 't' to the supplied output stream 'out'
/// output format: ([LEFT], VAL, [RIGHT]) : [SIZE]
/// example empty: ( NIL ) : 0
/// example 3,4,7 in a balanced tree: ((3), 4, (7)) : 3
void btree_print(const btree* t, FILE* out){
	

	if(t->number_of_nodes == 0){
		fprintf(out, "( NIL ) : 0\n");
	}
	else{
	 	fprintf(out, "%s : %d\n", btree_node_print(t->root), t->number_of_nodes);
		 	
	}

	
}


