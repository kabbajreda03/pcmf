#pragma once
#include "pnl/pnl_vector.h"

namespace input_parsers
{

	class OptionInputParser
	{
	public:
		virtual int get_underlying_number() const = 0;
		virtual double get_maturity() const = 0;
		virtual PnlVect * get_monitoringDates() const = 0;
		virtual PnlVect * get_strikes() const = 0;
		virtual ~BarrierOptionInputParser() {};
	};

}