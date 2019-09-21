#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int *matrix;
    int n;
} Graph;

Graph creatGrahp(int n);
void addEdge(Graph graph, int v1, int v2);
int adjacent(Graph graph, int v1, int v2);
int getAdjacentVertices(Graph graph, int v, int *output);
void dropGraph(Graph graph);

int main()
{
    int n, output[100];
    Graph g = creatGrahp(100);

    addEdge(g, 0, 1);
    addEdge(g, 0, 2);
    addEdge(g, 1, 2);
    addEdge(g, 1,3);

    n = getAdjacentVertices(g, 1, output);
    if (n == 0)
        printf("Khong co dinh ke!\n");
    else 
    {
        printf("Danh sach cac dinh ke voi dinh : \n");
        for (int i = 0; i < n; i++)
            printf("%d", output[i]);
    }

    printf("\n");
    dropGraph(g);
    return 0;
}

Graph creatGrahp(int n)
{
    Graph g;
    g.n = n;
    g.matrix = (int *)malloc(n * n * sizeof(int));
    return g;
}

void addEdge(Graph graph, int v1, int v2)
{
    graph.matrix[v1*graph.n + v2] = 1;
    // graph.matrix[v2*graph.n + v1] = 1;
}

int adjacent(Graph graph, int v1, int v2)
{
    return graph.matrix[v1*graph.n + v2];
}

int getAdjacentVertices(Graph graph, int v, int *output)
{
    int total = 0;
    for (int i = 0; i < graph.n; i++)
    {
        if(adjacent(graph, v, i))
        {
            output[total++] = i;
        }
    }
    return total;
}

void dropGraph(Graph graph)
{
    free(graph.matrix);
    graph.matrix = NULL;
    graph.n = 0;
}