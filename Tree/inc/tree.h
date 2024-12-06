#ifndef TREE_H
#define TREE_N

#include <string.h>
#include <stdio.h>
#include "../../inc/Errors.h"

typedef char * nodeElem;

typedef struct NODE
{
    NODE * left   = NULL;
    NODE * right  = NULL;

    NODE * parent = NULL;

    nodeElem data = NULL;
} NODE;

typedef struct
{
    NODE * root = NULL;

    char * text = NULL;

    const char * data_base_file = "./Bases/base1.txt";

    bool status = 0;
} TREE;

int    TreeCtor               (TREE * tree, const char * data_base_file);
int    TreeDtor               (NODE * parent);
int    RecurcyDtor            (NODE * parent);
int    FillTheTree            (TREE * tree);
int    StrDataToNodeData      (char * text, int * counter, NODE * node);
NODE * RecursyAddNode         (NODE * node, char * text, int * counter);
int    SaveTreeToFile         (TREE * tree);
int    RecurcyFileWriter      (FILE * base, NODE * node);

#endif