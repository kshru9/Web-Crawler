#ifndef ___Crawler
#define ___Crawler


#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <sys/stat.h>
#include <pthread.h>
using namespace std;

class Crawler
{
  // setting public for testing purposes
public:
  ofstream log;  // logging
  ofstream lout; // links dumping

  // Parameters declaration
  int maxLinks;
  int pagesLimit;

  // queue for storing linked websites
  queue<string> linkQueue;

  // map for storing visited websites
  map<string, bool> discoveredSites;
  // map for a simple website ranker
  map<string, int> ranker;

  // for storing total processed pages till now
  int totalVisitedPages = 0;

  // for storing total downloaded pages
  int totalDownloadPages = 0;

  // Constructor
  Crawler()
  {
    
  }

  // Destructor
  ~Crawler()
  {
    log.close();
    lout.close();
  }

  // Public functions

  // Initialize the Crawler.
  void initialize();
  // Downloads a website and save it in buffer folder
  string downloader(string url);
  // Parse a file from the buffer and update parameters{concurrency part}
  void parseFile(string filename);
  // Start a crawler to discover a specific website.
  void runCrawler();
  // Show the results of the crawling
  void showResults();
} myCrawler;

// child thread for downloading and parsing
void *childThread(void* url);

#endif