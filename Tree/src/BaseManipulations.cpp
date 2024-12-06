#include "../inc/tree.h"

int FillTheTree (TREE * tree)
{
    my_assert (tree);

    size_t file_size = 0;

    tree->text = FileToStr (tree->data_base_file, &file_size);

    int counter = 0;

    if (tree->text[counter++] == '{')
    {
        counter++;
        StrDataToNodeData (tree->text, &counter, tree->root);
    }
    else
    {
        COLOR_PRINT (RED, "Wrong data type before root\n");
        return -1;
    }

    tree->root->left  = RecursyAddNode (tree->root, tree->text, &counter);                                     // Recurcy fill left
    tree->root->right = RecursyAddNode (tree->root, tree->text, &counter);                                     // Recurcy fill right

    return 0;
}

NODE * RecursyAddNode (NODE * node, char * text, int * counter)
{
    TREE_READ_ASSERT_ (text, counter)

    while (isspace (text[*counter]))
        (*counter)++;

    if (text[*counter] == '{')
    {
        *counter += 2;                                                              // Skip {"

        NODE * new_node = (NODE *) calloc (1, sizeof (NODE));

        new_node->parent = node;

        StrDataToNodeData (text, counter, new_node);

        new_node->left  = RecursyAddNode (new_node, text, counter);

        new_node->right = RecursyAddNode (new_node, text, counter);

        while (isspace (text[*counter]))
            (*counter)++;

        *counter += 1;

        return new_node;
    }
    else if (text[*counter] == '}')
    {
        return NULL;
    }
    else
    {
        COLOR_PRINT (RED, "something went wrong while filling the tree\n");
        return 0;
    }

    return NULL;
}

int StrDataToNodeData (char * text, int * counter, NODE * node)
{
    TREE_READ_ASSERT_ (text, counter);

    node->data = text + *counter;

    while (text[*counter] != '"')
        (*counter)++;
    
    text[*counter] = '\0';

    *counter += 1;

    return 0;
}

int SaveTreeToFile (TREE * tree)
{
    TREE_READ_ASSERT_ (tree->data_base_file, tree->root)

    FILE * base = fopen (tree->data_base_file, "w+");
    my_assert (base);

    fprintf (base, "{\"%s\"\n", tree->root->data);

    if (tree->root->left)
    {
        RecurcyFileWriter (base, tree->root->left);
    }
    if (tree->root->right)
    {
        RecurcyFileWriter (base, tree->root->right);
    }
    fprintf (base, "}");

    my_assert (!fclose (base));

    return 0;
}

int RecurcyFileWriter (FILE * base, NODE * node)
{
    my_assert (base);
    my_assert (node);

    fprintf (base, "\t{\"%s\"\n", node->data);

    if (node->left)
    {
        RecurcyFileWriter (base, node->left);
    }

    if (node->right)
    {
        RecurcyFileWriter (base, node->right);
    }

    fprintf (base, "}\n");

    return 0;
}
