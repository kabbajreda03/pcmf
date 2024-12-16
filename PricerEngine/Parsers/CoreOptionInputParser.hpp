#pragma once
#include <nlohmann/json.hpp>
#include "OptionInputParser.hpp"


namespace input_parsers
{
	class CoreOptionInputParser : public OptionInputParser
	{
	private:
		nlohmann::json json;
	public:
		int get_underlying_number() const final;
		double get_maturity() const;
		PnlVect * get_monitoringDates() const;
		PnlVect * get_strikes() const;
		CoreOptionInputParser(const nlohmann::json &j) : json(j)
		{};
		~CoreOptionInputParser() {};
	};
}