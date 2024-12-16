#include <cmath>
#include "BlackScholesModel.hpp"
#include <iostream>

using namespace input_parsers;
using namespace generators;
using namespace models;


BlackScholesModel::BlackScholesModel(BlackScholesModelParameters& params, const RandomGeneration & rng) : 
random_generator_(rng),
parameters(params)
{
    generated_asset_paths_ = pnl_mat_create(nb_monitoring_dates()+1, underlying_number());
    gaussian_vector_for_simulation_ = pnl_vect_create(underlying_number());
	bs_helper = BlackScholesHelper();
}

const PnlMat* const BlackScholesModel::simulate_asset_paths_from_start(const PnlVect * const spot)
{
	pnl_mat_set_row(generated_asset_paths_, spot, 0);
	fill_remainder_of_generated_asset_paths(1);
	return generated_asset_paths_;
}


void BlackScholesModel::fill_remainder_of_generated_asset_paths(int from_line)
{
	int last_line = generated_asset_paths_->m;
	int underlying_number_ = underlying_number();
	for (int line = from_line; line < last_line; line++)
	{
		random_generator_.get_one_gaussian_sample(gaussian_vector_for_simulation_);
		for (int i = 0; i < underlying_number_; i++)
		{
			double current_spot = MGET(generated_asset_paths_, line - 1, i);
			double new_underlying_value = bs_helper.compute_one_asset_simulation_with_precomputed_parameters(i, current_spot, gaussian_vector_for_simulation_);
			MLET(generated_asset_paths_, line, i) = new_underlying_value;
		}
	}
}
