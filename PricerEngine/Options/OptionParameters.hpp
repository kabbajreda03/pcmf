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
		double maturity;
		PnlVect * strikes;
        PnlVect * monitoringDates;
		
		OptionParameters() :
			underlying_number(0),
			maturity(0),
			monitoringDates(nullptr),
			strikes(nullptr),

		{};

		OptionParameters(const input_parsers::OptionInputParser &input_parser) 
		{
			underlying_number = input_parser.get_underlying_number();
			maturity = input_parser.get_maturity();
			monitoringDates = pnl_vect_copy(input_parser.get_monitoringDates());
			strikes = pnl_vect_copy(input_parser.get_strikes());
		};

		OptionParameters(int underlyings, double mat, PnlVect * md, PnlVect * stk) :
			underlying_number(underlyings),
			maturity(mat)
			
		{
            monitoringDates = pnl_vect_copy(md);
			strikes = pnl_vect_copy(stk);
		};


		OptionParameters(const OptionParameters & other) :
			underlying_number(other.underlying_number),
			maturity(other.maturity)
		{
            monitoringDates = pnl_vect_copy(other.monitoringDates);
			strikes = pnl_vect_copy(other.strikes);
		}

		OptionParameters(OptionParameters && other) :
			underlying_number(other.underlying_number),
			maturity(other.maturity),
			monitoringDates(other.monitoringDates),
			strikes(other.strikes)
		{
            other.monitoringDates = nullptr;
			other.strikes = nullptr;
		}

		OptionParameters& operator=(OptionParameters && other) 
		{
			if (this != &other)
			{
				underlying_number = other.underlying_number;
				maturity = other.maturity;
				monitoringDates = other.monitoringDates;
				strikes = other.strikes;
				other.strikes = nullptr;
                other.monitoringDates = nullptr;
			}
			return *this;
		}


		OptionParameters& operator=(const OptionParameters & other)
		{
			if (this != &other)
			{
				underlying_number = other.underlying_number;
				maturity = other.maturity;
				monitoringDates = pnl_vect_copy(other.monitoringDates);
				strikes = pnl_vect_copy(other.strikes);
			}
			return *this;
		}

		~OptionParameters()
		{
			pnl_vect_free(&strikes);
		}
	};

}