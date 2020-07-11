#include <cstring>
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>

#include <memory>
#include <signal.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>

#include <QString>
#include <QStringList>
#include <QThread>

#include "CurrencyController.h"
#include "DecisionMaker.h"
#include "MarketProvider/IAccessProvider.h"
#include "MarketProvider/MarketAccessProvider.h"
#include "MarketProvider/FileMarketProvider.h"

using namespace std;

bool isAllowedToRun = true;


double currentPrice = 0, previousPrice = 0;

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

    unique_ptr<IAccessProvider> provider = make_unique<FileMarketProvider>(argv[1]);
    DecisionMaker decisionMaker;
    CurrencyController currencyController(&currentPrice);

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    previousPrice = provider->getCurrentPrice();

    while(isAllowedToRun)
    {
        currentPrice = provider->getCurrentPrice();

        if (!currentPrice) continue;

        if (currentPrice < 0) break;

        if (currentPrice != previousPrice)
        {
            cout << "Price changed to " << currentPrice << endl;
            previousPrice = currentPrice;

            double fraction = decisionMaker.supplyNewValue(currentPrice);

            if (fraction > 0)
            {
                currencyController.buy(fraction);
            }
            if (fraction < 0)
            {
                currencyController.sell(fraction*(-1));
            }
        }

        QThread::sleep(0.5);
    }

    return 0;
}