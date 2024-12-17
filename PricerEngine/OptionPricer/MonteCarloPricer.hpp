#pragma once
#include "pnl/pnl_vector.h"
#include "RandomGeneration.hpp"
#include "UnderlyingModel.hpp"
#include "Option.hpp"

class MonteCarloPricer
{
private:
	const unsigned long sample_number;

public:
	MonteCarloPricer(const unsigned long sample_nb) : sample_number(sample_nb) {};
	void price(models::UnderlyingModel & underlying_model, options::Option &option, PnlVect * spots, double &price, double &confidence_interval) const;
	void price_at(const double time, models::UnderlyingModel & underlying_model, options::Option &option, PnlMat * past, double &price, double &confidence_interval) const;
};