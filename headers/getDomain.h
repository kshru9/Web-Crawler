#include <iostream>
#include <regex>
using namespace std;

string replaceFirstOccurrence(string &s, const string &toReplace, const string &replaceWith)
{
	size_t pos = s.find(toReplace);
	if (pos == string::npos)
		return s;
	return s.replace(pos, toReplace.length(), replaceWith);
}

string getDomain(string url)
{
	// Removing http://, https:// and htts://
	regex exp1("https*://");
	regex exp2("htts://");
	url = regex_replace(url, exp1, "");
	url = regex_replace(url, exp2, "");

	// Removing anything after first /
	size_t t = url.find_first_of("/");
	if (t != string::npos)
		url.erase(t);

	t = url.find_first_of("#");
	if (t != string::npos)
		url.erase(t);

	t = url.find_first_of("?");
	if (t != string::npos)
		url.erase(t);

	if (url.size() > 3)
		if (url.substr(0, 4) == "www.")
		{
			url = replaceFirstOccurrence(url, "www.", "");
		}
	return url;
}