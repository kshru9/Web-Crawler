#include <iostream>
#include <regex>
using namespace std;

string replaceFirstOccurrence(string& s, const string& toReplace, const string& replaceWith)
{
    size_t pos = s.find(toReplace);
    if (pos == string::npos) return s;
    return s.replace(pos, toReplace.length(), replaceWith);
}

string getDomain(string url){
    // Removing http://, https:// and htts://
    regex exp1("https*://");
    regex exp2("htts://");
    url = regex_replace(url, exp1, "");
    url = regex_replace(url,exp2,"");
    
    // Removing anything after first /
    size_t a = url.find_first_of("/");
    if(a != string::npos)url.erase(a);
    
    
    a = url.find_first_of("#");
    if(a != string::npos)url.erase(a);
    
    a = url.find_first_of("?");
    if(a != string::npos)url.erase(a);
    
    // url = replaceFirstOccurrence(url, "www.", "");
    
    // consider domain from the first identified '.' in the link
    a = url.find('.');
    if(a != string::npos){
        url = url.substr(a+1, url.length()-1);
    }  
    
    return url;
}