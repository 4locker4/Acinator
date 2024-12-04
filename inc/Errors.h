///@file Errors.h
#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "Utils.h"

#define my_assert(expr) if (!(expr))                                                                \
                        {                                                                           \
                            printf ("ERROR: \"" #expr "\", file %s, function %s, line %d\n",        \
                                        __FILE__, __func__, __LINE__);                              \
                            exit (1);                                                               \
                        }

#define CHECKED_(expr) if (!(expr))                                                                 \
                            {                                                                       \
                            COLOR_PRINT (RED, "ERROR, your error`s number is: <%d>\n",              \
                                              "Information:\n",                                     \
                                              "File: <%s>           Line: <%d>\n",                  \
                                               expr, __FILE__, __LINE__)}

#define TREE_READ_ASSERT_(text, counter) if (!(text && counter))                                    \
                        {                                                                           \
                            if (!(text))                                                            \
                            {                                                                       \
                                printf ("ERROR: \"" #text "\", file %s, function %s, line %d\n",    \
                                        __FILE__, __func__, __LINE__);                              \
                                exit (1);                                                           \
                            }                                                                       \
                            else if (!(counter))                                                    \
                            {                                                                       \
                                printf ("ERROR: \"" #counter "\", file %s, function %s, line %d\n", \
                                        __FILE__, __func__, __LINE__);                              \
                                exit (1);                                                           \
                            }                                                                       \
                        }

#endif