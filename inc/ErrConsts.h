#ifndef ERR_CONSTS_H
#define ERR_CONSTS_H

const char * data_base_file = "./Bases/base1.txt";

const char * err_text[] = 
                {
                    "There is null pointer to node\n",
                    ""
                };

enum ERRORS_NUMS
{
    OK              = 0,
    NODE_IS_NULL    = 1 << 0,
    NODE_DATA_NULL  = 1 << 1,
};

enum ACTION
{
    MAKE_A_DISH = 1,
    COMPARE,
    CHECK_BASE,
    EXIT_WITHOUT_SAVE,
    EXIT_AND_SAVE,
    DESTROY
};

#endif