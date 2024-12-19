#include <iostream>
#include "json_reader.hpp"
#include "pricer.hpp"
#include <memory>


using namespace std;
using namespace input_parsers;
using namespace models;
using namespace options;
using namespace generators;
using namespace pricer;

Option * createOption(string optionType, OptionParameters &param) {

    if (optionType == "ConditionalBasket") {
        return new ConditionalBasketOption(param);
    } 
    else if (optionType == "ConditionalMax") {
        
        return new ConditionalMaxOption(param);
    }
    else {
        cerr << "Unknown option type: " << optionType << endl;
        exit(1);
    }
}

BlackScholesPricer::BlackScholesPricer(nlohmann::json &jsonParams) {
    jsonParams.at("VolCholeskyLines").get_to(volatility);
    jsonParams.at("MathPaymentDates").get_to(paymentDates);
    jsonParams.at("Strikes").get_to(strikes);
    jsonParams.at("DomesticInterestRate").get_to(interestRate);
    jsonParams.at("RelativeFiniteDifferenceStep").get_to(fdStep);
    jsonParams.at("SampleNb").get_to(nSamples);
    std::string optionType;
    jsonParams.at("PayoffType").get_to(optionType);
    nAssets = volatility->n;
    CoreOptionInputParser option_parser(jsonParams);
	CoreBlackScholesModelInputParser model_parser(jsonParams);
    BlackScholesModelParameters bs_param(model_parser);
	bs_model = new BlackScholesModel(bs_param);
	OptionParameters option_parameters(option_parser);
	option = createOption(optionType, option_parameters);
	mc_pricer = new MonteCarloPricer(nSamples);
}

BlackScholesPricer::~BlackScholesPricer() {
    pnl_vect_free(&paymentDates);
    pnl_vect_free(&strikes);
    pnl_mat_free(&volatility);
    delete bs_model;
    delete option;
    delete mc_pricer;
}

void BlackScholesPricer::print() {
    std::cout << "nAssets: " << nAssets << std::endl;
    std::cout << "fdStep: " << fdStep << std::endl;
    std::cout << "nSamples: " << nSamples << std::endl;
    std::cout << "strikes: ";
    pnl_vect_print_asrow(strikes);
    std::cout << "paymentDates: ";
    pnl_vect_print_asrow(paymentDates);
    std::cout << "volatility: ";
    pnl_mat_print(volatility);
}

void BlackScholesPricer::priceAndDeltas(const PnlMat *past, double currentDate, bool isMonitoringDate, double &price, double &priceStdDev, PnlVect* &deltas, PnlVect* &deltasStdDev) {
    price = 0.;
    priceStdDev = 0.;
    deltas = pnl_vect_create_from_zero(nAssets);
    deltasStdDev = pnl_vect_create_from_zero(nAssets);
    mc_pricer->price_at(currentDate, *bs_model, *option, past, price, priceStdDev, deltas, deltasStdDev);
}