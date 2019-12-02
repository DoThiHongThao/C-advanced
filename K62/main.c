#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

void readFile(Graph g, char *filename);
void readFile2(Graph g, char *filename);

JRB getEdgeValue_node(Graph graph, int v1, int v2);
double shortestPath_dibo(Graph graph, int start, int stop, int *path, int *numVertices);

int main()
{
    Graph g = createGraph();
    char filename[] = "dothi.txt";

    Graph g2 = createGraph();
    readFile2(g2, filename);
    readFile(g, filename);

    int on = 0;

    int selection;
    do
    {
        printf("\n\n--------------MENU-------------\n"
               "1. In danh sach ma tran ke\n"
               "2. Danh sach ke cua moi thanh tri!\n"
               "3. Danh sach !\n"
               "4. Tim duong di ngan nhat!\n"
               "5. Tim duong di bo ngan nhat!\n"
               "0. Thoat!\n"
               "---> Chon: ");
        scanf("%d", &selection);

        switch (selection)
        {
        case 1:
        {
            int num = getNumOfV(g);
            printf("\nMa tran ke cua do thi: \n");
            // printf("%d\n", num);
            JRB node1, node2;
            printf("\t");
            jrb_traverse(node1, g.vertices)
            {
                int id = jval_i(node1->key);
                printf("%d\t", id);
            }
            printf("\n");
            jrb_traverse(node1, g.vertices)
            {
                int id1 = jval_i(node1->key);
                printf("%d\t", id1);
                jrb_traverse(node2, g.vertices)
                {
                    int id2 = jval_i(node2->key);
                    if (hasEdge(g, id1, id2))
                        printf("1\t");
                    else
                        printf("0\t");
                }
                printf("\n");
            }
        }
        break;
        case 2:
        {
            JRB node;
            jrb_traverse(node, g.vertices)
            {
                int id = jval_i(node->key);
                int output[1000];
                int value = indegree(g, id, output);
                printf("- Castle %d: ", id);
                for (int i = 0; i < value; i++)
                {
                    printf("%d ", output[i]);
                }
                printf("\n");
            }
        }
        break;
        case 3:
        {
            printf("\nDanh sach thanh tri chi co the den no truc tiep bang cach di bo!\n");
            JRB node;
            printf("La thanh tri: ");
            int max = 0;
            jrb_traverse(node, g.vertices)
            {
                int id = jval_i(node->key);
                int output[1000];
                int value = indegree(g, id, output);
                if (value > max)
                    max = value;
                for (int i = 0; i < value; i++)
                {
                    int gia_tri = getEdgeValue(g, id, output[i]);
                    if (gia_tri < 50)
                        break;
                    if (i == value - 1)
                        printf("%d ", id);
                }
            }
            printf("\n");
            printf("Danh sach cac thanh tri co nhieu duong di noi truc tiep nhat!\n");
            printf("La cac thanh tri: ");
            jrb_traverse(node, g.vertices)
            {
                int id = jval_i(node->key);
                int output[1000];
                int value = indegree(g, id, output);
                if (value == max)
                    printf("%d ", id);
            }
            printf("\n");
        }
        break;
        case 4:
        {
            int id1, id2;
            printf("Nhap thanh tri xuat phat: ");
            scanf("%d", &id1);
            printf("Nhap thanh tri dich: ");
            scanf("%d", &id2);
            int path[1000], number;
            int dodai = shortestPath(g, id1, id2, path, &number);
            if (dodai == INFINITIVE_VALUE)
                printf("\nROUTE NOT FOUND");
            else
            {
                printf("Thoi gian: %d\n", dodai);
                printf("Thu tu thanh tri: ");
                for (int i = 0; i < number; i++)
                {
                    printf("%d ", path[i]);
                }
            }
        }
        break;
        case 5:
        {

            int id1, id2;
            printf("Nhap thanh tri xuat phat: ");
            scanf("%d", &id1);
            printf("Nhap thanh tri dich: ");
            scanf("%d", &id2);
            int path[1000], number;
            int dodai = shortestPath(g2, id1, id2, path, &number);
            if (dodai == INFINITIVE_VALUE)
                printf("\nROUTE NOT FOUND");
            else
            {
                printf("Thoi gian: %d\n", dodai);
                printf("Thu tu thanh tri: ");
                for (int i = 0; i < number; i++)
                {
                    printf("%d ", path[i]);
                }
            }
        }
        break;
        case 0:
            break;
        default:
            printf("---> Nhap lai!\n");
            break;
        }
    } while (selection != 0);

    dropGraph(g);
    dropGraph(g2);
    return 0;
}

void readFile(Graph g, char *filename)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("Error: File Fail!\n");
        exit(1);
    }

    get_line(is);
    int n = atoi(is->fields[0]);
    int m = atoi(is->fields[1]);
    // printf("%d %d\n", n, m);

    int id1, id2;
    int time;
    while (get_line(is) > 0)
    {
        id1 = atoi(is->fields[0]);
        id2 = atoi(is->fields[1]);
        time = atoi(is->fields[2]);
        // printf("%d \n", id1);

        addVertex(g, id1, "");
        addVertex(g, id2, "");
        addEdge(g, id1, id2, time);
    }
}

void readFile2(Graph g, char *filename)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("Error: File Fail!\n");
        exit(1);
    }

    get_line(is);
    int n = atoi(is->fields[0]);
    int m = atoi(is->fields[1]);
    // printf("%d %d\n", n, m);

    int id1, id2;
    int time;
    while (get_line(is) > 0)
    {
        id1 = atoi(is->fields[0]);
        id2 = atoi(is->fields[1]);
        time = atoi(is->fields[2]);
        // printf("%d \n", id1);

        addVertex(g, id1, "");
        addVertex(g, id2, "");
        if (time >= 50)
            addEdge(g, id1, id2, time);
    }
}

JRB getEdgeValue_node(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node == NULL)
        return NULL;
    tree = (JRB)jval_v(node->val);
    node = jrb_find_int(tree, v2);
    if (node == NULL)
        return NULL;
    else
        return node;
}

// double shortestPathnt start, int stop, int *path, int *numVertices)
// {
//     double distance[1000];
//     int previous[1000], u, visit[1000];

//     for (int i = 0; i < 1000; i++)
//     {
//         distance[i] = INFINITIVE_VALUE;
//         visit[i] = 0;
//         previous[i] = 0;
//     }
//     distance[start] = 0;
//     previous[start] = start;
//     visit[start] = 1;

//     Dllist ptr, queue, node;
//     queue = new_dllist();
//     dll_append(queue, new_jval_i(start));

//     while (!dll_empty(queue))
//     {
//         double min = INFINITIVE_VALUE;
//         dll_traverse(ptr, queue)
//         {
//             u = jval_i(ptr->val);

//             if (min > distance[u])
//             {
//                 min = distance[u];
//                 // printf("%d\n", min);
//                 node = ptr;
//             }
//         }

//         u = jval_i(node->val);
//         visit[u] = 1;
//         dll_delete_node(node);
//         if (u == stop)
//             break;

//         int output[1000];
//         int n = outdegree(graph, u, output);

//         for (int i = 0; i < n; i++)
//         {
//             int v = output[i];
//             double w = getEdgeValue(graph, u, v);
//             if (distance[v] > distance[u] + w)
//             {
//                 distance[v] = distance[u] + w;
//                 previous[v] = u;
//             }
//             if (visit[v] == 0)
//             {
//                 dll_append(queue, new_jval_i(v));
//             }
//         }
//     }

//     double distance_s_t = distance[stop];
//     int count = 0;
//     if (distance[stop] != INFINITIVE_VALUE)
//     {
//         path[count++] = stop;
//         while (stop != start)
//         {
//             stop = previous[stop];
//             path[count++] = stop;
//         }
//         *numVertices = count;
//     }
//     swapArray(path, count);
//     return distance_s_t;
// }
