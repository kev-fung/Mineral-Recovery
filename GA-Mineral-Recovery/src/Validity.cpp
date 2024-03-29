#include "Validity.h"
#include "CUnit.h"
#include <vector>

using namespace std;


Validity::Validity(int nunits, int sVec) : num_units(nunits), sizeVec(sVec)
{
	units = new int[nunits * 3];
	for (int i = 0; i < nunits * 3; i++) units[i] = 0;
}


Validity::~Validity()
{
	delete[] units;
}


void Validity::mark_units(int unit_num) {
	if (units[unit_num*3 + 2] == 1) return;			// This unit has already been seen
	units[unit_num*3 + 2] = 1;						// Mark that we have now seen the unit

	// If conc_num does not point at a circuit outlet recursively call the function
	int counter = 0;
	if (units[unit_num*3 + 0] < num_units) {
		mark_units(units[unit_num*3 + 0]);
	}
	else {
		if (units[unit_num*3 + 0] == num_units) seen_conc = true;
		else seen_tail = true;
	}

	// If tails_num does not point at a circuit outlet recursively call the function
	if (units[unit_num*3 + 1] < num_units && counter <= 50)
		mark_units(units[unit_num*3 + 1]);

	else {
		if (units[unit_num*3 + 1] == num_units + 1) seen_tail = true;
		else seen_conc = true;
	}
}


bool Validity::Check_Validity(vector<int> circuit_vector) {
	/*
	Checks:
	//-----
	1. every unit accessible from the feed
	2. every unit has route forward to both outlets
	3. no self-recycle
	4. destination for both products is not the same

	Returns:
	//-----
	  True if all criteria are met
	  False  otherwise.
	*/

	//units.resize(num_units);

	// Convert vector index to unit number
	for (int i = 0; i < num_units; i++) {
		units[i * 3 + 0] = circuit_vector[i * 2 + 1];
		units[i * 3 + 1] = circuit_vector[i * 2 + 2];
	}

	feed_num = circuit_vector[0];

	// Check if all units can see the feed
	for (int i = 0; i < num_units; i++) {
		units[i * 3 + 2] = 0;
		//units[i].mark = false;
	}

	mark_units(feed_num);

	for (int i = 0; i < num_units; i++) {
		if (units[i*3 + 2] == 0) {
			return false;
		}
	}

	// Check if exits can be seen
	for (int j = 0; j < num_units; j++) {
		for (int i = 0; i < num_units; i++) {
			units[i*3 + 2] = 0;
		}
		seen_conc = false;
		seen_tail = false;

		mark_units(j);

		if (!seen_conc || !seen_tail) {
			return false;
		}
	}

	// Check no message send to itself
	int id = 0;
	for (int i = 1; i < circuit_vector.size() - 1; i += 2) {
		if (circuit_vector[i] == id || circuit_vector[i + 1] == id) {
			return false;
		}
		id += 1;
	}

	// Check no pairs are the same
	for (int i = 1; i < (circuit_vector.size() - 1); i += 2) {
		if (circuit_vector[i] == circuit_vector[i + 1]) {
			return false;
		}
	}

	return true;
}