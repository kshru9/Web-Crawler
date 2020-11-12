#ifndef ___Crawler
#define ___Crawler

#include <map>
#include <queue>
#include <iostream>
#include <string>
using namespace std;

void sort(map<string, int>& M);

class Crawler
{
  // setting public for testing purposes
public:
	ofstream log;

  int maxLinks;
	int pagesLimit;
	// int depthLimit;
  string intialLink = "https://www.google.com";

  queue<string> mainQueue;
  map<string, bool> discoveredSites;
  map<string, int> ranker;
  int totalVisitedPages = 0;

	// Constructor
  Crawler(int mL, int pL)
  {
		// this->depthLimit = dL;
    this->maxLinks = mL;
		this->pagesLimit = pL;
  }

	// Destructor
	~Crawler()
	{
		log.close();
	}

	// Public functions
  void initialize();
  void runCrawler();
  void showResults();
};

#endif
