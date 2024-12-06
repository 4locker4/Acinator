#include "../inc/acinator.h"
#include "../inc/ErrConsts.h"

static NODE * Guesser                (NODE * node);
static NODE * AddNode                (NODE * node, FILE * stream);
static char * GetStr                 (FILE * stream);
static int    RecurcyDumpFill        (FILE * file, NODE * node);
static int    CompareTwoObj          (NODE * root);
static bool   Comparator             (NODE * node, char * str, Stack_t * answ_stack);
static int    PrintDifference        (Stack_t * obj_stack, NODE * node, char * obj);
static int    GraphDump              (NODE * node);
static int    Dump                   (NODE * node);
static char * GetStackPathToObj      (Stack_t * obj_stack, NODE * root, char * obj);
NODE *        NodeCreater            (char * node_data, NODE * left, NODE * right, NODE * parent);

static const char * data_base_file = "./Bases/base1.txt";

int main ()
{
    TREE tree = {};

    TreeCtor (&tree, data_base_file);

    FillTheTree (&tree);

    StartAcination (&tree);
}

int StartAcination (TREE * tree)
{
    if (!tree->status)
    {
        printf ("First create a tree\n");

        return 0;
    }
    COLOR_PRINT (GREEN, "\nHello! I`m Acinator. I can do some tricks. Look!\n\n");
    
    while (true)
    {
        printf ("1  -  if You want me to pick up a dish for You\n"
                "2  -  if You want to determ object\n"
                "3  -  if You want to compare two dishes\n"
                "4  -  if You want to see data base\n\n"
                "5  -  if You want to exit without save Your changes\n"
                "6  -  if You want to exit and save new data base\n\n");
        
        int action = 0;

        if (!scanf ("%d", &action))
        {
            COLOR_PRINT (RED, "You entered wrong data\n");

            BufferCleaner ();

            continue;
        }

        BufferCleaner ();

        switch (action)
        {
            case MAKE_A_DISH:
            {
                printf ("\n");

                Guesser (tree->root);
                break;
            }
            case DETERM:
            {
                Stack_t obj_stack = {};

                char *  user_answ = NULL;

                printf ("Enter the dish:\t");

                user_answ = GetStackPathToObj (&obj_stack, tree->root, user_answ);

                printf ("\n");
                if (user_answ) PrintDifference (&obj_stack, tree->root, user_answ);

                break;
            }
            case COMPARE:
            {
                CompareTwoObj (tree->root);

                break;
            }
            case CHECK_BASE:
            {
                GraphDump (tree->root);
                break;
            }
            case EXIT_WITHOUT_SAVE:
            {
                return 1;
            }
            case EXIT_AND_SAVE:
            {
                SaveTreeToFile (tree);

                break;
            }
            default:
            {
                COLOR_PRINT (RED, "You entered wrong num, sorry\n");
            }
        }
    }    

    return 0;

}

NODE * Guesser (NODE * node)
{
    VERIFY (node);

    char answer = 0;

    printf ("Do you want some %s\n[y\\n]\t", node->data);

    if (!scanf ("%c", &answer))
    {
        printf ("There is a mistake while reading your answer, try again\n");

        BufferCleaner ();

        Guesser (node);
        return 0;
    }
        
    printf ("\n");

    BufferCleaner ();

    switch (answer)
    {
        case USER_SAY_YES:
        {
            if (!node->right)
            {
                COLOR_PRINT (GREEN, "Yes, I did this, HaHaHa\n");
                COLOR_PRINT (GREEN, "Thank`s for playing\n\n");

                return 0;
            }
            else
                Guesser (node->right);
            break;
        }
        case USER_SAY_NO:
        {
            if (!node->left)
            {
                AddNode (node, stdin);
            }
            else
                Guesser (node->left);
            break;
        }
        default:
        {
            printf ("Please, put only 'y' or 'n'\nYou put %c\n", answer);

            Guesser (node);
        }
    }

    return 0;
}

NODE * AddNode (NODE * node, FILE * stream)
{
    my_assert (node);
    my_assert (stream);

    printf ("Okey boy, You got me. I don`t know what is this. Tell me what is it?\n");

    char * users_answ = GetStr (stdin);
    node->right = NodeCreater (users_answ, NULL, NULL, node);

    node->left  = NodeCreater (node->data, NULL, NULL, node);

    printf ("Now tell me, what are the differences between %s and %s?\n", (node->right)->data, (node->left)->data);

    node->data = GetStr (stdin);

    printf ("Nice, next time I will guess this meal\n");

    return 0;
}

char * GetStr (FILE * stream)
{
    my_assert (stream);

    size_t len = 0;
    size_t mnim_len = START_STR_SIZE;

    char * strng = (char *) calloc (mnim_len, sizeof (char));
    my_assert (strng);

    while ((strng[len] = fgetc (stream)) != '\n')
    {
        len++;

        if (len == mnim_len)
        {
            mnim_len *= 2;

            strng = (char *) realloc (strng, mnim_len);
            my_assert (strng);
        }
    }

    strng[len] = '\0';

    strng = (char *) realloc (strng, len + 1);
    my_assert (strng);

    printf ("%s\n", strng);

    return strng;
}

int CompareTwoObj (NODE * root)
{
    my_assert (root);

    printf ("Enter first dish:\t");

    Stack_t obj1_stack = {};
    char * obj1 = GetStackPathToObj (&obj1_stack, root, obj1);

    printf ("\nEnter another dish:\t");

    Stack_t obj2_stack = {};
    char * obj2 = GetStackPathToObj (&obj2_stack, root, obj2);

    printf ("\n\n");

    bool first_answ  = 0;
    bool second_answ = 0;

    COLOR_PRINT (GREEN, "This is how they are similar:\n");

    while (obj1_stack.size > 0 && obj2_stack.size > 0)
    {
        first_answ  = StackPop (&obj1_stack);
        second_answ = StackPop (&obj2_stack);

        if (first_answ != second_answ)
        {
            StackPush (&obj1_stack, first_answ);
            StackPush (&obj2_stack, second_answ);

            break;
        }

        if (!first_answ)
        {
            printf ("They are not %s\n", root->data);
            root = root->left;
        }
        else
        {
            printf ("They are %s\n", root->data);
            root = root->right;
        }
    }

    COLOR_PRINT (MANGETA, "Now there are difference between %s and %s\n", obj1, obj2);
    COLOR_PRINT (CYAN, "Let`s start from %s\n", obj1);

    NODE * node_obj1 = root;
    NODE * node_obj2 = root;

    PrintDifference (&obj1_stack, node_obj1, obj1);

    COLOR_PRINT (YELLOW, "And begin with %s:\n", obj2);

    PrintDifference (&obj2_stack, node_obj2, obj2);

    return  0;
}

char * GetStackPathToObj (Stack_t * obj_stack, NODE * root, char * obj)
{
    my_assert (obj_stack);
    my_assert (root);

    obj = GetStr (stdin);

    StackCtor (obj_stack, STANDART_SIZE);

    if (!Comparator (root, obj, obj_stack))
    {
        printf ("Sorry, Your %s didn`t find\n", obj);

        return 0;
    }

    return obj;
}

int PrintDifference (Stack_t * obj_stack, NODE * node, char * obj)
{
    my_assert (obj_stack);
    my_assert (node);

    bool first_answ = 0;

    while (obj_stack->size > 0)
    {
        first_answ  = StackPop (obj_stack);

        if (!first_answ)
        {
            COLOR_PRINT (CYAN, "\t%s is not %s\n", obj, node->data);
            node = node->left;
        }
        else
        {
            COLOR_PRINT (CYAN, "\tIt is %s\n", node->data);
            node = node->right;
        }
    }

    return 0;
}

bool Comparator (NODE * node, char * str, Stack_t * answ_stack)
{
    my_assert (str);

    if (!node)
        return DIDNT_FIND;

    if (!strcasecmp (str, node->data))
        return GOT_ELEM;
    
    if (Comparator (node->left, str, answ_stack))
    {
        StackPush (answ_stack, LEFT);

        return GOT_ELEM;
    }

    if (Comparator (node->right, str, answ_stack))
    {
        StackPush (answ_stack, RIGHT);

        return GOT_ELEM;
    }

    return DIDNT_FIND;
}

int Dump (NODE * node)
{
    my_assert (node);

    FILE * dump = fopen (DUMP_FILE, "a+");
    my_assert (dump);

    fprintf (dump, "        parent: %p      \n\n"
                   "        node:   %p      \n"
                   "        data:   %s      \n"
                   "left: %p                right: %p\n"
                   "---------------------------------------\n", node->parent, node, node->data, node->left, node->right);
    my_assert (!fclose (dump));

    return 0;
}

int GraphDump (NODE * node)
{
    my_assert (node);

#ifdef PRINT
    printf ("\n\n\nIn graph dump\n\n\n");
#endif

    FILE * dump = fopen (GRAPH_DUMP_FILE, "w+");
    my_assert (dump);

    fprintf (dump, "digraph list{\n"
                    "\trankdir=LR;\n"
                    "\tbgcolor=\"#FFFFFF\";"
                    "\tnode[color=\"black\", fontsize=14];\n"
                    "\tfixedsize=true;\n");

    RecurcyDumpFill (dump, node);

    fprintf (dump, "}\n");

    my_assert (!fclose (dump));

    system ("dot -Tsvg ../Acinator/Logs/GraphDump.dot -o ../Acinator/Logs/GraphDump.svg");

    system ("ren ../Acinator/Logs/GraphDump.svg GraphDump.html");

    return 0;
}

int RecurcyDumpFill (FILE * file, NODE * node)
{
    my_assert (file);
    my_assert (node);

    const char * color = GOD_SAY_YES_COLOR;

    if (!node->left && !node->right)
        color = GOD_SAY_NO_COLOR;

    fprintf (file, "\tQ%p[style=filled, shape=record, fillcolor=\"%s\", width=2, label =\""
                   "{%s | {{parent | %p} | {left | %p} | {right | %p}}}\" ]\n",
                   node, color, node->data, node->parent, node->left, node->right);

    if (node->left) RecurcyDumpFill (file, node->left);
    if (node->right) RecurcyDumpFill (file, node->right);

    if (node->left)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->left);
    if (node->right)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->right);
    fprintf (file, "\n");

    return 0;
}