#pragma once

#include "pnl/pnl_matrix.h"
#include "RandomGeneration.hpp"

namespace models
{

	class UnderlyingModel
	{
	protected:
		int interest_rate_;
	public:
		UnderlyingModel() {};
		double interest_rate() { return interest_rate_; }
		virtual const PnlMat* const simulate_asset_paths_unsafe(const double from_time, const PnlMat * const past_values) = 0;
		virtual const PnlMat* const simulate_asset_paths_from_start(const PnlVect * const spot) = 0;
		virtual ~UnderlyingModel() {};
	};

}