#include <iostream>
#include <regex>
using namespace std;

string replaceFirstOccurrence(string& s, const string& toReplace, const string& replaceWith)
{
    std::size_t pos = s.find(toReplace);
    if (pos == string::npos) return s;
    return s.replace(pos, toReplace.length(), replaceWith);
}

string getDomain(string url){
    // Removing http:// or https://
    regex exp1("https*://");
    url = regex_replace(url, exp1, "");
    
    // Removing anything after first /
    size_t a = url.find_first_of("/");
    if(a != string::npos)url.erase(a);
    
    
    a = url.find_first_of("#");
    if(a != string::npos)url.erase(a);
    
    a = url.find_first_of("?");
    if(a != string::npos)url.erase(a);
    
    url = replaceFirstOccurrence(url, "www.", "");
    
    
    return url;
}