#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"

typedef JRB Graph;
JRB stations;

void fileOpenRead(FILE **f, char filename[]);
void inputDataFromTxt(Graph g, char filename[]);

Graph createGraph();
int adjacent(Graph g, char *v1, char *v2);
void addEdge(Graph g, char *v1, char *v2);
int getAdjacentVertices(Graph g, char *v, char *output[]);
void dropGraph(Graph g);

int main()
{
    stations = make_jrb();
    Graph g = createGraph();
    char filename[] = "data.txt";

    inputDataFromTxt(g, filename);

    char *output[20];

    char ten_ga_tim_kiem[20];
    printf("Nhap ten ga: ");
    gets(ten_ga_tim_kiem);

    int adjacent_station_number = getAdjacentVertices(g, ten_ga_tim_kiem, output);

    printf("So ga lien ke voi ga %s la: %d\n", ten_ga_tim_kiem, adjacent_station_number);
    for (int i = 0; i < adjacent_station_number; i++)
    {
        printf("%s\n", output[i]);
    }

    dropGraph(g);
    return 0;
}

void inputDataFromTxt(Graph g, char filename[])
{
    int stations_input = 0, lines_input = 0;
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        exit(1);
    }

    while (get_line(is) > 0)
    {
        if (strcmp(is->fields[0], "[STATIONS]") == 0)
        {
            stations_input = 1;
            lines_input = 0;
            continue;
        }

        if (strcmp(is->fields[0], "[LINES]") == 0)
        {
            stations_input = 0;
            lines_input = 1;
            continue;
        }

        if (stations_input == 1)
        {
            char stations_name[100];
            strcpy(stations_name, is->fields[2]);

            for (int i = 3; i < is->NF; i++)
            {
                strcat(stations_name, " ");
                strcat(stations_name, is->fields[i]);
            }
            (void)jrb_insert_str(stations, strdup(is->fields[0]), new_jval_s(strdup(stations_name)));
        }

        if (lines_input == 1)
        {

            JRB find_i, find_j;
            for (int i = 2; i < is->NF - 1; i++)
            {
                find_i = jrb_find_str(stations, is->fields[i]);
                find_j = jrb_find_str(stations, is->fields[i + 1]);
                addEdge(g, jval_s(find_i->val), jval_s(find_j->val));
                
                // for (int j = i + 1; j < is->NF; j++)
                // {
                //     find_j = jrb_find_str(stations, is->fields[j]);
                //     addEdge(g, jval_s(find_i->val), jval_s(find_j->val));
                // }
            }
        }
    }
}

int getAdjacentVertices(Graph g, char *v, char *output[])
{
    JRB find_v = jrb_find_str(g, v);
    if (find_v == NULL)
        return 0;
    JRB tree = (JRB)jval_v(find_v->val);

    int total = 0;

    JRB node;
    jrb_traverse(node, tree)
    {
        output[total] = malloc(sizeof(char) * strlen(jval_s(node->key)));
        strcpy(output[total], jval_s(node->key));
        total++;
    }
    return total;
}

Graph createGraph()
{
    return make_jrb();
}

int adjacent(Graph g, char *v1, char *v2)
{
    JRB find_v1 = jrb_find_str(g, v1);
    if (find_v1 == NULL)
        return 0;

    JRB tree = (JRB)jval_v(find_v1->val);
    if (jrb_find_str(tree, v2) == NULL)
        return 0;
    return 1;
}

void addEdge(Graph g, char *v1, char *v2)
{

    if (!adjacent(g, v1, v2))
    {
        JRB tree;
        JRB find = jrb_find_str(g, v1);
        if (find == NULL)
        {
            tree = make_jrb();
            jrb_insert_str(g, v1, new_jval_v(tree));
        }
        else
            tree = (JRB)jval_v(find->val);
        jrb_insert_str(tree, v2, new_jval_i(1));
    }
}

void dropGraph(Graph g)
{
    JRB node;
    jrb_traverse(node, g)
        jrb_free_tree(jval_v(node->val));
    jrb_free_tree(g);
}