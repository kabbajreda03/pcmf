#pragma once
#include <nlohmann/json.hpp>
#include "BlackScholesModelInputParser.hpp"

namespace input_parsers
{
	class CoreBlackScholesModelInputParser : public BlackScholesModelInputParser
	{
	private:
		nlohmann::json json;
	public:
		int get_underlying_number() const;
		PnlMat* get_cholesky_lines() const;
		double get_interest_rate() const;
		PnlVect * get_monitoringDates() const;
		int get_sample_number() const;
		double get_fdStep() const;
		CoreBlackScholesModelInputParser(const nlohmann::json& j) : json(j)
		{};

		virtual ~CoreBlackScholesModelInputParser() {};
	};
}