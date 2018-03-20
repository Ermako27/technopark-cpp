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
		}
		
		if (strchr(operations, str[i]) != NULL && flag == 0)
		{
			lens_of_nums[k] = len_of_num;
			len_of_num = 0;
			k += 1;
			flag = 1;

		}
	}

	lens_of_nums[k] = len_of_num;
	print_array(lens_of_nums, *nums_count);

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

int* long_substraction(int* int_a, int* int_b, int length, int a_len, int b_len); // вычитание
// -a+(-b); -a+b; a+(-b); a+b
int* long_sum(int* int_a, int* int_b, int length, int a_len, int b_len) // суммирование длинных чисел (падает с segm на 40 разрядах)
{
	int dec, div, new_dec; 
	int *result = malloc(sizeof(int) * length);




	// случай, когда на вход подается два числа с унарным минусом -a+(-b)-> 
 //    -> выполнить сложение, добавив к результату минус в конце 
    if (int_a[a_len-1] < 0 && int_b[b_len-1] < 0)
    {
    	// printf("###\n");
    	int_a[a_len-1] *= -1;
    	int_b[b_len-1] *= -1;
    	// print_array(int_a,length);
    	// print_array(int_b,length);

    	int* res = long_sum(int_a, int_b, length, a_len, b_len);
    	// printf("Sum result: ");
    	// print_array(abs_res,length);
    	// printf("\n");

    	int k=0;
    	int flag = 1;
    	for(int i = length-1; i >= 0; i--)
    	{
    		if(res[i] != 0 && flag ==1) // идем с конца так как разряды стоят в обратном порядке
    		{
    			k = i; // запоминаем где закончились лишние нули
    			flag = 0;
    		}
    	}
    	// printf("IN K: %d \n", k);
    	res[k] *= -1;
    	// printf("Sum result with minus: ");
    	// print_array(abs_res, length);
    	// printf("\n");
    	return res;
    }

    // случай -a+b <=> b-a
    if (int_a[a_len-1] < 0 && int_b[b_len-1] >= 0)
    {	
    	int_a[a_len-1] *= -1;
    	int *res = long_substraction(int_b, int_a, length, b_len, a_len);
    	return res;
    }

    // случай a+(-b) <=> a-b
    if (int_a[a_len-1] >=0 && int_b[b_len-1] < 0)
    {
    	int_b[b_len-1] *= -1;
    	int *res = long_substraction(int_a, int_b, length, a_len, b_len);
    	return res;
    }


	// проставляем разряды нулями
    for (int i = 0; i < length; i++)
    	result[i] = 0;

	// само сложение
    for (int i = 0; i < length-1; i++)
    {
    	// поразрядное сложение
    	dec = int_a[i] + int_b[i];
    	if (dec < 10)
    	{
    		if (result[i] + dec < 10)
    			result[i] += dec;
    		else
    		{
    			new_dec = (result[i] + dec) % 10; // целая часть
     			div = (result[i] + dec) / 10; // дробная часть
    			result[i + 1] += div;
    			result[i] = new_dec;   			
    		}
    		div = 0;
    	}
    	else if (dec >= 10)
    	{
    		new_dec = (result[i] + dec) % 10;
            div = (result[i] + dec) / 10;
            result[i + 1] += div;
            result[i] = new_dec;
    	}
    }
    return result;
}


// a-b(a>b;a,b); a-b(a<b;a,b); -a-b(-a,b); 
int* long_substraction(int* int_a, int* int_b, int length, int a_len, int b_len) // вычитание
{
	int dec, div, new_dec, flag = 1; 
	int *result = malloc(sizeof(int)*length);

	// проставляем разряды нулями
    for (int i = 0; i < length; i++)
    	result[i] = 0;

    // случай -a-b - только первое чилсо с унарным минусом(на вход подается -a,b) ->
    // -> сложить и в конце добавить минус, то есть умножить на -1 так как вернется массив с интовскими разрядами
    if (int_a[a_len-1] < 0 && int_b[b_len-1] > 0)
    {
    	// printf("***\n");
    	int_a[a_len-1] *= -1; // умножаем на -1 чтобы сложились два числа без унарных минусов
    	// int_b[b_len-1] *= -1;
    	// print_array(int_a,length);
    	// print_array(int_b,length);

    	int* abs_res = long_sum(int_a, int_b, length, a_len, b_len);
    	// printf("Sum result: ");
    	// print_array(abs_res,length);
    	// printf("\n");

    	int k=0;
    	int flag = 1;
    	// находим позицию старшего разряда
    	for(int i = length-1; i >= 0; i--)
    	{
    		if(abs_res[i] != 0 && flag ==1) // идем с конца так как разряды стоят в обратном порядке
    		{
    			k = i; // запоминаем где закончились лишние нули
    			flag = 0;
    		}
    	}
    	abs_res[k] *= -1; // умножаем старший разряд на -1
    	// printf("Sum result with minus: ");
    	// print_array(abs_res, length);
    	// printf("\n");
    	return abs_res;
    }

    // случай  a-(-b) - только второе число с унарным минусом(на вход a,-b) ->
    // -> сложить, не добавляя минус в конце
    if (int_a[a_len-1] > 0 && int_b[b_len-1] < 0)
    {
    	// printf("$$$\n");
    	int_b[b_len-1] *= -1;
    	// print_array(int_a,length);
    	// print_array(int_b,length);

    	int* abs_res = long_sum(int_a, int_b, length, a_len, b_len);
    	// printf("Sum result: ");
    	// print_array(abs_res,length);
    	// printf("\n");

    	return abs_res;    	
    }

    // случай -a-(-b) <=> -a+b <=> b-a
    if (int_a[a_len-1] < 0 && int_b[b_len-1] < 0)
    {
    	// printf("HI\n");
    	int_a[a_len-1] *= -1;
    	int_b[b_len-1] *= -1;
    	int *abs_res = long_substraction(int_b, int_a, length, b_len, a_len);
    	return abs_res;
    }

// случай когда вычитаем из меньшего числа, a-b; a < b
    if ((a_len < b_len) || ((a_len==b_len) && (int_a[a_len-1] < int_b[b_len-1]))) 
    {
    	// printf("!\n");
    	// printf("lasta: %d lastb: %d\n", int_a[a_len-1], int_b[b_len-1]);
    	if (int_a[a_len-1] == 0 && int_b[b_len-1] < 0) // случай 0-(-123) -> 123
    	{
    		int_b[b_len-1] *= -1;
    		return int_b;
    	}
    	int* abs_res = long_substraction(int_b, int_a, length, b_len, a_len);
    	for (int i = length-1; i >= 0; i--)
    	{
    		if (abs_res[i] !=0 && flag == 1)
    		{
    			flag = 0;
    			abs_res[i] = -1*abs_res[i];
    		}
    	}
    	return abs_res;
    }

    if ((a_len > b_len) || ((a_len==b_len) && (int_a[a_len-1] >= int_b[b_len-1]))) // случай когда вычитаем из большего числа, числа без унарного минуса
    {
    	// printf("?\n");
    	// printf("lasta: %d lastb: %d\n", int_a[a_len-1], int_b[b_len-1]);
    	// printf("alen: %d blen: %d\n", a_len,b_len);
    	if (int_a[a_len-1] < 0 && int_b[b_len-1] == 0) // случай -123-0
    	{
    		return int_a;
    	}
    	for (int i = 0; i < a_len; i++)
    	{
    		// printf("int_b[i]: %d\n", int_b[i]);
    		if (int_a[i] < int_b[i])
    		{
    			result[i] = int_a[i] + 10 - int_b[i];
    			int_a[i+1] -= 1;
    		}
    		else
    		{
    			result[i] = int_a[i] - int_b[i];
    			// printf("int_a[i] %d int_b[i] %d result[i] %d\n", int_a[i], int_b[i], result[i]);
    		}
    	}
    	print_array(result,length);
    	return result;
    }
    
}

void wrapper(char* a, char* b, int* (*operation)(int*, int*, int, int, int))
{
	int length; // длина результата
	char *res; // итоговая строка с числом
	int* result;
	int a_len = strlen(a);
	int b_len = strlen(b);
	// интовские массивы для вычислений (нужно перевести в такие массивы так как на вход - строки)
	int *int_a, *int_b;


	// определяем длину массива суммы, и создаем 2 массива типа int, хранящие разряды чисел
	// для корректного вычисления память выделяется одинакового размера и заполняется нулями
	if (a_len >= b_len)
	{
		length = a_len + 1;
		// интовские массивы для вычислений (нужно перевести в такие массивы так как на вход - строки)
		int_a = malloc(length*sizeof(int));
		int_b = malloc(length*sizeof(int));
    	for (int i = 0; i < length; i++)
    	{
			int_a[i] = 0;
			int_b[i] = 0;
    	}
	}
	else
	{
		length = b_len + 1;
		// интовские массивы для вычислений (нужно перевести в такие массивы так как на вход - строки)
		int_a = malloc(length*sizeof(int));
		int_b = malloc(length*sizeof(int));
    	for (int i = 0; i < length; i++)
    	{
			int_a[i] = 0;
			int_b[i] = 0;
    	}
	}

	// на данном этапе массивы выглядят как {0, 0, 0, 0} {0, 0, 0 ,0}
	// заполнение массивов разрядами
	if (a[0] != '-')
	{
		for (int i = 0; i < a_len; i++)
			int_a[i] = a[a_len-i-1] - '0';
	}
	else // если число отрицательное 
	{
		for (int i = 0; i < a_len-1; i++)
			int_a[i] = a[a_len-i-1] - '0';
		a_len -= 1; // длина уменьшается так как минус не является цифрой
		int_a[a_len-1] *= -1; // последний разряд умножается на -1
	}

	if (b[0] != '-')
	{
		for (int i = 0; i < b_len; i++)
			int_b[i] = b[b_len-i-1] - '0';
	}
	else
	{
		for (int i = 0; i < b_len-1; i++)
			int_b[i] = b[b_len-i-1] - '0';
		b_len -= 1;
		int_b[b_len-1] *= -1;
	}

	// // пусть складываются два числа 123+3
	// // значтит в функцию подаются массивы следующего вида {3, 2, 1, 0} {3, 0, 0, 0} 
	// // остальные операции аналогично
    result = operation(int_a, int_b, length, a_len, b_len); // операция передаваямая через указатель


 //    // отсечение лишних нулей для корректного преобразования в массива разрядов в строку
    int k=0;
    int flag = 1;
    for(int i = length-1; i >= 0; i--)
    {
    	if(result[i] != 0 && flag ==1) // идем с конца так как разряды стоят в обратном порядке
    	{
    		k = i; // запоминаем где закончились лишние нули
    		flag = 0;
    	}
    }

    // printf("K %d\n", k);
    if (result[k] < 0) // преобразование в строку если значение result < 0
    {
    	res = malloc(sizeof(char) * (k+2));
    	res[0] = '-';
   		for (int i = 0; i < k+1; i++)
   		{
    		res[i+1] = abs(result[k-i]) + '0';
    		// printf("res[i]: %c\n", res[i+1]);
   		}
    }

    else // преобразование если значение result > 0
    {
    	if (k == 0) // если результат вычитания  = 0
    	{
    		res = malloc(sizeof(char) * 1);
    		for (int i = 0; i < k+1; i++)
    			res[i] = result[k-i] + '0';
    	}
    	else
		{
		    res = malloc(sizeof(char) * k);
    		for (int i = 0; i < k+1; i++)
    			res[i] = result[k-i] + '0';	
		}
    }

    free(result);

	printf("%s\n", res);
}

// void long_mult(char *a, char *b)
// {
// 	int length; // длина результата
// 	int dec, div, new_dec; 
// 	char *res; // итоговая строка с числом
// 	int a_len = strlen(a);
// 	int b_len = strlen(b);
// 	// интовские массивы для вычислений (нужно перевести в такие массивы так как на вход - строки)
// 	int int_a[a_len]; 
// 	int int_b[b_len];

// 	// интовское поразрядное представление чисел
// 	for (int i = 0; i < a_len; i++)
// 		int_a[i] = a[i] - '0';
// 	for (int i = 0; i < b_len; i++)
// 		int_b[i] = b[i] - '0';

// 	// определяем длину массива суммы
// 	if (a_len >= b_len)
//     	length = a_len + 1;
// 	else
//     	length = b_len + 1;

//     // промежуточный интовский массив для хранения результата (для вычислений)
//     int *result = malloc(sizeof(int) * length);

//     // проставляем разряды нулями
//     for (int i = 0; i < length; i++)
//     	result[i] = 0;

//     for (i = 0; i < b_len; i++)
//     {
//         for (int j = 0; j < a_len; j++)
//         {
//             decimal = m_int_1[m_count_1 - j - 1] * m_int_2[m_count_2-i - 1];
//             if (decimal < 10)
//             {
//                 if (res[j+i] + decimal < 10)
//                 {
//                     res[j+i] += decimal;
//                 }
//                 else
//                 {
//                     new_dec = (res[j+i] + decimal) % 10;
//                     div = (res[j+i] + decimal) / 10;
//                     res[j+i] = new_dec;
//                     res[j+i+1] += div;  
//                 }
//                 div = 0;

//             }
//             else if (decimal >= 10)
//             {
//                 new_dec = (res[j+i] + decimal) % 10;
//                 div = (res[j+i] + decimal) / 10;
//                 res[j + i + 1] += div;
//                 res[j + i] = new_dec;
//             }
//         }
//     }
// }


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

	printf("----------------\n");

	// wrapper(data[0], data[1], long_substraction);
	wrapper(data[0], data[1], long_sum);
	free_matrix_rows(data,nums_count);



	// shunting_yard(line);


	return 0;
}