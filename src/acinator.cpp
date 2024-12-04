#include "../inc/acinator.h"
#include "../inc/ErrConsts.h"

static int    RecurcyDtor              (NODE * parent);
static int    StrDataToNodeData        (char * text, int * counter, NODE * node);
static NODE * RecursyAddNode           (NODE * node, char * text, int * counter);
static NODE * FillRightLeftNodes       (NODE * new_node, char * text, int * counter);
static NODE * Guesser                  (NODE * node);
static NODE * AddNode                  (NODE * node, FILE * stream);
static char * GetStr                   (FILE * stream);
static int    Verificator              (NODE * node);
static int    RecurcyDumpFill          (FILE * file, NODE * node);
static int    CompareTwoObj            (NODE * root);
static bool   Comparator               (NODE * node, char * str, Stack_t * answ_stack);
static int    DataBaseDestroyer        (const char * file_directory);
static int    SaveTreeToFile           (TREE * tree);
static int    RecurcyFileWriter        (FILE * base, NODE * node);
static int    PrintDifference          (Stack_t * obj_stack, bool first_answ, NODE * node, char * obj);
static int    GraphDump                (NODE * node);
static int    Dump                     (NODE * node);

int main ()
{
    TREE * tree = (TREE *) calloc (1, sizeof (TREE));
    my_assert (tree);

    TreeCtor (tree, data_base_file);

    FillTheTree (tree);

    StartAcination (tree);
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
                "2  -  if You want to compare two dishes\n"
                "3  -  if You want to see data base\n\n"
                "4  -  if You want to exit without save Your changes\n"
                "5  -  if You want to exit and save new data base\n\n"
                "6  -  if You want to destroy this data base\n\n");
        
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
            case DESTROY:
            {
                DataBaseDestroyer (tree->data_base_file);

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

    if (tree->root->left)  RecurcyDtor ((NODE *) tree->root->left);
    if (tree->root->right) RecurcyDtor ((NODE *) tree->root->right);

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

    if (parent->left) RecurcyDtor ((NODE *) parent->left);

    if (parent->right) RecurcyDtor ((NODE *) parent->right);

    parent->data   = NULL;

    free (parent->data);
    parent->parent = NULL;

    free (parent->left);
    parent->left   = NULL;

    free (parent->right);
    parent->right  = NULL;

    return 0;
}

int FillTheTree (TREE * tree)
{
    my_assert (tree);

    size_t file_size = 0;

#ifdef PRINT
    printf ("Start reading file...\n");
#endif

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

    tree->root->left = RecursyAddNode (tree->root, tree->text, &counter);                                     // Recurcy fill left

    while (isspace (tree->text[counter++]));

    tree->root->right = RecursyAddNode (tree->root, tree->text, &counter);                                    // Recurcy fill right

#ifdef PRINT
    printf ("Node right data %p\n",((NODE *) tree->root->right));
#endif

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

        new_node->left  = FillRightLeftNodes ((NODE *) new_node, text, counter);

#ifdef PRINT
        if (new_node->left)
            printf ("already readed no: %s\n", ((NODE *) new_node->left)->data);
#endif

        new_node->right = FillRightLeftNodes ((NODE *) new_node, text, counter);

#ifdef PRINT
        if (new_node->right)
            printf ("already readed yes: %s\n", ((NODE *) new_node->right)->data);
#endif;

#ifdef PRINT
        Dump (new_node);
#endif
        return new_node;
    }
    else
    {
        COLOR_PRINT (RED, "something went wrong while filling the tree\n");
        return 0;
    }

    return 0;
}

NODE * FillRightLeftNodes (NODE * new_node, char * text, int * counter)
{
    TREE_READ_ASSERT_ (text, counter);

    while (isspace (text[*counter]))
        (*counter)++;
        
    if (text[*counter] != '}')
    {
        new_node  = RecursyAddNode (new_node, text, counter);

        while (isspace (text[*counter]))
            (*counter)++;
            
        *counter += 1;
    }
    else
    {
        new_node = NULL;

        return new_node;
    }

    return new_node;
}

int StrDataToNodeData (char * text, int * counter, NODE * node)
{
    TREE_READ_ASSERT_ (text, counter);

#ifdef PRINT
    printf ("Readed data from file\t| \t");
#endif

    node->data = text + *counter;

    while (text[*counter] != '"')
        (*counter)++;
    
    text[*counter] = '\0';

    *counter += 1;

#ifdef PRINT
    printf ("%s\n", node->data);
#endif

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
    }
        
    printf ("\n");

    BufferCleaner ();

    switch (answer)
    {
        case 'y':
        {
            if (!node->right)
            {
                COLOR_PRINT (GREEN, "Yes, I did this, HaHaHa\n");
                COLOR_PRINT (GREEN, "Thank`s for playing\n\n");

                return 0;
            }
            else
                Guesser ((NODE *) node->right);
            break;
        }
        case 'n':
        {
            if (!node->left)
            {
                AddNode (node, stdin);
            }
            else
                Guesser ((NODE *) node->left);
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

// Матвей,я знаю, что функция говна, я переделаю
NODE * AddNode (NODE * node, FILE * stream)
{
    my_assert (node);
    my_assert (stream);

    printf ("Okey boy, You got me. I don`t know what is this. Tell me what is it?\n");

    node->right = (NODE *) calloc (1, sizeof (NODE));

    char * users_answ = GetStr (stdin);
    ADD_LAST_NODE_ ((NODE *) node->right, users_answ, node);

    node->left = (NODE *) calloc (1, sizeof (NODE));

    ADD_LAST_NODE_ ((NODE *) node->left, node->data, node);

    printf ("Now tell me, what are the differences between %s and %s?\n", ((NODE *) node->right)->data, ((NODE *) node->left)->data);

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

    strng[len] = fgetc (stream);

    while (strng[len] != '\n')
    {
        strng[++len] = fgetc (stream);

        if (len == mnim_len - 3)
        {
            mnim_len *= 2;

            strng = (char *) realloc (strng, mnim_len);
            my_assert (strng);
        }
    }

    strng[len] = '\0';

    strng = (char *) realloc (strng, len + 1);
    my_assert (strng);

    return strng;
}

int CompareTwoObj (NODE * root)
{
    my_assert (root);

    printf ("Enter first dish:\t");
    char * obj1 = GetStr (stdin);

    Stack_t obj1_stack = {};

    StackCtor (&obj1_stack, STANDART_SIZE);

    if (!Comparator (root, obj1, &obj1_stack))
    {
        printf ("Sorry, Your %s didn`t find\n", obj1);

        return 0;
    }

    printf ("\nEnter another dish:\t");
    char * obj2 = GetStr (stdin);

    printf ("\n\n");

    Stack_t obj2_stack = {};

    StackCtor (&obj2_stack, STANDART_SIZE);

    if (!Comparator (root, obj2, &obj2_stack))
    {
        printf ("Sorry, Your %s didn`t find\n", obj2);

        return 0;
    }

    bool first_answ  = 0;
    bool second_answ = 0;

    COLOR_PRINT (GREEN, "This is how they are similar:\n");

    while (obj1_stack.size > 0 && obj2_stack.size > 0)
    {
        first_answ  = StackPop (&obj1_stack);
        second_answ = StackPop (&obj2_stack);

        if (first_answ != second_answ)
        {
            break;
        }

        if (!first_answ)
        {
            printf ("They are not %s\n", root->data);
            root = (NODE *) root->left;
        }
        else
        {
            printf ("They are %s\n", root->data);
            root = (NODE *) root->right;
        }
    }

    COLOR_PRINT (MANGETA, "Now there are difference between %s and %s\n", obj1, obj2);
    COLOR_PRINT (CYAN, "Let`s start from %s\n", obj1);

    NODE * node_obj1 = root;
    NODE * node_obj2 = root;

    PrintDifference (&obj1_stack, first_answ, node_obj1, obj1);

    COLOR_PRINT (YELLOW, "And begin with %s:\n", obj2);

    PrintDifference (&obj2_stack, second_answ, node_obj2, obj2);

    return  0;
}

int PrintDifference (Stack_t * obj_stack, bool first_answ, NODE * node, char * obj)
{
    my_assert (obj_stack);
    my_assert (node);

        if (!first_answ)
    {
        COLOR_PRINT (CYAN, "\t%s is not %s\n", obj, node->data);
        node = (NODE *) node->left;
    }
    else
    {
        COLOR_PRINT (CYAN, "\tIt is %s\n", node->data);
        node = (NODE *) node->right;
    }

    while (obj_stack->size > 0)
    {
        first_answ  = StackPop (obj_stack);

        if (!first_answ)
        {
            COLOR_PRINT (CYAN, "\t%s is not %s\n", obj, node->data);
            node = (NODE *) node->left;
        }
        else
        {
            COLOR_PRINT (CYAN, "\tIt is %s\n", node->data);
            node = (NODE *) node->right;
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
    
    if (Comparator ((NODE *) node->left, str, answ_stack))
    {
        StackPush (answ_stack, LEFT);

        return GOT_ELEM;
    }

    if (Comparator ((NODE *) node->right, str, answ_stack))
    {
        StackPush (answ_stack, RIGHT);

        return GOT_ELEM;
    }

    return DIDNT_FIND;
}

int DataBaseDestroyer (const char * file_name)
{
    my_assert (file_name);

    system ("rm");
    system (file_name);

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
        RecurcyFileWriter (base, (NODE *) tree->root->left);
    }
    if (tree->root->right)
    {
        RecurcyFileWriter (base, (NODE *) tree->root->right);
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
        RecurcyFileWriter (base, (NODE *) node->left);
    }

    if (node->right)
    {
        RecurcyFileWriter (base, (NODE *) node->right);
    }

    fprintf (base, "}\n");

    return 0;
}

int Verificator (NODE * node)
{
    int error = 0;

    if (!node)
    {
        error |= NODE_IS_NULL;
        printf ("pointer to node is NULL\n");
    }

    if (!node->data)
    {
        error |= NODE_DATA_NULL;
    }

    return error;
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

    if (node->left) RecurcyDumpFill (file, (NODE *) node->left);
    if (node->right) RecurcyDumpFill (file, (NODE *) node->right);

    if (node->left)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->left);
    if (node->right)
        fprintf (file, "\tQ%p -> Q%p\n", node, node->right);
    fprintf (file, "\n");

    return 0;
}