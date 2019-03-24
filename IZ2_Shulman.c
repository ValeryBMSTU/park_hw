// Шульман Виталий АПО-13
// Задача B-1. Калькулятор для числовых множеств
// Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над множествами натуральных чисел. Входные данные (выражение с описанием множеств и производимых над ними операций) подаются на стандартный поток ввода программы, результат вычислений должен подаваться на стандартный поток вывода. 
// Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' - разность множеств, '()' - задание приоритета вычислений.
// Множества задаются в виде[el1, el2, ...], где элементами являются обычные натуральные числа.
// Множества могут быть пустыми.
// Между операциями, множествами и элементами множеств может быть любое количество пробелов.
// При вычислениях должны учитываться приоритеты операций (в том числе, заданные вручную при помощи круглых скобочек).
// Результирующее множество должно выводиться без пробелов, в отсортированном порядке.
// В случае, если введенное выражение содержит ошибку, необходимо вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define STRING_BUFFER_START_SIZE 256
#define NUMBER_BUFFER_START_SIZE 16
#define STR_EXTRA_SIZE 3
#define OPER_STECK_START_SIZE 8
#define OPERANDS_BUFFER_COUNT 64
#define OPERANDS_BUFFER_SIZE 16
#define RESIZE_RATIO 2
#define COUNT_OF_OPER_TYPE 5

#define POS_0 0
#define POS_1 1
#define POS_2 2
#define POS_3 3 
#define POS_4 4 
#define POS_5 5
#define POS_6 6
#define POS_ERROR -1

// Ввод исходной строки
_Bool input(char** string);
// Проход по строке в поисках операций, операндов или конца выражения
int** set_calc(const char* const string, bool* error_flag);
// Выбор действия после встречи операций, операндов или конца выражения
_Bool operation(int* position, const int next_oper, int** operands, int* oper_count, int* string_pos);
// Произведение операции над операндами
_Bool calc_operands(int* operand_1, int* operand_2, char operation);
// Определение текущей операции на вершине стека
int operation_choice(char operation);
// Вывод результата
void print_result(int* result);
// Освобождение памяти
void free_memory(int** result, char* string);
// Добавление операнда в массив операндов
bool add_operand(int** operands, int* i, const char* string, char** number, int* number_curent_size, int* oper_count);

// Решение с помощью алгоритма Бауэра-Замельзона (Польская запись).
int main()
{
	char* string = NULL;
	int** result = NULL;
	bool error_flag = false;;

	if (input(&string)) {
		result = set_calc(string, &error_flag);
		if (error_flag) {
			printf("[error]");		}
		else {
			print_result(*result);
		}
	}
	else
	{
		printf("[error]");
	}
	free_memory(result, string);
	return 0;
}

// Ввод исходной строки
_Bool input(char** string)
{
	char buffer[STRING_BUFFER_START_SIZE] = { '\0' };
	if (!((buffer == fgets(buffer, STRING_BUFFER_START_SIZE, stdin)) && buffer[0] != '\n')) {
		return false;
	}
	else {
		if (((*string) = (char*)malloc(sizeof(char) * (strlen(buffer) + STR_EXTRA_SIZE))) == NULL) {
			return false;
		}
		int i = 0;
		for (int j = 0; buffer[j] != '\0' && buffer[j] != '\n'; j++) {
			if (buffer[j] != ' ') {
				(*string)[i] = buffer[j];
				i++;
			}
		}
		(*string)[i] = '\n';
		(*string)[i + 1] = '\0';
	}
	int bracket_open = 0, bracket_close = 0;
	int qbracket_open = 0, qbracket_close = 0;
	for (int i = 0; (*string)[i] != '\0'; i++) {
		if ((*string)[i] == '(') {
			bracket_open++;
		}
		else if ((*string)[i] == ')') {
			bracket_close++;
		}
		if ((*string)[i] == '[') {
			qbracket_open++;
		}
		else if ((*string)[i] == ']') {
			qbracket_close++;
		}
	}
	if (bracket_open != bracket_close || qbracket_open != qbracket_close) {
		return false;
	}

	return true;
}

// Проход по строке в поисках операций, операндов или конца выражения
int** set_calc(const char* const string, bool* error_flag)
{
	int** operands = NULL;
	if ((operands = (int**)malloc(sizeof(int*)*OPERANDS_BUFFER_COUNT)) == NULL) {
		*error_flag = true;
	}
	for (int i = 0; i < OPERANDS_BUFFER_COUNT && (*error_flag) != true; i++)
	{
		if ((operands[i] = (int*)malloc(sizeof(int)*OPERANDS_BUFFER_SIZE)) == NULL) {
			for (int j = 0; j < i; j++)
			{
				free(operands[j]);
			}
			free(operands);
			*error_flag = true;
		}
	}
	if (!(*error_flag)) {
		char* number = NULL;
		if ((number = (char*)malloc(sizeof(char)*NUMBER_BUFFER_START_SIZE)) == NULL) {
			*error_flag = true;
		}
		int number_curent_size = NUMBER_BUFFER_START_SIZE;
		int prev_oper_type = 0, next_oper_type = 0;
		static int oper_count = 0;

		for (int i = 0; string[i] != '\0' && *error_flag != true; i++) {

			switch (string[i]) {
			case '\\':
				next_oper_type = POS_0;
				break;
			case 'U':
				next_oper_type = POS_1;
				break;
			case '^':
				next_oper_type = POS_2;
				break;
			case '(':
				next_oper_type = POS_3;
				break;
			case ')':
				next_oper_type = POS_4;
				break;
			case '\n':
				next_oper_type = POS_5;
				break;
			case '[':
				if (add_operand(operands, &i, string, &number, &number_curent_size, &oper_count)) {
					next_oper_type = POS_6;
				}
				else {
					next_oper_type = POS_ERROR;
				}
				break;
			default:
				next_oper_type = POS_ERROR;
				break;
			}

			if (next_oper_type == POS_ERROR) {
				*error_flag = true;
			}
			if (next_oper_type != POS_6) {
				if (!operation(&prev_oper_type, next_oper_type, operands, &oper_count, &i)) {
					*error_flag = true;
				}
			}
		}
		free(number);
	}
	return operands;
}

// Выбор действия после встречи операций, операндов или конца выражения
_Bool operation(int* prev_oper_type, const int next_oper_type, int** operands, int* oper_count, int* string_pos)
{
	static char oper_steck[OPER_STECK_START_SIZE] = { 'x' }; // Стек операций
	static int steck_index = 0;
	static char operations_type[5] = { '\\','U','^','(',')' };
	static const char jump_table[5][6] = {
		/*		  /   U   ^   (   )   <   */
				{'1','1','1','1','?','x'},// >
				{'2','1','1','1','4','4'},// /
				{'4','2','1','1','4','4'},// U
				{'4','4','2','1','4','4'},// ^
				{'1','1','1','1','3','?'} // (
	};

	switch (jump_table[*prev_oper_type][next_oper_type])
	{
	case '1': // Заслать операцию в стек
		steck_index++;
		oper_steck[steck_index] = operations_type[next_oper_type];
		*prev_oper_type = operation_choice(oper_steck[steck_index]);
		break;
	case '2': // Произвести над 2 операндами операцию из стека
		if ((*oper_count) < 2){
			return false;
		}
		if (!calc_operands(operands[*oper_count - 2], operands[*oper_count - 1], oper_steck[steck_index])) {
			return false;
		}
		(*oper_count)--;
		*prev_oper_type = operation_choice(oper_steck[steck_index]);
		break;
	case '3': // Удалить верхнюю операцию из стека
		steck_index--;
		*prev_oper_type = operation_choice(oper_steck[steck_index]); // Установить позицию в соответствии с текущей операцией на вершине стека
		break;
	case '4':
		if ((*oper_count) < 2) {
			return false;
		}
		if (!calc_operands(operands[*oper_count - 2], operands[*oper_count - 1], oper_steck[steck_index])) {
			return false;
		}
		(*oper_count)--;
		steck_index--; 
		(*string_pos)--; // Уменьшаем индекс, чтобы вновь считывать ')' до тех пор, пока на вершине стека не встретится '('
		*prev_oper_type = operation_choice(oper_steck[steck_index]);
		break;
	case 'x':
		break;
	case '?': 
		return false;
		break;
	default:
		return false;
		break;
	}
	return true;
}

// Произведение операции над операндами
_Bool calc_operands(int *first_operand, int *second_operand, char operation)
{
	int len_first_oper = first_operand[0];
	int flag = 0;
	int* tmp_operand = (int*)malloc(sizeof(int)*NUMBER_BUFFER_START_SIZE);
	int new_len = 0;
	switch (operation)
	{
	case '\\':
		for (int i = 1; i <= first_operand[0]; i++) {	// first_operand[0] - количество чисел в операнде
			for (int j = 1; j <= second_operand[0]; j++) { // сами числа начинаются с индекса = 1
				if (first_operand[i] == second_operand[j]) {
					flag = 1;
				}
			}
			if (flag == 0) {
				tmp_operand[new_len + 1] = first_operand[i];
				new_len++;
			}
			flag = 0;
		}
		first_operand[0] = new_len;
		for (int i = 1; i <= new_len; i++) {
			first_operand[i] = tmp_operand[i];
		}
		break;
	case 'U':
		for (int j = 1; j <= second_operand[0]; j++)
		{
			flag = 0;
			for (int i = 1; i <= first_operand[0] && flag != 1; i++)
			{
				if (first_operand[i] == second_operand[j]) {
					flag = 1;
				}
			}
			if (flag != 1)
			{
				first_operand[len_first_oper + 1] = second_operand[j];
				len_first_oper++;
			}
		}
		first_operand[0] = len_first_oper;
		break;
	case '^':
		for (int i = 1; i <= first_operand[0]; i++) {
			for (int j = 1; j <= second_operand[0]; j++) {
				if (first_operand[i] == second_operand[j]) {
					tmp_operand[new_len + 1] = first_operand[i];
					new_len++;
				}
			}
		}
		first_operand[0] = new_len;
		for (int i = 1; i <= new_len; i++) {
			first_operand[i] = tmp_operand[i];
		}
		break;
	}
	free(tmp_operand);
	return true;
}

// Определение текущей операции на вершине стека
int operation_choice(char operation)
{
	const char mass[COUNT_OF_OPER_TYPE] = { 'x','\\','U','^','(' };
	for (int i = 0; i < COUNT_OF_OPER_TYPE; i++) {
		if (mass[i] == operation) {
			return i;
		}
	}
	return POS_ERROR;
}

// Вывод результата
void print_result(int* result)
{
	printf("[");
	for (int i = 1; i <= result[0]; i++)
	{
		if (i != result[0]) {
			printf("%d,", result[i]);
		}
		else {
			printf("%d", result[i]);
		}
	}
	printf("]");
}

// Освобождение памяти
void free_memory(int** result, char* string) {

	if (result != NULL) {
		for (int i = 0; i < OPERANDS_BUFFER_COUNT; i++) {
			if (result[i] != NULL) {
				free(result[i]);
			}
		}
		free(result);
	}
	if (string != NULL) {
		free(string);
	}
}

bool add_operand(int** operands, int* i, const char* string, char** number, int* number_curent_size, int* oper_count) {
	(*i)++;
	int digit_count = 0, digit_index = 0;
	bool error_flag = false;
	while (string[(*i)] != ']' && error_flag != true) {
		if (string[(*i)] != ',') {
			for (digit_index = 0; string[(*i)] != ',' && string[(*i)] != ']' && error_flag != true; (*i)++, digit_index++) {
				if (digit_index >= (*number_curent_size)) {
					if (((*number) = (char*)realloc((*number), (sizeof(char) * (*number_curent_size)) * RESIZE_RATIO)) == NULL) {
						error_flag = true;
					}

				}
				(*number)[digit_index] = (string[(*i)]);
			}
			if (!error_flag) {
				(*number)[digit_index] = '\0';
				operands[(*oper_count)][++digit_count] = atoi((*number));
			}
		}
		else {
			(*i)++;
		}
	}
	operands[(*oper_count)][0] = digit_count;
	(*oper_count)++;
	if (error_flag) {
		return false;
	}
	else {
		return true;
	}
}