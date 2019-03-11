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
#define STRINGS_SIZE 4
#define TRUE 1 // Мой С90 не поддерживает bool
#define FALSE 0 

int reg_down(char** const source_strings, const int count, char*** new_strings);

int resize(char*** strings, const char* const buffer, const int count, int* curent_count);

int string_lwr(char *new_string, const char* const source_string);

void memory_release(const int result_count, char** const result_strings, const int count, char** const strings);

int input(char*** strings, int* count, FILE* format);

int main() {
	int count = 0;
	int result_count = 0;
	char** strings = NULL;
	char** result_strings = NULL;
	FILE* format = stdin;

	if (!input(&strings, &count, format)) {
		printf("[error]");
	}
	else {
		result_count = reg_down(strings, count, &result_strings);
		if (count == result_count) {
			for (int i = 0; i < result_count; i++) {
				printf("%s", result_strings[i]);
			}
		}
		else {
			printf("[error]");
		}
	}
	memory_release(result_count, result_strings, count, strings);
	return 0;
}

int reg_down(char** const source_strings, const int count, char*** new_strings) {
	int proc_strings_count = 0;

	if (new_strings == NULL || source_strings == NULL) {
		return proc_strings_count;
	}

	(*new_strings) = (char**)malloc(sizeof(char*) * (count));
	if ((*new_strings) == NULL) {
		return proc_strings_count;
	}

	for (; proc_strings_count < count; proc_strings_count++) {
		(*new_strings)[proc_strings_count] = (char*)malloc(sizeof(char) * (strlen(source_strings[proc_strings_count]) + 1));
		if ((*new_strings)[proc_strings_count] == NULL) {
			break;
		}

		if (!string_lwr((*new_strings)[proc_strings_count], source_strings[proc_strings_count])) {
			break;
		}
	}
	return proc_strings_count;
}

int resize(char*** strings, const char* const buffer, const int count, int* curent_size) {

	if (count == *curent_size || count == 0)
	{
		*curent_size *= 2;
		if (strings == NULL) {
			return FALSE;
		}
		char** p = (*strings);
		(*strings) = (char**)realloc(p, sizeof(char*) * (*curent_size));
		if ((*strings) == NULL) {
			return FALSE;
		}
	}

	(*strings)[count] = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	if ((*strings)[count] == NULL) {
		return FALSE;
	}

	for (int j = 0; buffer[j] != '\0'; j++) {
		(*strings)[count][j] = buffer[j];
	}
	(*strings)[count][strlen(buffer)] = '\0';
	return TRUE;
}

int string_lwr(char *new_string, const char* const source_string)
{
	int j = 0;
	for (; source_string[j] != '\0'; j++) {
		if ((source_string[j] >= 'A') && (source_string[j] <= 'Z')) {
			new_string[j] = (source_string[j] - 'A') + 'a';
		}
		else {
			new_string[j] = source_string[j];
		}
	}
	new_string[strlen(source_string)] = '\0';
	if (strlen(new_string) == strlen(source_string)) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void memory_release(const int result_count, char** const result_strings, const int count, char** const strings)
{
	for (int i = 0; i < result_count; i++) {
		if (result_strings[i] != NULL) {
			free(result_strings[i]);
		}
	}
	if (result_strings != NULL) {
		free(result_strings);
	}
	for (int i = 0; i < count; i++) {
		if (strings[i] != NULL) {
			free(strings[i]);
		}
	}
	if (result_strings != NULL) {
		free(strings);
	}
	return;
}

int input(char*** strings, int* count, FILE* format)
{
	int curent_size = STRINGS_SIZE;
	int resize_ok = TRUE;
	for (char buffer[BUF_SIZE] = { "\0" }; resize_ok;) {
		if (fgets(buffer, BUF_SIZE, format) == NULL) {
			break;
		}
		if (buffer[0] == '\n') {
			break;
		}
		resize_ok = resize(&(*strings), buffer, (*count), &curent_size);
		if (resize_ok) {
			(*count)++;
		}
	}

	return resize_ok;
}