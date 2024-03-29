#include "CStream.h"
#include "CUnit.h"
#include <cmath>


CUnit::CUnit()
{}


CUnit::CUnit(int id, int conc_num, int tail_num, vector<double> fraction, vector<double> circuit_feed) :id(id), conc_id(conc_num), tail_id(tail_num) 
{
	num_components = fraction.size();
	this->conc_frac.resize(num_components);
	vector<double> temp(num_components);

	for (int i = 0; i < num_components; i++)
	{
		this->conc_frac[i] = fraction[i];
		temp[i] = 0;
	}

	feed = CStream(temp);
	feed_old = CStream(temp);
	tail = CStream(temp);
	conc = CStream(temp);
}


CUnit::~CUnit()
{}


void CUnit::output_con_tail() {
	this->conc = (this->feed * this->conc_frac);
	this->tail = (this->feed - this->conc);
}


void CUnit::send_streams(CUnit & unit_conc, CUnit & unit_tail) {
	unit_conc.feed += this->conc;
	unit_tail.feed += this->tail;
}


void CUnit::store_feed() {
	this->feed_old = this->feed;
}


double CUnit::rel_tol_calc() {
	vector<double> rel_tol(num_components);

	double tolerance = -1e9;

	for (int i = 0; i < num_components; i++) {
		rel_tol[i] = abs(this->feed.M[i] - this->feed_old.M[i]) / this->feed_old.M[i];
		tolerance = (rel_tol[i] > tolerance ? rel_tol[i] : tolerance);
	}

	return tolerance;
}

class CUnit;