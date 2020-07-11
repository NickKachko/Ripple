/**
 * Interface for price providers
 * @file IAccessProvider.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#ifndef I_ACCESS_PROVIDER
#define I_ACCESS_PROVIDER

#include <cstring>
#include <iostream>
#include <vector>

class IAccessProvider
{
public:
    IAccessProvider(std::string curr);
    virtual ~IAccessProvider();
    virtual double getCurrentPrice() = 0;

protected:
    void saveData();
    void retrieveData();

    std::vector<double> values;
    const std::string currency;
    const std::string fileName;
};

#endif