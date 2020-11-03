#  Т.к. объекты sympy добавлены в пространство имен, то pandas dataframe автомоатически использует символьное представление
#
from pyimsl.math.cubSplineInterpECnd import cubSplineInterpECnd
from pyimsl.math.cubSplineValue import cubSplineValue
from pyimsl.math.cubSplineIntegral import cubSplineIntegral
import matplotlib.pyplot as plt
from math import sin, cos, pi
#from sympy import *
#from sympy.core.function import diff
#from sympy.core.symbol import Symbol
#from sympy.core.symbol import 
#from sympy import symbol, diff, integrate, init_printing
import pandas as pd
# Define parameters
print("Lab 7 Var 01", "3430302/90003", "Biga V.S", "F(x) = sin^2(x)", "[a;b] = [-pi/4; pi/4]", sep='\n', end='\n\n')
a = -pi/4
b = pi/4
F = lambda x : (sin(x))**2
number_partions = [10, 20, 40, 80, 160]
x_value = lambda i: a + i*((b - a)/(4*N))
x_spline = lambda i: a + i*((b - a)/(N))

#init_printing (use_unicode = True)
# x = Symbol('x')
# print("Integral F(x) = ", integrate(F(x), x))
# print("First diff F(x) = ", diff(sin(x)**2, x))
# print("Second diff F(x) = ", diff(sin(x)**2, x, 2))
# print("Third diff F(x) = ", diff(sin(x)**2, x, 3))

integral_func = lambda x: (x/2 - sin(x)*cos(x)/2) - (a/2 - sin(a)*cos(a)/2)
first_diff = lambda x : 2*sin(x)*cos(x)
second_diff = lambda x : 2*(-sin(x)**2 + cos(x)**2)
third_diff = lambda x: -8*sin(x)*cos(x)

# Сплайн создается для N разбиений, аппроксимируется на 4 N
#for N in number_partions:
for N in number_partions[0:1]:
    # Create Spline
    xdata = [x_spline(i) for i in range(N + 1)]
    ydata = list(map(lambda i: F(i), xdata))
    approx_spline = cubSplineInterpECnd(xdata, ydata)
    # Подготовка к выводу
    x = [x_value(i) for i in range(4*N + 1)]
    y = list(map(lambda x: F(x), x))
    spline_value = list(map(lambda x: cubSplineValue(x, approx_spline), x))
    error = [abs(y[i] - spline_value[i]) for i in range(4*N + 1)]


    integral_value_data = [integral_func(i) for i in range(4*N + 1)]
    integral_interpolant = list(map(lambda x_i: cubSplineIntegral(a, x_i, approx_spline), x))
    error_integral = [integral_value_data[i] - integral_interpolant[i]  for i in range(4*N + 1)]

    first_diff_data = list(map(lambda x: first_diff(x), x))
    diff_1 = list(map(lambda x_i: cubSplineValue(x_i, approx_spline, 1), x))
    error_diff_1 = [abs(diff_1[i] - first_diff_data[i])  for i in range(4*N + 1)]

    second_diff_data = list(map(lambda x: second_diff(x), x))
    diff_2 = list(map(lambda x_i: cubSplineValue(x_i, approx_spline, 2), x))
    error_diff_2 = [abs(diff_2[i] - second_diff_data[i])  for i in range(4*N + 1)]

    third_diff_data = list(map(lambda x: third_diff(x), x))
    diff_3 = list(map(lambda x_i: cubSplineValue(x_i, approx_spline, 3), x))
    error_diff_3 = [abs(diff_3[i] - third_diff_data[i])  for i in range(4*N + 1)]
    
    # все массивы соразмерные, поэтому уместно использовать zip()
    dataframe = pd.DataFrame(list(zip(x, y, spline_value, error, 
                                      integral_value_data, integral_interpolant, error_integral,
                                      first_diff_data, diff_1, error_diff_1,
                                      second_diff_data, diff_2, error_diff_2,
                                      third_diff_data, diff_3, error_diff_3)),
                                      
                                      columns = ['x', 'f(x)','Interpolant', 'error',
                                                 'Integral', 'Interpolant Integral', 'error',
                                                 'Diff 1', 'Interpolant Diff 1', 'error',
                                                 'Diff 2', 'Interpolant Diff 2', 'error',
                                                 'Diff 3', 'Interpolant Diff 3', 'error',]
                                      )
    # чтобы печатать всю таблицу
    pd.set_option('display.max_rows', None)
    pd.set_option('display.max_columns', None)
    pd.set_option('display.width', None)
    pd.set_option('display.max_colwidth', -1)


    dataframe
    #print(dataframe)
    # Plot
    fig, ax = plt.subplots()  
    ax.plot(x, spline_value, label='Interpolant')  
    ax.plot(x, y, label='Function sin^2(x)')  
    ax.set_xlabel('x')  
    ax.set_ylabel('y')  
    ax.set_title("Plot N = " + str(4*N))  
    ax.legend()  
    plt.show()
    # поправил, чтобы не срабатывал
    if (N > number_partions[-1]):
        print("Press any key to continue")
        input()
    print("-------"*14)
    
