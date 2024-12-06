/// @file ACINATOR_H
#ifndef ACINATOR_H
#define ACINATOR_H

#include <string.h>

#include "Utils.h"
#include "Errors.h"
#include "ErrConsts.h"
#include "../Tree/inc/tree.h"

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

const int    USER_SAY_NO        = 'n';
const int    USER_SAY_YES       = 'y';

#define VERIFY(node);

enum ACTION
{
    MAKE_A_DISH = 1,
    DETERM,
    COMPARE,
    CHECK_BASE,
    EXIT_WITHOUT_SAVE,
    EXIT_AND_SAVE,
};

int    StartAcination           (TREE * tree);

#endif // ACINATOR_H