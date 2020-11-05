#include <map>
#include <queue>
#include <iostream>
#include <string>
#include <sys/stat.h> 
#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"
using namespace std;

bool ishttp(string);

class Crawler
{
  // setting public for testing purposes
public:
  ofstream log; // logging
  ofstream lout; // links dumping

  // Parameters declaration
  int maxLinks;
  int pagesLimit;
  string intialLink = "https://www.google.com";

  // queue for storing linked websites
  queue<string> linkQueue;
  // queue for storing downloaded filenames
  queue<string> fileQueue;

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
  void initialize();
  void fileDownloader(string url);
  void parseFile(string filename);
  void runCrawler();
  void showResults();
};

// Initialize the Crawler.
void Crawler::initialize()
{
  log.open("logs.txt");
  log << "Crawler initialized" << endl;
  
  // for dumping links 
  lout.open("links.txt");

  // Add initial urls
  linkQueue.push(intialLink);

  // Deleting old instance of buffer directory
  system("rm -d buffer");

  // Creating the buffer directory
  mkdir("buffer", 07770);
}

// Downloads a website and save it in buffer folder 
void Crawler::fileDownloader(string url){
  string html;
  // check if the downloaded website is http
  // then use appropriate HTML downloader
  if (ishttp(url))
  {
    html = httpDownloader(url);
  }
  else
  {
    html = httpsDownloader(url);
  }
  log << "File Downloaded." << endl;

  // !!!Attendion: try acquiring lock for totalDownloadPages
  string filename = "./buffer/htmlFile_" + to_string(totalDownloadPages++) + ".html";
  ofstream fout(filename);
  fout << html << endl;
  fout.close();

  // !!!Attention: Acquire lock
  fileQueue.push(filename);
}

// Parse a file from the buffer and update parameters
void Crawler::parseFile(string filename){
  ifstream f(filename); //taking file as inputstream
  string html;
  ostringstream ss;
  ss << f.rdbuf();
  html = ss.str();
  f.close();

  // Deleting the file using system command
  string com = "rm ./buffer/"+filename;
  system(com.c_str());
  


  // Get urls from the getLinks()
  log << "getLinks() called." << endl;
  set<string> linkedSites = getLinks(html, maxLinks);
  log << "Links returned: " << linkedSites.size() << endl;

  // Pushing links into linkQueue if they are unvisited
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
}

// Start a crawler to discover a specific website.
void Crawler::runCrawler()
{
  log << "Crawler initialised." << endl;

  while (!linkQueue.empty() && totalVisitedPages < pagesLimit)
  {
    // Decide first that if we need to 
    // 1. Download a file
    // 2. Parse a file
    // Appropriately create a thread and call the producer and consumer functions
    // For downloading purpose, we can use the parent thread temporarily
    // Or the initialize can just download 10-20 websites serially{as told by prakash}
    // MAJOR WORK is remaining here.


    // Ordering the download for the website
    string currentSite = linkQueue.front();
    linkQueue.pop();
    fileDownloader(currentSite);

    // Calling the parser thing
    string filename = fileQueue.front();
    fileQueue.pop();
    parseFile(filename);


    // end of crawler loop
  }

  log << "Crawling completed." << endl << endl;
}

void Crawler::showResults()
{
  system("clear");

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

bool ishttp(string website)
{
  return website.compare(0, 8, "https://");
}
