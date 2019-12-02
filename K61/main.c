#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/jrb.h"

int checkArgc(int argc, char *argv[]);
int readfilesanPham(JRB sanPham, char *filename);
void readfilekhoHang(Graph khoHang, JRB soSanPham, char *filename, int m);

void chucNang4(Graph khoHang, JRB sanPham, JRB soSanPham);
void chucNang5(Graph khoHang, JRB sanPham, JRB soSanPham, int ID_sanPham, int ID_khoHang);

int main(int argc, char *argv[])
{
    int check_Argc = checkArgc(argc, argv);
    if (check_Argc == 0)
    {
        printf("Warning: Command Line!\n");
        exit(0);
    }

    Graph khoHang = createGraph();
    JRB sanPham = make_jrb();
    JRB soSanPham = make_jrb();
    JRB node;

    int m = readfilesanPham(sanPham, "sanpham.txt");
    readfilekhoHang(khoHang, soSanPham, "khohang.txt", m);

    switch (check_Argc)
    {
    case 1:
        printf("C-Advance, HK 20182\n");
        break;
    case 2:
    {
        jrb_traverse(node, sanPham)
            printf("%s %d\n", jval_s(node->val), jval_i(node->key));
    }
    break;
    case 3:
    {
        int ID1 = atoi(argv[3]);
        int ID2 = atoi(argv[4]);
        int value = (int)getEdgeValue(khoHang, ID1, ID2);
        if (value == INFINITIVE_VALUE)
            printf("-1\n");
        else
            printf("%d km\n", value);
    }
    break;
    case 4:
        chucNang4(khoHang, sanPham, soSanPham);
        break;
    case 5:
    {
        int ID_sanPham = atoi(argv[4]);
        int ID_khoHang = atoi(argv[5]);
        chucNang5(khoHang, sanPham, soSanPham, ID_sanPham, ID_khoHang);
    }
    break;
    case 6:
    {
        int ID_sanPham = atoi(argv[4]);
        int soLuong = atoi(argv[5]);
        int ID_ganNhat = atoi(argv[6]);
        int ID_ke = atoi(argv[7]);

        double time = 0;

        JRB findSoSanPham = jrb_find_int(soSanPham, ID_ganNhat);
        JRB tree = (JRB)jval_v(findSoSanPham->val);
        JRB findSoLuong = jrb_find_int(tree, ID_sanPham);

        int soLuongKhoGanNhat = jval_i(findSoLuong->val);
        if (soLuongKhoGanNhat >= soLuong)
        {
            printf("Dat hang thanh cong, Thoi gian giao: %.0f\n", time = 30);
        }
        else
        {
            findSoSanPham = jrb_find_int(soSanPham, ID_ke);
            tree = (JRB)jval_v(findSoSanPham->val);
            findSoLuong = jrb_find_int(tree, ID_sanPham);

            int soLuongKhoKe = jval_i(findSoLuong->val);
            if (soLuongKhoGanNhat + soLuongKhoKe < soLuong)
                printf("Dat hang that bai!\n");
            else
            {
                time = 30 + 60 * getEdgeValue(khoHang, ID_ganNhat, ID_ke) / 30;
                int gio = (int) time / 60;
                int phut = (int) time % 60;
                printf("Dat hang thanh cong!\n");
                printf("Thoi gian giao hang la %d gio %d phut\n", gio, phut);
            }
        }
    }
    break;
    default:
        break;
    }

    dropGraph(khoHang);
    return 0;
}

void chucNang5(Graph khoHang, JRB sanPham, JRB soSanPham, int ID_sanPham, int ID_khoHang)
{
    JRB find_khoHang = jrb_find_int(khoHang.vertices, ID_khoHang);
    if (find_khoHang == NULL)
    {
        printf("Khong co kho hang!\n");
        exit(2);
    }
    JRB find_sanPham = jrb_find_int(sanPham, ID_sanPham);
    if (find_sanPham == NULL)
    {
        printf("Khong co san pham!\n");
        exit(2);
    }

    printf("%s\n", jval_s(find_khoHang->val));
    JRB findSoSanPham = jrb_find_int(soSanPham, ID_khoHang);
    JRB tree = (JRB)jval_v(findSoSanPham->val);

    JRB findSoLuong = jrb_find_int(tree, ID_sanPham);
    printf("%s %d\n", jval_s(find_sanPham->val), jval_i(findSoLuong->val));
    printf("--- Cac kho hang ke: \n");

    int luuKhoHang[1000];
    int soLuongKhoKe = outdegree(khoHang, ID_khoHang, luuKhoHang);
    for (int i = 0; i < soLuongKhoKe; i++)
    {
        printf("%s\n", getVertex(khoHang, luuKhoHang[i]));
        findSoSanPham = jrb_find_int(soSanPham, luuKhoHang[i]);
        tree = (JRB)jval_v(findSoSanPham->val);
        findSoLuong = jrb_find_int(tree, ID_sanPham);
        printf("%s %d\n", jval_s(find_sanPham->val), jval_i(findSoLuong->val));
    }
}

void chucNang4(Graph khoHang, JRB sanPham, JRB soSanPham)
{
    JRB node, node1;
    jrb_traverse(node, khoHang.vertices)
    {
        printf("------------\n");
        int ID_khoHang = jval_i(node->key);
        printf("%s\n", jval_s(node->val));

        JRB findSoSanPham = jrb_find_int(soSanPham, ID_khoHang);
        JRB tree = (JRB)jval_v(findSoSanPham->val);

        jrb_traverse(node1, tree)
        {
            JRB findSanPham = jrb_find_int(sanPham, jval_i(node1->key));
            printf("%s %d\n", jval_s(findSanPham->val), jval_i(node1->val));
        }
    }
}

int checkArgc(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-t") == 0)
        return 1;
    if (argc == 3 && strcmp(argv[1], "-s") == 0)
        return 2;
    if (argc == 5 && strcmp(argv[1], "-w") == 0)
        return 3;
    if (argc == 4 && strcmp(argv[1], "-a") == 0)
        return 4;
    if (argc == 6 && strcmp(argv[1], "-h") == 0)
        return 5;
    if (argc == 8 && strcmp(argv[1], "-g") == 0)
        return 6;
    return 0;
}

int readfilesanPham(JRB sanPham, char *filename)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("Warning: File Fail!\n");
        exit(1);
    }
    get_line(is);
    int m = atoi(is->fields[0]);

    while (get_line(is) > 0)
        jrb_insert_int(sanPham, atoi(is->fields[1]), new_jval_s(strdup(is->fields[0])));
    return m;
}

void readfilekhoHang(Graph khoHang, JRB soSanPham, char *filename, int m)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("Warning: File Fail!\n");
        exit(1);
    }
    get_line(is);
    int n = atoi(is->fields[0]);

    for (int i = 1; i <= n; i++)
    {
        get_line(is);
        JRB tp_sanPham = make_jrb();
        int ID_khoHang = atoi(is->fields[1]);
        addVertex(khoHang, ID_khoHang, is->fields[0]);
        for (int j = 1; j <= m; j++)
        {
            get_line(is);
            jrb_insert_int(tp_sanPham, atoi(is->fields[0]), new_jval_i(atoi(is->fields[1])));
        }
        jrb_insert_int(soSanPham, ID_khoHang, new_jval_v(tp_sanPham));
    }

    get_line(is);
    int k = atoi(is->fields[0]);
    while (get_line(is) > 0)
        addEdge(khoHang, atoi(is->fields[0]), atoi(is->fields[1]), atof(is->fields[2]));
}