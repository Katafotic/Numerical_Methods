#include "imsl.h"
#include <iostream>
#include <fstream> 
#include <stdlib.h>

const char path_to_matrix[] = "/Experiences/Numerical_Methods/Lab1/RGEmtr029.dat";
const char path_to_vector[] = "/Experiences/Numerical_Methods/Lab1/RGErhs029.dat";

const int n = 11;

const bool print_approximate_data = true;
const bool print_factorization_result = false;

const float parameter[3] = { 1.0000000,  0.0156250,  0.0000000};

class FloatLineralSystem
{
	//	Решение системы A*x = b с оценкой числа обусловленности и относительной погрешности решения системы.
	//	Факторизация матрицы A.
	//	Вывод левой части системы при найденном решении.
	//	Вывод вектора невязки. 
		 
	private:
		// nullptr use as flag for delete arrays in destructor
		int dim;
		float* delta_A;
		float* delta_b;
		float* solution;// = nullptr; 
		float* factor = nullptr;
		int* pvt = nullptr;
		float* nullity_vector = nullptr;
		// IMSL need: pointer cond != NULL
		float* cond = delta_A;
		float* approx_left_side =  nullptr;
	public:

		void print_square_matrix(char* title, float* matrix, int dimension)
		{
			imsl_f_write_matrix(title, dimension, dimension, matrix, 0);
		}	

		void generate_solution(bool need_factorization){
			if(need_factorization) {
				printf("%s\n", "Start generate_solution(need_factorization=true");
				factor = new float[121];				
				pvt = new int[11];
				solution = new float[11];
				solution = imsl_f_lin_sol_gen(dim, delta_A, delta_b, IMSL_CONDITION, cond, IMSL_FACTOR_USER, factor, pvt, 0);
			} 
			else { 
				std::cout << "start generate solution\n";
				solution = imsl_f_lin_sol_gen(dim, delta_A, delta_b, IMSL_CONDITION, cond, 0);
				imsl_f_write_matrix("\nSolution vector", 1, dim, solution, 0);
			}
			std::cout << std::endl;
		}

		void calculate_nullity_vector(){
			
			nullity_vector = new float[dim];
			for (int i=0; i < dim; i++)
				nullity_vector[i] = delta_b[i];
			
			approx_left_side = new float[dim];
			compute_approx_left_side();
			
			for (int i=0; i < dim; i++)
				nullity_vector[i] -= approx_left_side[i];
		}

		void compute_approx_left_side(){
			// умножение матрицы delta_A на вектор solution
			if (approx_left_side == nullptr)
				approx_left_side = new float[dim];

			if (solution == nullptr) {
				std::cout << "The system has not yet been resolved. Start FloatLineralSystem.generate_solution() for search system solution" << std::endl << std::endl << "Solution:" << std::endl;
				generate_solution(false);
			}
		//		for (int i = 0; i < dim; i++){
		//			int sum = 0;
		//			for (int j = 0; j < dim; j++)
		//				sum += delta_A[dim*i+j]*solution[j];
		//			approx_left_side[i] = sum;	
		//		std::cout << "compute scalar product\n";
			for(int i = 0; i < dim; i++) 
				approx_left_side[i] = scalar_multiplication((delta_A + dim*i), delta_b, dim);
		}


		float scalar_multiplication(float* row_matrix, float* vector_column, int number_matrix_rows){
			float cell = 0;
			for(int i = 0; i < number_matrix_rows; ++i) 
			 	cell += (*(row_matrix+i))*(*(vector_column+i));
			return cell;
		}

		void result_deviation(){
		//		Относительная погрешность решения = norm(X - delta_X)/norm(X) <= cond(A)*norm(r)/norm(b)
		//		Указано использовать манхэттенскую норму.
			if(solution == nullptr) {
				std::cout << "\nThe system has not yet been resolved. Start FloatLineralSystem.generate_solution() for search system solution" << std::endl << std::endl << "Solution:" << std::endl;
				generate_solution(false);
			}

			if(nullity_vector == nullptr) {
				std::cout << "\nThe residual vector was not calculated. Start FloatLineralSystem.calculate_nullity_vector() for calculate it." << std::endl;
				calculate_nullity_vector();
			}
			
			std::cout << "Cond(A) = " << *cond << std::endl << "1/cond(A) = " << ((float) 1)/(*cond) << std::endl; 
			float deviation = (*cond)*imsl_f_vector_norm(dim, nullity_vector, IMSL_ONE_NORM, 0)/imsl_f_vector_norm(dim, delta_b, IMSL_ONE_NORM, 0);
			std::cout << "Относительная погрешность решения = norm(X - delta_X)/norm(X) <= cond(A)*norm(r)/norm(b) = " << deviation << std::endl << std::endl;
			imsl_f_write_matrix("Вектор невязки:", 1, dim, nullity_vector, 0);
		}

		FloatLineralSystem(float* matrix_A, float* vector_b, int dimension, float parameter, bool print_approximate_data)
		{
			std::cout << "Start construction class with parameter = " << parameter << std::endl;	
			
			dim = dimension;
			delta_A = new float[dimension*dimension];
			delta_b = new float[dimension]; 
			
			for (int i=0; i < dimension*dimension; i++)
				delta_A[i] = *(matrix_A + i);
			delta_A[0] += 1*parameter;
			
			for (int i=0; i< dimension; i++)
				delta_b[i] = *(vector_b + i);
			delta_b[0] += 4*parameter;

			if (print_approximate_data)
			{
				imsl_f_write_matrix("\nMatrix delta_A", dim, dim, delta_A, IMSL_WRITE_FORMAT, "% 13.7f", 0);
				imsl_f_write_matrix("\nVector delta_b", 1, dim, delta_b, IMSL_WRITE_FORMAT, "% 13.7f", 0);
			}
		}

		~FloatLineralSystem()
		{
			std::cout << "start FloatLineralSystem destructor\n";
			delete[] delta_A;
			delete[] delta_b;
			//delete[] solution;
			imsl_free(solution);

			if (approx_left_side != nullptr)
				delete[] approx_left_side;
			if (nullity_vector != nullptr)
				delete[] nullity_vector;
			
			if (factor != nullptr){
				delete[] factor;
				delete[] pvt;
			

			}
		}
};

// -------------------------------------------------------------------------------------------------------------------------------


int main()
{	printf ("\033c");
	std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
	std::cout << "Let's start running the program." << std::endl;
	std::cout << "Lab 1 Var 29" << std::endl << "3430302/90003" << std::endl << "Biga V.S." << std::endl;
	
	std::ifstream input_matrix(path_to_matrix);
	std::ifstream input_vector(path_to_vector);
	if (!input_matrix.is_open() or !input_vector.is_open())
		std::cout << "ERROR. Cannot open first file";
	
	float* mtr = new float[n*n];
	float* rhs = new float[n]; 

	for (int i=0; i<n*n; i++)
		input_matrix >> mtr[i];

	imsl_f_write_matrix("Matrix A", n, n, mtr, 0);
	if (!input_vector.is_open())
		std::cout << "ERROR. Cannot open second file";

	for (int i=0; i<n; i++)
		input_vector >> rhs[i];

	imsl_f_write_matrix("Vector b", 1, n, rhs, 0);

	input_matrix.close();
	input_vector.close();
		
	std::cout << "\n\n\nTest....\n";

	for(int i = 0; i < 3; ++i) {
		std::cout << "=========================================================================================================================================================" << std::endl;
		std::cout << "If parameter p = " << parameter[i] << std::endl;
		std::cout << "FloatSystem" << std::endl;
		
		FloatLineralSystem fl_sys1(mtr, rhs, n, parameter[i], print_approximate_data) ;
		fl_sys1.generate_solution(print_factorization_result);
		fl_sys1.compute_approx_left_side();
		fl_sys1.result_deviation();
	}

	delete[] mtr;
	delete[] rhs;
	return 0;
}
