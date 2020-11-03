#include "imsl.h"
#include <iostream>
#include <fstream> 
#include <stdlib.h>

template<typename T>
void print_square_matrix(T* matrix, char* title, int dimension, char flag) {
	/*
		Компилятор отказывается собирать программу:

			./LineralSystem_template.cpp:18:52: error: cannot convert ‘float*’ to ‘double*’
			   18 |   imsl_d_write_matrix(title, dimension, dimension, matrix, 0);
			      |                                                    ^~~~~~
			      |                                                    |
			      |                                                    float*

		Т.е. при компиляции совершается прогон, для связывания происходит сверка типов, где и выявляется несоответствие функции с объявленным типом T = float и библиотечной функции. 
		Примечательно,что если не делать вызовов шаблонной функции в main, то программа скомпилируется и будет работать. Ошибок не будет.		
	
		Стоит подумать о перегрузке обычной функции, которая будет выполнять роль промежуточного интерфейса. Можно ли здесь использовать анонимную функцию?
	 */
	std::cout << "In function print_square_matrix() flag = " << flag << std::endl;

	if(flag == 'f') {
		imsl_f_write_matrix(title, dimension, dimension, matrix, 0);
		return;
	}
	else if(flag == 'd') {
		imsl_d_write_matrix(title, dimension, dimension, matrix, 0);
	}
	else {
		std::cout << "ERROR. The templated function print_square_matrix received a non-expected value for the '" << flag << "' flag." << std::endl; 
		exit(-3);
	}
	
}

int main()
{
	std::cout << "Test programm" << std::endl;
	float* data = new float[121];
	std::ifstream inf("/Experiences/Numerical_Methods/Lab1/RGEmtr029.dat");
	if (!inf.is_open())
		std::cout << "ERROR. Cannot open file";

	for (int i=0; i<121; i++)
		inf >> data[i];

	std::cout << "Check reading. Input Data:" << std::endl;
	for (int i=0; i< 11; i++, std::cout << std::endl)
		for (int j=0; j< 11; j++)
			std::cout << data[11*i+j] << "   ";

	inf.close();

	// Если закомментировть строку ниже, то программа скомпилируется и будет работать.
	//print_square_matrix<float>(data, "Test template function", 11, 'f');	
	return 0;
}
