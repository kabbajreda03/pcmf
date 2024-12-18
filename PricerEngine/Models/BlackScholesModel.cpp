#include <cmath>
#include "BlackScholesModel.hpp"
#include <iostream>

#define TIME_PRECISION 0.00001

using namespace input_parsers;
using namespace generators;
using namespace models;


BlackScholesModel::BlackScholesModel(BlackScholesModelParameters& params, const RandomGeneration & rng) : 
random_generator_(rng),
parameters(params)
{	
	int monitoring_times = nb_monitoring_dates();
	final_simulation_date_ = GET(monitoring_dates(),monitoring_times-1);
	timestep_ = static_cast<double> (final_simulation_date_)/ monitoring_times;
	interest_rate_ = interest_rate();
    generated_asset_paths_ = pnl_mat_create(nb_monitoring_dates()+1, underlying_number());
	std::cout<<"Size of asset paths :"<<generated_asset_paths_->m << " " << generated_asset_paths_->n<<"\n";
    gaussian_vector_for_simulation_ = pnl_vect_create(underlying_number());
	bs_helper = BlackScholesHelper(underlying_number(), interest_rate_, timestep_, cholesky_lines());
}


const PnlMat* const BlackScholesModel::simulate_asset_paths_unsafe(const double from_time, const PnlMat * const past_values)
{
	pnl_mat_set_subblock(generated_asset_paths_, past_values, 0, 0);
	int number_of_values = past_values->m;
	double timespan_to_monitoring = (number_of_values - 1) * timestep_ - from_time;
	bool is_at_monitoring_time = timespan_to_monitoring < TIME_PRECISION;	
	if (!is_at_monitoring_time)
	{
		add_one_simulation_to_generated_asset_paths_unsafe(number_of_values-1, timespan_to_monitoring, past_values);
	}
	fill_remainder_of_generated_asset_paths(number_of_values);
	return generated_asset_paths_;
}


const PnlMat* const BlackScholesModel::simulate_asset_paths_from_start(const PnlVect * const spot)
{
	pnl_mat_set_row(generated_asset_paths_, spot, 0);

	fill_remainder_of_generated_asset_paths(1);

	return generated_asset_paths_;
}


void BlackScholesModel::add_one_simulation_to_generated_asset_paths_unsafe(int at_line, double timelength, const PnlMat * const past_values)
{
	random_generator_.get_one_gaussian_sample(gaussian_vector_for_simulation_);
	double sqrt_timelength = sqrt(timelength);
	for (int i = 0; i < underlying_number(); i++)
	{
		double current_spot = MGET(past_values, at_line, i);
		double new_underlying_value = bs_helper.compute_one_asset_simulation(i, current_spot, interest_rate(), sqrt_timelength, gaussian_vector_for_simulation_);					
		MLET(generated_asset_paths_, at_line, i) = new_underlying_value;
	}
}


void BlackScholesModel::fill_remainder_of_generated_asset_paths(int from_line)
{
	int last_line = generated_asset_paths_->m;
	double timestep;
	for (int line = from_line; line < last_line; line++)
	{
		random_generator_.get_one_gaussian_sample(gaussian_vector_for_simulation_);
		for (int i = 0; i < underlying_number(); i++)
		{
			if(line==1)
			 	timestep = GET(monitoring_dates(),line-1);
			else
				timestep = GET(monitoring_dates(),line-1) - GET(monitoring_dates(),line-2);
			double sqrt_timelength = sqrt(timestep);
			double current_spot = MGET(generated_asset_paths_, line - 1, i);
			double new_underlying_value = bs_helper.compute_one_asset_simulation(i, current_spot, interest_rate(), sqrt_timelength, gaussian_vector_for_simulation_);
			MLET(generated_asset_paths_, line, i) = new_underlying_value;
		}
	}
}

void BlackScholesModel::shift_asset(PnlMat *path, PnlMat *shifted, double t,double h, int d)
{   
    pnl_mat_clone(shifted,path);
    int idx = (int) (t/timestep_);
    for(int i=idx; i<shifted->m; i++){
        MLET(shifted, i, d) = MGET(path, i, d)*(1+h);
    }
}

BlackScholesModel::~BlackScholesModel()
{
	pnl_mat_free(&generated_asset_paths_);
	pnl_vect_free(&gaussian_vector_for_simulation_);
	generated_asset_paths_ = nullptr;
	gaussian_vector_for_simulation_ = nullptr;
}
