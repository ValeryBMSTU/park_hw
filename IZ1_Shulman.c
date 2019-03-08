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

#define BUF_SIZE 256
#define ASCII_SHIFT 32
#define TRUE 1

int reg_down(char** const source_strings, const int count, char*** new_strings);

int resize(char*** st, const char* buffer, const int count);

int main() {
	int count = 0;
	int error_flag = 0;
	char** strings = NULL;

	for (char buffer[BUF_SIZE] = { "\0" }; error_flag != 1; count++) {
		if (fgets(buffer, BUF_SIZE, stdin) == NULL)	{
			break;
		}
		if (buffer[0] == '\n') {
			break;
		}
		error_flag = resize(&strings, buffer, count);
	}
	if ((count < 1) || (error_flag == TRUE)) {
		printf("[error]");
	}
	else {
		char** result_strings = NULL;
		const int k = reg_down(strings, count, &result_strings);

		for (int i = 0; i < k; i++) {
			printf("%s", result_strings[i]);
			free(result_strings[i]);
		}
		free(result_strings);
	}
	for (int i = 0; i < count; i++) {
		free(strings[i]);
	}
	free(strings);

	getchar();
	return 0;
}

int reg_down(char** const source_strings, const int count, char*** new_strings) {
	if (new_strings == NULL)
	{
		return -1;
	}

	int i = 0; //счетчик обработанных строк
	(*new_strings) = (char**)malloc(sizeof(char*) * (count));
	if ((*new_strings) == NULL) {
		return -1;
	}

	for (; i < count; i++) {
		(*new_strings)[i] = (char*)malloc(sizeof(char) * (strlen(source_strings[i]) + 1));
		if ((*new_strings)[i] == NULL) {
			break;
		}

		for (int j = 0; source_strings[i][j] != '\0'; j++)
			if ((source_strings[i][j] >= 'A') && (source_strings[i][j] <= 'Z')) {
				(*new_strings)[i][j] = source_strings[i][j] + ASCII_SHIFT;
			}
			else {
				(*new_strings)[i][j] = source_strings[i][j];
			}
		(*new_strings)[i][strlen(source_strings[i])] = '\0';
	}
	return i;
}

int resize(char*** strings, const char* buffer, const int count) {
	char** p = (*strings);
	(*strings) = (char**)realloc(p, sizeof(char*) * (count + 1));
	if ((*strings) == NULL) {
		return -1;
	}

	(*strings)[count] = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	if ((*strings)[count] == NULL) {
		return -1;
	}

	for (int j = 0; buffer[j] != '\0'; j++)	{
		(*strings)[count][j] = buffer[j];
	}
	(*strings)[count][strlen(buffer)] = '\0';
	return 0;
}