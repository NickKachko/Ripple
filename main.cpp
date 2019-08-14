#include <iostream>
#include <fstream>
#include <vector>
#include <limits.h>

using namespace std;

vector<double> values;
double money = 500.0;
double ripples = 0.0;
double buyPrice = INT_MAX, sellPrice = INT_MAX;
int currentIndex = 0, buyCount = 0, sellCount = 0;


void buy(double fraction)
{
    if (money)
    {
        double minusAmount = fraction * money;
        ripples += minusAmount / values[currentIndex];
        money -= minusAmount;
        buyCount++;
        // cout << "Buy " << sellPrice << " " << values[currentIndex] << " money left " << money << endl;
        buyPrice = values[currentIndex];
        sellPrice = 0;
    }
}

void sell(double fraction)
{
    if (ripples)
    {
        double minusAmount = ripples * fraction;
        money += minusAmount * values[currentIndex];
        ripples -= minusAmount;
        sellCount++;
        // cout << "Sell " << buyPrice << " " << values[currentIndex] << " ripples left " << ripples << endl;
        sellPrice = values[currentIndex];
        buyPrice = INT_MAX;
    }
}

void readFromFile(string fileName, int firstCount)
{
    ifstream ifile(fileName, ios::in);
    vector<double> scores;
    int i = 0;

    //check to see that the file was opened correctly:
    if (!ifile.is_open()) {
        cerr << "There was a problem opening the input file!\n";
        exit(1);//exit or do additional error checking
    }

    double num = 0.0;
    //keep storing values from the text file so long as data exists:
    while (ifile >> num && i < firstCount) {
        values.insert(values.begin(), num);
        // values.push_back(num);
        i++;
    }

    //verify that the scores were stored correctly:
    for (int i = 0; i < 20; ++i) {
        cout << values[i] << endl;
    }
}

auto simple = [&] (vector<double> &input, int current) -> double
{
    if (input.size() < 2 || input.size() < current - 1)
    return 0;

    if (current == 0)
    {
        // buy();
        return 1;
    }

    if (input[current] < input[current - 1])
    {
        // buy();
        return 1;
    }
    if (input[current] > input[current - 1])
    {
        // sell();
        return -1;
    }
    return 0;
};

auto simpleBuySell = [&] (const vector<double> &input, const int current) -> double
{
    if (input.size() < 2 || input.size() < current - 1)
    return 0;


    if (input[current] > buyPrice)
    {
        // sell();
        // cout << "Buy and current " << buyPrice << " " << input[current] << endl;
        return -1;
    }

    if (input[current] < sellPrice)
    {
        // buy();
        return 1;
    }

    return 0;
};

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

int main()
{

    readFromFile("data.txt", 250);

    for (int i = 0; i < 50; i++)
    {
        currentIndex = 0;
        money = 500;
        ripples = 0;
        while (currentIndex < values.size())
        {
            double fraction = movingAverage(values, currentIndex, i);
            // cout << "Answer " << fraction << endl;
            if (fraction > 0)
            {
                buy(fraction);
            }
            if (fraction < 0)
            {
                sell(fraction*(-1));
            }

            if (currentIndex == values.size() - 1) sell(1);
            currentIndex++;
            // cout << "Size at \t" << currentIndex << " M " << money << endl;
        }
        // currentIndex--;
        // sell(1);
        // cout << "Size\t" << i << " M " << money << endl;
        cout << money << endl;
        // cout << "Selling at " << values[currentIndex] << endl;
    }

    // cout << "USD " << money << " and ripples " << ripples << endl;
    // cout << "Buy count " << buyCount << " sell count " << sellCount << endl;
    // system("PAUSE");
    return 0;
}