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
  // Crawler(depthLimit, maxLinks, pagesLimit)
  Crawler myCrawler(stoi(argv[1]), stoi(argv[2]), stoi(argv[3]));

  myCrawler.initialize();
  myCrawler.runCrawler();
  myCrawler.showResults();

  return 0;
}