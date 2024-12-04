/// @file ACINATOR_H
#ifndef ACINATOR_H
#define ACINATOR_H

#include <string.h>

#include "Utils.h"
#include "Errors.h"
#include "ErrConsts.h"

#include "../Stack/inc/Stack.h"
#include "../Stack/src/Defines.h"

const int START_STR_SIZE = 48;

const char * ACINATOR_DATA      = "../Acinator/AcinatorData.txt";

const char * DUMP_FILE          = "Logs/Dump.txt";
const char * GRAPH_DUMP_FILE    = "Logs/GraphDump.dot";

const char * GOD_SAY_NO_COLOR   = "#FF7B61";
const char * GOD_SAY_YES_COLOR  = "#B2EC5D";
const char * ROOT_COLOR         = "#9794F9";

const char * ROOT_DATA          = "Soup";

const bool   GOT_ELEM           = true;
const bool   DIDNT_FIND         = false;

const bool   LEFT               = false;
const bool   RIGHT              = true;

#define VERIFY(node);

typedef char nodeElem;

typedef struct
{
    void * left   = NULL;
    void * right  = NULL;

    void * parent = NULL;

    nodeElem * data = NULL;
} NODE;

typedef struct
{
    NODE * root = NULL;

    const char * data_base_file = NULL;

    bool status = 0;
} TREE;

int    TreeCtor                 (TREE * tree, const char * data_base_file);
int    TreeDtor                 (NODE * parent);
int    StartAcination           (TREE * tree);
int    FillTheTree              (TREE * tree);
int    StrDataToNodeData        (char * text, int * counter, NODE * node);
int    GraphDump                (NODE * node);
NODE * RecursyAddNode           (NODE * node, char * text, int * counter);
int    Dump                     (NODE * node);
NODE * FillRightLeftNodes       (NODE * new_node, char * text, int * counter);
NODE * Guesser                  (NODE * node);
int    StartAcination           (TREE * tree);
NODE * AddNode                  (NODE * node, FILE * stream);
char * GetStr                   (FILE * stream);
int    Verificator              (NODE * node);
int    RecurcyDumpFill          (FILE * file, NODE * node);
int    CompareTwoObj            (NODE * root);
bool   Comparator               (NODE * node, char * str, Stack_t * answ_stack);
int    DataBaseDestroyer        (const char * file_directory);
int    SaveTreeToFile           (TREE * tree);
int    RecurcyFileWriter        (FILE * base, NODE * node);

#endif // ACINATOR_H