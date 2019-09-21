#include <stdio.h>
#include <stdlib.h>
#include "jrb.h"
#include "dllist.h"

typedef JRB Graph;

Graph createGraph();
int adjacent(Graph g, int v1, int v2);
void addEdge(Graph g, int v1, int v2);
int getAdjacentVertices(Graph g, int v, int *output);
void dropGraph(Graph g);

void printVertex(int v);
void BFS(Graph g, int start, int stop, void (*func)(int));

int main()
{
    int output[100];
    Graph g = createGraph();

    addEdge(g, 0, 1);
    addEdge(g, 1, 2);
    addEdge(g, 2, 3);
    addEdge(g, 2, 4);
    addEdge(g, 4, 5);

    printf("\nBFS: dinh 1 den 4: ");
    BFS(g, 1, 4, printVertex);
    printf("\n");

    printf("BFS: dinh 1 to all: ");
    BFS(g, 1, -1, printVertex);
    printf("\n");
    dropGraph(g);
    return 0;
}

void printVertex(int v)
{
    printf("%5d", v);
}

void BFS(Graph g, int start, int stop, void (*func)(int))
{
    int visited[1000] = {};
    int output[100];

    Dllist queue = new_dllist();

    dll_append(queue, new_jval_i(start));

    while (!dll_empty(queue))
    {
        Dllist node = dll_first(queue);
        int u = jval_i(node->val);
        dll_delete_node(node);

        if (!visited[u])
        {
            func(u);
            visited[u] = 1;
            if (u == stop)
                return;
            int n = getAdjacentVertices(g, u, output);
            for (int i = 0; i < n; i++)
            {
                int v = output[i];
                if (!visited[v])
                    dll_append(queue, new_jval_i(v));
            }
        }
    }
}

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