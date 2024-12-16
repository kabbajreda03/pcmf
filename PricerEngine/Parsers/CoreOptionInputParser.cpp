#include "CoreBarrierOptionInputParser.hpp"
#include "json_helper.hpp"

using namespace input_parsers;

int CoreBarrierOptionInputParser::get_underlying_number() const
{
	return 0;
}

double CoreBarrierOptionInputParser::get_maturity() const
{
	return 0.0;
}

PnlVect * CoreBarrierOptionInputParser::get_monitoringDates() const
{
	PnlVect * result;
	json.at("MathPaymentDates").get_to(result);
	return result;
}

PnlVect * CoreBarrierOptionInputParser::get_strikes() const
{
	PnlVect * result;
	json.at("Strikes").get_to(result);
	return result;
}