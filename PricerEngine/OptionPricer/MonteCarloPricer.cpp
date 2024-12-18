#include <cmath>
#include "MonteCarloPricer.hpp"
#include "MonteCarloRoutine.hpp"
#include "pnl/pnl_matrix.h"

using namespace options;
using namespace models;
using namespace pricer;


void MonteCarloPricer::price(UnderlyingModel & underlying_model, Option &option, PnlVect * spots, double &price, double &confidence_interval) const
{
	MonteCarloRoutineAtOrigin mco(underlying_model, option, sample_number, spots);
	mco.price(price, confidence_interval);
}

void MonteCarloPricer::price_at(const double time, models::UnderlyingModel & underlying_model, options::Option &option, const PnlMat * past, double &price, double &confidence_interval) const
{
	MonteCarloRoutineAtTimeT mct(underlying_model, option, sample_number, past, time);
	mct.price(price, confidence_interval);
}