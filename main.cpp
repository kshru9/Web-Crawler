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
#include "Crawler.cpp"
// gives a myCrawler object from default

using namespace std;

int main(int argc, const char *argv[])
{
  // Crawler(maxLinks, pagesLimit)
  myCrawler.maxLinks = stoi(argv[1]);
  myCrawler.pagesLimit = stoi(argv[2]);

  myCrawler.initialize();
  myCrawler.runCrawler();
  myCrawler.showResults();

  return 0;
}