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

MarketAccessProvider::MarketAccessProvider(std::string curr) : currency(curr), fileName(currency + "_" + VALUES_FILE_NAME)
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

    return currentPrice;
}

void MarketAccessProvider::saveData()
{
    QJsonArray valuesArray;
    std::copy(values.begin(), values.end(), std::back_inserter(valuesArray));

    QFile file(QString::fromStdString(fileName));
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(valuesArray).toJson());
    file.close();

    cout << "Successfully stored " << valuesArray.size() << " items" << endl;
}

void MarketAccessProvider::retrieveData()
{
    QFile file(QString::fromStdString(fileName));

    if (!file.exists())
    {
        cout << "No file " << fileName << " exists!" << endl;
        return;
    }

    file.open(QFile::ReadOnly);

    QString raw = file.readAll();
    QJsonArray array = QJsonDocument::fromJson(raw.toUtf8()).array();

    for (int i = 0; i < array.size(); i++)
    {
        values.push_back(array[i].toDouble());
    }

    file.close();
    cout << "Successfully read " << values.size() << " items" << endl;
}