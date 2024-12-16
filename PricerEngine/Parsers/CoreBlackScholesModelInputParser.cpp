#include "CoreBlackScholesModelInputParser.hpp"
#include "json_helper.hpp"

using namespace input_parsers;

int CoreBlackScholesModelInputParser::get_underlying_number() const
{
	return 0;
}

PnlMat* CoreBlackScholesModelInputParser::get_cholesky_lines() const
{
	PnlMat* result;
	json.at("VolCholeskyLines").get_to(result);
	return result;
}

double CoreBlackScholesModelInputParser::get_interest_rate() const
{
	double result;
	json.at("DomesticInterestRate").get_to(result);
	return result;
}

PnlVect * CoreBlackScholesModelInputParser::get_monitoringDates() const
{
	PnlVect * result;
	json.at("MathPaymentDates").get_to(result);
	return result;
}

int CoreBlackScholesModelInputParser::get_sample_number() const{
    int result;
    json.at("SampleNb").get_to(result);
    return result;
}

double CoreBlackScholesModelInputParser::get_fdStep() const{
    int result;
    json.at("RelativeFiniteDifferenceStep").get_to(result);
    return result;
}