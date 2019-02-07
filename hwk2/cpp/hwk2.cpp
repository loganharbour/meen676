#include "TriDiagonal.h"

#include <fstream>
#include <iomanip>
#include <math.h>
#include <string>

/**
 * Saves a vector to a csv.
 */
void saveCsv(const std::vector<double> x, const std::string filename)
{
	std::ofstream f;
	f.open(filename);
	for (unsigned int i = 0; i < x.size(); ++i)
		f << std::setprecision(12) << x[i] << std::endl;
	f.close();
}

/**
 * Solves the 1D heat-conduction (with convection) problem with N nodes.
 */
void solveRod(unsigned int N)
{
	// Initialize geometry and material properties
	double L = 1;             // [m]
	double dx = L / (N - 1);  // [m]
	double d = 0.05;          // [m]
	double k = 400.0;         // [W/m-C]
	double h = 0.5;           // [W/m^2-C]
	double T0 = 100.0;        // [C]
	double Tinf = 25.0;       // [C]

	// Initialize system A theta = b
	TriDiagonal A(N - 1);
	std::vector<double> theta(N - 1);
	std::vector<double> b(N - 1);

	// Constant coefficients
	double m = h / (k * d);
	double a_p = m * dx + 2.0 / dx;
	double a_w = 1.0 / dx;
	double a_e = 1.0 / dx;

	// Fill system
	A.setTopRow(a_p, -a_e);
	b[0] = a_w * (T0 - Tinf);
	for (unsigned int i = 1; i < N - 2; ++i)
		A.setMiddleRow(i, -a_w, a_p, -a_e);
	A.setBottomRow(-a_w, m * dx / 2 + h / k + a_e);

	// Solve system in place and save
	A.solveTDMA(b, theta);
	saveCsv(theta, "../results/theta_" + std::to_string(N) + ".csv");
}

int main()
{
	// Run each requested case
	solveRod(6);
	solveRod(11);
	solveRod(21);
	solveRod(41);
	solveRod(81);

	return 0;
}
