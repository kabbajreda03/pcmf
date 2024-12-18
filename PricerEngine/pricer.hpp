#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"
#include "MonteCarloPricer.hpp"
#include "CoreOptionInputParser.hpp"
#include "CoreBlackScholesModelInputParser.hpp"
#include "json_helper.hpp"
#include "PnlRandomGeneration.hpp"
#include "BlackScholesModel.hpp"
#include "OptionParameters.hpp"
#include "BlackScholesModelParameters.hpp"
#include "Option.hpp"
#include "ConditionalBasketOption.hpp"
#include "ConditionalMaxOption.hpp"


class BlackScholesPricer {
public:
    PnlMat *volatility;
    PnlVect *paymentDates;
    PnlVect *strikes;
    int nAssets;
    double interestRate;
    double fdStep;
    int nSamples;
    models::BlackScholesModel * bs_model;
    options::Option * option;
    pricer::MonteCarloPricer * mc_pricer;
    

    BlackScholesPricer(nlohmann::json &jsonParams);
    ~BlackScholesPricer();
    void priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev);
    void print();
};
