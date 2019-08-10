all: connector.cpp
	g++ connector.cpp -std=c++17 -lcurl -o connector