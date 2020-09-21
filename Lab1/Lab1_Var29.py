# -*- coding: utf-8 -*-
import numpy as np
import imsl.linalg as la
from sys import exit
from os.path import isfile

dimension = 11
parameter = np.array([ 1.0000000, 0.0156250, 0.0000000], dtype=float)

matrix_file='/Experiences/Numerical_Methods/Lab1/RGEmtr029.dat'
vector_file='/Experiences/Numerical_Methods/Lab1/RGErhs029.dat'

print_factorization_result = False
print_approximate_data = False

#NumPy data types https://numpy.org/devdocs/user/basics.types.html
def get_value(filename, dimension=dimension, subject='matrix') -> float:
    """ Read matrix or vector from file and return numpy array """
    with open(filename, 'r') as input:
        if (subject == 'matrix') :
            matrix = np.empty([dimension, dimension], dtype=float)
            for line in range(dimension):
                matrix[line] = np.fromfile(input, dtype=float, count=dimension, sep=' ')
            print_array('Matrix A:', matrix, subject='matrix')
            return matrix
        elif (subject == 'vector'):
            vector = np.fromfile(input, count=dimension, dtype=float, sep='\n')
            print_array("Vector B  :", vector, subject='vector')
            return vector
        else:
            exit("ERROR. Function get_value received not provided parameter subject.")

def print_array(title:str, array, subject:str, dimension:int = dimension):
    """ Print Numpy array with title """
    print('\n', title, end='\n')
    if (subject=='matrix'):
    # Если элементы матрицы хранятся по столбцам, может распечатать транспонированную матрицу см. np.shape
    # Массив numpy не итерируемый объект. Его нельзя перебрать используя for и shape
        for row in range(dimension):
            for column in range(dimension):
                print("{:<12.7f}".format(array[row][column]), end=' ')
            print('\n\n')
        print('\n\n')
    elif (subject=='vector'):
        for column in range(dimension):
            print("{:<12.7f}".format(array[column]), end='  ')
        print('\n')
    else:
        exit('ERROR. Печать функцией print_array полученного ей типа данных ещё не реализована.')

def testcase(delta_A, delta_B, solve):
    """ Сопоставление результатов Вычислений """
    print('\n','\t'*4, testcase.__doc__, end='' )
    print_array("delta_A*x", (np.matmul(delta_A, solve)), dimension=11, subject='vector')
    print_array("Solve", solve, dimension=11, subject='vector')

if __name__ == '__main__':
    print('Biga V.S.', '3430302/90003', 'Lab 1', 'Variant 029', sep='\n', end='\n\n')
    print_array('Параметр p пробегает значения:', parameter, dimension = 3, subject='vector')

    if not (  (isfile(matrix_file))  or (isfile(vector_file)) ):
        exit("ERROR. Cannot open input files.")
    # Для вывода длинных объектов
    np.set_printoptions(linewidth=230)
    A = get_value(matrix_file, dimension, subject='matrix')
    B = get_value(vector_file, dimension, subject='vector')

    # Определитель для оценки матрицы А
    determinat_A = np.linalg.det(A)
    print('determinat(А) = ', determinat_A)
    for i in parameter:
        print('----'*33, '\nIf parameter p = ', i, end='\n\n')

        delta_A = np.copy(A)
        delta_A[0, 0] = delta_A[0, 0] + i

        determinat_delta_A = np.linalg.det(delta_A)

        delta_B = np.copy(B)
        delta_B[0] += 4*i

        if print_approximate_data == True:
            print_array('Matrix delta_A:', delta_A, subject='matrix')
            print('Для оценки матрицы delta_A:  det(delta_А) = ', determinat_delta_A)
            print_array('Vector delta_B:', delta_B, subject='vector')

        # create LU class
        # class LU in documentation https://docs.roguewave.com/en/imsl/pynl/current/docs/generated/generated/imsl.linalg.LU.solve.html
        LU_System_Solution = la.LU(delta_A)

        print('Matrix factorization. IMSL can return alert message.', end='\n'*3)
        # LU factorization with partial pivoting https://en.wikipedia.org/wiki/LU_decomposition#LU_factorization_with_partial_pivoting
        # Факторизация с частичным поворотом. PA=LU , где P - матрица перестановок строк.
        P, L, U = LU_System_Solution.factor_full()
        if print_factorization_result == True:
            print_array('Матрица перестановок:', P, subject='matrix')
            print_array('Lower matrix:', L, subject='matrix')
            print_array('Upper matrix:', U, subject='matrix')

        # В формуле для оценки погрешности указано использовать манхэттанскую норму
        numpy_condition_number = np.linalg.cond(delta_A, p=1)
        # IMSL compute only 1 norm
        imsl_condition_number = LU_System_Solution.cond()
        condition_number = imsl_condition_number if (imsl_condition_number > numpy_condition_number) else numpy_condition_number
        print('Число обусловленности матрицы delta_A = ', condition_number)

        solve = LU_System_Solution.solve(delta_B)
        print_array('Solve vector: ', solve, subject='vector')

        r = delta_B - np.matmul(delta_A, solve)
        print_array('Nullity vector: ', r, subject='vector')

        print('Оценка погрешности решения')
        supremum_delta_X = (condition_number*np.linalg.norm(r, ord=1))/np.linalg.norm(delta_B, ord=1)
        print('norm(delta_x)/norm(x) <= ', supremum_delta_X)

        testcase(delta_A, delta_B, solve)
