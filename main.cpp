#include <iostream>
#include "Crawler.h"
#include "Crawler.cpp"
// gives a Crawler object named `myCrawler` from default

using namespace std;
using namespace std::chrono;

int main(int argc, const char *argv[])
{
	srand(time(0));

  // Crawler maxLinks pagesLimit threads
  myCrawler.maxLinks = stoi(argv[1]);
  myCrawler.pagesLimit = stoi(argv[2]);
  myCrawler.maxThreads = stoi(argv[3]);

  
	auto t1 = chrono::steady_clock::now();
  myCrawler.initialize();
  myCrawler.runCrawler();
  myCrawler.showResults();
	auto t2 = chrono::steady_clock::now();


	int res = duration_cast<milliseconds>(t2 - t1).count();

	cout << "FINISHED." << endl;
	cout << "Elapsed time in milliseconds : "
			<< res << endl;

	ofstream fout("OUTPUT/crawler_timings.csv", std::ios_base::app);
	fout << myCrawler.maxThreads << ", " << res << endl;
	fout.close();

  return 0;
}
