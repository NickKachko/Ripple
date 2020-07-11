/**
 * Entity that provides access to history pricing loaded from file
 * @file FileMarketProvider.h
 *
 * @author Nikita Kachko <justmynk@gmail.com>
 */

#include <cstring>

#include "IAccessProvider.h"

class FileMarketProvider : public IAccessProvider
{
public:
    FileMarketProvider(std::string curr);
    virtual ~FileMarketProvider();
    double getCurrentPrice() override;

private:
    uint index;
};