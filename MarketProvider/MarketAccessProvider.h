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

#include "IAccessProvider.h"

class MarketAccessProvider : public IAccessProvider
{
    friend int writeCallback(void *contents, size_t size, size_t nmemb, void *p);
public:
    MarketAccessProvider(std::string curr);
    virtual ~MarketAccessProvider();
    double getCurrentPrice() override;

private:
    CURL *mCurl;
    std::string mContent;
};