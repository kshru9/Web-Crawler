#include <map>
#include <queue>
#include <iostream>
using namespace std;

#include "headers/httpsDownloader.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"

class Crawler
{
  // setting public for testing purposes
public:
  int depthLimit = 0;
  int maxLinks = 0;
	int pagesLimit = 0;

  queue<string> mainQueue;
  map<string, bool> discoveredSites;

	// Constructor
  Crawler(int dL, int mL, int pL)
  {
    this->depthLimit = dL;
    this->maxLinks = mL;
		this->pagesLimit = pL;
  }

	// Public functions
  void initialize();
  void runCrawler();
  void showResults();
};


// Initialize the Crawler.
void Crawler::initialize()
{
  // Add initial urls
  mainQueue.push("https://www.linkedin.com/");
}

// Start a crawler to discover a specific website.
void Crawler::runCrawler()
{
	int totalVisitedPages = 0;
  cout << "Crawler initialised." << endl;
  ofstream lout("links.txt");

  // Only discover more if haven't reached the depthLimit
  while (
		!mainQueue.empty() && 
		mainQueue.size() < depthLimit && totalVisitedPages<=pagesLimit
	){
    cout << "Current depth:" << mainQueue.size() << endl; //$
		system("clear");
    string currentSite = mainQueue.front();
    mainQueue.pop();

    discoveredSites[currentSite] = true;

    // TODO: downloader
    set<string> linkedSites;

    // Get urls from parser
    linkedSites = getLinks(httpsDownloader((currentSite)), maxLinks);

    cout << "Links returned: " << linkedSites.size() << endl;

    for (auto i : linkedSites)
    {
      if (!discoveredSites[i])
      {
        mainQueue.push(i);
        lout << i << endl;
      }
    }
		totalVisitedPages++;
  }

  cout << "Crawling completed." << endl;
  lout.close();
}

void Crawler::showResults(){
	
}