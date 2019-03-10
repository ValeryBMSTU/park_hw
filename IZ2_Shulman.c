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

#define STRING_BUFFER_SIZE 256
#define NUMBER_BUFFER_SIZE 16
#define ADD_STR_SIZE 3
#define OPER_STEC_SIZE 8
#define M 64
#define N 16
#define TRUE 1
#define FALSE 0
#define POS_0 0
#define POS_1 1
#define POS_2 2
#define POS_3 3 
#define POS_4 4 
#define POS_5 5
#define POS_6 6
#define POS_ERROR -1
// Ввод исходной строки
int input(char** string);
// Проход по строке в поисках операций, операндов или конца выражения
int** set_calc(const char* const string, int* error_flag);
// Выбор действия после встречи операций, операндов или конца выражения
int operation(int* position, const int next_oper, int** operands, int* oper_count, unsigned int* string_pos);
// Произведение операции над операндами
int calc_operands(int* operand_1, int* operand_2, char operation);
// Определение текущей операции на вершине стека
int operation_choice(char operation);
// Вывод результата
int print_result(int* result);
// Освобождение памяти
int free_memory(int** result, char* string);

// Решение с помощью алгоритма Бауэра-Замельзона (Польская запись).
int main()
{
	char* string = NULL;
	int error_flag = FALSE;;

	if (!input(&string)) {
		int** result = set_calc(string, &error_flag);
		if (error_flag) {
			printf("[error]");
			free(string);
		}
		else {
			print_result(*result);
			free_memory(result, string);
		}
	}
	else
	{
		printf("[error]");
	}
	system("pause");
	return 0;
}

// Ввод исходной строки
int input(char** string)
{
	char buffer[STRING_BUFFER_SIZE] = { '\0' };
	if (!((buffer == fgets(buffer, STRING_BUFFER_SIZE, stdin)) && buffer[0] != '\n')) {
		return TRUE;
	}
	else {
		(*string) = (char*)malloc(sizeof(char) * (strlen(buffer) + ADD_STR_SIZE));
		if ((*string) == NULL) {
			return TRUE;
		}
		int j = 0;
		(*string)[j] = '(';
		j++;
		for (unsigned int i = 0; buffer[i] != '\0' && buffer[i] != '\n'; i++) {
			if (buffer[i] != ' ') {
				(*string)[j] = buffer[i];
				j++;
			}
		}
		(*string)[j] = ')';
		(*string)[j + 1] = '\0';
	}
	int c1 = 0, c2 = 0;
	for (int i = 0; (*string)[i] != '\0'; i++) {
		if ((*string)[i] == '(') {
			c1++;
		}
		else if ((*string)[i] == ')') {
			c2++;
		}
	}
	if (c1 != c2) {
		free((*string));
		return TRUE;
	}

	//printf("%s", string);
	return FALSE;
}

// Проход по строке в поисках операций, операндов или конца выражения
int** set_calc(const char* const string, int* error_flag)
{
	int** operands = NULL;
	if ((operands = (int**)malloc(sizeof(int**)*N)) == NULL) {
		*error_flag = TRUE;
		return operands;
	}
	for (int i = 0; i < N; i++)
	{
		if ((operands[i] = (int*)malloc(sizeof(int*)*M)) == NULL) {
			for (int j = 0; j < i; j++)
			{
				free(operands[j]);
			}
			free(operands);
			*error_flag = TRUE;
			return operands;
		}
	}
	int position = 0, next_oper = 0;
	static int oper_count = 0;

	for (unsigned int i = 0, digit_count = 0; string[i] != '\0'; i++) {

		char number[NUMBER_BUFFER_SIZE] = { '\0' };

		switch (string[i]) {
		case '\\':
			next_oper = POS_0;
			break;
		case 'U':
			next_oper = POS_1;
			break;
		case '^':
			next_oper = POS_2;
			break;
		case '(':
			next_oper = POS_3;
			break;
		case ')':
			next_oper = POS_4;
			break;
		case '\n':
			next_oper = POS_5;
			break;
		case '[':
			i++;
			digit_count = 0;
			while (string[i] != ']') {
				if (string[i] != ',') {
					for (int digit_index = 0; string[i] != ',' && string[i] != ']'; i++, digit_index++) {
						number[digit_index] = (string[i]);
					}
					operands[oper_count][++digit_count] = atoi(number);
					for (int digit_index = 0; number[digit_index] != '\0' && digit_index < NUMBER_BUFFER_SIZE; digit_index++) {
						number[digit_index] = '\0';
					}
				}
				else {
					i++;
				}
			}
			operands[oper_count][0] = digit_count;
			oper_count++;
			next_oper = POS_6;
			break;
		default:
			next_oper = POS_ERROR;
			break;
		}
		if (next_oper == POS_ERROR) {
			*error_flag = TRUE;
			return operands;
		}
		if (next_oper != 6) {
			if (operation(&position, next_oper, operands, &oper_count, &i)) {
				*error_flag = TRUE;
				return operands;
			}
		}
	}
	return operands;
}

// Выбор действия после встречи операций, операндов или конца выражения
int operation(int* position, const int next_oper, int** operands, int* oper_count, unsigned int* string_pos)
{
	static char oper_stec[OPER_STEC_SIZE] = { 'x' }; // Стек операций
	static int stec_index = 0;
	static char operations_type[5] = { '\\','U','^','(',')' };
	static const char matrix[5][6] = {
		/*		  /   U   ^   (   )   <   */
				{'1','1','1','1','?','x'},// >
				{'2','1','1','1','4','4'},// /
				{'4','2','1','1','4','4'},// U
				{'4','4','2','1','4','4'},// ^
				{'1','1','1','1','3','?'} // (
	};

	switch (matrix[*position][next_oper])
	{
	case '1': // Заслать операцию в стек
		stec_index++;
		oper_stec[stec_index] = operations_type[next_oper];
		*position = operation_choice(oper_stec[stec_index]);
		break;
	case '2': // Произвести над 2 операндами операцию из стека
		if (calc_operands(operands[*oper_count - 2], operands[*oper_count - 1], oper_stec[stec_index])) {
			return TRUE;
		}
		(*oper_count)--;
		*position = operation_choice(oper_stec[stec_index]);
		break;
	case '3': // Удалить верхнюю операцию из стека
		stec_index--;
		*position = operation_choice(oper_stec[stec_index]); // Установить позицию в соответствии с текущей операцией на вершине стека
		break;
	case '4':
		if (calc_operands(operands[*oper_count - 2], operands[*oper_count - 1], oper_stec[stec_index])) {
			return TRUE;
		}
		(*oper_count)--;
		stec_index--;
		(*string_pos)--;
		*position = operation_choice(oper_stec[stec_index]);
		break;
	case 'x':
		break;
	case '?': 
		return TRUE;
		break;
	default:
		return TRUE;
		break;
	}
	return FALSE;
}

// Произведение операции над операндами
int calc_operands(int *operand_1, int *operand_2, char operation)
{
	int len_oper_1 = operand_1[0];
	int flag = 0;
	int operand_3[256];
	int new_len = 0;
	switch (operation)
	{
	case '\\':
		for (int i = 1; i <= operand_1[0]; i++) {
			for (int j = 1; j <= operand_2[0]; j++) {
				if (operand_1[i] == operand_2[j]) {
					flag = 1;
				}
			}
			if (flag == 0) {
				operand_3[new_len + 1] = operand_1[i];
				new_len++;
			}
			flag = 0;
		}
		operand_1[0] = new_len;
		for (int i = 1; i <= new_len; i++) {
			operand_1[i] = operand_3[i];
		}
		break;
	case 'U':
		for (int j = 1; j <= operand_2[0]; j++)
		{
			flag = 0;
			for (int i = 1; i <= operand_1[0] && flag != 1; i++)
			{
				if (operand_1[i] == operand_2[j]) {
					flag = 1;
				}
			}
			if (flag != 1)
			{
				operand_1[len_oper_1 + 1] = operand_2[j];
				len_oper_1++;
			}
		}
		operand_1[0] = len_oper_1;
		break;
	case '^':
		for (int i = 1; i <= operand_1[0]; i++) {
			for (int j = 1; j <= operand_2[0]; j++) {
				if (operand_1[i] == operand_2[j]) {
					operand_3[new_len + 1] = operand_1[i];
					new_len++;
				}
			}
		}
		operand_1[0] = new_len;
		for (int i = 1; i <= new_len; i++) {
			operand_1[i] = operand_3[i];
		}
		break;
		//default:
			//break;
	}


	return FALSE;
}

// Определение текущей операции на вершине стека
int operation_choice(char operation)
{
	switch (operation)
	{
	case 'x':
		return POS_0;
	case '\\':
		return POS_1;
	case 'U':
		return POS_2;
	case '^':
		return POS_3;
	case '(':
		return POS_4;
	default:
		return POS_ERROR;
	}
}

// Вывод результата
int print_result(int* result)
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

	return 0;
}

// Освобождение памяти
int free_memory(int** result, char* string) {

	for (int i = 0; i < N; i++) {
		free(result[i]);
	}
	free(result);
	free(string);

	return 0;
}