#pragma once
#include <utility>
#include <iostream>
#include "pnl/pnl_vector.h"
#include "UnderlyingModel.hpp"
#include "BlackScholesModelInputParser.hpp"

namespace models {

	struct BlackScholesModelParameters
	{
		int underlying_number;
		PnlMat * cholesky_lines;
        double interest_rate;
        PnlVect * monitoringDates;
		int nb_monitoring_dates;
		int sample_number;
        double fdStep;
		PnlVect * spots;

		BlackScholesModelParameters() :
			underlying_number(0),
            cholesky_lines(nullptr),
            interest_rate(0),
			monitoringDates(nullptr),
			nb_monitoring_dates(0),
			sample_number(0),
			fdStep(0),
			spots(nullptr)
		{};

		BlackScholesModelParameters(const input_parsers::BlackScholesModelInputParser &input_parser, PnlVect * spots_) 
		{
			underlying_number = input_parser.get_underlying_number();
            cholesky_lines = input_parser.get_cholesky_lines();
            interest_rate = input_parser.get_interest_rate();
			monitoringDates = pnl_vect_copy(input_parser.get_monitoringDates());
			sample_number = input_parser.get_sample_number();
			fdStep = input_parser.get_fdStep();
			nb_monitoring_dates = monitoringDates->size;
			spots = pnl_vect_copy(spots_);
		};
    };
}
