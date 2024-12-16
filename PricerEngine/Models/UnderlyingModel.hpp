#pragma once

#include "pnl/pnl_matrix.h"
#include "RandomGeneration.hpp"

namespace models
{

	class UnderlyingModel
	{
	public:
		UnderlyingModel() {};
		virtual const PnlMat* const simulate_asset_paths_unsafe(const double from_time, const PnlMat * const past_values) const = 0;
		virtual const PnlMat* const simulate_asset_paths_from_start(const PnlVect * const spot) const = 0;
		virtual ~UnderlyingModel() {};
	};

}