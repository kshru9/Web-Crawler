#include <iostream>
#include <chrono>
#include "Crawler.h"
#include "Crawler.cpp"
// gives a Crawler object named `myCrawler` from default

using namespace std;

int main(int argc, const char *argv[])
{
  // Crawler maxLinks pagesLimit no_of_threads
  myCrawler.maxLinks = stoi(argv[1]);
  myCrawler.pagesLimit = stoi(argv[2]);
	myCrawler.maxThreads = stoi(argv[3]);

  
	auto start = chrono::steady_clock::now();

	myCrawler.initialize();
  myCrawler.runCrawler();
  myCrawler.showResults();
	cout << "FINISHED." << endl;

	auto end = chrono::steady_clock::now();

	cout<< "Elapsed time in milliseconds : "
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;

  return 0;
}