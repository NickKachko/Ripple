/**
 * Class that decides when to buy and when to sell
 * @file DecisionMaker.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include "DecisionMaker.h"
#include <iostream>

using namespace std;

DecisionMaker::DecisionMaker()
{
    movingAverage = [&] (const int avgSpan) -> double
    {
        // static const int avgSpan = 10;
        const double current = values.size() - 1;
        double average = 0.0;
        if (values.size() < 2 || current < avgSpan)
        return 0;

        for (int i = current - avgSpan; i < current; i++)
        {
            average += values[i];
        }
        average /= avgSpan;

        if (values[current] > average)
        {
            // cout << "Average is less, buying " << average << " " << values[current] << endl;
            return 1;
        }

        if (values[current] < average)
        {
            // cout << "Average is more, selling " << average << " " << values[current] << endl;
            return -1;
        }

        return 0;
    };
}

DecisionMaker::~DecisionMaker()
{
    cout << "Deleting DecisionMaker" << endl;
}

double DecisionMaker::supplyNewValue(double value)
{
    values.push_back(value);
    return movingAverage(avgCount);
}