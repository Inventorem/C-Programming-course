#include <stdio.h>
#include <stdlib.h>

// AVL Nodes Struct
typedef struct AVL_Node {
    char height;
    int value;
    struct AVL_Node* left;
    struct AVL_Node* right;
} Node;

char max(char a, char b) {
    return a > b ? a : b;
}

char GetHeight(Node* node) {
    return !node ? 0 : node->height;
}

char CalcHeight(Node* node) {
    if (!node)
        return 0;
    return (1 + max(GetHeight(node->left), GetHeight(node->right)));
}

Node* LeftRotation(Node* root) {
    Node* new = root->right;
    root->right = new->left;
    new->left = root;

    root->height = CalcHeight(root);
    new->height = CalcHeight(new);

    return new;
}

Node* RightRotation(Node* root) {
    Node* new = root->left;
    root->left = new->right;
    new->right = root;

    root->height = CalcHeight(root);
    new->height = CalcHeight(new);

    return new;
}

char GetBalance(Node* root) {
    if (!root)
        return 0;
    return CalcHeight(root->left) - CalcHeight(root->right);
}

Node* BalanceTree(Node* root) {
    root->height = CalcHeight(root);

    if (GetBalance(root) == -2) {
        if (root->right && GetBalance(root->right) > 0)
            root->right = RightRotation(root->right);
        return LeftRotation(root);
    }

    if (GetBalance(root) == 2) {
        if (root->left && GetBalance(root->left) < 0)
            root->left = LeftRotation(root->left);
        return RightRotation(root);
    }

    return root;
}

Node* AddNode(Node* root, Node* node, int key, int index) {
    // If node is new
    if (!node) {
        root[index].height = 1;
        root[index].value = key;

        root[index].left = NULL;
        root[index].right = NULL;

        node = &root[index];
        return node;
    }

    if (key <= node->value)
        node->left = AddNode(root, node->left, key, index);
    else
        node->right = AddNode(root, node->right, key, index);

    return BalanceTree(node);
}

// Clear "non-array" tree
void FreeTree(Node* root) {
    if (!root)
        return;
    FreeTree(root->left);
    FreeTree(root->right);
    free(root);
}

int main() {
    int n = 0;

    // Check parameters
    if (!scanf("%d", &n))
        return 0;
    if (n == 0) {
        printf("0");
        return 0;
    }

    // Create tree
    Node* avl_root = (Node*)malloc(sizeof(Node) * n);
    Node* avl_tree = NULL;

    // Add values
    int value = 0;
    for (int i = 0; i < n; i++) {
        if (!scanf("%d", &value))
            return 0;
        avl_tree = AddNode(avl_root, avl_tree, value, i);
    }

    // Calc and output the height
    printf("%d", CalcHeight(avl_tree));

    free(avl_root);
    return EXIT_SUCCESS;
}
