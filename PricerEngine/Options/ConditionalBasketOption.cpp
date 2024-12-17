#include "ConditionalBasketOption.hpp"

using namespace options;

PnlVect * ConditionalBasketOption::get_payoff(const PnlMat * const underlying_paths) const
{
    int n_monitoring_dates = parameters.monitoringDates->size;
    int n_assets = underlying_paths->n;
    double dt = GET(parameters.monitoringDates, n_monitoring_dates - 1) / (underlying_paths->m);
    PnlVect* payoffs = pnl_vect_create_from_zero(n_monitoring_dates);

    bool previous_payoffs_zero = true;
    
    for (int m = 0; m < n_monitoring_dates && previous_payoffs_zero; m++) {
        int date_index = (int)(GET(parameters.monitoringDates, m) / dt);
            
        double basket_value = 0.0;
        for (int n = 0; n < n_assets; n++) {
            basket_value += MGET(underlying_paths, date_index, n);
        }
        basket_value /= n_assets;
            
        double current_strike = GET(parameters.strikes, m);
            
        double current_payoff = std::max(basket_value - current_strike, 0.0);
            
        if (current_payoff > 0.0) {
            previous_payoffs_zero = false;
            LET(payoffs,m) = current_payoff;
        }
    }
    
    return payoffs;
}
