#include <cmath>
#include "MonteCarloRoutine.hpp"


void MonteCarloRoutine::price(double &price, double &confidence_interval) const
{
	double runningSum = 0;
	double runningSquaredSum = 0;
    PnlVect* payoffs;
	double interest_rate = underlying_model.interest_rate();
    const PnlVect* monitoring_dates = option.get_monitoringDates();
    pnl_vect_print(monitoring_dates);
	PnlVect* times_to_monitoring = pnl_vect_create_from_scalar(option.get_nb_monitoringDates(), t);
	pnl_vect_minus_vect(times_to_monitoring, monitoring_dates);

	for (int i = 0; i < option.get_nb_monitoringDates(); ++i) {
    	double x = GET(times_to_monitoring, i);
        LET(times_to_monitoring, i) = std::exp(interest_rate * x);
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
	confidence_interval = sqrt(fabs(variance) / sample_number);
}

void MonteCarloRoutine::delta(PnlVect *deltas, PnlVect *deltas_std, const PnlMat *past, double t){
    
    PnlVect* payoffs;
    models::BlackScholesModel* derived_model = dynamic_cast<models::BlackScholesModel*>(&underlying_model);

    double interest_rate = derived_model->interest_rate();
    const PnlVect* monitoring_dates = option.get_monitoringDates();
	PnlVect* times_to_monitoring = pnl_vect_create_from_scalar(option.get_nb_monitoringDates(), t);
    pnl_vect_minus_vect(times_to_monitoring, monitoring_dates);
    for (int i = 0; i < option.get_nb_monitoringDates(); ++i) {
    	double x = GET(times_to_monitoring, i);
        LET(times_to_monitoring, i) = std::exp(interest_rate * x);
    }
    double fdstep =  derived_model->fdStep();
    PnlVect *sum_squares = pnl_vect_create_from_zero(derived_model->underlying_number());
    PnlMat *shifted = pnl_mat_create(monitoring_dates->size + 1, derived_model->underlying_number());
    for(int i=0; i<sample_number; i++){
		const PnlMat * const generated_path = get_generated_path();        
		for(int d=0;d<derived_model->underlying_number();d++){
            
            derived_model->shift_asset(generated_path, shifted, t, fdstep, d);
            payoffs = option.get_payoff(shifted);
            double diff = pnl_vect_scalar_prod(payoffs, times_to_monitoring);
            derived_model->shift_asset(generated_path,shifted, t, -fdstep, d);
            payoffs = option.get_payoff(shifted);
            diff -= pnl_vect_scalar_prod(payoffs, times_to_monitoring);
            LET(deltas,d) = GET(deltas,d) + diff;
            LET(sum_squares,d) = GET(sum_squares,d) + diff*diff;
            
        }
    }
    for(int d = 0; d<derived_model->underlying_number();d++){
        double S_t = MGET(past, past->m-1, d);
        double mean = GET(deltas,d)/(2.0*fdstep*sample_number);
        double var = fabs(GET(sum_squares,d)/(4.0*fdstep*fdstep*sample_number)-mean*mean);
        LET(deltas,d) = mean / S_t;
        LET(deltas_std,d) = sqrt(var)/(S_t*sqrt(sample_number));
    }
    pnl_mat_free(&shifted);
    pnl_vect_free(&sum_squares);
}