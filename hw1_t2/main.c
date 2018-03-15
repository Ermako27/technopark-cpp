#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INCORRECT_BRACKETS -1
#define STACK_UNDERFLOW -2
#define STACK_OVERFLOW -3


typedef struct Node_tag // структура стека
{ 
    char* value;
    struct Node_tag *next;
} Node_t;

void print_array(int array[], int l) // функция печати массива
{
	for (int i = 0; i < l; ++i)
	{
		printf("%d | ", array[i]);
	}
}



void free_matrix_rows(char **data, int n)
{
    for (int i = 0; i < n; i++)
       // free можно передать NULL
       free(data[i]);

    free(data);
}

void push(Node_t **head, char* value) 
{
    Node_t *tmp = malloc(sizeof(Node_t));

    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}



char* pop(Node_t **head) 
{
    Node_t *out;
    char* value;

    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}



void printStack(const Node_t* head) 
{
    printf("stack >");
    while (head) 
    {
        printf("%s ", head->value);
        head = head->next;
    }
}


char** createNumsArray(char *str, int *nums_count) // создание массива строк, которые являются числами из выражения, чтобы оттуда было удобнее пушить в стек
{
	char operands[10] = "0123456789";
	char operations[6] = "()+-*/";

	*nums_count = 0; // общее количество чисел в строке
	int len_of_num = 0; // длина отдельного числа
	int l = strlen(str) - 1; // длина самого выражения

	int flag = 1; // флаг показывающий был ли знак до цифры, если да, то с этой цифры начинается новое число, 1-был, 0-не был

	for (int i = 0; i < l; i++) // цикл для нахождения общего количества чисел в строке
	{
		if ((flag == 1) && (strchr(operands, str[i]) != NULL))
		{
			*nums_count += 1;
			flag = 0;
		}

		if (strchr(operations, str[i]) != NULL)
		{
			flag = 1;
		}
	}


	flag = 1;
	int k = 0; // счетчик массива lens_of_nums
	int lens_of_nums[*nums_count]; // массив, содержащий длины каждого числа

	for (int i = 0; i < l; i++) // вычисление длинны каждого слова
	{
		if ((strchr(operands, str[i]) != NULL) || str[i] == '-')
		{
			if (flag == 1 && str[i] == '-')
				len_of_num++;
			if (strchr(operands, str[i]) != NULL)
			{
				len_of_num++;
				flag = 0;
			}
			// printf("%c, %d, %d\n", str[i], flag, len_of_num);
		}
		
		if (strchr(operations, str[i]) != NULL && flag == 0)
		{
			lens_of_nums[k] = len_of_num;
			len_of_num = 0;
			k += 1;
			flag = 1;

		}
	}

	// lens_of_nums[k] = len_of_num;
	// print_array(lens_of_nums, *nums_count);
	// printf("\n");


	// выделение памяти для матрицы строк

	char **data = calloc(*nums_count, sizeof(char*));
    if (!data)
        return NULL;

    for (int i = 0; i < *nums_count; i++)
    {
        data[i] = malloc(lens_of_nums[i] * sizeof(char));
        if (!data[i])
        {
            free_matrix_rows(data, *nums_count);
            return NULL;
        }
    }



    // заполнение массива чисел

    int p = 0;
    int j = 0;
    flag = 1;

    for (int i = 0; i < l; i++)
	{
		if ((strchr(operands, str[i]) != NULL) || str[i] == '-')
		{
			if (flag == 1 && str[i] == '-')
			{
				data[p][j] = str[i];
				j++;
			}
			if (strchr(operands, str[i]) != NULL)
			{
				data[p][j] = str[i];
				j++;
				flag = 0;
			}
		}
		
		if (strchr(operations, str[i]) != NULL && flag == 0)
		{
			p += 1;
			j = 0;
			flag = 1;

		}
	}
	return data;

}



void shunting_yard(char* str) // алгорит сортировочной станции
{	


	Node_t *Q_head = NULL;
	Node_t *W_head = NULL;
	char operations[6] = "()+-*/";
	char operands[10] = "0123456789";

	int l = strlen(str) - 1;
	for (int i = 0; i < l; i++) // проходим по всей строке
	{
		if (strchr(operations, str[i]) != NULL)
		{
			char *operand = malloc(sizeof(char)*1);
			*operand = str[i];
			push(&W_head, operand);
		}
	}
	printStack(W_head);

}



int brackets_check(char *str) // проверка правильности проставления скобок, если все верно, посчитать количество скобок
{
	printf("str: %s\n", str);
	int open_count = 0; // количество открывающих скобок
	int close_count = 0; // количество закрывающий скобок
	int l = strlen(str); // длина строки 

	for(int i = 0; i < l-1; ++i)
	{
		if (str[i] == '(')
			open_count++;
		if (str[i] == ')')
			close_count++;
	}
	if (open_count == close_count)
		return open_count;
	else
		return INCORRECT_BRACKETS;
}






int main(void)
{
	int nums_count;
	char **data;
	int brackets;
	char *line;

	size_t len = 0;
	printf("Введите выражение а: ");
	getline(&line, &len, stdin); // ввод строки

	printf("Ваше выражение: %s\n", line);
	data = createNumsArray(line, &nums_count);

	for (int i = 0; i < nums_count; i++)
	{
		printf("%s\n", data[i]);
	}

	// shunting_yard(line);







	return 0;
}