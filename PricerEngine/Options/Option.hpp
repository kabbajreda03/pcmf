#pragma once

#include "pnl/pnl_matrix.h"
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

    virtual double payoff(const PnlMat * const underlying_paths) const = 0;
		PnlVect* strikes() const { return parameters.strikes; }
		int underlying_number() const { return parameters.underlying_number; }
    PnlVect* monitoring_dates() const { return parameters.monitoringDates; }
    PnlVect* volatility() const { return parameters.volatility; }
};
}