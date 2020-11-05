#include <map>
#include <queue>
#include <iostream>
#include <string>
using namespace std;

#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"

void sort(map<string, int>&);
bool ishttp(string);

class Crawler
{
  // setting public for testing purposes
public:
	ofstream log;


	//int depthLimit;
  // Parameters declaration
  int maxLinks;
	int pagesLimit;
  string intialLink = "https://www.google.com";

  // queue for storing linked websites
  queue<string> mainQueue;
  // map for storing visited websites
  map<string, bool> discoveredSites;
  // map for a simple website ranker 
  map<string, int> ranker;

  // for storing html pages downloaded till now
  int totalVisitedPages = 0;

	// Constructor
  Crawler(int mL, int pL)
  {
		//this->depthLimit = dL;
    this->maxLinks = mL;
		this->pagesLimit = pL;
  }

	// Destructor
	~Crawler()
	{
		log.close();
	}

	// Public functions
  void initialize();
  void runCrawler();
  void showResults();
};


// Initialize the Crawler.
void Crawler::initialize()
{
	log.open("logs.txt");
	log << "Crawler initialized" << endl;

  // Add initial urls
  mainQueue.push(intialLink);
}



// Start a crawler to discover a specific website.
void Crawler::runCrawler()
{
  log << "Crawler initialised." << endl;

  ofstream lout("links.txt");

  // Only discover more if haven't reached the depthLimit
  while (
		!mainQueue.empty() && 
		// mainQueue.size() < depthLimit && 
		totalVisitedPages<pagesLimit
	){

    // pop the front website in queue
		string currentSite = mainQueue.front();
    mainQueue.pop();

		log << "Processing: " << currentSite << endl;

    // Download the website html
		log << "Calling Downloader." << endl;

		string html;

    // check if the downloaded website is http 
    // then use appropriate HTML downloader
		if(ishttp(currentSite))
		{
			html = httpDownloader(currentSite);
		} else {
			html = httpsDownloader(currentSite);
		}

		log << "File Downloaded." << endl;

		system("clear");
    cout << "Size of mainQueue:" << mainQueue.size() << endl;
    cout << "Link no :" << totalVisitedPages+1 << endl;
		cout << "HTML file length: " << html.size() << endl;

		// Get urls from the getLinks()
		log << "getLinks() called." << endl;
    set<string> linkedSites = getLinks(html, maxLinks);
    log << "Links returned: " << linkedSites.size() << endl;


		// Pushing links into mainQueue if they are unvisited
    for (auto i : linkedSites)
    {
			lout << i << endl;
			ranker[getDomain(i)]++;
      if (!discoveredSites[i])
      {
				discoveredSites[i] = true;
        mainQueue.push(i);
      }
    }
		totalVisitedPages++;
		
		log << "Link no. " << totalVisitedPages << " processed." << endl << endl;
		// end of crawler loop
  }

  log << "Crawling completed." << endl << endl;
  lout.close();
}


void Crawler::showResults(){
  system("clear");

  cout << "-----------------------------------------------------" << endl;
  cout << "Parameters:" << endl;
  cout << "-----------------------------------------------------" << endl;
  cout << "Max Links extracted from a website:" << "\t" << maxLinks << endl;
  cout << "Max pages downloaded:" << "\t" << pagesLimit << endl;

  cout << "" << endl;
  
  cout << "-----------------------------------------------------" << endl;
  cout << "Web rankings" << "\t" << "(" << "Total Visited Websites:" << "\t" << totalVisitedPages << ")" << endl;
  cout << "-----------------------------------------------------" << endl;
  
  sort(ranker);
}

void sort(map<string, int>& M) 
{
	// Here if greater<int> is used to make 
	// sure that elements are stored in 
	// descending order of keys. 
	multimap<int, string, greater <int> > MM; 

	for (auto& it : M){
		MM.insert(make_pair(it.second, it.first));
	}

	int r = 1;
  cout << "Rank" << "\t" << "Domain Name" << endl;
  cout << "" << endl;
	for(auto i: MM){
		cout << r++ << "\t\t" << i.second << " : " << i.first << endl;
	}
} 

bool ishttp(string website)
{
  return website.compare(0, 8, "https://");
}
