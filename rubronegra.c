#include <stdio.h>
#include <stdlib.h>

/* ===================== ENUM COR ===================== */
typedef enum { RED, BLACK } Color;

/* ===================== ESTRUTURA DO NÓ ===================== */
typedef struct RBNode {
    int data;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

/* ===================== VARIÁVEIS GLOBAIS ===================== */
RBNode *root = NULL;
RBNode *NIL;

/* ===================== FUNÇÃO CRIAR NÓ ===================== */
RBNode* createNode(int data) {
    RBNode *node = (RBNode*) malloc(sizeof(RBNode));
    node->data = data;
    node->color = RED;
    node->left = node->right = node->parent = NIL;
    return node;
}

/* ===================== ROTAÇÕES ===================== */
void leftRotate(RBNode **root, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void rightRotate(RBNode **root, RBNode *x) {
    RBNode *y = x->left;
    x->left = y->right;

    if (y->right != NIL)
        y->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

/* ===================== FIX INSERT ===================== */
void fixInsert(RBNode **root, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left;
            if (y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
}

/* ===================== INSERÇÃO ===================== */
void insert(int data) {
    RBNode *z = createNode(data);
    RBNode *y = NIL;
    RBNode *x = root;

    while (x != NIL) {
        y = x;
        if (z->data < x->data)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == NIL)
        root = z;
    else if (z->data < y->data)
        y->left = z;
    else
        y->right = z;

    fixInsert(&root, z);
}

/* ===================== BUSCA ===================== */
RBNode* search(RBNode *node, int key) {
    if (node == NIL || key == node->data)
        return node;
    if (key < node->data)
        return search(node->left, key);
    return search(node->right, key);
}

/* ===================== TRANSPLANTE ===================== */
void transplant(RBNode **root, RBNode *u, RBNode *v) {
    if (u->parent == NIL)
        *root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

/* ===================== MÍNIMO ===================== */
RBNode* minimum(RBNode *node) {
    while (node->left != NIL)
        node = node->left;
    return node;
}

/* ===================== FIX DELETE ===================== */
void fixDelete(RBNode **root, RBNode *x) {
    while (x != *root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
        }
    }
    x->color = BLACK;
}

/* ===================== REMOÇÃO ===================== */
void delete(int key) {
    RBNode *z = search(root, key);
    if (z == NIL) {
        printf("Valor nao encontrado.\n");
        return;
    }

    RBNode *y = z;
    RBNode *x;
    Color yOriginalColor = y->color;

    if (z->left == NIL) {
        x = z->right;
        transplant(&root, z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(&root, z, z->left);
    } else {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y;
        else {
            transplant(&root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(&root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);
    if (yOriginalColor == BLACK)
        fixDelete(&root, x);
}

/* ===================== TRAVESSIAS ===================== */
void preorder(RBNode *node) {
    if (node != NIL) {
        printf("%d ", node->data);
        preorder(node->left);
        preorder(node->right);
    }
}

void inorder(RBNode *node) {
    if (node != NIL) {
        inorder(node->left);
        printf("%d ", node->data);
        inorder(node->right);
    }
}

void postorder(RBNode *node) {
    if (node != NIL) {
        postorder(node->left);
        postorder(node->right);
        printf("%d ", node->data);
    }
}

/* ===================== MENU ===================== */
void menu() {
    int op, value;
    do {
        printf("\n--- ARVORE RUBRO-NEGRA ---\n");
        printf("1. Inserir\n");
        printf("2. Buscar\n");
        printf("3. Remover\n");
        printf("4. Pre-Ordem\n");
        printf("5. Em Ordem\n");
        printf("6. Pos-Ordem\n");
        printf("0. Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1:
                printf("Valor: ");
                scanf("%d", &value);
                insert(value);
                break;
            case 2:
                printf("Valor: ");
                scanf("%d", &value);
                if (search(root, value) != NIL)
                    printf("Encontrado!\n");
                else
                    printf("Nao encontrado.\n");
                break;
            case 3:
                printf("Valor: ");
                scanf("%d", &value);
                delete(value);
                break;
            case 4:
                preorder(root);
                printf("\n");
                break;
            case 5:
                inorder(root);
                printf("\n");
                break;
            case 6:
                postorder(root);
                printf("\n");
                break;
        }
    } while (op != 0);
}

/* ===================== MAIN ===================== */
int main() {
    NIL = (RBNode*) malloc(sizeof(RBNode));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NULL;
    root = NIL;

    menu();
    return 0;
}
