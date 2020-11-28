#ifndef ___Crawler
#define ___Crawler

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <chrono>
#include <thread>
#include <condition_variable>

#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>

#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"
#include "thread_safe/int_ts.cpp"
#include "thread_safe/map_ts.cpp"
#include "thread_safe/queue_ts.cpp"

#define _now high_resolution_clock::now()

using namespace std;
using namespace std::chrono;

#define RED "\033[31m"	 /* Red */
#define CYAN "\033[36m"	 /* Cyan */
#define GREEN "\033[32m" /* Green */
#define BLUE "\033[34m"	 /* Blue */
#define C_END "\033[0m"

class Crawler
{
	// setting public for testing purposes
public:
	ofstream log; // logging

	int_ts workingThreads; // total no of threads working
	int_ts pagesLimitReached;
	// for storing total processed pages till now
	int_ts totalVisitedPages;

	mutex timingLock;
	vector<vector<double>> threadTimings;

	// lock and cond_var for parent
	bool ready = false;
	condition_variable cv;
	mutex cv_m;

	// Parameters
	int maxLinks = 1000;
	int pagesLimit = 100;
	int maxThreads = 10;

	// MAKE THESE THREAD SAFE
	// queue for storing linked websites
	queue_ts linkQueue;
	// map for storing visited websites
	map_ts<string, bool> discoveredSites;
	// for storing page relations
	map_ts<string, set<string>> pageRank;
	

	// Constructor
	Crawler()
	{
		totalVisitedPages.assign(0);
		workingThreads.assign(0);
		pagesLimitReached.assign(0);
	}

	// Destructor
	~Crawler()
	{
		log << "current queue size: " << linkQueue.size() << endl;
		log.close();
	}

	// Public Functions

	/*
		Initialize the Crawler.
	*/
	void initialize();

	/*
		Downloads a website and save it in buffer folder
	*/
	string downloader(string url);

	/*
		Parse a file from the buffer and update parameters{concurrency part}
	*/
	void parseFile(string filename);

	/*
		Start a crawler to discover a specific website.
	*/
	void runCrawler();

	/*
		Show the results of the crawling
	*/
	void showResults();

	/*
		Create a single thread
	*/
	void createThread();

	/*
		Sleeping the main thread
	*/
	void gotosleep();

	/*
		Awake the main thread
	*/
	void awake();

} myCrawler;

/*
	Child thread for downloading + parsing + updating shared variables
*/
void childThread(string url, int th_no);

#endif