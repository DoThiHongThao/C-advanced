#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"

#define MAX 100

typedef struct
{
  char name[80];
  char number[20];
} phoneData;

void fileOpenRead(FILE **f, char filename[])
{
  if ((*f = fopen(filename, "r")) == NULL)
  {
    printf("Can't open file: %s\n", filename);
    exit(0);
  }
}

void fileOpenWrite(FILE **f, char filename[])
{
  if ((*f = fopen(filename, "w")) == NULL)
  {
    printf("Can't open file: %s\n", filename);
    exit(0);
  }
}

void readFile(JRB *book)
{
  FILE *fp = NULL;
  fileOpenRead(&fp, "danhba.txt");
  char temp[100];
  char *name, *phone;

  while (fgets(temp, 100, fp) != 0)
  {
    name = strtok(temp, "\t");
    phone = strtok(NULL, "\n");
    jrb_insert_str(*book, strdup(name), new_jval_s(strdup(phone)));
  }
  fclose(fp);
}

void inputDataFromKeyboard(JRB *book)
{
  phoneData data;

  printf("Nhap ten: ");
  setbuf(stdin, NULL);
  gets(data.name);

  printf("Nhap so dien thoai: ");
  setbuf(stdin, NULL);
  gets(data.number);

  JRB find = jrb_find_str(*book, data.name);
  if (find == NULL)
  {
    (void)jrb_insert_str(*book, strdup(data.name), new_jval_s(strdup(data.number)));
  }
  else
  {
    find->val = new_jval_s(strdup(data.number));
  }
}

void saveFile(JRB book)
{
  FILE *fp = NULL;
  JRB node; 
  fileOpenWrite(&fp, "danhba.txt");

  jrb_traverse(node, book)
  {
    fprintf(fp, "%s\t%s\n", jval_s(node->key), jval_s(node->val));
  }

  fclose(fp);
}

int main()
{
  JRB book, node;
  book = make_jrb();
  readFile(&book);

  int selection;

  do
  {
    printf("\nMenu !\n"
           "1. Them contact\n"
           "2. Tim kiem so dien thoai khi cho them contact\n"
           "3. Xoa contact\n"
           "4. In danh sach contact\n"
           "5. Exit\n" // Luu lai file truoc khi thoat
           "---> Chon: ");
    scanf("%d", &selection);
    printf("\n");
    switch (selection)
    {
    case 1:
      inputDataFromKeyboard(&book);
      break;
    case 2:
    {
      printf("Nhap ten can tim kiem: ");
      char name_find[80];
      setbuf(stdin, NULL);
      gets(name_find);
      JRB find = jrb_find_str(book, name_find);
      if (find == NULL)
      {
        printf("Khong tim thay !\n");
      }
      else
      {
        printf("So dien thoai la: %s\n", jval_s(find->val));
      }
    }
    break;
    case 3:
    {
      printf("Nhap ten can xoa: ");
      char name_find[80];
      setbuf(stdin, NULL);
      gets(name_find);
      JRB find = jrb_find_str(book, name_find);
      if (find == NULL)
      {
        printf("Khong tim thay !\n");
      }
      else
      {
        jrb_delete_node(find);
        printf("Xoa thanh cong !\n");
      }
    }
    break;
  case 4:
    printf("%-40s %s\n", "Ho va ten", "SDT");

    jrb_traverse(node, book)
    {
      printf("%-40s %s\n", jval_s(node->key), jval_s(node->val));
    }
    break;

  case 5:
    saveFile(book);
    break;
  default:
    printf("\nNhap lai gia tri!\n");
    break;
  }
}
while (selection != 5);

jrb_free_tree(book);
return 0;
}