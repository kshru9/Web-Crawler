#include <map>
#include <queue>
#include <iostream>
#include <bits/stdc++.h>
#include <string>
using namespace std;


#include "headers/httpsDownloader.h"
#include "headers/httpDownloader.h"
//
#include "headers/getLinks.h"
#include "headers/getDomain.h"

void sort(map<string, int>& M);

class Crawler
{
  // setting public for testing purposes
public:
  int maxLinks = 0;
	int pagesLimit = 0;
  string intialLink = "https://www.google.com/";

  queue<string> mainQueue;
  map<string, bool> discoveredSites;
  map<string, int> ranker;
  int totalVisitedPages = 0;

	// Constructor
  Crawler(int mL, int pL)
  {
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
  mainQueue.push(intialLink);
}

// Start a crawler to discover a specific website.
void Crawler::runCrawler()
{
	
  cout << "Crawler initialised." << endl;
  ofstream lout("links.txt");

  while (
		!mainQueue.empty() && 
		totalVisitedPages<=pagesLimit
	){
    cout << "Current depth:" << mainQueue.size() << endl; //$
		system("clear");
    string currentSite = mainQueue.front();
    mainQueue.pop();

    discoveredSites[currentSite] = true;

    // Get urls from parser
    set<string> linkedSites = getLinks(httpsDownloader((currentSite)), maxLinks);

    cout << "Links returned: " << linkedSites.size() << endl;

    for (auto i : linkedSites)
    {
      ranker[getDomain(i)] +=1;
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
  // system("clear");

  cout << "---------------------------------------------" << endl;
  cout << "Parameters:" << endl;
  cout << "---------------------------------------------" << endl;
  cout << "Max Links extracted from a website:" << maxLinks << endl;
  cout << "Max pages downloaded:" << pagesLimit << endl;

  cout << "" << endl;
  
  cout << "---------------------------------------------" << endl;
  cout << "Web rankings" << "(" << "Total Visited Websites:" << "\t" << totalVisitedPages-1 << ")" << endl;
  cout << "---------------------------------------------" << endl;
  
  sort(ranker);
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