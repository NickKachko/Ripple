/**
 * Interface for price providers
 * @file IAccessProvider.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include "IAccessProvider.h"
#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

using namespace std;

const string VALUES_FILE_NAME = "values.txt";


IAccessProvider::IAccessProvider(std::string curr) : currency(curr), fileName(currency + "_" + VALUES_FILE_NAME)
{
    cout << "Creating IAccessProvider" << endl;
}

IAccessProvider::~IAccessProvider()
{
    cout << "Deleting IAccessProvider" << endl;
}

void IAccessProvider::saveData()
{
    QJsonArray valuesArray;
    std::copy(values.begin(), values.end(), std::back_inserter(valuesArray));

    QFile file(QString::fromStdString(fileName));
    file.open(QFile::WriteOnly);
    file.write(QJsonDocument(valuesArray).toJson());
    file.close();

    cout << "Successfully stored " << valuesArray.size() << " items" << endl;
}

void IAccessProvider::retrieveData()
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