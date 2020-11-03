#include <imsl.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#define PI 3.1415

double a = -PI/4, b = PI/4;

double F(double x){
	return sin(x)*sin(x);
}

double x_i(int i, int N){
	return a + i*(b - a)/(N);
}

int main(int argc, char const *argv[])
{	
	int N = 75;

	double* xdata = new double[N + 1];
	double* ydata = new double[N + 1];

	for (int i = 0; i <= N; ++i)
	{
		xdata[i] = x_i(i, N);
		ydata[i] = F(xdata[i]);
	}

	Imsl_d_ppoly* pp = imsl_d_cub_spline_interp_e_cnd(N + 1, xdata, ydata, 0);
	double* x_value = new double[4*N + 1];
	double* y_value = new double[4*N + 1];

	std::cout << "Definite Integral sin(x)^2 from a = -pi/4 to X" << std::endl << 
					std::setw(12) << "X" << "\t" <<"Y" << std::endl;	
	for (int i = 0; i <= 4*N; ++i)
	{
	 	x_value[i] = x_i(i, 4*N);
	 	y_value[i] = imsl_d_cub_spline_integral(a, x_value[i], pp);
	 	std::cout << std::setw(12) << x_value[i] << "\t" << y_value[i] << std::endl;
	} 

	delete[] xdata;
	delete[] ydata;
 	delete[] x_value;
 	delete[] y_value;
	return 0;
}