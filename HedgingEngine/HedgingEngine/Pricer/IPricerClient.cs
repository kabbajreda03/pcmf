using GrpcPricing.Protos;
using MarketData;
using Grpc.Net.Client;


namespace HedgingEngine.Pricer
{
    public interface IPricerClient
    {
        void ComputePriceAndDeltas(DataFeed[] past, bool monitoringDateReached, double time);
        ReqInfo Heartbeat();
        double Price { get; }
        double PriceStdDev { get; }
        double[] Deltas { get; }
        double[] DeltasStdDev { get; }
    }
}

