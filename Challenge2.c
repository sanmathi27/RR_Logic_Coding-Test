// Approach to Binary Search Tree (BST) Implementation

//Defined Node Structure:
//Each node has an integer 'data' and pointers 'left' and 'right' to its left and right children.

//Created Node Function:
//Dynamically allocates memory for a new node and initialize its fields.

//Insertion Function:
//recursive function insert to insert a new node into the BST based on its value, maintaining BST properties.

//Search Function:
//recursive function search to find node with a specific key in the BST.

//Traversal Functions:
//inorderTraversal to print nodes in sorted order.

//Deletion Function:
//remove a node from the BST while preserving its structure.

// 8. Main Function:
//demonstrated the usage of above functions in a main function.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Node structure for BST
typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

Node *findMin(Node *node);

// Function to create a new Node
Node *createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        perror("Error creating a new node");
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//Function to insert a node
Node *insert(Node *root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

//Function to delete a node and rearrange the structure of the tree
Node *deleteNode(Node *root, int data) {
    if (root == NULL) return root;
    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } else {
        // Node found, delete it
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }
        // Node with two children: Get smallest in the right subtree
        Node *temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }
    return root;
}

//Function to find the smallest element in the right subtree
Node *findMin(Node *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

//Function to search for the given node
bool search(Node *root, int data) {
    if (root == NULL) return false;
    if (root->data == data) return true;
    if (data < root->data) {
        return search(root->left, data);
    } else {
        return search(root->right, data);
    }
}

//Function to find the inorder sort of the tree elements
void inorder(Node *root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

//Main function with user interaction prompts
int main() {
    Node *root = NULL;
    char prompt[20];
    int data;

    while (true) {
        printf("Enter prompt (add <integer>, del <integer>, search <integer>, print, exit): ");
        scanf("%s", prompt);

        if (strcmp(prompt, "add") == 0) {
            scanf("%d", &data);
            root = insert(root, data);
        } else if (strcmp(prompt, "del") == 0) {
            scanf("%d", &data);
            root = deleteNode(root, data);
        } else if (strcmp(prompt, "search") == 0) {
            scanf("%d", &data);
            if (search(root, data)) {
                printf("%d exists in the tree.\n", data);
            } else {
                printf("%d does not exist in the tree.\n", data);
            }
        } else if (strcmp(prompt, "print") == 0) {
            printf("Binary Search Tree (inorder traversal): ");
            inorder(root);
            printf("\n");
        } else if (strcmp(prompt, "exit") == 0) {
            break;
        } else {
            printf("Invalid prompt. Please try again.\n");
        }
    }

    return 0;
}
