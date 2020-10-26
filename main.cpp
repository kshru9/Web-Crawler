// g++ main.cpp -lssl && ./a.out
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

#include "Crawler.h"

using namespace std;
using namespace std::regex_constants;

int main(int argc, const char *argv[])
{
  // Crawler(depthLimit, maxLinks, pagesLimit)
  Crawler myCrawler(1000, 10, 5);

  //config = readConfigFile();

  myCrawler.initialize();
  myCrawler.runCrawler();

  myCrawler.showResults();

  return 0;
}

/*
bool ishttp(string website)
{
  return website.compare(0, 8, "https://");
}
*/