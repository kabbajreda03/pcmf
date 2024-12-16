#include "ConditionalMaxOption.hpp"

using namespace options;

double ConditionalMaxOption::get_payoff(const PnlMat * const underlying_paths) const
{
	double total_payoff = 0.0;
    int n_monitoring_dates = parameters.monitoringDates->size;
    int n_assets = underlying_paths->n;
    double dt = GET(parameters.monitoringDates, n_monitoring_dates - 1) / (underlying_paths->m - 1);
    
    bool previous_payoff_zero = true; // Track if previous payoff was 0
    
    for (int m = 0; m < n_monitoring_dates; m++) {
        
        int date_index = (int)(GET(parameters.monitoringDates, m) / dt);
            
        double max_value = MGET(underlying_paths, date_index, 0);
        for (int n = 1; n < n_assets; n++) {
            max_value = std::max(max_value, MGET(underlying_paths, date_index, n));
        }
            
        double current_strike = GET(parameters.strikes, m);
            
        double current_payoff = std::max(max_value - current_strike, 0.0);
        if (previous_payoff_zero) {    
            total_payoff += current_payoff;
		}
		
        previous_payoff_zero = (current_payoff == 0.0);
    }
    
    return total_payoff;
}
