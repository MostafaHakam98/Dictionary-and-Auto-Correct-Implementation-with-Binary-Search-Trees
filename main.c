#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// Node of a tree
typedef struct Node {

    char* word;
    struct Node * left;
    struct Node * right;

} Node;

// Tree
typedef struct Tree {
    Node* root;
    int height;
    int size;
} Tree;

//.............................................................................................................................................

// Initializes a tree
Tree* init() {

    Tree* tptr = (Tree*) malloc(sizeof(Tree));
    tptr->root = NULL;
    tptr->height = 0;
    tptr->size = 0;

    return tptr;
}

// Creates a new node
Node* newNode(char* tempWord) {

    Node* tempNode = (Node*) malloc(sizeof(Node));
    tempNode-> word = (char*) malloc(30*sizeof(char));
    strcpy(tempNode-> word, tempWord);
    tempNode-> left = NULL;
    tempNode-> right = NULL;

    return tempNode;
}

// Inserts a node
Node* insert(Tree* tree, Node* node, char* tempWord) {

    if(!node)
        return newNode(tempWord);
    if(strcmp(node-> word, tempWord) > 0)
        node-> left = insert(tree, node-> left, tempWord);
    if(strcmp(node-> word, tempWord) < 0)
        node-> right = insert(tree, node-> right, tempWord);

    return node;
}

// Get height of tree
int height(Node *root) {

    if(!root)
        return -1;

    int leftHeight = height(root-> left);
    int rightHeight = height(root-> right);

    if (leftHeight > rightHeight)
        return leftHeight+1;
    else
        return rightHeight+1;
}

// Searches for word
Node* search(Node* root, char *tempWord) {

    if(root) {
        if(strcmp(root-> word, tempWord) == 0)
            return root;

        if(strcmp(root-> word, tempWord) > 0) {
            if(root-> left)
                return search(root->left, tempWord);
            else
                return root;
        }

        if(strcmp(root-> word,tempWord) < 0) {
            if(root-> right)
                return search(root-> right, tempWord);
            else
                return root;
        }
    }
}

// Searches for Successor
Node* searchSuccessor(Node *root, char* leaf) {

    Node* succ = NULL;
    while (root) {
        if (strcmp(leaf, root->word)<0) {
            succ = root;
            root = root->left;
        }
        else if (strcmp(leaf, root->word)>0)
            root = root->right;
        else
           break;
    }

    return succ;
}

// Searches for Predecessor
Node* searchPredecessor(Node *root, char* leaf) {

    Node* pred = NULL;
    while (root) {
        if (strcmp(leaf,root->word)<0)
            root = root->left;

        else if (strcmp(leaf,root->word)>0) {
            pred = root;
            root = root-> right;
        }
        else
           break;
    }

    return pred;
}

// Function to create dictionary tree
Tree* createTree(char* filename){

    Tree* tree;
    tree = init(); //Creates tree storing dictionary data
    char* tempWord = (char*)malloc(30*sizeof(char)); //Word to be added to tree
    FILE* fptr = fopen(filename , "r");

    while (!feof(fptr)){

        fscanf(fptr, "%s", tempWord);
        tree->root = insert(tree, tree->root, tempWord);
        tree->size ++;
    }

    return tree;
}

// Main dictionary function
void dictionary(char* filename){

    Tree* dict;
    dict = createTree(filename);

    printf("\t\t\t\t\tDictionary: %s\n\n\t\t\t\t\t\tHeight: %d levels\n\n\t\t\t\t\t\tSize: %d words\n\n\n************************************************************************************************************************\n\nPlease enter a sentence: ", filename, height(dict->root), dict->size);

    char* string = (char*) malloc(200*sizeof(char)); // String of words entered by user
    char* word = (char*) malloc(30*sizeof(char)); // Single words of Strings
    Node* tempNode = (Node*) malloc(sizeof(Node)); //Node containing single words

    gets(string);
    word = strtok(string, " ");

    while(word) {

        Node* tempNode = search(dict->root, word);
        Node* temp = tempNode;

        if(strcmp(tempNode->word, word) == 0)
            printf("\n- %s\n     Correct spelling\n", word);
        else {
            printf("\n- %s\n     Wrong spelling\n     Suggested words:  (%s), ", word, tempNode->word);
            tempNode = searchSuccessor(dict->root, temp->word);
            printf("(%s), ", tempNode->word);
            tempNode = searchPredecessor(dict->root, temp->word);
            printf("(%s)\n", tempNode->word);
        }
        word = strtok(NULL," ");
    }
    printf("\n\n");

    free(string);
    free(word);
    free(tempNode);
}

//.............................................................................................................................................

int main(int argc, char**argv) {

    if(argc!=2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	dictionary(argv[1]);

	return 0;
}
