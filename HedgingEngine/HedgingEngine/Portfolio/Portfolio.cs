using System;
using System.Collections.Generic;
using MarketData;

namespace HedgingEngine.Portfolio
{
    public class PortfolioState
    {
        public Dictionary<string, double> Composition { get; private set; }
        public double Cash { get; private set; }

        public PortfolioState(double initialCash)
        {
            Composition = new Dictionary<string, double>();
            Cash = initialCash;
        }

        public void UpdateComposition(Dictionary<string, double> newDeltas, Dictionary<string, double> spotPrices, double portfolioValue)
        {

            foreach (var asset in newDeltas.Keys)
            {
                Composition[asset] = newDeltas[asset];
            }

            double newStockValue = GetStockValue(spotPrices);

            Cash = (portfolioValue -newStockValue);
        }

        public double GetDiscountedCashValue(double rate, double time)
        {
            return Cash * Math.Exp(-rate * time);
        }

        public double CalculatePortfolioValue(Dictionary<string, double> spotPrices, double rate, double time)
        {
            double value = GetDiscountedCashValue(rate, time);
            value += GetStockValue(spotPrices);
            return value;
        }

        private double GetStockValue(Dictionary<string, double> spotPrices)
        {
            double stockValue = 0.0;
            foreach (var asset in Composition.Keys)
            {
                if (spotPrices.ContainsKey(asset))
                {
                    stockValue += Composition[asset] * spotPrices[asset];
                }
            }
            return stockValue;
        }
    }
}
