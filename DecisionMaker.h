/**
 * Class that decides when to buy and when to sell
 * @file DecisionMaker.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include <functional>
#include <vector>

class DecisionMaker
{
public:
    DecisionMaker();
    ~DecisionMaker();
    double supplyNewValue(double value);

private:
    std::vector<double> values;
    std::function<double (const int)> movingAverage;
    int avgCount = 19;
    // auto movingAverage = [&] (const int current, const int avgSpan) -> double
    // {
    //     // static const int avgSpan = 10;
    //     double average = 0.0;
    //     if (values.size() < 2 || values.size() < current - 1 || current < avgSpan)
    //     return 0;

    //     for (int i = current - avgSpan; i < current; i++)
    //     {
    //         average += values[i];
    //     }
    //     average /= avgSpan;

    //     if (values[current] > average)
    //     {
    //         // cout << "Average is less, buying " << average << " " << values[current] << endl;
    //         return 1;
    //     }

    //     if (values[current] < average)
    //     {
    //         // cout << "Average is more, selling " << average << " " << values[current] << endl;
    //         return -1;
    //     }

    //     return 0;
    // };
};