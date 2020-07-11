/**
 * Class that decides when to buy and when to sell
 * @file CurrencyController.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include "CurrencyController.h"
#include <iostream>
#include <float.h>

using namespace std;

CurrencyController::CurrencyController(double* currentPrice) : pCurrentPrice(currentPrice)
{
    buyPrice = DBL_MAX, sellPrice = DBL_MAX;
    money = 500;
    currency = 0;
}

CurrencyController::~CurrencyController()
{
    cout << "Deleting CurrencyController" << endl;
}


void CurrencyController::sell(double fraction)
{
    if (currency && pCurrentPrice != nullptr)
    {
        double minusAmount = currency * fraction;
        money += minusAmount * (*pCurrentPrice);
        currency -= minusAmount;
        cout << "Sell " << buyPrice << " " << (*pCurrentPrice) << " current money " << money << endl;
        sellPrice = (*pCurrentPrice);
        buyPrice = DBL_MAX;
    }
}

void CurrencyController::buy(double fraction)
{
    if (money && pCurrentPrice != nullptr)
    {
        double minusAmount = fraction * money;
        currency += minusAmount / (*pCurrentPrice);
        money -= minusAmount;
        cout << "Buy " << sellPrice << " " << (*pCurrentPrice) << " current currency " << currency << endl;
        buyPrice = (*pCurrentPrice);
        sellPrice = 0;
    }
}