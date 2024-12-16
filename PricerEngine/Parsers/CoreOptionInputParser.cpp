#include "CoreOptionInputParser.hpp"
#include "json_helper.hpp"

using namespace input_parsers;

int CoreOptionInputParser::get_underlying_number() const
{
	return 0;
}

PnlVect * CoreOptionInputParser::get_strikes() const
{
	PnlVect * result;
	json.at("Strikes").get_to(result);
	return result;
}

PnlVect* CoreOptionInputParser::get_volatility() const
{
	PnlVect * result;
	json.at("volatilities").get_to(result);
	return result;
}


PnlVect* CoreOptionInputParser::get_monitoringDates() const
{
	PnlVect * result;
	json.at("paymentDates").get_to(result);
	return result;
}
