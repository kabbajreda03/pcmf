#pragma once
#include <vector>
#include "pnl/pnl_vector.h"

namespace models
{

	class BlackScholesHelper
	{
	public:
		BlackScholesHelper() {};

        
		double compute_one_asset_simulation(const int underlying_position, const double spot, const double interest_rate, const double sqrt_timelength, const PnlVect * const gaussian_vector) const;
		double compute_one_asset_simulation_with_precomputed_parameters(const int underlying_position, const double spot, const PnlVect * const gaussian_vector) const;
    }

}