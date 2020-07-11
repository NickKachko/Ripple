/**
 * Entity that provides access to history pricing loaded from file
 * @file FileMarketProvider.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include "FileMarketProvider.h"
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

using namespace std;


FileMarketProvider::FileMarketProvider(std::string curr) : IAccessProvider(curr)
{
    index = 0;
    retrieveData();
}

FileMarketProvider::~FileMarketProvider()
{
    cout << "Deleting FileMarketProvider" << endl;
}

double FileMarketProvider::getCurrentPrice()
{
    if (index < values.size())
    {
        return values[index++];
    }
    else
    {
        return -1;
    }

}