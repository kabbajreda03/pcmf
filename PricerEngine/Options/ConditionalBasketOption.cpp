#include "ConditionalBasketOption.hpp"

using namespace options;

PnlVect * ConditionalBasketOption::get_payoff(const PnlMat * const underlying_paths)
{
    int n_monitoring_dates = get_nb_monitoringDates();
    int n_assets = get_underlying_number();
    const PnlVect* monitoring_dates = get_monitoringDates();
    PnlVect* strikes_vect = get_strikes();
    PnlVect* payoffs = pnl_vect_create_from_zero(n_monitoring_dates);

    bool previous_payoffs_zero = true;
    
    for (int m = 0; m < n_monitoring_dates && previous_payoffs_zero; m++) {
        double basket_value = 0.0;
        for (int n = 0; n < n_assets; n++) {
            basket_value += MGET(underlying_paths, m+1, n);
        }
        basket_value /= n_assets;
        double current_strike = GET(strikes_vect, m);
            
        double current_payoff = std::max(basket_value - current_strike, 0.0);
        
        if (current_payoff > 0.0) {
            previous_payoffs_zero = false;
            LET(payoffs,m) = current_payoff;
        }
    }
    
    return payoffs;
}
