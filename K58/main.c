#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

int checkArgc(int argc, char *argv[]);
int readFile(Graph g, char *filename);

int main(int argc, char *argv[])
{
    int check = checkArgc(argc, argv);
    if (check == 0)
    {
        printf("Command Fail!\n");
        exit(0);
    }

    Graph g = createGraph();
    if (argc > 2)
        readFile(g, argv[2]);

    switch (check)
    {
    case -1:
        printf("Supported command: help, i2n, n2i, cc, cr, odg, rel.\n");
        break;
    case -2:
        printf("C advanced 20142 final exam.\n");
        break;
    case 2:
    {
        int ID = atoi(argv[3]);
        printf("%s\n", getVertex(g, ID));
    }
    break;
    case 3:
    {
        JRB node;
        jrb_traverse(node, g.vertices)
        {
            if (strcmp(jval_s(node->val), argv[3]) == 0)
                printf("%d\n", jval_i(node->key));
        }
    }
    break;
    case 4:
    {
        int ID1 = atoi(argv[3]);
        int ID2 = atoi(argv[4]);
        int output[100], out;
        int input[100], in;
        out = outdegree(g, ID1, output);
        int save[100], cnt_save = 0;
        for (int i = 0; i < out; i++)
        {
            in = indegree(g, output[i], input);
            for (int j = 0; j < in; j++)
                if (input[j] == ID2)
                    save[cnt_save++] = output[i];
        }

        for (int i = 0; i < cnt_save; i++)
            printf("%s\n", getVertex(g, save[i]));
    }
    break;
    case 5:
    {
        int ID1 = atoi(argv[3]);
        int ID2 = atoi(argv[4]);
        int output[100], out;
        int input[100], in;
        out = indegree(g, ID1, output);
        int save[100], cnt_save = 0;
        for (int i = 0; i < out; i++)
        {
            in = outdegree(g, output[i], input);
            for (int j = 0; j < in; j++)
                if (input[j] == ID2)
                    save[cnt_save++] = output[i];
        }

        for (int i = 0; i < cnt_save; i++)
            printf("%s\n", getVertex(g, save[i]));
    }
    break;
    case 6:
    {
        int value = atoi(argv[3]);
        JRB node;
        int output[100];
        jrb_traverse(node, g.vertices)
        {
            if (outdegree(g, jval_i(node->key), output) > value)
                printf("%s\n", jval_s(node->val));
        }
    }
    break;
    case 7:
    {
        int ID = atoi(argv[3]);
        int output[100], output2[100];
        int out, out2;

        out = outdegree(g, ID, output);
        for (int i = 0; i < out; i++)
        {
            printf("%s\n", getVertex(g, output[i]));
            out2 = outdegree(g, output[i], output2);
            for (int i = 0; i < out2; i++)
            {
                printf("%s\n", getVertex(g, output2[i]));
            }
        }
    }
    break;
    default:
        break;
    }

    dropGraph(g);
    return 0;
}

int checkArgc(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "help") == 0)
            return -1;
        if (strcmp(argv[1], "about") == 0)
            return -2;
    }
    if (argc == 4)
    {
        if (strcmp(argv[1], "i2n") == 0)
            return 2;
        if (strcmp(argv[1], "n2i") == 0)
            return 3;
        if (strcmp(argv[1], "odg") == 0)
            return 6;
        if (strcmp(argv[1], "rel") == 0)
            return 7;
    }
    if (argc == 5)
    {
        if (strcmp(argv[1], "cc") == 0)
            return 4;
        if (strcmp(argv[1], "cr") == 0)
            return 5;
    }
    return 0;
}

int readFile(Graph g, char *filename)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("File fail!\n");
        exit(1);
    }

    get_line(is);
    int n = atoi(is->fields[0]);
    int m = atoi(is->fields[1]);

    for (int i = 0; i < n; i++)
    {
        get_line(is);
        addVertex(g, atoi(is->fields[1]), is->fields[0]);
    }

    for (int i = 0; i < m; i++)
    {
        get_line(is);
        addEdge(g, atoi(is->fields[0]), atoi(is->fields[1]), 1);
    }
}
