#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "fields.h"

int so_sinh_vien;

typedef struct
{
    int MSSV;
    char *name[3];
    double diem[3];
    double tongdiem;
} SINHVIEN;

int compare(Jval j1, Jval j2)
{
    SINHVIEN *sv1, *sv2;
    sv1 = (SINHVIEN *)j1.v;
    sv2 = (SINHVIEN *)j2.v;

    if (sv1->tongdiem < sv2->tongdiem)
        return 1;
    else if (sv1->tongdiem > sv2->tongdiem)
        return -1;
    else
    {
        for (int i = 2; i >= 0; i--)
        {
            if (strcpy(sv1->name[i], sv2->name[i]) > 0)
                return 1;
            else if (strcpy(sv1->name[i], sv2->name[i]) < 0)
                return -1;
        }
    }
    return 0;
}

void inputData(JRB *root, char *filename)
{
    IS is;
    SINHVIEN *sv;
    is = new_inputstruct(filename);
    if (is == NULL)
    {
        exit(1);
    }

    get_line(is);
    so_sinh_vien = atoi(is->fields[0]);
    //  printf("So sinh vien: %d\n", so_sinh_vien);

    while (get_line(is) > 0)
    {
        if (is->NF > 1)
        {
            sv = malloc(sizeof(SINHVIEN));

            sv->MSSV = atoi(is->fields[0]);
            for (int i = 0; i < 3; i++)
            {
                sv->name[i] = malloc(sizeof(char));
                strcpy(sv->name[i], is->fields[i + 1]);
            }

            for (int i = 0; i < 3; i++)
            {
                sv->diem[i] = atof(is->fields[i + 4]);
            }

            sv->tongdiem = 0;
            for (int i = 0; i < 3; i++)
            {
                sv->tongdiem += sv->diem[i];
            }

            (void)jrb_insert_gen(*root, new_jval_v((void *)sv), new_jval_v(NULL), compare);
        }
    }
}

int checkDiemChuan(SINHVIEN sv, double diemchuan)
{
    if (sv.diem[0] <= 0 || sv.diem[1] <= 0 || sv.diem[2] <= 0)
        return 0;
    else if (sv.tongdiem < diemchuan)
        return 0;
    return 1;
}

double timDiemChuan(JRB root, int soThiSinh) // Ke ca co nhieu thi sinh cung so diem !
{
    JRB node;
    SINHVIEN *sv;
    int i = 0;
    jrb_traverse(node, root)
    {
        i++;
        sv = (SINHVIEN *)node->key.v;
        if (i == soThiSinh)
            return sv->tongdiem;
    }
    return 0;
}

int checkDiemChuanTheoSoThiSinh(SINHVIEN sv, double diemchuan)
{
    if (sv.diem[0] <= 0 || sv.diem[1] <= 0 || sv.diem[2] <= 0)
        return 0;
    else if (sv.tongdiem <= diemchuan)
        return 0;
    return 1;
}

void inDanhSach(JRB root, double diemchuan, int checkDiem())
{
    JRB node;
    SINHVIEN *sv;
    int i = 0;

    double diemchuanthucsu;
    printf("\n%-5s %-10s %-24s %-10s %-5s %-5s %s\n", "STT", "MSSV", "Ho_Dem_Ten", "Tong_Diem", "Toan", "Ly", "Hoa");
    jrb_traverse(node, root)
    {
        sv = (SINHVIEN *)node->key.v;
        if (checkDiem(*sv, diemchuan) != 0)
        {
            i++;
            printf("%-5d %-10d %-6s %-6s %-10s %-10.2lf %-5.2lf %-5.2lf %-5.2lf\n", i, sv->MSSV,
                   sv->name[0], sv->name[1], sv->name[2], sv->tongdiem, sv->diem[0], sv->diem[1], sv->diem[2]);
            diemchuanthucsu = sv->tongdiem;
        }
    }
    printf("\nSo thi sinh trung tuyen: %d"
           "\nDiem chuan: %.2lf\n",
           i, diemchuanthucsu);
}

int main()
{
    JRB root;
    root = make_jrb();

    inputData(&root, "diem.txt");

    double diemchuan = -1;
    int selection;
    do
    {
        printf("\n----------------------------\n"
               "1. Nhap diem chuan\n"
               "2. In danh sach thi sinh trung tuyen\n"
               "3. Tim diem chuan\n"
               "0. Thoat !\n"
               "---> Chon: ");
        scanf("%d", &selection);

        switch (selection)
        {
        case 1:
            printf("Nhap diem chuan: ");
            scanf("%lf", &diemchuan);
            if (diemchuan > 30 || diemchuan < 0)
            {
                printf("Nhap lai diem chuan!\n");
                diemchuan = -1;
            }
            break;

        case 2:
            if (diemchuan == -1)
            {
                printf("Ban chua nhap diem chuan!\n");
            }
            else
                inDanhSach(root, diemchuan, checkDiemChuan);
            break;
        case 3:
        {
            printf("Nhap so thi sinh: ");
            int soThiSinh; // Check 0 <= diemchuan <= 30
            scanf("%d", &soThiSinh);
            double diemchuan = timDiemChuan(root, soThiSinh);

            printf("\nDanh sach trung tuyen!\n");
            inDanhSach(root, diemchuan, checkDiemChuanTheoSoThiSinh);
        }
        break;
        case 0:
            break;
        default:
            printf("Nhap lai !\n");
            break;
        }

    } while (selection != 0);

    return 0;
}