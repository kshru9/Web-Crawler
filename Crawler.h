#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <sys/stat.h>

#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"
using namespace std;

class Crawler
{
  // setting public for testing purposes
public:
  ofstream log;  // logging
  ofstream lout; // links dumping

  // Parameters declaration
  int maxLinks;
  int pagesLimit;

  // queue for storing linked websites
  queue<string> linkQueue;

  // map for storing visited websites
  map<string, bool> discoveredSites;
  // map for a simple website ranker
  map<string, int> ranker;

  // for storing total processed pages till now
  int totalVisitedPages = 0;

  // for storing total downloaded pages
  int totalDownloadPages = 0;

  // Constructor
  Crawler(int mL, int pL)
  {
    this->maxLinks = mL;
    this->pagesLimit = pL;
  }

  // Destructor
  ~Crawler()
  {
    log.close();
    lout.close();
  }

  // Public functions

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

  void childThread(string url);
};

void Crawler::initialize()
{
  log.open("logs.txt");
  log << "Crawler initialized" << endl;

  // for dumping links
  lout.open("links.txt");

  // Add initial urls from initialLinks.txt
  ifstream lin("initialLinks.txt");
  if (lin)
  {
    int n;
    lin >> n;
    string a;
    for (int i = 0; i < n; i++)
    {
      lin >> a;
      if (a != "")
      {
        linkQueue.push(a);
      }
      else
      {
        break;
      }
    }
  }
  else
  {
    cout << "Error reading file: \"initialLinks.txt\"" << endl;
    linkQueue.push("https://www.google.com");
  }
}

string Crawler::downloader(string url)
{
  string html;
  // check if the downloaded website is http, then use appropriate HTML downloader
  if (url.substr(0, 8) == string("https://"))
  {
    html = httpsDownloader(url);
  }
  else
  {
    html = httpDownloader(url);
  }

  return html;
}

void Crawler::childThread(string url)
{
  // downloading the file
  string html = downloader(url);
  log << "file downloaded." << endl;
  cout << "file downloaded." << endl;

  // extracting all the links from it
  set<string> linkedSites = getLinks(html, maxLinks);
  log << "links extracted." << endl;
  cout << "links extracted." << endl;


  // updating the shared variables
  // lock(linkedQueue, linkedSites, discoveredSites, totalVisitedPages)
  for (auto i : linkedSites)
  {
    lout << i << endl;
    ranker[getDomain(i)]++;
    if (!discoveredSites[i])
    {
      discoveredSites[i] = true;
      linkQueue.push(i);
    }
  }
  totalVisitedPages++;
  cout << "shared variables updated." << endl;
  log << "shared variables updated." << endl;
  // unlock()
}

void Crawler::runCrawler()
{
  while (!linkQueue.empty() && totalVisitedPages < pagesLimit)
  {
    // Creating a thread
    // lock(linkQueue)
    string currentSite = linkQueue.front();
    linkQueue.pop();
    // unlock()


    childThread(currentSite);




    // end of crawler loop
  }

  log << "Crawling completed." << endl
      << endl;
}

void Crawler::showResults()
{
  // system("clear");

  cout << "-----------------------------------------------------" << endl;
  cout << "Parameters:" << endl;
  cout << "-----------------------------------------------------" << endl;
  cout << "Max Links extracted from a website:"
       << "\t" << maxLinks << endl;
  cout << "Max pages downloaded:"
       << "\t" << pagesLimit << endl;

  cout << "" << endl;

  cout << "-----------------------------------------------------" << endl;
  cout << "Web rankings"
       << "\t"
       << "("
       << "Total Visited Websites:"
       << "\t" << totalVisitedPages << ")" << endl;
  cout << "-----------------------------------------------------" << endl;

  map<int, string, greater<int>> mm;

  for (auto &it : ranker)
  {
    mm[it.second] = it.first;
  }

  int r = 1;
  cout << "Rank"
       << "\t"
       << "Domain Name" << endl;
  cout << "" << endl;
  for (auto i : mm)
  {
    cout << r++ << "\t\t" << i.second << " : " << i.first << endl;
  }
}
