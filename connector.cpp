#include <cstring>
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QStringList>
#include <QThread>

using namespace std;

char memory[5000];
std::string content;

// vector<double> values;
double money = 500.0;
double ripples = 0.0;
double buyPrice = INT_MAX, sellPrice = INT_MAX;
int buyCount = 0, sellCount = 0;


double currentPrice = 0, previousPrice = 0;

void buy(double fraction)
{
    if (money)
    {
        double minusAmount = fraction * money;
        ripples += minusAmount / currentPrice;
        money -= minusAmount;
        buyCount++;
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
        sellCount++;
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



class Connector
{
public:
    Connector()
    {
        mCurl = curl_easy_init();
        curl_easy_setopt(mCurl, CURLOPT_URL, "https://cex.io/api/last_price/XRP/USD");
        curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, WriteCB);
        curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this);
    }

    ~Connector()
    {
        curl_easy_cleanup(mCurl);
    }

    void performRequest()
    {
        CURLcode res;
        mContent = "";
        res = curl_easy_perform(mCurl);
        // cout << content << endl;

        if (res != CURLE_OK)
        {
            cout << "ERROR with curl " << curl_easy_strerror(res) << endl;
        }
        // else
        // {
        //     cout << "SUCCESS " << res << endl;
        // }
    }

    static int WriteCB(void *contents, size_t size, size_t nmemb, void *p)
    {
        // cout << "Have " << size << " " << nmemb << " and " << p << endl;
        // cout << (char*)contents << endl;

        static_cast<Connector*>(p)->appendData(std::string((char*)contents, nmemb));

        return nmemb;
    }

    std::string getData()
    {
        return mContent;
    }

    void appendData(std::string dataToAdd)
    {
        mContent += dataToAdd;
    }

private:
    CURL *mCurl;
    std::string mContent;
};


int main()
{
    QJsonObject jsonObject;
    Connector connector;
    vector<double> values = {0.2528 , 0.2527 , 0.2525 , 0.253 , 0.2531 , 0.2534 , 0.2539 , 0.254 , 0.2541 , 0.2536 , 0.2534 , 0.2535 , 0.2532 , 0.2543 , 0.2555 , 0.255 , 0.2516 , 0.2525 , 0.2529 , 0.253 , 0.2531 , 0.253 , 0.2542 , 0.2548 , 0.255 , 0.254};

    // connector.performRequest();

    // jsonObject  = QJsonDocument::fromJson(QString::fromStdString(connector.getData()).toUtf8()).object();

    // cout << jsonObject.keys().join(" ").toStdString() << endl;
    // cout << jsonObject.value("lprice").toString().toStdString() << endl;
    // cout << jsonObject.value("lprice").toString().toDouble() << endl;




    // cout << "Nikita, done " << connector.getData() << endl;

    // QThread::sleep(20);
    // connector.performRequest();
    // cout << "Nikita, done " << connector.getData() << endl;




    while(true)
    {
        connector.performRequest();
        jsonObject  = QJsonDocument::fromJson(QString::fromStdString(connector.getData()).toUtf8()).object();
        currentPrice = jsonObject.value("lprice").toString().toDouble();

        if (currentPrice != previousPrice)
        {
            cout << "Price changed to " << currentPrice << endl;
            previousPrice = currentPrice;

            values.push_back(currentPrice);

            double fraction = movingAverage(values, values.size() - 1, 16);

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