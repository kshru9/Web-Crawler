
#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"
#include "Crawler.h"


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
		string currentSite = mainQueue.front();
    mainQueue.pop();

		log << "Processing: " << currentSite << endl;

    // Download the website html
		log << "Calling Downloader." << endl;
		string html = httpsDownloader(currentSite);
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

void Crawler::showResults()
{
	system("clear");
	cout << "-----------------------------------------------------" << endl;
	cout << "Parameters:" << endl;
	cout << "-----------------------------------------------------" << endl;
	cout << "Max Links from a website:"
			 << "\t" << maxLinks << endl;
	cout << "Max pages downloaded:"
			 << "\t" << pagesLimit << endl;
	// cout << "Max threads working:"
	// 		 << "\t" << maxThreads << endl;
	cout << "Total visited pages:"
			 << "\t" << totalVisitedPages << endl;

	cout << "" << endl;

	cout << "-----------------------------------------------------" << endl;
	cout << "Web rankings" << endl;
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

	cout << "-----------------------------------------------------" << endl;
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

    // begin() returns to the first value of multimap. 
    multimap<int,string> :: iterator it; 
    for (it=MM.begin() ; it!=MM.end() ; it++) 
        cout << (*it).second << " : " << (*it).first << endl; 
} 
