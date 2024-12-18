#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "OptionParameters.hpp"

namespace options {

class Option
{
    
  private:
    OptionParameters& parameters;

  public: 

    Option(OptionParameters &params) :
      parameters{params}
    {};

    int get_underlying_number() { return parameters.underlying_number;};
    double get_maturity() { return parameters.maturity;};
    PnlVect* get_strikes() { return parameters.strikes;};
    const PnlVect* get_monitoringDates() { return parameters.monitoringDates;};
    int get_nb_monitoringDates() { return parameters.monitoringDates->size;};

    virtual PnlVect* get_payoff(const PnlMat * const underlying_paths) = 0;

};
}