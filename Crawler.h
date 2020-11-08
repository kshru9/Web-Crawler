#ifndef ___Crawler
#define ___Crawler


#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h> 
using namespace std;

class Crawler
{
  // setting public for testing purposes
public:
  ofstream log;  // logging

  int workingThreads = 0; // total no of threads working
  int maxThreads = 3; // max threads allowed
  bool pagesLimitReached = false;
  // bool done = false;


  pthread_mutex_t wT_lock; // lock for workingThreads varialbe

  pthread_mutex_t mainLock; // main lock covering all other varialbles such as linkQueue, discoveredSites, ranker, totalVisitedPages


	// lock and cond_var for sleeping of parent thread and awaking by child thread
  pthread_cond_t parent_cond;
  pthread_mutex_t parent_lock;


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

  // Constructor
  Crawler()
  {
    pthread_mutex_init(&wT_lock, NULL);
    pthread_mutex_init(&mainLock, NULL);
    pthread_mutex_init(&parent_lock, NULL);
    pthread_cond_init(&parent_cond, NULL);
  }

  // Destructor
  ~Crawler()
  {
    log.close();
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
  // for creating a single thread
  void createThread();
	// sleeping the main thread
	void gotosleep();

} myCrawler;

// child thread for downloading and parsing
void *childThread(void* _url);

#endif