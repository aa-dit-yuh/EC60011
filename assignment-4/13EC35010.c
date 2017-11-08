#include<stdio.h>
#include<stdlib.h>
 
struct node
{
    int key;
    struct node *left, *right;
};

struct node *newNode(int item)
{
    struct node *temp =  (struct node *)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

void inorder(struct node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->key);
        inorder(root->right);
    }
}

struct node* search(struct node* root, int key)
{
    if (root == NULL || root->key == key)
       return root;
    if (root->key < key)
       return search(root->right, key);
    return search(root->left, key);
}

struct node* insert(struct node* node, int key)
{
    if (node == NULL) return newNode(key);
    if (key < node->key)
        node->left  = insert(node->left, key);
    else
        node->right = insert(node->right, key);
    return node;
}
 
struct node * minValueNode(struct node* node)
{
    struct node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}
 
struct node* delete(struct node* root, int key)
{
    if (root == NULL) return root;
 
    if (key < root->key)
        root->left = delete(root->left, key);
    else if (key > root->key)
        root->right = delete(root->right, key);
    else
    {
        if (root->left == NULL)
        {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }
        struct node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = delete(root->right, temp->key);
    }
    return root;
}

int main()
{
    int operation, key;
    struct node *root;
    
    while(1) {
        printf("\nEnter two space-separated integers:\n");
        printf("1(Insertion) (key)           // Example: 1 10\n");
        printf("2(Deletion)  (key)           // Example: 2 9\n");
        printf("3(Find key)  (key)           // Example: 3 5\n");
        printf("4(Exit the program)\n");
        scanf("%d", &operation);

        switch (operation) {
        case 1:
            scanf("%d", &key);
            root = insert(root, key);
            inorder(root);
            break;
        case 2:
            scanf("%d", &key);
            root = delete(root, key);
            inorder(root);
            break;
        case 3:
            scanf("%d", &key);
            if(search(root, key) != NULL)
                printf("TRUE");
            else
                printf("FALSE");
            break;
        case 4:
            return 0;
        }
        fflush(stdout);
    }
}