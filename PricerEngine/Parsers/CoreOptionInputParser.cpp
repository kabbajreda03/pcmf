#include "CoreOptionInputParser.hpp"
#include "json_helper.hpp"

using namespace input_parsers;

int CoreOptionInputParser::get_underlying_number() const
{
	PnlMat* result;
	json.at("VolCholeskyLines").get_to(result);
	return result->n;
}

double CoreOptionInputParser::get_maturity() const
{
	PnlVect * result;
	json.at("MathPaymentDates").get_to(result);
	return GET(result, result->size-1);
}


PnlVect * CoreOptionInputParser::get_monitoringDates() const
{
	PnlVect * result;
	json.at("MathPaymentDates").get_to(result);
	return result;
}

PnlVect * CoreOptionInputParser::get_strikes() const
{
	PnlVect * result;
	json.at("Strikes").get_to(result);
	return result;
}