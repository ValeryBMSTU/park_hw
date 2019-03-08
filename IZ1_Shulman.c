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

int resize(char*** strings, const char* const buffer, const int count);

int string_lwr(char *new_string, const char* const source_string);

int main() {
	int count = 0, k = 0;
	int error_flag = FALSE;
	char** strings = NULL;
	char** result_strings = NULL;

	for (char buffer[BUF_SIZE] = { "\0" }; error_flag != 1; count++) {
		if (fgets(buffer, BUF_SIZE, stdin) == NULL)	{
			break;
		}
		if (buffer[0] == '\n') {
			break;
		}
		error_flag = resize(&strings, buffer, count);
	}
	if ((count < 1) || error_flag) {
		printf("[error]");
	}
	else {
		k = reg_down(strings, count, &result_strings);
		if (count == k)	{
			for (int i = 0; i < k; i++) {
				printf("%s", result_strings[i]);
			}
		}
		else {
			printf("[error]");
		}
	}

	for (int i = 0; i < k; i++)	{
		free(result_strings[i]);
	}
	free(result_strings);
	for (int i = 0; i < count; i++) {
		free(strings[i]);
	}
	free(strings);

	return 0;
}

int reg_down(char** const source_strings, const int count, char*** new_strings) {
	if (new_strings == NULL)	{
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

		if (string_lwr((*new_strings)[i], source_strings[i]) != strlen(source_strings[i]))	{
			break;
		}
	}
	return i;
}

int resize(char*** strings, const char* const buffer, const int count) {

	static int curent_size = STRINGS_SIZE;

	if (count == curent_size || count == 0)
	{
		curent_size *= 2;
		char** p = (*strings);
		(*strings) = (char**)realloc(p, sizeof(char*) * (curent_size));
		if ((*strings) == NULL) {
			return -1;
		}
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

int string_lwr(char *new_string, const char* const source_string)
{
	int j = 0;
	for (; source_string[j] != '\0'; j++)
		if ((source_string[j] >= 'A') && (source_string[j] <= 'Z')) {
			new_string[j] = (source_string[j] - 'A') + 'a';
		}
		else {
			new_string[j] = source_string[j];
		}
	new_string[strlen(source_string)] = '\0';
	return j;
}