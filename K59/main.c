#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/jrb.h"

int checkArgc(int argc, char *argv[]);
int readFile(Graph g, char *filename);

int main(int argc, char *argv[])
{
    Graph g = createGraph();
    int check_Argc = checkArgc(argc, argv);
    if (check_Argc == 0)
    {
        printf("Nhap sai tham so dong lenh!\n");
        exit(0);
    }

    if (argc > 2)
        readFile(g, argv[2]);
    JRB node;
    switch (check_Argc)
    {
    case 1:
        printf("C Advanced, HK20152\n");
        break;

    case 2:
    {
        jrb_traverse(node, g.vertices)
        {
            printf("%d %s\n", jval_i(node->key), jval_s(node->val));
        }
    }
    break;
    case 3:
    {
        int ID1, ID2;
        ID1 = atoi(argv[3]);
        ID2 = atoi(argv[4]);

        double value = getEdgeValue(g, ID1, ID2);
        if (value == INFINITIVE_VALUE)
            printf("-1\n");
        else
            printf("Khoang cah giua %d va %d la: %.1f\n", ID1, ID2, value);
    }
    break;
    case 4:
    {
        int ID1, ID2;
        ID1 = atoi(argv[3]);
        ID2 = atoi(argv[4]);

        int path[1000];
        int numVer;
        double value = shortestPath(g, ID1, ID2, path, &numVer);

        if (value == INFINITIVE_VALUE)
            printf("-1\n");
        else
        {
            printf("\nKhoang cach: %.1f\n", value);
            for (int i = 0; i < numVer; i++)
            {
                printf("%s\n", getVertex(g, path[i]));
            }
        }
    }
    break;

    case 5:
    {
        int ID = atoi(argv[3]);
        int output[1000];
        int cnt = outdegree(g, ID, output);
        if (cnt <= 0)
            printf("Khong co duong truyen di may nao ca!\n");
        else
        {
            printf("\n%d\n", cnt);
            for (int i = 0; i < cnt; i++)
            {
                printf("%d %s %.0f\n", output[i], getVertex(g, output[i]), getEdgeValue(g, ID, output[i]));
            }
        }
    }
    break;
    case 6:
    {
        int n = argc - 3;

        printf("%d\n", n);
        int arr[1000];
        for (int i = 0; i < n; i++)
        {  
            arr[i] = atoi(argv[3+i]);
            printf("%d %s\n", atoi(argv[3+i]), getVertex(g, atoi(argv[3 + i])));
        }

        int cnt = 0;
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                int value = (int) getEdgeValue(g, arr[i], arr[j]);
                if (value != INFINITIVE_VALUE)
                    cnt++;
            }
        }
        printf("%d\n", cnt);
        for (int i = 0; i < n - 1; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                int value = (int) getEdgeValue(g, arr[i], arr[j]);
                if (value != INFINITIVE_VALUE)
                    printf("%d %d %d\n", arr[i], arr[j], value);
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

int readFile(Graph g, char *filename)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("File fail!\n");
        exit(1);
    }

    get_line(is);
    int m = atoi(is->fields[0]);

    for (int i = 0; i < m; i++)
    {
        get_line(is);
        addVertex(g, atoi(is->fields[0]), is->fields[1]);
    }

    get_line(is);
    int n = atoi(is->fields[0]);
    for (int i = 0; i < n; i++)
    {
        get_line(is);
        addEdge(g, atoi(is->fields[0]), atoi(is->fields[1]), atoi(is->fields[2]));
    }
}
int checkArgc(int argc, char *argv[])
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0)
            return 1;
    }
    if (argc == 3)
        if (strcmp(argv[1], "-v") == 0)
            return 2;

    if (argc == 5)
    {
        if (strcmp(argv[1], "-w") == 0)
            return 3;
        if (strcmp(argv[1], "-p") == 0)
            return 4;
    }
    if (argc == 4)
    {
        if (strcmp(argv[1], "-n") == 0)
            return 5;
    }
    if (strcmp(argv[1], "-s") == 0)
        return 6;
    return 0;
}