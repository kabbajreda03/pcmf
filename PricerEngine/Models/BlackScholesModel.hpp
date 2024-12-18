#pragma once

#include <vector>
#include "UnderlyingModel.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "BlackScholesModelParameters.hpp"
#include "BlackScholesHelper.hpp"

namespace models
{

	class BlackScholesModel : public UnderlyingModel
	{

	public:
        BlackScholesModel(BlackScholesModelParameters& params, const generators::RandomGeneration & rng);
        int underlying_number() {return parameters.underlying_number;};
        PnlMat* cholesky_lines() {return parameters.cholesky_lines;};
        double interest_rate() {return parameters.interest_rate;};
        PnlVect* monitoring_dates() {return parameters.monitoringDates;};
        int nb_monitoring_dates() {return parameters.nb_monitoring_dates;};
        int sample_number() {return parameters.sample_number;};
        double fdStep() {return parameters.fdStep;};
		PnlVect* get_spots(){ return parameters.spots;};
		double get_maturity(){ return final_simulation_date_;}

		const PnlMat* const simulate_asset_paths_unsafe(const double from_time, const PnlMat * const past_values);
		const PnlMat* const simulate_asset_paths_from_start(const PnlVect * const spot);

		void shift_asset(const PnlMat *path, PnlMat *shifted, double t,double h, int d);

        ~BlackScholesModel();
	private:

        BlackScholesModelParameters& parameters;
		const generators::RandomGeneration &random_generator_;

		double final_simulation_date_;
		double timestep_;
		PnlMat * generated_asset_paths_;
		PnlVect * gaussian_vector_for_simulation_;

        BlackScholesHelper bs_helper;

		void add_one_simulation_to_generated_asset_paths_unsafe(int at_line, double timelength, const PnlMat * const past_values);
		void fill_remainder_of_generated_asset_paths(int from_line);
	};

}