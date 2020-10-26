#include <map>
#include <queue>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

#include "headers/httpsDownloader.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"

void sort(map<string, int>& M);

class Crawler
{
  // setting public for testing purposes
public:
  int depthLimit = 0;
  int maxLinks = 0;
	int pagesLimit = 0;

  queue<string> mainQueue;
  map<string, bool> discoveredSites;
  map<string, int> ranker;

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

    ranker[getDomain(currentSite)] +=1;

    // TODO: downloader

    // Get urls from parser
    set<string> linkedSites = getLinks(httpsDownloader((currentSite)), maxLinks);

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
  system("clear");
  cout << "Web rankings" << endl;

  sort(ranker);
  // for (auto i: ranker){
  //   cout << i.first << "\t" << i.second << endl;
  // }
}

void sort(map<string, int>& M) 
{
    // Here if greater<int> is used to make 
    // sure that elements are stored in 
    // descending order of keys. 
    multimap<int, string, greater <int> > MM; 

    for (auto& it : M){
      MM.insert(make_pair(it.second, it.first));
    }

    // begin() returns to the first value of multimap. 
    multimap<int,string> :: iterator it; 
    for (it=MM.begin() ; it!=MM.end() ; it++) 
        cout << (*it).second << "\t" << (*it).first << endl; 
} 