#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <algorithm>
#include <functional>

using namespace std;
using namespace std::regex_constants;

string _replaceFirstOccurrence(string& s, const string& toReplace, const string& replaceWith)
{
	size_t pos = s.find(toReplace);
	if (pos == string::npos) return s;
	return s.replace(pos, toReplace.length(), replaceWith);
}

set<string> getLinks(string html, int maxLinks);

set<string> getLinks(string html, int maxLinks)
{
	cout << "getLinks() called." << endl;

  // Extracting all the links from the html file
  const regex url_re(R"!!(<\s*A\s+[^>]*href\s*=\s*"([^"]*)")!!", icase);

  set<string> res_1 = {
		sregex_token_iterator(html.begin(), html.end(), url_re, 1),
		sregex_token_iterator{}};

  regex exp(".*\\..*");

  set<string> links;


  for (string i : res_1)
  {
    // A string starting from "https://" is considered as a link
    // A link should not contain \<>{}
		string a = i;

		size_t t = a.find_first_of("?");
    if(t != string::npos)a.erase(t);
    
		t = a.find_first_of("#");
    if(t != string::npos)a.erase(t);
    
		t = a.find_first_of(";");
    if(t != string::npos)a.erase(t);
    

    if(
        regex_match(a, exp) &&
        a.size() > 7 && // so that next one doesn't fails
        a.substr(0, 8) == "https://" &&
        a.find('\\') == string::npos &&
        a.find('<') == string::npos &&
        a.find('>') == string::npos &&
        a.find('{') == string::npos &&
        a.find('}') == string::npos &&
        a.find(' ') == string::npos
		){

      links.insert(a);
      if (links.size() > maxLinks)
      {
        break;
      }
    }
  }

	cout << "getLinks() returned." << endl;
  return links;
}