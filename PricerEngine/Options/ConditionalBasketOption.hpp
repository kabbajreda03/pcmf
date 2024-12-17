#pragma once
#include <utility>
#include <iostream>
#include "pnl/pnl_matrix.h"
#include "pnl/pnl_vector.h"
#include "OptionParameters.hpp"
#include "Option.hpp"

namespace options {
	class ConditionalBasketOption : public Option
	{

	public:
		ConditionalBasketOption(OptionParameters &params) :
			Option(params)
		{};

		PnlVect* get_payoff(const PnlMat * const underlying_paths) const;
	};

}