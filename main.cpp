#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<double> values;
double money = 500.0;
double ripples = 0.0;
double buyPrice = 0.0, sellPrice = 0.0;
int currentIndex = 0, buyCount = 0, sellCount = 0;


void buy()
{
    if (money)
    {
        ripples += money / values[currentIndex];
        money = 0;
        buyCount++;
        buyPrice = values[currentIndex];
        sellPrice = 0;
    }
}

void sell()
{
    if (ripples)
    {
        money += ripples * values[currentIndex];
        ripples = 0;
        sellCount++;
        sellPrice = values[currentIndex];
        buyPrice = 0;
    }
}

void readFromFile(string fileName)
{
    ifstream ifile(fileName, ios::in);
    vector<double> scores;

    //check to see that the file was opened correctly:
    if (!ifile.is_open()) {
        cerr << "There was a problem opening the input file!\n";
        exit(1);//exit or do additional error checking
    }

    double num = 0.0, temp = 0.0;
    //keep storing values from the text file so long as data exists:
    while (ifile >> num) {
        values.insert(values.begin(), num);
        //values.push_back(num);
        ifile >> temp;
        ifile >> temp;
        ifile >> temp;
    }

    //verify that the scores were stored correctly:
    for (int i = 0; i < values.size() / 10; ++i) {
        cout << values[i] << endl;
    }
}

int main()
{

    readFromFile("data.txt");

    currentIndex = 0;
    buy();
    currentIndex = 1;
    while (currentIndex < values.size())
    {
        /*if (values[currentIndex] < values[currentIndex - 1])
        {
            buy();
        }
        if (values[currentIndex] > values[currentIndex - 1])
        {
            sell();
        }*/

        if (values[currentIndex] > buyPrice)
        {
            sell();
        }
        if (values[currentIndex] < sellPrice)
        {
            buy();
        }
        currentIndex++;
    }
    currentIndex--;
    sell();

    cout << "USD " << money << " and ripples " << ripples << endl;
    cout << "Buy count " << buyCount << " sell count " << sellCount << endl;
    system("PAUSE");
    return 0;
}