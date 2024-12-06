#include "../inc/tree.h"

int TreeCtor (TREE * tree, const char * data_base_file)
{
    my_assert (data_base_file);
    my_assert (tree);

    tree->root = (NODE *) calloc (1, sizeof (NODE));
    my_assert (tree->root);

    tree->root->parent = NULL;

    tree->status = true;
    tree->data_base_file = data_base_file;

    return 0;
}

int TreeDtor (TREE * tree)
{
    my_assert (tree);

    tree->status = false;
    
    free (tree->text);

    if (tree->root->left)  RecurcyDtor (tree->root->left);
    if (tree->root->right) RecurcyDtor (tree->root->right);

    free (tree->root->left);
    tree->root->left  = NULL;

    free (tree->root->right);
    tree->root->right = NULL;

    tree->root->data  = NULL;

    free (tree->root);

    return 0;
}

int RecurcyDtor (NODE * parent)
{
    my_assert (parent);

    if (parent->left) RecurcyDtor (parent->left);

    if (parent->right) RecurcyDtor (parent->right);

    parent->data   = NULL;

    free (parent->data);
    parent->parent = NULL;

    free (parent->left);
    parent->left   = NULL;

    free (parent->right);
    parent->right  = NULL;

    return 0;
}

NODE * NodeCreater (char * node_data, NODE * left, NODE * right, NODE * parent)
{
    my_assert (node_data);
    my_assert (parent);

    NODE * node = (NODE *) calloc (1, sizeof (NODE));
    my_assert (node);

    node->data   = node_data;
    node->left   = left;
    node->right  = right;
    node->parent = parent;

    return node;
}