#pragma once
#include "pnl/pnl_vector.h"

namespace input_parsers
{
	class BlackScholesModelInputParser
	{
	public:
		virtual int get_underlying_number() const = 0;
		virtual PnlMat* get_cholesky_lines() const = 0;
		virtual double get_interest_rate() const = 0;
		virtual PnlVect * get_monitoringDates() const = 0;
		virtual int get_sample_number() const = 0;
		virtual double get_fdStep() const = 0;
		virtual ~BlackScholesModelInputParser() {};
	};
}