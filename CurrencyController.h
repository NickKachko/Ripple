/**
 * Class that decides when to buy and when to sell
 * @file CurrencyController.h
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include <functional>
#include <vector>

class CurrencyController
{
public:
    CurrencyController(double* currentPrice = nullptr);
    ~CurrencyController();
    void sell(double fraction);
    void buy(double fraction);

private:
    double money;
    double currency;
    double* pCurrentPrice;
    double buyPrice, sellPrice;
};