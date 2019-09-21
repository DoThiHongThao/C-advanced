#include <stdio.h>
#include <stdlib.h>
#include "jrb.h"

typedef JRB Graph;

Graph createGraph()
{
    return make_jrb();
}

int adjacent(Graph g, int v1, int v2)
{
    JRB find_v1 = jrb_find_int(g, v1);
    if (find_v1 == NULL)
        return 0;

    JRB tree = (JRB)jval_v(find_v1->val);
    if (jrb_find_int(tree, v2) == NULL)
        return 0;
    return 1;
}

// void addEdge(Graph g, int v1, int v2)
// {
//     JRB tree;
//     JRB find_v1 = jrb_find_int(g, v1);
//     if (find_v1 == NULL)
//     {
//         tree = make_jrb();
//         jrb_insert_int(g, v1, new_jval_v(tree));
//     }
//     else
//         tree = (JRB)jval_v(find_v1->val);
//     jrb_insert_int(tree, v2, new_jval_i(1));
// }

void addEdge(Graph g, int v1, int v2)
{

    if (!adjacent(g, v1, v2))
    {
        JRB tree;
        JRB find = jrb_find_int(g, v1);
        if (find == NULL)
        {
            tree = make_jrb();
            jrb_insert_int(g, v1, new_jval_v(tree));
        }
        else
            tree = (JRB)jval_v(find->val);
        jrb_insert_int(tree, v2, new_jval_i(1));
    }

    if (!adjacent(g, v2, v1))
        addEdge(g, v2, v1);
}

int getAdjacentVertices(Graph g, int v, int *output)
{
    JRB find_v = jrb_find_int(g, v);
    if (find_v == NULL)
        return 0;
    JRB tree = (JRB)jval_v(find_v->val);

    int total = 0;

    JRB node;
    jrb_traverse(node, tree)
    {
        output[total++] = jval_i(node->key);
    }
    return total;
}

void dropGraph(Graph g)
{
    JRB node;
    jrb_traverse(node, g)
        jrb_free_tree(jval_v(node->val));
    jrb_free_tree(g);
}

int main()
{
    int output[100];
    Graph g = createGraph();

    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 2);
    addEdge(g, 1, 3);

    int m = adjacent(g, 0, 1);
    if (m == 1)
        printf("0 voi 1 Ke nhau!\n");
    else
        printf("Khong ke nhau!\n");

    int n = getAdjacentVertices(g, 1, output);
    if (n == 0)
        printf("Khong co dinh ke!\n");
    else
    {
        printf("Danh sach cac dinh ke voi dinh 1: ");
        for (int i = 0; i < n; i++)
            printf("%d", output[i]);
    }

    printf("\n");
    dropGraph(g);
    return 0;
}