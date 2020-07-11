/**
 * Entity that provides access to pricing, loads/stores vector of previous values
 * @file MarketAccessProvider.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include "MarketAccessProvider.h"
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

using namespace std;

const string VALUES_FILE_NAME = "values.txt";

int writeCallback(void *contents, size_t size, size_t nmemb, void *p)
{
    // cout << "Have " << size << " " << nmemb << " and " << p << endl;
    // cout << (char*)contents << endl;

    static_cast<MarketAccessProvider*>(p)->mContent += string((char*)contents, nmemb);

    return nmemb;
}

MarketAccessProvider::MarketAccessProvider(std::string curr) : IAccessProvider(curr)
{
    QString url = QString::fromStdString("https://cex.io/api/last_price/" + currency + "/USD");
    cout << "Creating MarketAccessProvider " << url.toStdString() << endl;
    mCurl = curl_easy_init();
    curl_easy_setopt(mCurl, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this);

    // exit(0);

    retrieveData();
}

MarketAccessProvider::~MarketAccessProvider()
{
    cout << "Deleting MarketAccessProvider" << endl;
    curl_easy_cleanup(mCurl);

    saveData();
}

double MarketAccessProvider::getCurrentPrice()
{
    CURLcode res;
    mContent = "";
    QJsonObject jsonObject;
    double currentPrice;

    res = curl_easy_perform(mCurl);
    // cout << content << endl;

    if (res != CURLE_OK)
    {
        cout << "ERROR with curl " << curl_easy_strerror(res) << endl;
    }

    jsonObject  = QJsonDocument::fromJson(QString::fromStdString(mContent).toUtf8()).object();
    currentPrice = jsonObject.value("lprice").toString().toDouble();

    if (values.size() && currentPrice != values.back())
    {
        values.push_back(currentPrice);
    }

    return currentPrice;
}