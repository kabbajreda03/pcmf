﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HedgingEngine.RebalancingOracle
{
    public interface IRebalancingOracle
    {
        public bool ShouldRebalance();
    }
}
