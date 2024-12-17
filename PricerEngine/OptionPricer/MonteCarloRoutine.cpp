#include <cmath>
#include "MonteCarloRoutine.hpp"


void MonteCarloRoutine::price(double &price, double &confidence_interval) const
{
	double runningSum = 0;
	double runningSquaredSum = 0;
    PnlVect* payoffs;
	double interest_rate = underlying_model.interest_rate();
    const PnlVect* monitoring_dates = option.get_monitoringDates();
	PnlVect* times_to_monitoring = pnl_vect_create_from_scalar(option.get_nb_monitoringDates(), t);
	pnl_vect_minus_vect(times_to_monitoring, monitoring_dates);

	for (int i = 0; i < option.get_nb_monitoringDates(); ++i) {
    	double x = GET(times_to_monitoring, i);
        LET(times_to_monitoring, i) = std::exp(-interest_rate * x);
    }
	for (unsigned long i = 0; i < sample_number; i++)
	{
		const PnlMat * const generated_path = get_generated_path();

		payoffs = option.get_payoff(generated_path);
        double discounted_payoff = pnl_vect_scalar_prod(payoffs, times_to_monitoring);
		runningSum += discounted_payoff;
		runningSquaredSum += discounted_payoff * discounted_payoff;
	}

	price = runningSum / sample_number;
	double variance = runningSquaredSum / sample_number - price * price;
	confidence_interval = 1.96 * sqrt(variance / sample_number);
}