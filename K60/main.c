#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GraphAPI/graph_jrb.h"
#include "libfdr/fields.h"
#include "libfdr/jrb.h"

void nhapSanPhamTuFile(JRB sanPham, char *filename);
void nhapDanhSachGiaoDich(Graph datHang, JRB sanPham, char *filename);

int main()
{
    JRB sanPham = make_jrb();
    JRB node;
    Graph datHang = createGraph();
    nhapSanPhamTuFile(sanPham, "products.txt");
    int selection;
    do
    {
        printf("\n\n"
               "1. In danh sach san pham\n"
               "2. In danh sach giao dich\n"
               "3. Hien thi muc do lien quan giua 2 san pham\n"
               "4. In danh sach cac san pham lien quan\n"
               "5. Hien thi moi lien he giua 2 san pham\n"
               "6. Thoat!\n"
               "---> Chon: ");
        scanf("%d", &selection);

        switch (selection)
        {
        case 1:
            jrb_traverse(node, sanPham)
            {
                printf("\nMa san pham: %d\n", jval_i(node->key));
                printf("Ten san pham: %s\n", jval_s(node->val));
            }
            break;
        case 2:
            nhapDanhSachGiaoDich(datHang, sanPham, "orderhistory.txt");
            break;

        case 3:
        {
            int ID1, ID2;
            printf("Nhap ID1: ");
            scanf("%d", &ID1);
            printf("Nhap ID2: ");
            scanf("%d", &ID2);
            double value = getEdgeValue(datHang, ID1, ID2);
            if (value == INFINITIVE_VALUE)
                printf("-1");
            else
                printf("Do lien quan giua 2 san pham: %.1f", value);
        }
        break;
        case 4:
        {
            int dinh[1000];
            int ID;
            printf("Nhap ma san pham: ");
            scanf("%d", &ID);
            int soDinhKe = outdegree(datHang, ID, dinh);

            JRB sapXep = make_jrb();
            for (int i = 0; i < soDinhKe; i++)
            {
                int value = (int)getEdgeValue(datHang, ID, dinh[i]);
                jrb_insert_int(sapXep, value, new_jval_i(dinh[i]));
            }

            printf("Cac san pham lien quan: ");
            jrb_rtraverse(node, sapXep)
            {
                JRB find = jrb_find_int(sanPham, jval_i(node->val));
                printf("%s ", find->val.s);
            }
            printf("\n");
        }
        break;
        case 5:
        {
            int ID1, ID2;
            printf("Nhap id1: ");
            scanf("%d", &ID1);
            printf("Nhap id2: ");
            scanf("%d", &ID2);
            int path[1000];
            int cnt = shortest_noWeight_path(datHang, ID1, ID2, path);
            if (cnt == 0)
                printf("Hai san pham khong co lien he voi nhau!\n");
            else
            {
                for (int i = 0; i < cnt; i++)
                {
                    printf("%s ", getVertex(datHang, path[i]));
                }
            }
            printf("\n");
            cnt = DFS(datHang, ID1, ID2, path);
            if (cnt == 0)
                printf("Hai san pham khong lien he voi nhau!\n");
            else
                for (int i = 0; i < cnt; i++)
                {
                    printf("%s ", getVertex(datHang, path[i]));
                }
        }
        break;
        case 6:
            break;
        default:
            printf("---> Nhap lai!\n");
            break;
        }

    } while (selection != 6);

    dropGraph(datHang);
    return 0;
}

void nhapDanhSachGiaoDich(Graph datHang, JRB sanPham, char *filename)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("File fail!\n");
        exit(1);
    }
    while (get_line(is) > 0)
    {
        for (int i = 0; i < is->NF; i++)
        {
            JRB find = jrb_find_int(sanPham, atoi(is->fields[i]));
            printf("%s ", jval_s(find->val));
            addVertex(datHang, atoi(is->fields[i]), jval_s(find->val));
            if (i == is->NF - 1)
                printf("\n");
        }
        for (int i = 0; i < is->NF - 1; i++)
        {
            int id1 = atoi(is->fields[i]);

            for (int j = i + 1; j < is->NF; j++)
            {
                int id2 = atoi(is->fields[j]);

                double value = getEdgeValue(datHang, id1, id2);
                if (value == INFINITIVE_VALUE)
                    addEdge(datHang, id1, id2, 1);
                else
                {
                    JRB node = jrb_find_int(datHang.edges, id1);
                    JRB tree = (JRB)jval_v(node->val);
                    node = jrb_find_int(tree, id2);
                    node->val.d += 1;

                    node = jrb_find_int(datHang.edges, id2);
                    tree = (JRB)jval_v(node->val);
                    node = jrb_find_int(tree, id1);
                    node->val.d += 1;
                    // jrb_insert_int(tree, id2, new_jval_d(value + is->NF - 1));
                }
            }
        }
    }
}

void nhapSanPhamTuFile(JRB sanPham, char *filename)
{
    IS is = new_inputstruct(filename);
    if (is == NULL)
    {
        printf("File fail!\n");
        exit(1);
    }
    while (get_line(is) > 0)
        jrb_insert_int(sanPham, atoi(is->fields[0]), new_jval_s(strdup(is->fields[1])));
}