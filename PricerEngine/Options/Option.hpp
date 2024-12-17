#pragma once

#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vect.h"
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

    virtual PnlVect* get_payoff(const PnlMat * const underlying_paths) const = 0;

};
}