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
#include <unistd.h>

#include "Crawler.h"

using namespace std;
using namespace std::regex_constants;

int main(int argc, const char *argv[])
{

  // Crawler( maxLinks, pagesLimit)
  //Crawler myCrawler(10, 5, "https://www.google.com/");
  Crawler myCrawler(stoi(argv[1]), stoi(argv[2]));

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