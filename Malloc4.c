/* 
Шульман Виталий АПО-13

Составить программу построчной обработки текста. Суть обработки - приведение каждого символа в каждой строке к нижнему регистру.

Программа считывает входные данные со стандартного ввода, и печатает результат в стандартный вывод.

Процедура приведения к нижнему регистру должна быть оформлена в виде отдельной функции, которой на вход подается массив строк (который необходимо обработать), количество переданных строк, а также указатель на переменную, в которой необходимо разместить результат - массив уже обработанных строк. 
В качестве возвращаемого значения функция должна возвращать количество строк, содержащихся в результирующем массиве. 

Программа должна уметь обрабатывать ошибки - такие как неверные входные данные(отсутствие входных строк) или ошибки выделения памяти и т.п. 
В случае возникновения ошибки нужно выводить об этом сообщение "[error]" и завершать выполнение программы. 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int reg_down(char** const mass, const int count, char*** new_mass) {
  int i;
  (*new_mass) = (char**)malloc(sizeof(char*) * (count));
  if ((*new_mass) == NULL) {
    printf("[error]");
    return i = 0;
  }

  for (i = 0; i < count; i++) {
    (*new_mass)[i] = (char*)malloc(sizeof(char) * (strlen(mass[i]) + 1));
    if ((*new_mass)[i] == NULL) {
      printf("[error]");
      return i;
    }

    for (int j = 0; mass[i][j] != '\0'; j++)
      if ((mass[i][j] > 64) && (mass[i][j] < 91)) {
        (*new_mass)[i][j] = mass[i][j] + 32;
      } else {
        (*new_mass)[i][j] = mass[i][j];
      }
    (*new_mass)[i][strlen(mass[i])] = '\0';
  }
  return i;
}

void resize(char*** st, const char* buffer, const int count, int* err) {
  char** p = (*st);
  (*st) = (char**)realloc(p, sizeof(char*) * (count + 1));
  if ((*st) == NULL) {
    *err = 1;
    return;
  }

  (*st)[count] = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
  if ((*st)[count] == NULL) {
    *err = 1;
    return;
  }

  for (int j = 0; buffer[j] != '\0'; j++) (*st)[count][j] = buffer[j];
  (*st)[count][strlen(buffer)] = '\0';
}

int main() {
  int count = 0;
  int err = 0;
  char** st = NULL;

  for (char buffer[256] = {"\0"}; err != 1; count++) {
    if (!((buffer == fgets(buffer, 256, stdin)) && buffer[0] != '\n')) break;
    resize(&st, buffer, count, &err);
  }
  if ((count < 1) || (err == 1)) {
    printf("[error]");
  } else {
    char** res = NULL;
    const int k = reg_down(st, count, &res);

    for (int i = 0; i < k; i++) {
      printf("%s", res[i]);
      free(res[i]);
    }
    free(res);
  }
  for (int i = 0; i < count; i++) {
    free(st[i]);
  }
  free(st);

  system("pause");
  return 0;
}