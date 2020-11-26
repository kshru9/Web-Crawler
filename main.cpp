#include <iostream>
#include "Crawler.cpp"
// gives a Crawler object named `myCrawler` from default

using namespace std;

int main(int argc, const char *argv[])
{
	srand(time(0));

  // Crawler maxLinks pagesLimit threads
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
			<< chrono::duration_cast<chrono::seconds>(end - start).count()
			<< " ms" << endl;


  return 0;
}
