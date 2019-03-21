#include "Header.h"
#include "Circuit.h"

using namespace std;


int main() {
	int numCircuits = 100; // TO TUNE
	int maxIte = 10000;

	// parameters for the Evaluate_Circuit function
	double tol = 1e-6;
	int max_iterations = 2000;

	// random seed
	srand(time(NULL));

	// Circuit and list of circuits
	vector<int> circuit(sizeVec, 0);
	vector<vector<int>> circuits(numCircuits, vector<int>(sizeVec, 0));

	// List of fitness values
	vector<double> fitVec(numCircuits, 0);

	
	// Generating the parents
	cout << "Generating parents" << endl;
	for (int i = 0; i < numCircuits; i++) {
		while(!Check_Validity(circuit)){  
			generateCircuit(circuit);
		}
		Circuit circ(100.0, -500.0, 10, 100);

		circuits[i] = circuit;
		fitVec[i] = circ.Evaluate_Circuit(circuit, tol, max_iterations);
	
		// Reinitialise the vector with 0
		for (int j = 0; j < sizeVec; j++) {
			circuit[j] = 0;
		}
	}
	cout << endl;


	// Check the vectors.
	cout << "Input Vectors" << endl;
	for (int i = 0; i < numCircuits; i++) {
		for (int j = 0; j < sizeVec; j++) {
			cout << circuits[i][j] << " ";
		}
		cout << " Fitness: " << fitVec[i] << endl;
	}
	cout << endl;


	vector<int> best_circuit;
	// Genetic algorithm
	cout << "Genetic algorithm" << endl;
	best_circuit = geneticAlgo(circuits, fitVec, maxIte, tol, max_iterations);
	//cout << endl;

	// Print the output.
	cout << "Output best vector" << endl;
	for (int j = 0; j < sizeVec; j++) {
		cout << best_circuit[j] << " ";
	}

	Circuit circ(100.0, -500.0, 10, 100);
	double best_fitness;
	best_fitness = circ.Evaluate_Circuit(best_circuit, tol, max_iterations);
	cout << "Fitness: " << best_fitness;
	cout << endl;
	

	system("pause");
}