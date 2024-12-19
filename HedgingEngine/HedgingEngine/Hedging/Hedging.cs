using System;
using System.Collections.Generic;
using System.Linq;
using HedgingEngine.Portfolio;
using HedgingEngine.Pricer;
using HedgingEngine.RebalancingOracle;
using MarketData;
using ParameterInfo;
using TimeHandler;

namespace HedgingEngine.Hedging
{
    public class HedgingState
    {
        private readonly IPricerClient _pricer;
        private readonly TestParameters _params;
        private PortfolioState _portfolio;
        private readonly FixedRebalancingOracle _oracle;
        private readonly List<DataFeed> _monitoringData;
        private readonly MathDateConverter _dateConverter;
        private readonly List<OutputData> _results;
        private readonly double _domesticRate;
        private DataFeed _initialFeed;

        public HedgingState(TestParameters parameters)
        {
            _pricer = new PricerClient();
            _params = parameters;
            _oracle = new FixedRebalancingOracle(_params.RebalancingOracleDescription.Period);
            _monitoringData = new List<DataFeed>();
            _dateConverter = new MathDateConverter(_params.NumberOfDaysInOneYear);
            _results = new List<OutputData>();
            _domesticRate = _params.AssetDescription.CurrencyRates[_params.AssetDescription.DomesticCurrencyId];
        }

        public List<OutputData> ProcessAllFeeds(DataFeed[] feeds)
        {
            if (feeds == null || feeds.Length == 0)
            {
                throw new ArgumentException("Feeds array cannot be null or empty");
            }

            // Initialize with t=0
            Initialize(feeds[0]);
            InitialHedge();

            // Process remaining dates starting from t=1
            for (int i = 1; i < feeds.Length; i++)
            {
                ProcessNewDate(feeds[i]);
            }

            return _results;
        }

        private void Initialize(DataFeed initialFeed)
        {
            _initialFeed = initialFeed;
        }

        private void InitialHedge()
        {
            if (_initialFeed == null)
            {
                throw new InvalidOperationException("HedgingEngine must be initialized first");
            }

            double time = _dateConverter.ConvertToMathDistance(
                _params.PayoffDescription.CreationDate,
                _initialFeed.Date
            );

            // Get initial price and deltas
            _pricer.ComputePriceAndDeltas(new[] { _initialFeed }, true, time);

            // Create portfolio with initial option price as cash
            _portfolio = new PortfolioState(_pricer.Price);

            // Set up initial deltas using SpotList keys
            var initialDeltas = new Dictionary<string, double>();
            var assets = _initialFeed.SpotList.Keys.ToList();
            for (int i = 0; i < _pricer.Deltas.Length; i++)
            {
                initialDeltas[assets[i]] = _pricer.Deltas[i];
            }

            _portfolio.UpdateComposition(initialDeltas, _initialFeed.SpotList, _pricer.Price);

            // Record initial result
            var output = new OutputData
            {
                Value = _pricer.Price,
                Date = _initialFeed.Date,
                Price = _pricer.Price,
                PriceStdDev = _pricer.PriceStdDev,
                Deltas = [.. _pricer.Deltas],
                DeltasStdDev = [.. _pricer.DeltasStdDev]
            };
            _results.Add(output);
        }

        private void ProcessNewDate(DataFeed feed)
        {
            if (_initialFeed == null || feed.Date == _initialFeed.Date)
            {
                return;
            }

            bool isMonitoringDate = _params.PayoffDescription.PaymentDates.Contains(feed.Date);
            if (isMonitoringDate)
            {
                _monitoringData.Add(feed);
            }

            double time = _dateConverter.ConvertToMathDistance(
                _params.PayoffDescription.CreationDate,
                feed.Date
            );

            DataFeed[] past = BuildPastArray(feed, isMonitoringDate);
            _pricer.ComputePriceAndDeltas(past, isMonitoringDate, time);

            if (_oracle.ShouldRebalance())
            {
                double portfolioTime = _dateConverter.ConvertToMathDistance(
                    _results.Last().Date,
                    feed.Date
                );
                double value = _portfolio.CalculatePortfolioValue(feed.SpotList, _domesticRate, portfolioTime);

                var newDeltas = new Dictionary<string, double>();
                var assets = _initialFeed.SpotList.Keys.ToList();
                for (int i = 0; i < _pricer.Deltas.Length; i++)
                {
                    newDeltas[assets[i]] = _pricer.Deltas[i];
                }

                _portfolio.UpdateComposition(newDeltas, feed.SpotList, value);

                // Record result only when rebalancing
                var output = new OutputData
                {
                    Date = feed.Date,
                    Value = value,
                    Deltas = [.. _pricer.Deltas],
                    DeltasStdDev = [.. _pricer.DeltasStdDev],
                    Price = _pricer.Price,
                    PriceStdDev = _pricer.PriceStdDev
                };
                _results.Add(output);
            }
        }

        private DataFeed[] BuildPastArray(DataFeed currentFeed, bool isMonitoringDate)
        {
            var result = new List<DataFeed>();
            result.Add(_initialFeed);
            result.AddRange(_monitoringData);

            if (!isMonitoringDate)
            {
                result.Add(currentFeed);
            }

            return result.ToArray();
        }

        public PortfolioState GetPortfolio()
        {
            return _portfolio;
        }
    }
}
