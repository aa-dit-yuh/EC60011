#include <iostream>
#include <queue>

using namespace std;

// TwoThreeNode class
class TwoThreeNode {
    private:
        // Insert into a node with 1 child
        void insert1Siblings(TwoThreeNode *newChild, int newSmallest) {
            int newKey = newChild->key[0];
            newChild->parent = this;

            if (newKey < child[0]->key[0]) {
                // newNode is inserted as first child of root
                child[1] = child[0];
                child[0] = newChild;
                key[0] = child[1]->getSmallest();
            }
            else {
                // newNode is iserted as second child of root
                child[1] = newChild;
                key[0] = newSmallest;
            }
        }

        // Insert into a node with 2 children
        void insert2Siblings(TwoThreeNode *newChild, int newSmallest) {
            int newKey = newChild->key[0];
            newChild->parent = this;

            if (newKey < child[0]->key[0]) {
                child[2] = child[1];
                child[1] = child[0];
                child[0] = newChild;

                key[1] = key[0];
                key[0] = child[1]->getSmallest();
                updateParentSmallest(newSmallest);
            }
            else if (newKey < child[1]->key[0]) {
                child[2] = child[1];
                child[1] = newChild;

                key[1] = key[0];
                key[0] = newSmallest;
            }
            else {
                child[2] = newChild;

                key[1] = newSmallest;
            }
        }

        // Insert into a node with 3 children
        void insert3Siblings(TwoThreeNode *newChild, int newSmallest) {
            int newKey = newChild->key[0];

            int splitSmallest = -1;
            TwoThreeNode *splitNode = new TwoThreeNode();
            splitNode->parent = parent;

            if (newKey < child[0]->key[0] || newKey < child[1]->key[0]) {
                // newChild is inserted in current node
                splitSmallest = key[0];
                splitNode->child[0] = child[1];
                splitNode->child[1] = child[2];
                splitNode->key[0] = key[1];

                child[1]->parent = splitNode;
                child[2]->parent = splitNode;
                newChild->parent = this;

                if (newKey < child[0]->key[0]) {
                    // newChild is inserted as first child
                    child[1] = child[0];
                    child[0] = newChild;

                    key[0] = child[1]->getSmallest();
                    updateParentSmallest(newSmallest);
                }
                else {
                    // newChild is inserted as second child
                    child[1] = newChild;

                    key[0] = newSmallest;
                }
            }
            else {
                // newChild is inserted in split node
                child[2]->parent = splitNode;
                newChild->parent = splitNode;
 
                if (newKey < child[2]->key[0]) {
                    // newChild is inserted as first child
                    splitSmallest = newSmallest;
                    splitNode->child[0] = newChild;
                    splitNode->child[1] = child[2];
                    splitNode->key[0] = key[1];
                }
                else {
                    // newChild is inserted as second child
                    splitSmallest = key[1];
                    splitNode->child[0] = child[2];
                    splitNode->child[1] = newChild;
                    splitNode->key[0] = newSmallest;
                }
            }

            child[2] = NULL;
            key[1] = -1;

            if (parent->parent == NULL) {
                // At root, so new root needs to be created
                TwoThreeNode *newNode = new TwoThreeNode();

                parent->child[0] = newNode;
                newNode->parent = parent;
                newNode->child[0] = this;
                parent = newNode;
            }

            parent->insert(splitNode, splitSmallest);
        }

        // Update the parent nods efor the smallest child value
        void updateParentSmallest(int data) {
            switch (sibNumber()) {
                case 0: if (parent->parent != NULL) parent->updateParentSmallest(data); break;
                case 1: parent->key[0] = data; break;
                case 2: parent->key[1] = data; break;
            }
        }

    public:
        int key[2];
        TwoThreeNode *parent, *child[3];

        // Constructor
        TwoThreeNode(int data = -1) {
            key[0] = data;
            key[1] = -1;
            parent = child[0] = child[1] = child[2] = NULL;
        }

        // Check if node is a leaf
        bool isLeaf() {
            return (child[0] == NULL);
        }

        // Get which sibling the node is
        int sibNumber() {
            for (int i = 0; i < 3; ++i) {
                if (this == parent->child[i]) return i;
            }
            return -1;
        }

        // Gets the value of the smallest data item in the subtree
        // rooted by this node
        int getSmallest() {
            TwoThreeNode *node = this;
            while (!node->isLeaf()) node = node->child[0];
            return node->key[0];
        }

        // Insertion
        void insert(TwoThreeNode *newChild, int newSmallest) {
            if (child[1] == NULL) insert1Siblings(newChild, newSmallest);
            else if (child[2] == NULL) insert2Siblings(newChild, newSmallest);
            else insert3Siblings(newChild, newSmallest); 
        }
};

// TwoThreeTree class
class TwoThreeTree {
    private:
        TwoThreeNode *root;

        // Find the appropriate operation point
        TwoThreeNode* findSpot(TwoThreeNode *node, int data) {
            if (node == NULL) return NULL;

            while (!node->isLeaf()) {
                if (node->key[0] == data || node->key[1] == data)
                    return NULL;
                if (node->key[0] == -1 || data < node->key[0])
                    node = node->child[0];
                else if (node->key[1] == -1 || data < node->key[1])
                    node = node->child[1];
                else
                    node = node->child[2];
            }

            if (node->key[0] == data) return NULL;
            return node->parent;
        }

    public:
        // Constructor
        TwoThreeTree() {
            root = new TwoThreeNode();
            root->child[0] = new TwoThreeNode();
            root->child[0]->parent = root;
        }

        // Insert
        bool insert(int data) {
            TwoThreeNode *newNode = new TwoThreeNode(data);
            TwoThreeNode *spot = root->child[0];

            if (spot->child[0] == NULL) {
                // First insertion
                newNode->parent = spot;
                spot->child[0] = newNode;
            }
            else {
                spot = findSpot(spot, data);
                if (spot == NULL) return false;
                spot->insert(new TwoThreeNode(data), data);
            }
            return true;
        }

        // Delete
        bool remove(int data)
        {
            if(root == NULL) return false;

            if(root->child[1] == NULL) { // Tree has only one element
                if (root->key[0] == data) {
                    root = new TwoThreeNode();
                    root->child[0] = new TwoThreeNode();
                    root->child[0]->parent = root;
                }
                else
                    return false;
            }

            if(root->child[0]->isLeaf()) {
                if(root->child[0]->key[0] ==data)
                {
                    root->child[0] = root->child[1];
                    root->child[1] = NULL;
                    return root;
                }
                else if(root->child[1]->key[0] == data)
                {
                    root->child[1] = NULL;
                    root->key[0] = root->child[0]->key[0];
                    return root;
                }
            }

            TwoThreeNode *p = root, *q = NULL;
            while(p != NULL && !p->isLeaf()) {
                q = p;
                if(data < p->key[0])
                    p = p->child[0];
                else if(p->child[2] == NULL || data < p->key[1])
                    p = p->child[1];
                else
                    p = p->child[2];
            }

            if(p != NULL && p->key[0] == data)
            {
                int flag = 0;
                while(q != NULL && !flag) {
                    if(q->child[2] != NULL) { // If p has 2 siblings
                        if(p == q->child[0]) {
                            q->child[0] = q->child[1];
                            q->child[1] = q->child[2];
                            q->child[2] = NULL;
                        }
                        else if(p == q->child[1]) {
                            q->child[1] = q->child[2];
                            q->child[2] = NULL;
                        }
                        else {
                            q->child[2] = NULL;
                        }
                        q->key[0] = q->child[1]->getSmallest();
                        q->key[1] = q->child[2]->getSmallest();
                        
                        flag = 1;
                    }
                    else { // If p has only 1 sibling
                        if(p == q->child[0]) {
                            q->child[0] = q->child[1];
                            q->child[1] = NULL;
                        }
                        else
                            q->child[1] = NULL;

                        p = q->parent;
                        if(p != NULL) {
                            // If atleast one of q's sibling has 3 children
                            if(q == p->child[0]) { // q is first child and second child/third child has 3 elements
                                if(p->child[1] != NULL && p->child[1]->child[2] != NULL) {
                                    q->child[1] = p->child[1]->child[0];
                                    p->child[1]->child[0] = p->child[1]->child[1];
                                    p->child[1]->child[1] = p->child[1]->child[2];
                                    p->child[1]->child[2] = NULL;

                                    p->child[1]->key[0] = p->child[1]->child[1]->getSmallest();
                                    p->child[1]->key[1] = p->child[1]->child[2]->getSmallest();
                                    q->key[0] = q->child[1]->getSmallest();
                                    q->key[1] = q->child[2]->getSmallest();

                                    q->child[1]->parent = q;
                                    
                                    flag = 1;
                                }
                                else if(p->child[2] != NULL && p->child[2]->child[2] != NULL) {
                                    q->child[1] = p->child[1]->child[0];
                                    p->child[1]->child[0] = p->child[1]->child[1];
                                    p->child[1]->child[1] = p->child[2]->child[0];
                                    p->child[2]->child[0] = p->child[2]->child[1];
                                    p->child[2]->child[1] = p->child[2]->child[2];
                                    p->child[2]->child[2] = NULL;

                                    p->child[1]->key[0] = p->child[1]->child[1]->getSmallest();
                                    p->child[1]->key[1] = p->child[1]->child[2]->getSmallest();
                                    p->child[2]->key[0] = p->child[2]->child[1]->getSmallest();
                                    p->child[2]->key[1] = p->child[2]->child[2]->getSmallest();
                                    q->key[0] = q->child[1]->getSmallest();
                                    q->key[1] = q->child[2]->getSmallest();

                                    q->child[1]->parent = q;
                                    p->child[1]->child[1]->parent = p->child[1];

                                    flag = 1;
                                }
                            }
                            else if(q == p->child[1]) // q is second child and first child/third child has 3 elements
                            {
                                if(p->child[0] != NULL && p->child[0]->child[2] != NULL)
                                {
                                    q->child[1] = q->child[0];
                                    q->child[0] = p->child[0]->child[2];
                                    p->child[0]->child[2] = NULL;

                                    p->child[0]->key[0] = p->child[0]->child[1]->getSmallest();
                                    p->child[0]->key[1] = p->child[0]->child[2]->getSmallest();
                                    q->key[0] = q->child[1]->getSmallest();
                                    q->key[1] = q->child[2]->getSmallest();

                                    q->child[0]->parent = q;
                                    
                                    flag = 1;
                                }
                                else if(p->child[2] != NULL && p->child[2]->child[2] != NULL)
                                {
                                    q->child[1] = p->child[2]->child[0];
                                    p->child[2]->child[0] = p->child[2]->child[1];
                                    p->child[2]->child[1] = p->child[2]->child[2];
                                    p->child[2]->child[2] = NULL;

                                    p->child[2]->key[0] = p->child[2]->child[1]->getSmallest();
                                    p->child[2]->key[1] = p->child[2]->child[2]->getSmallest();
                                    q->key[0] = q->child[1]->getSmallest();
                                    q->key[1] = q->child[2]->getSmallest();

                                    q->child[1]->parent = q;

                                    flag = 1;
                                }
                            }
                            else if(q == p->child[2]) // q is third child and first/second child has 3 elements
                            {
                                if(p->child[1] != NULL && p->child[1]->child[2] != NULL)
                                {
                                    q->child[1] = q->child[0];
                                    q->child[0] = p->child[1]->child[2];
                                    p->child[1]->child[2] = NULL;

                                    p->child[1]->key[0] = p->child[1]->child[1]->getSmallest();
                                    p->child[1]->key[1] = p->child[1]->child[2]->getSmallest();
                                    q->key[0] = q->child[1]->getSmallest();
                                    q->key[1] = q->child[2]->getSmallest();

                                    q->child[0]->parent = q;
                                    
                                    flag = 1;
                                }
                                else if(p->child[2] != NULL && p->child[2]->child[2] != NULL)
                                {
                                    q->child[1] = q->child[0];
                                    q->child[0] = p->child[1]->child[1];
                                    p->child[1]->child[1] = p->child[1]->child[0];
                                    p->child[1]->child[0] = p->child[0]->child[2];
                                    p->child[0]->child[2] = NULL;

                                    p->child[1]->key[0] = p->child[1]->child[1]->getSmallest();
                                    p->child[1]->key[1] = p->child[1]->child[2]->getSmallest();
                                    q->key[0] = q->child[1]->getSmallest();
                                    q->key[1] = q->child[2]->getSmallest();

                                    q->child[0]->parent = q;
                                    p->child[1]->child[0]->parent = p->child[1];

                                    flag = 1;
                                }
                            }

                            if(!flag)
                            {
                                // If no sibling of q has 3 children
                                if(q == p->child[0])
                                {
                                    p->child[1]->child[2] = p->child[1]->child[1];
                                    p->child[1]->child[1] = p->child[1]->child[0];
                                    p->child[1]->child[0] = q->child[0];

                                    p->child[1]->key[0] = p->child[1]->child[1]->getSmallest();
                                    p->child[1]->key[1] = p->child[1]->child[2]->getSmallest();

                                    p->child[1]->child[0]->parent = p->child[1];
                                }
                                else if(q == p->child[1])
                                {
                                    p->child[0]->child[2] = q->child[0];
                                    p->child[0]->key[1] = p->child[0]->child[2]->getSmallest();
                                    p->child[0]->child[2]->parent = p->child[0];
                                }
                                else if(q == p->child[2])
                                {
                                    p->child[1]->child[2] = q->child[0];
                                    p->child[1]->key[1] = p->child[1]->child[2]->getSmallest();
                                    p->child[1]->child[2]->parent = p->child[0];
                                }
                            }
                            p = q;
                            q = q->parent;
                        }
                        else
                        {
                            // Return new root
                            q->child[0]->parent = NULL;
                            return q->child[0];
                        }
                    }		
                }

                while(q != NULL)
                {
                    q->key[0] = q->child[1]->getSmallest();
                    q->key[1] = q->child[2]->getSmallest();
                    q = q->parent;
                }
                return root;
            }
            else
            {
                printf("Key Doesn't Exist!\n");
                return root;
            }
        }

        // Print
        void print() {
            if (root == NULL) return;
            
            queue<TwoThreeNode *> q;
            q.push(root);

            while (1) {
                int currentLevel = q.size();
                if (currentLevel == 0)
                    break;
                
                while (currentLevel--) {
                    TwoThreeNode *n = q.front();
                    q.pop();
                    if (n->key[0] != -1 || n->key[1] != -1) {
                        cout << "(" << n->key[0];
                        if (n->key[1] != -1)
                            cout << ", " << n->key[1];
                        cout << ") ";
                    }
                    if (n->child[0] != NULL)
                        q.push(n->child[0]);
                    if (n->child[1] != NULL)
                        q.push(n->child[1]);
                    if (n->child[2] != NULL)
                        q.push(n->child[2]);
                }
                cout << endl;
            }
        }
};

// Main function
int main(int argc, char **argv) {
    int choice, key;
    TwoThreeTree ttTree;
    
    while (true) {
        cout << "Input choices:\n"
                "1 followed by space followed by the key to insert   1 (Key)\n"
                "2 followed by space followed by the key to delete   2 (Key)\n"
                "Exit from the program                               3\n";
        cin >> choice;

        switch (choice) {
        case 1:
            cin >> key;
            ttTree.insert(key);
            ttTree.print();
            break;
        case 2:
            cin >> key;
            ttTree.remove(key);
            ttTree.print();
            break;
        case 3:
            return 0;
        default:
            cout << "Invalid input";
            break;
        }
    }
    return 0;
}
