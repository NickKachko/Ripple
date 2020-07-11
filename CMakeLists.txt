cmake_minimum_required(VERSION 3.1.0)

project(trader)

set (CMAKE_PREFIX_PATH "/home/mkachko/Qt/5.13.0/gcc_64")
set (CMAKE_CXX_STANDARD 17)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core REQUIRED)

add_executable(trader
    connector.cpp
    CurrencyController.h
    CurrencyController.cpp
    DecisionMaker.h
    DecisionMaker.cpp
    MarketProvider/MarketAccessProvider.cpp
    MarketProvider/MarketAccessProvider.h
    MarketProvider/FileMarketProvider.cpp
    MarketProvider/FileMarketProvider.h
    MarketProvider/IAccessProvider.cpp
    MarketProvider/IAccessProvider.h
)

target_link_libraries(trader curl Qt5::Core)