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
            if(_dayCount == Period)
            {
                _dayCount = 1;
                return true;
            }
            _dayCount++;
            return false;
        }
    }
}
