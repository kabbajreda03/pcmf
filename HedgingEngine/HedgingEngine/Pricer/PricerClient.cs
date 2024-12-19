using System;
using System.Linq;
using Grpc.Net.Client;
using GrpcPricing.Protos;
using MarketData;

namespace HedgingEngine.Pricer
{
    public class PricerClient : IPricerClient
    {
        public GrpcChannel Channel { get; private set; }
        public GrpcPricer.GrpcPricerClient client;

        public double Price { get; private set; }
        public double PriceStdDev { get; private set; }
        public double[] Deltas { get; private set; }
        public double[] DeltasStdDev { get; private set; }

        public PricerClient(string serverAddress = "http://localhost:50051")
        {
            var httpHandler = new HttpClientHandler();
            // Return `true` to allow certificates that are untrusted/invalid
            httpHandler.ServerCertificateCustomValidationCallback =
                HttpClientHandler.DangerousAcceptAnyServerCertificateValidator;
            Channel = GrpcChannel.ForAddress(serverAddress,
                new GrpcChannelOptions { HttpHandler = httpHandler});
            client = new GrpcPricer.GrpcPricerClient(Channel);

            Price = 0;
            PriceStdDev = 0;
            Deltas = Array.Empty<double>();
            DeltasStdDev = Array.Empty<double>();
            
        }

        public void ComputePriceAndDeltas(DataFeed[] past, bool monitoringDateReached, double time)
        {
            var output = PriceAndDeltas(past, monitoringDateReached, time);
            Price = output.Price;
            PriceStdDev = output.PriceStdDev;
            Deltas = output.Deltas.ToArray();
            DeltasStdDev = output.DeltasStdDev.ToArray();
        }

        private PricingOutput PriceAndDeltas(DataFeed[] past, bool monitoringDateReached, double time)
        {
            var input = CreatePricingInput(past, monitoringDateReached, time);
            return client.PriceAndDeltas(input);
        }

        public ReqInfo Heartbeat()
        {
            ReqInfo info = client.Heartbeat(new Empty());
            Console.WriteLine("Message reçu : " + info.DomesticInterestRate);
            Console.WriteLine("Message reçu : " + info.RelativeFiniteDifferenceStep);
            Console.WriteLine("Message reçu : " + info.SampleNb);

            return info;
        }

        private static PricingInput CreatePricingInput(DataFeed[] past, bool monitoringDateReached, double time)
        {
            var input = new PricingInput();

            foreach (var pastLine in past)
            {
                var line = new PastLines();
                foreach (double price in pastLine.SpotList.Values)
                {
                    Console.WriteLine("Price : " + price);
                    line.Value.Add(price);
                }
                input.Past.Add(line);
            }

            input.MonitoringDateReached = monitoringDateReached;
            input.Time = time;
            return input;
        }

    }
}
