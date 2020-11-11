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
#include <chrono>

using namespace std;
using namespace std::chrono;

class Crawler
{
	// setting public for testing purposes
public:
	ofstream log; // logging

	int workingThreads = 0; // total no of threads working
	bool pagesLimitReached = false;
	// bool done = false;

	pthread_mutex_t timingLock;
	vector<vector<double>> threadTimings;

	// mainLock for all shared variables
	pthread_mutex_t mainLock;

	// lock and cond_var for parent
	pthread_cond_t parent_cond;
	pthread_mutex_t parent_lock;

	// Parameters
	int maxLinks = 1000;
	int pagesLimit = 100;
	int maxThreads = 10;

	// queue for storing linked websites
	queue<string> linkQueue;

	// map for storing visited websites
	map<string, bool> discoveredSites;

	// map for a simple website ranker
	map<string, int> webRanking;

	// for storing total processed pages till now
	int totalVisitedPages = 0;

	// Constructor
	Crawler()
	{
		pthread_mutex_init(&timingLock, NULL);
		pthread_mutex_init(&mainLock, NULL);

		pthread_mutex_init(&parent_lock, NULL);
		pthread_cond_init(&parent_cond, NULL);
	}

	// Destructor
	~Crawler()
	{
		log << "current queue size: " << linkQueue.size() << endl;

		log.close();
		ofstream tout("th_timings.csv");
		for(auto i: threadTimings){
			tout << i[0] << ',' << i[1] << ',' << i[2] << endl;
		}
	}

	// Public Functions

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
	// C a single thread
	void createThread();
	// Sleeping the main thread
	void gotosleep();

} myCrawler;

// child thread for downloading and parsing
void *childThread(void *_url);

#endif