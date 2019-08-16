/**
 * Entity that provides access to pricing, loads/stores vector of previous values
 * @file MarketAccessProvider.cpp
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include <cstring>
#include <iostream>
#include <curl/curl.h>
#include <vector>

class MarketAccessProvider
{
    friend int writeCallback(void *contents, size_t size, size_t nmemb, void *p);
public:
    MarketAccessProvider(std::string curr);
    ~MarketAccessProvider();
    double getCurrentPrice();

    std::vector<double> values;
private:
    void saveData();
    void retrieveData();

    CURL *mCurl;
    std::string mContent;
    const std::string currency;
    const std::string fileName;
};