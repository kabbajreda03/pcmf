#include "ConditionalMaxOption.hpp"

using namespace options;

PnlVect* ConditionalMaxOption::get_payoff(const PnlMat * const underlying_paths)
{
    int n_monitoring_dates = get_nb_monitoringDates();
    int n_assets = get_underlying_number();
    const PnlVect* monitoring_dates = get_monitoringDates();
    PnlVect* strikes_vect = get_strikes();
    PnlVect* payoffs = pnl_vect_create_from_zero(n_monitoring_dates);

    bool previous_payoff_zero = true;
    
    for (int m = 0; m < n_monitoring_dates; m++) {
        double max_value = MGET(underlying_paths, m+1, 0);
        for (int n = 1; n < n_assets; n++) {
            max_value = std::max(max_value, MGET(underlying_paths, m+1, n));
        }
            
        double current_strike = GET(strikes_vect, m);

        double current_payoff = std::max(max_value - current_strike, 0.0); 
        if (previous_payoff_zero) {    
            LET(payoffs,m) = current_payoff;
		}
        if(current_payoff==0.0 || !previous_payoff_zero){
            previous_payoff_zero = true;
        }else{
            previous_payoff_zero = false;
        }
    }
    
    return payoffs;
}
