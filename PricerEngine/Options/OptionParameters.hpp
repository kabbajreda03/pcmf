#pragma once
#include <utility>
#include <iostream>
#include "pnl/pnl_vector.h"
#include "UnderlyingModel.hpp"
#include "OptionInputParser.hpp"

namespace options {

	struct OptionParameters
	{
		int underlying_number; 
		PnlVect * strikes;
        PnlVect * monitoringDates;
		PnlVect* volatility;

		OptionParameters() :
			underlying_number(0),
			monitoringDates(nullptr),
			strikes(nullptr),
			volatility(nullptr)
		{};

		OptionParameters(const input_parsers::OptionInputParser &input_parser) 
		{
			underlying_number = input_parser.get_underlying_number();
			monitoringDates = pnl_vect_copy(input_parser.get_monitoringDates());
			strikes = pnl_vect_copy(input_parser.get_strikes());
			volatility = pnl_vect_copy(input_parser.get_volatility());
		};

		OptionParameters(int underlyings, PnlVect * md, PnlVect * stk, PnlVect* vol) :
			underlying_number(underlyings)
			
		{
            monitoringDates = pnl_vect_copy(md);
			strikes = pnl_vect_copy(stk);
			volatility = pnl_vect_copy(vol);
		};


		OptionParameters(const OptionParameters & other) :
			underlying_number(other.underlying_number)
		{
            monitoringDates = pnl_vect_copy(other.monitoringDates);
			strikes = pnl_vect_copy(other.strikes);
			volatility = pnl_vect_copy(other.volatility);
		}

		OptionParameters(OptionParameters && other) :
			underlying_number(other.underlying_number),
			monitoringDates(other.monitoringDates),
			strikes(other.strikes),
			volatility(other.volatility)
		{
            other.monitoringDates = nullptr;
			other.strikes = nullptr;
			other.volatility = nullptr;
		}

		OptionParameters& operator=(OptionParameters && other) 
		{
			if (this != &other)
			{
				underlying_number = other.underlying_number;
				monitoringDates = other.monitoringDates;
				strikes = other.strikes;
				volatility = other.volatility;
				other.strikes = nullptr;
                other.monitoringDates = nullptr;
				other.volatility = nullptr;
			}
			return *this;
		}


		OptionParameters& operator=(const OptionParameters & other)
		{
			if (this != &other)
			{
				underlying_number = other.underlying_number;
				monitoringDates = pnl_vect_copy(other.monitoringDates);
				strikes = pnl_vect_copy(other.strikes);
				volatility = pnl_vect_copy(other.volatility);
			}
			return *this;
		}

		~OptionParameters()
		{
			pnl_vect_free(&strikes);
		}
	};

}