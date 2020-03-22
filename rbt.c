#include "rbt.h"

// red = 1
// black = 0

rb_node *newRBTNode(date * k) {
    rb_node *temp = malloc(sizeof(rb_node)); //free
    
    temp->data_date.day = k->day;
    temp->data_date.month = k->month;
    temp->data_date.year = k->year;

    temp->listPtr = NULL;
    temp->counter = 1;

    temp->color = 1; 
    temp->left = temp->right = temp->parent = NULL; 
    return temp; 
}

//free a red black tree recursively
void free_rb(rb_node *root) {
	if (root == NULL)
		return;
    // if (root->right != NULL) 
		free_rb(root->right);

	// if (root->left != NULL) 
		free_rb(root->left);

	freeRBTNode(root);
}

//this function was used to check the rotations 
void inorder(rb_node *root) {
    if (root != NULL) { 
        inorder(root->left); 
        printf("%d/%d/%d \n", root->data_date.day, root->data_date.month, root->data_date.year); 
        if (root->parent != NULL){
            // printf("parent of %d/%d/%d: %d/%d/%d\n", root->data, root->parent->data);
        	printf("parent of %d/%d/%d----->%d/%d/%d \t",  \
                root->data_date.day, root->data_date.month, root->data_date.year,  \
                root->parent->data_date.day, root->parent->data_date.month, root->parent->data_date.year);
        }
        //printf("%d\n",root->color);
        if (root->color)
      		printf("Node is RED\n");
      	else 
      		printf("Node is BLACK\n");
        inorder(root->right); 
    } 
} 

 

void RotateLeft(rb_node **root, rb_node *x){
	if (!x || !x->right){ //if x==NULL  or x->right == NULL
		return;
	}
	//y stored pointer of right child of x
    rb_node *y = x->right;

    //store y's left subtree's pointer as x's right child
    x->right = y->left;

    //update parent pointer of x's right
    if (x->right != NULL)
        x->right->parent = x;

    //update y's parent pointer
    y->parent = x->parent;

    //if x's parent is null make y as root of tree
    if (x->parent == NULL)
        (*root) = y;

    //store y at the place of x
    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;

    // make x as left child of y
    y->left = x;

    //update parent pointer of x
    x->parent = y;
}



//Right Rotation (Similar to LeftRotate)
void RotateRight(rb_node **root, rb_node *y){
	if (!y || !y->left)
		return;
	rb_node *x = y->left;
	y->left = x->right;
	if (x->right != NULL)
		x->right->parent = y;
	x->parent = y->parent;
	if (x->parent == NULL)
		*root = x;
	else if (y == y->parent->left)
		y->parent->left = x;
	else y->parent->right = x;
	x->right = y;
	y->parent = x;
}

//Utility function to fixup the red black tree after binary search tree insert
void FixViolation(rb_node **root,rb_node *z){
	//iterate until z is not the root and z's parent color is red
    //and z is not child of root
	while (z!= *root && z!= (*root)->left && z!= (*root)->right && z->parent->color == 1){
		rb_node *y = NULL;

		//find uncle and store him in y
		if (z->parent && z->parent->parent && z->parent == z->parent->parent->left)
			y = z->parent->parent->right;
		else 
			y = z->parent->parent->left;


        //Uncle is BLACK, there are four cases (LL, LR, RL and RR)
        //else{
        if(y == NULL || y->color == 0){
        	// Left-Left (LL) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Right Rotate Grandparent
            if (z->parent == z->parent->parent->left && z == z->parent->left){
            	int col = z->parent->color;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = col;
                RotateRight(root,z->parent->parent);
            }
            // Left-Right (LR) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Left Rotate Parent
            // (iii) Right Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left && z == z->parent->right){
                int col = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = col;
                RotateLeft(root,z->parent);
                // RotateRight(root,z->parent->parent);
                RotateRight(root,z->parent);    
            }

           	// Right-Right (RR) case, do following
            // (i)  Swap color of parent and grandparent
            // (ii) Left Rotate Grandparent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right && z == z->parent->right){
            	int col = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = col;
                RotateLeft(root,z->parent->parent);
            }

            // Right-Left (RL) case, do following
            // (i)  Swap color of current node  and grandparent
            // (ii) Right Rotate Parent
            // (iii) Left Rotate Grand Parent
            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right && z == z->parent->left){
            	int col = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = col;
                RotateRight(root,z->parent);
                // RotateLeft(root,z->parent->parent);
                RotateLeft(root,z->parent);
            }
        }
        // If uncle is RED, do following
        // (i)  Change color of parent and uncle as BLACK
        // (ii) Change color of grandparent as RED
        // (iii) Move z to grandparent
        else{//uncle is red 
        	y->color = 0;
        	z->parent->color = 0;
        	z->parent->parent->color = 1;
        	z = z->parent->parent;
        }
	}
	(*root)->color = 0; // root always black 
}


void insert(rb_node **root, rb_node *z){
    // Allocate memory for new node
    // rb_node *z = (rb_node*)malloc(sizeof(rb_node));

     //if root is null make z as root
    if (*root == NULL) {
        z->color = 0;
        (*root) = z;
    } else {
        rb_node *y = NULL;
        rb_node *x = (*root);

        // Follow standard BST insert steps to first insert the node
        while (x != NULL){
            y = x;
            // if (z->data < x->data)
            if (earlier(&z->data_date, &x->data_date) == 0){
                x->counter++;
                free(z);
                return;
            }
            // if (earlier(&z->data_date, &x->data_date) == 1)
            else if (earlier(&z->data_date, &x->data_date) == 1)
            // if (strcmp(z->v->id, x->v->id) < 0)
                x = x->left;
            else
                x = x->right;
        }
        z->parent = y;
        // if (z->data > y->data)
        if (earlier(&z->data_date, &y->data_date) == 0){
            y->counter++;
            free(z);
            return;
        }
        // if (earlier(&z->data_date, &y->data_date) == -1)
        else if (earlier(&z->data_date, &y->data_date) == -1)
        // if (strcmp(z->v->id, y->v->id) > 0)
            y->right = z;
        else
            y->left = z;

        z->color = 1; // color of new node is red

        // call FixViolation to fix reb-black tree's property if it
        // is voilated due to insertion.
        FixViolation(root, z);
    }
}

/* Given a non-empty binary search tree, return the node with minimum 
   key value found in that tree. Note that the entire tree does not 
   need to be searched. */
rb_node * minValueNode(rb_node* node) { 
    rb_node* current = node; 
  
    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL) 
        current = current->left; 
  
    return current; 
} 


//return 1 if date was found in the tree
list_node * rbtSearch(rb_node *root, date my_date){
    if (root == NULL)
        return NULL;
    
    if (earlier(&root->data_date, &my_date) == 0) //found
        return root->listPtr;
    else if (earlier(&root->data_date, &my_date) == -1) //
        rbtSearch(root->left, my_date);
    else
        rbtSearch(root->right, my_date);

    return NULL;
}

//given a pointer to a red black node 
//prints its data 
void print_rb_node(rb_node *node){
	printf("data_date: %d/%d/%d\n", node->data_date.day, node->data_date.month, node->data_date.year);
    printf("pointer to list %p", node->listPtr);  
}

//return 0 if id not found in tree
//return 1 if id is in the tree
// int lookup_rb_tree(rb_node *root, int id){
// 	rb_node * res;
// 	res = search(root,id);
// 	if (res == NULL){
// 		// printf("%d not found\n",id);
//         return 0;
// 	}
// 	else{
// 		print_rb_node(res);
//         return 1;
// 	}
// }


//takes a pointer to a red black node and frees what is needed
void freeRBTNode(rb_node *node){
    //
    // free(node->listPtr);
	free(node);
}