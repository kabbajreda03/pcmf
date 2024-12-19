using ParameterInfo.RebalancingOracleDescriptions;
using System;
using TimeHandler;

namespace HedgingEngine.RebalancingOracle
{
    public class FixedRebalancingOracle : IRebalancingOracle
    {
        public int Period { get; private set; }
        private int _dayCount;

        public FixedRebalancingOracle(int period)
        {
            Period = period;
            _dayCount = 1;
        }

        public bool ShouldRebalance()
        {
            _dayCount++;
            return _dayCount % Period == 0;
        }
    }
}
