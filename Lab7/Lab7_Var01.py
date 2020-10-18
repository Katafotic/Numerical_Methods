from pyimsl.math.cubSplineInterpECnd import cubSplineInterpECnd
from pyimsl.math.cubSplineValue import cubSplineValue
import matplotlib.pyplot as plt
from math import sin, pi
# Define parameters
print("Lab 7 Var 01", "3430302/90003", "Biga V.S", "F(x) = sin^2(x)", "[a;b] = [-pi/4; pi/4]", sep='\n', end='\n\n')
a = -pi/4
b = pi/4
F = lambda x : (sin(x))**2
number_partions = [10, 20, 40, 80, 160]
x_value = lambda i: a + i*((b - a)/(4*N))
x_spline = lambda i: a + i*((b - a)/(N))
# Сплайн создается для N разбиений, аппроксимируется на 4 N
for N in number_partions:
    # Create Spline
    xdata = [x_spline(i) for i in range(N + 1)]
    ydata = list(map(lambda i: F(i), xdata))
    approx_spline = cubSplineInterpECnd(xdata, ydata)
    # Подготовка к выводу
    x = [x_value(i) for i in range(4*N + 1)]
    y = list(map(lambda x: F(x), x))
    spline_value = list(map(lambda x: cubSplineValue(x, approx_spline), x))
    epslilon = [abs(y[i] - spline_value[i]) for i in range(4*N + 1)]
    # Показ данных
    print("Spline create on  N = ", N, " dots.   max error = ", max(epslilon), end='\n\t')
    print("Difference spline and F(x) on ", 4*N, " dots")
    print("      x                y            Interpolant        error")
    for i in range(4*N + 1):
        print('%13.10f    %13.10f    %13.10f    %13.14f' % (x[i], y[i], spline_value[i], epslilon[i]))
    
    print('Estimator for error: ', ((b-a)/(4*N))**4)
    # Plot
    fig, ax = plt.subplots()  
    ax.plot(x, spline_value, label='Interpolant')  
    ax.plot(x, y, label='Function sin^2(x)')  
    ax.set_xlabel('x')  
    ax.set_ylabel('y')  
    ax.set_title("Plot N = " + str(4*N))  
    ax.legend()  
    plt.show()
    print("-------"*14)
