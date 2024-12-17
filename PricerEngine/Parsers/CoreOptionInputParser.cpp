#include "CoreOptionInputParser.hpp"
#include "json_helper.hpp"

using namespace input_parsers;

int CoreOptionInputParser::get_underlying_number() const
{
	return 0;
}

double CoreOptionInputParser::get_maturity() const
{
	return 0.0;
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