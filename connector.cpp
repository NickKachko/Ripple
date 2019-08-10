#include <cstring>
#include <stdio.h>
#include <iostream>
#include <curl/curl.h>

using namespace std;

char memory[5000];
std::string content;

int WriteCB(void *contents, size_t size, size_t nmemb, void *p)
{
    cout << "Have " << size << " " << nmemb << " and " << p << endl;
    cout << (char*)contents << endl;
    content += std::string((char*)contents, nmemb);
    return nmemb;
}

int main()
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    char t[5000];
    if (curl)
    {
        cout << memory << endl;
        curl_easy_setopt(curl, CURLOPT_URL, "https://cex.io/api/last_price/XRP/USD");
        // curl_easy_setopt(curl, CURLOPT_WRITEDATA, memory);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCB);
        res = curl_easy_perform(curl);
        cout << content << endl;

        if (res != CURLE_OK)
        {
            cout << "ERROR with curl " << curl_easy_strerror(res) << endl;
        }
        else
        {
            cout << "SUCCESS " << res << endl;
        }
        // cout << strlen(t)<< endl;
        curl_easy_cleanup(curl);
    }
    else
    {
        cout << "No handle" << endl;
    }

    cout << "Nikita, done" << endl;
    return 0;
}