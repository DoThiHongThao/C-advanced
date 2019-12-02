#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

void readFile(Graph g, char *filename, JRB name_ID);

int main()
{
    Graph g = createGraph();
    char filename[] = "Flight.txt";

    JRB name_ID = make_jrb();
    readFile(g, filename, name_ID);
    // int selection;
    // do
    // {
    //     printf("\n1. In danh sach san pham\n"
    //            "0. Thoat!\n"
    //            "---> Chon: ");
    //     scanf("%d", &selection);

    //     switch (selection)
    //     {
    //     case 1:

    //         break;

    //     case 0:
    //         break;
    //     default:
    //         printf("---> Nhap lai!\n");
    //         break;
    //     }

    // } while (selection != 0);

    dropGraph(g);
    return 0;
}

void readFile(Graph g, char *filename, JRB name_ID)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("Error: File Fail!\n");
        exit(1);
    }

    int i = 0;
    const char s[2] = "=";
    char *nameID = (char *)malloc(sizeof(char) * 30), *name;
    while (get_line(is) > 0)
    {
        nameID = strtok(is->text1, s);
        
        name = strtok(NULL, s);
    }
}
