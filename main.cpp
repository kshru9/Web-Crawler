#include <iostream>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <regex>
#include <queue>
#include <map>
#include <set>
#include <unistd.h>

#include "Crawler.h"

using namespace std;

int main(int argc, const char *argv[])
{
    auto start = chrono::steady_clock::now();
    // Crawler(maxLinks, pagesLimit)
    Crawler myCrawler(stoi(argv[1]), stoi(argv[2]));

    myCrawler.initialize();
    myCrawler.runCrawler();
    myCrawler.showResults();

	auto end = chrono::steady_clock::now();

	cout<< "Elapsed time in milliseconds : "
			<< chrono::duration_cast<chrono::milliseconds>(end - start).count()
			<< " ms" << endl;
  

  return 0;
}
