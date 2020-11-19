  #include "imsl.h"
#include <iostream>
#include <fstream> 
#include <iomanip>  
#include <limits>

const char path_to_matrix[] = "/Experiences/Numerical_Methods NO GIT/Lab5/RSImtr029.dat";

int TERMINAL_WIDTH = 230;


//...

int main()
{
    const int dim = 27;
    const double parameter[3] = { 1.0000000,  0.0156250,  0.0000000};

    double mtr[dim*dim];
    double eigenvalues[dim], eigenvectors[dim*dim], residual[dim*dim], 
           lambda_x[dim], a_x[dim], temp[dim];


    imsl_page(IMSL_SET_PAGE_WIDTH, &TERMINAL_WIDTH);
    std::cout << "Lab 5 Var 29" << std::endl << "3430302/90003" << std::endl << "Biga V.S." << std::endl;
    
    std::ifstream input_matrix(path_to_matrix);
    
    if ( !input_matrix.is_open() )
        std::cout << "ERROR. Cannot open input file";

    for (int i=0; i < dim*dim; i++)
        input_matrix >> mtr[i];

    imsl_d_write_matrix("Input matrix A", dim, dim, mtr, IMSL_WRITE_FORMAT, "%6.1f", 0);

    input_matrix.close();       

    std::cout << "\n\nLet's start running the program." << std::endl;




 //   ...

    for (int i = 0; i < 3; ++i)
    {

        std::cout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
        std::cout << "parameter P = " << parameter[i] << std::endl;

        // Копирование матрицы
        //double matrix[dim*dim];
        double* matrix = new double[dim*dim];

        for (int i = 0; i < dim*dim; ++i)
            matrix[i] = mtr[i];

        matrix[0] += parameter[i];

        double eigenvectors[dim*dim], eigenvalues[dim];

//        ...
        // Вставка
        imsl_d_eig_sym(dim, matrix,
                       IMSL_RETURN_USER, eigenvalues,
                       IMSL_VECTORS_USER, eigenvectors,
                       0); // Нахождение собственных чисел и собственных векторов

        imsl_d_write_matrix("Eigenvalues are: ", dim, 1, eigenvalues, 0);
        imsl_d_write_matrix("Eigenvectors are: ", dim, dim, eigenvectors, 0);

        // Вычисление вектора невязки
        for (int j = 0; j < dim; ++j)
        {
            // Разбиение матрицы собственных векторов на отдельные вектора
            /* Странный момент. Берем срез по столбцам, хотя массив храниться в памяти по строкам. Транспонированная матрица? */
            for (int k = 0; k < dim; ++k) temp[k] = eigenvectors[k*dim + j];
            // Перемножаем матрицу a и собственные вектора x
            std::cout << "\n0000000000000000000000000000000000000000000000000000000000000000000000000000000000000\n\n\n";
            for (int k = 0; k < dim; ++k) std::cout << temp[k] << "\t";
            std::cout << "\n0000000000000000000000000000000000000000000000000000000000000000000000000000000000000\n\n\n";

            // Вставка: проверка
            std::cout << "\nWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW" << "\n\n";
            std::cout << "\n\n\n\t\t\t i ===== " << j+1 << std::endl;
            for (int k = 0; k < dim; ++k) std::cout << temp[k] << "\t";
            //            
            imsl_d_mat_mul_rect("A*x", 
                                IMSL_RETURN_USER, a_x,
                                IMSL_A_MATRIX, dim, dim, matrix,
                                IMSL_X_VECTOR, dim, temp,
                                0);
            imsl_d_write_matrix("TEST A*x_i", dim, 1, a_x, 0);
        // Ради интереса:
            std::cout << "***************************\n" << "\nTest print\n\n";
            imsl_d_write_matrix("A*x", 1, dim, a_x, IMSL_WRITE_FORMAT, "%6.1f", 0);

            // Считаем произведение собственного числа на собственный вектор
            for (int l = 0; l < dim; ++l) lambda_x[l] = eigenvalues[j]*temp[l]; 
            for (int m = 0; m < dim; ++m) residual[m*dim + j] = lambda_x[m] - a_x[m];
        }

        imsl_d_write_matrix("Vectors of residuals:", dim, dim, residual, 0);

        //...   
        char any_key_value;
        std::cout << "\nPress any key to go to next.\n\n";
        std::cin >> any_key_value;

    }


}
