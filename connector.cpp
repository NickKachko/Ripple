#include <cstring>
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <QString>
#include <QStringList>
#include <QThread>

#include "MarketAccessProvider.h"

using namespace std;

bool isAllowedToRun = true;

double money = 500.0;
double ripples = 0.0;
double buyPrice = INT_MAX, sellPrice = INT_MAX;


double currentPrice = 0, previousPrice = 0;

void buy(double fraction)
{
    if (money)
    {
        double minusAmount = fraction * money;
        ripples += minusAmount / currentPrice;
        money -= minusAmount;
        cout << "Buy " << sellPrice << " " << currentPrice << " current ripples " << ripples << endl;
        buyPrice = currentPrice;
        sellPrice = 0;
    }
}

void sell(double fraction)
{
    if (ripples)
    {
        double minusAmount = ripples * fraction;
        money += minusAmount * currentPrice;
        ripples -= minusAmount;
        cout << "Sell " << buyPrice << " " << currentPrice << " current money " << money << endl;
        sellPrice = currentPrice;
        buyPrice = INT_MAX;
    }
}


auto movingAverage = [&] (const vector<double> &input, const int current, const int avgSpan) -> double
    {
        // static const int avgSpan = 10;
        double average = 0.0;
        if (input.size() < 2 || input.size() < current - 1 || current < avgSpan)
        return 0;

        for (int i = current - avgSpan; i < current; i++)
        {
            average += input[i];
        }
        average /= avgSpan;

        if (input[current] > average)
        {
            // cout << "Average is less, buying " << average << " " << input[current] << endl;
            return 1;
        }

        if (input[current] < average)
        {
            // cout << "Average is more, selling " << average << " " << input[current] << endl;
            return -1;
        }

        return 0;
    };
void my_handler(int s){
    printf("Caught signal %d\n",s);
    isAllowedToRun = false;

}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cout << "Error, number of arguments should be 1!" << endl;
        return 1;
    }


    MarketAccessProvider provider(argv[1]);

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    previousPrice = provider.values.back();

    while(isAllowedToRun)
    {
        currentPrice = provider.getCurrentPrice();

        if (!currentPrice) continue;

        if (currentPrice != previousPrice)
        {
            cout << "Price changed to " << currentPrice << endl;
            previousPrice = currentPrice;

            provider.values.push_back(currentPrice);

            double fraction = movingAverage(provider.values, provider.values.size() - 1, 50);

            if (fraction > 0)
            {
                buy(fraction);
            }
            if (fraction < 0)
            {
                sell(fraction*(-1));
            }
        }

        QThread::sleep(2);
    }

    return 0;
}