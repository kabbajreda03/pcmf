using System;
using System.Text.Json;
using Grpc.Net.Client;
using HedgingEngine.Pricer;
using HedgingEngine.Hedging;
using MarketData;
using Microsoft.VisualBasic;
using MultiCashFlow.Common;
using ParameterInfo;
using ParameterInfo.JsonUtils;
using TimeHandler;

namespace HedgingEngine
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length != 3)
            {
                Console.WriteLine("Usage: program financial-param.json MarketData.csv portfolio.json");
                return;
            }

            try
            {
                // Get paths from arguments
                string financialParamPath = args[0];
                string marketDataPath = args[1];
                string outputPath = args[2];

                // Read financial parameters
                string jsonContent = File.ReadAllText(financialParamPath);
                var financialParams = JsonIO.FromJson(jsonContent);

                // Read market data
                var dataFeeds = MarketDataReader.ReadDataFeeds(marketDataPath);


                // Create and run hedging engine
                var hedgingEngine = new HedgingState(financialParams);
                var results = hedgingEngine.ProcessAllFeeds(dataFeeds.ToArray());

                // Write results to json file
                var jsonOptions = new JsonSerializerOptions { WriteIndented = true };
                string outputJson = JsonSerializer.Serialize(results, jsonOptions);
                File.WriteAllText(outputPath, outputJson);
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
                Environment.Exit(1);
            }
        }
    }
}