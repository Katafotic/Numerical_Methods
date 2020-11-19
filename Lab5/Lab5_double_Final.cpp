#include "imsl.h"
#include <iostream>
#include <fstream> 
#include <iomanip>  
#include <limits>

const char path_to_matrix[] = "/Experiences/Numerical_Methods NO GIT/Lab5/RSImtr029.dat";

int TERMINAL_WIDTH = 230;


int main()
{
    const int dim = 27;
    const double parameter[3] = { 1.0000000,  0.0156250,  0.0000000};

    double mtr[dim*dim];
    double eigenvalues[dim], eigenvectors[dim*dim], residual[dim*dim], 
           lambda_x[dim], a_x[dim], temp[dim];


    imsl_page(IMSL_SET_PAGE_WIDTH, &TERMINAL_WIDTH);
    std::cout << "Lab 5 Var 29" << std::endl << "3430302/90003" << std::endl << "Biga V.S." << std::endl << "Dimension: " << dim << std::endl;
    std::cout << "Type double" << std::endl;
    
    std::ifstream input_matrix(path_to_matrix);
    
    if ( !input_matrix.is_open() )
        std::cout << "ERROR. Cannot open input file";

    for (int i=0; i < dim*dim; i++)
        input_matrix >> mtr[i];

    imsl_d_write_matrix("Input matrix A", dim, dim, mtr, IMSL_WRITE_FORMAT, "%6.1f", 0);

    input_matrix.close();       

    std::cout << "\n\nLet's start running the program." << std::endl;

    // цикл перебирающий врианты с double
    for (int i = 0; i < 3; ++i)
    {

        std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
        std::cout << "parameter P = " << parameter[i] << std::endl;
        std::cout << "Type double" << std::endl;

        // Копирование матрицы
        // double matrix[dim*dim];
        double* matrix = new double[dim*dim];

        for (int i = 0; i < dim*dim; ++i)
            matrix[i] = mtr[i];

        matrix[0] += parameter[i];
        imsl_d_write_matrix("Matrix after add parameter", dim, dim, matrix, IMSL_WRITE_FORMAT, "%12.7f", 0);
        double eigenvectors[dim*dim], eigenvalues[dim];

        imsl_d_eig_sym(dim, matrix,
                       IMSL_RETURN_USER, eigenvalues,
                       IMSL_VECTORS_USER, eigenvectors,
                       0); // Нахождение собственных чисел и собственных векторов

        imsl_d_write_matrix("Eigenvalues are: ", dim, 1, eigenvalues, IMSL_WRITE_FORMAT, "%12.7f", 0);
        imsl_d_write_matrix("Eigenvectors are: ", dim, dim, eigenvectors, IMSL_WRITE_FORMAT, "%12.7f", 0);

        // Вычисление вектора невязки
        for (int j = 0; j < dim; ++j)
        {
            // Разбиение матрицы собственных векторов на отдельные вектора
            for (int k = 0; k < dim; ++k) temp[k] = eigenvectors[k*dim + j];

            imsl_d_mat_mul_rect("A*x", 
                                IMSL_RETURN_USER, a_x,
                                IMSL_A_MATRIX, dim, dim, matrix,
                                IMSL_X_VECTOR, dim, temp,
                                0);
            // Считаем произведение собственного числа на собственный вектор
            for (int l = 0; l < dim; ++l) lambda_x[l] = eigenvalues[j]*temp[l]; 
            for (int m = 0; m < dim; ++m) residual[m*dim + j] = lambda_x[m] - a_x[m];
        }

        imsl_d_write_matrix("Vectors of residuals:", dim, dim, residual, 0);


        // Проверка ортогональности
        std::cout << std::endl << "Check ortogonality" << std::endl << std::endl;
        for (int k = 0; k < dim - 1; ++k)
        {
            for (int p = k + 1; p < dim; ++p)
            {
                double tmp_dot_value = 0;
                for (int coordinate = 0; coordinate < dim; ++coordinate)   tmp_dot_value += residual[k*dim + coordinate]*residual[p*dim + coordinate];
                std::cout << "Scalar dot of " << std::setw(3) << k + 1 << " and " << std::setw(3) << p + 1 << " vectors = " <<  std::setw(15) << tmp_dot_value;
                double check_value = 0.0125;
                if (tmp_dot_value < check_value)
                    std::cout << "\t----->\t Vectors are ortogonal." << std::endl;
                else
                    std::cout << "\t----->\t Vectors are not ortogonal." << std::endl;
            }
        }
            // Подсчет perfomance index
        {
            double max_perfomance_index = 0;
            double min_perfomance_index, tmp_perfomance_index;
            double tmp_vector[dim], tmp_residual[dim];
            for (int vector_No = 0; vector_No < dim; ++vector_No)
            {
                for (int j = 0; j < dim; ++j)   tmp_residual[j] = residual[vector_No*dim + j];
                for (int j = 0; j < dim; ++j)   tmp_vector[j] = eigenvectors[j*dim + vector_No];
                
                tmp_perfomance_index = imsl_d_vector_norm(dim, tmp_residual, IMSL_ONE_NORM, 0)/(10*1*(std::numeric_limits<double>::epsilon()));
                tmp_perfomance_index = tmp_perfomance_index/(imsl_d_matrix_norm(dim, dim, matrix, IMSL_ONE_NORM, 0)*imsl_d_vector_norm(dim, tmp_vector, IMSL_ONE_NORM, 0));

                if (tmp_perfomance_index > max_perfomance_index)
                    max_perfomance_index = tmp_perfomance_index;
                if (i == 0)
                    min_perfomance_index = tmp_perfomance_index;
                if (tmp_perfomance_index < min_perfomance_index)
                    min_perfomance_index = tmp_perfomance_index;
            }
            // Вывод результатов для индекса выполнения
            std::cout << std::endl
                      << "Max perfomance index = " << max_perfomance_index << std::endl
                      << "Для сравнения:" << std::endl
                      << "Min perfomance index = " << min_perfomance_index << std::endl;                    
        }
    }
}