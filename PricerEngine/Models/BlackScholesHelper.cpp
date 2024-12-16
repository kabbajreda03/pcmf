#include <cmath>
#include "pnl/pnl_matrix.h"
#include "BlackScholesHelper.hpp"

using namespace models;
using namespace std;


double BlackScholesHelper::compute_one_asset_simulation(const int underlying_position, const double spot, const double interest_rate, const double sqrt_timelength, const PnlVect * const gaussian_vector) const
{
	BlackScholesHelper::UnderlyingParameter tmp_underlying_parameter(computed_underlying_parameters_[underlying_position]);
	double sigma_i = tmp_underlying_parameter.sigma_;
	tmp_underlying_parameter.spot_coefficient_ = (interest_rate - 0.5 * sigma_i * sigma_i) * sqrt_timelength * sqrt_timelength;
	tmp_underlying_parameter.sigma_sqrt_timestep_ = sigma_i * sqrt_timelength;

	return asset_simulation_helper(tmp_underlying_parameter, spot, gaussian_vector);
}


double BlackScholesHelper::compute_one_asset_simulation(const int underlying_position, const double spot, const double interest_rate, const double sqrt_timelength, const PnlVect * const gaussian_vector) const
{   
	double moved_spot = spot * exp(underlying_parameter.spot_coefficient_);
	const PnlVect * const cholesky_line = underlying_parameter.cholesky_line_;
	double time_factor = underlying_parameter.sigma_sqrt_timestep_;
	double scalar_product = pnl_vect_scalar_prod(cholesky_line, gaussian_vector);
	double gaussian_factor = exp(time_factor * scalar_product);

	return moved_spot * gaussian_factor;
}
