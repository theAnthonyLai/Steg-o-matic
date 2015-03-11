#include "provided.h"
#include "http.h"
#include <string>
using namespace std;

bool WebSteg::hideMessageInPage(const string& url, const string& msg, string& host)
{
    string webPageText;
    HTTP().get(url, webPageText);
    if (Steg::hide(webPageText, msg, host))
        return true;
    return false;
}

bool WebSteg::revealMessageInPage(const string& url, string& msg)
{
    string webPageText;
    HTTP().get(url, webPageText);
    if (Steg::reveal(webPageText, msg))
        return true;
    return false;
}

unsigned int computeHash(unsigned short key) {
    return key;
}

unsigned int computeHash(std::string key) {
    int result = 0;
    for (int i = 0; i < key.size(); i++)
        result += (i+1)*key[i];
    return result;
}