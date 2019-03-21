#include "Header.h"
#include "Circuit.h"

// Number of units, only has to be defined once
const int num_units = 10;
int sizeVec = 2 * num_units + 1;

void generateCircuit(vector<int> &vals) {
	// Set the feed.
	vals[0] = rand() % num_units;
	for (int i = 1; i < sizeVec; i++){
		vals[i] = rand() % (num_units + 2);
	}
}


vector<int> geneticAlgo(vector<vector<int>> &circuits, vector<double> &fitVec, int iter, double tol, int max_iterations) {
	int num0, num1, cnt;

	int sizeCirc = circuits[0].size(); // size of the vector of int
	int numCircuits = circuits.size(); // number of circuits

	vector<vector<int>> children(numCircuits, vector<int>(sizeCirc, 0));

	vector<int> circuit0(sizeCirc, 0);
	vector<int> circuit1(sizeCirc, 0);

	int it = 0;
	
	int criterium = 1000;

	double min;
	double max;
	
	int indmax; 
	int cnt_indmax = 0; // keeping track if the best vector is still the first vector of the list 


	vector<int> best_parent(sizeCirc, 0);
	vector<int> best_child(sizeCirc, 0);
	
	while(it < iter && cnt_indmax < criterium) {
		// Keep track of the children.
		cnt = 0;

		// To get the max and min values of fitness
		min = fitVec[0];
		max = fitVec[0];

		// Index of the best vector
		indmax = 0;

		// The best goes on unchanged.
		for (int i = 1; i < numCircuits; i++) {
			if (fitVec[i] > max) {
				max = fitVec[i];
				indmax = i;
			}
			if (fitVec[i] < min) min = fitVec[i];
		}
		cnt++;

		// Check if indmax has not changed (i.e. the first vector is the best and still the same)
		if (indmax == 0) {
			cnt_indmax++;
		}
		else {
			cnt_indmax = 0;
		}

		children[0] = circuits[indmax];
		
		
		/*cout << "best parent: " << endl;
		for (int i = 0; i < sizeVec; i++) {
			cout << circuits[indmax][i] << " ";
		}
		cout << endl;*/


		// Find the range of fitness.
		vector<double> intervals(numCircuits + 1);
		intervals[0] = 0;
		double range = 0;
		for (int i = 0; i < numCircuits; i++) {
			fitVec[i] -= (min - 1e-3);  // We subtract the min minus a little bit.
			range += fitVec[i];
			intervals[i + 1] = range;
		}


		// Iterate until we have all the children.
		while (cnt != numCircuits) {
			// Find the indexes of the parents.
			double num0, num1;
			int ind0 = 0, ind1 = 0;
			while (ind0 == ind1) {
				// Pick a random number.
				num0 = ((double)rand() / (RAND_MAX));
				num0 *= range;
				num1 = ((double)rand() / (RAND_MAX));
				num1 *= range;

				for (int i = 1; i < numCircuits + 1; i++) {
					if (num0 > intervals[i - 1] && num0 <= intervals[i]) ind0 = i - 1;
					if (num1 > intervals[i - 1] && num1 <= intervals[i]) ind1 = i - 1;
				}
			}

			// Do the cross-over.
			num0 = ((double)rand() / (RAND_MAX)); // cross-over probability
			if (num0 > 0.8 && num0 < 1) { // TO TUNE
				// Select a random point in the vector.
				int pivot = rand() % (sizeCirc - 1) + 1;

				// Do the two children.
				for (int i = 0; i < pivot; i++) {
					circuit0[i] = circuits[ind1][i];
					circuit1[i] = circuits[ind0][i];
				}
				for (int i = pivot; i < sizeCirc; i++) {
					circuit0[i] = circuits[ind0][i];
					circuit1[i] = circuits[ind1][i];
				}
			}
			// Case when the cross-over doesn't happen.
			else {
				circuit0 = circuits[ind0];
				circuit1 = circuits[ind1];
			}


			// Do the mutation.
			int randInt;
			int stepSize = 3;
			num0 = ((double)rand() / (RAND_MAX)); // mutation probability

			// Mutate the feed.
			if (num0 >= 0 && num0 < 0.01) { // TO TUNE
				randInt = rand() % stepSize + 1;
				circuit0[0] = (circuit0[0] + randInt) % num_units;
				randInt = rand() % stepSize + 1;
				circuit1[0] = (circuit1[0] + randInt) % num_units;
			}
			// Mutate the rest of the circuit.
			for (int i = 1; i < sizeCirc; i++) {
				num0 = ((double)rand() / (RAND_MAX)); // mutation probability
				
				if (num0 >= 0 && num0 < 0.01) { // TO TUNE
					randInt = rand() % stepSize + 1;
					circuit0[i] = (circuit0[i] + randInt) % (num_units + 2);
					randInt = rand() % stepSize + 1;
					circuit1[i] = (circuit1[i] + randInt) % (num_units + 2);
				}
			}

			// Checking validity of the potential valid child.
			if (Check_Validity(circuit0)) {
				children[cnt] = circuit0;
				cnt++;
			}
			if (cnt < numCircuits && Check_Validity(circuit1)) {
				children[cnt] = circuit1;
				cnt++;
			}
		}  // end while on the children.


		// Updating the parents vector with the children.
		for (int i = 0; i < numCircuits; i++) {
			circuits[i] = children[i];

			/*cout << "first child " << endl;
			for (int j = 0; j < sizeVec; j++) {
				cout << circuits[0][j] << " ";
			}*/
			
			assert(circuits[i] == children[i]);

			// Compute the new fitness
			Circuit circ(100.0, -500.0, 10, 100); // rebuilding the object
			fitVec[i] = circ.Evaluate_Circuit(children[i], tol, max_iterations);
		}

		indmax = 0;
		max = fitVec[0];
		for (int i = 1; i < numCircuits; i++) {
			if (fitVec[i] > max) {
				max = fitVec[i];
				indmax = i;
			}
		}

		best_child = circuits[indmax];

		/*cout << "best child: " << endl;
		for (int i = 0; i < sizeVec; i++) {
			cout << circuits[indmax][i] << " ";
		}

		cout << "fitness: " << fitVec[indmax];
		cout << endl;*/

		it++;
		cout << it << endl;
		cout << endl << endl;
	}  // end of the while loop on the iterations.

	return best_child;
}