#include "Crawler.h"

#include <netdb.h>
#include <unistd.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"

#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock

void Crawler::initialize()
{
  log.open("logs.txt");
  log << "Crawler initialized" << endl;

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

// string Crawler::downloader(string url)
// {
//   string html;
//   // check if the downloaded website is http, then use appropriate HTML downloader
  
//   html = httpsDownloader(url);
//   return html;
// }
std::pair<string, double> Crawler::downloader(string url)
{
    // Clock Start
    high_resolution_clock::time_point startTime = high_resolution_clock::now();
    
    string html;
    // check if the downloaded website is http, then use appropriate HTML downloader
    double totaldTime = 0;
    
    html = httpsDownloader(url);

    // Clock End
    high_resolution_clock::time_point endTime = high_resolution_clock::now();
    totaldTime = duration<double, milli>(endTime - startTime).count();

    // return html, totaldTime;
    return std::make_pair(html, totaldTime);
}

void Crawler::gotosleep(){
	cout << "going to sleep now" << endl;

	lock(&parent_lock);             
	// while(done==0){
		pthread_cond_wait(&parent_cond, &parent_lock);
	// }            
	unlock(&parent_lock);
	
	cout << "awaken now." << endl; 
}

void Crawler::createThread(){
	lock(&mainLock);
		string currentSite = linkQueue.front();
		linkQueue.pop();
		totalVisitedPages++;
	unlock(&mainLock);

	lock(&wT_lock);
		workingThreads++;
	unlock(&wT_lock);

  cout << "creating a thread, totalworkingthreads now: " << workingThreads << endl;
  pthread_t th;
  char* ch = (char*)malloc(1+currentSite.size()*sizeof(char));
  strcpy(ch, currentSite.c_str());
  

  int ret_val = pthread_create(
		&th, NULL, childThread, (void*)ch);
}

void Crawler::runCrawler()
{
  
  while (1)
  {
    int w_threads;
    int queue_size;
    int visitedpages;

    // locking and getting the values of working thread, size of queue and total visited pages
		lock(&wT_lock);
    lock(&mainLock);

    w_threads = workingThreads;
    queue_size = linkQueue.size();
    visitedpages = totalVisitedPages;
    
    unlock(&mainLock);
    unlock(&wT_lock);


		if(pagesLimitReached || visitedpages>=pagesLimit){
			// pagesLimit reached
			pagesLimitReached = true;
      
			if(w_threads){
        // cout << "going to sleep now pageslimit maxed and w_threads==0" << endl;
        
				gotosleep();
			}
			else {
				// exit
        cout << "EXITING AS ALL THREADS ARE COMPLETED & pagelimit reached." << endl;
        break;
			}
		}
		else{
			// pagesLimit not reached
			if (w_threads < maxThreads && queue_size>0){
        // create thread
        // cout << "w_threads < maxThreads && queue_size>0 <<" << endl;
				createThread();
      }
			else if(w_threads == 0){
        // break
        cout << "EXITING AS NO THREADS WORKING. & pagelimit not reached"<<endl;
				break;
			}
			else{
        // sleep
        // cout << "else" << endl;
				gotosleep();
			}
		}

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



void *childThread(void *_url)
{
  string url((char*)_url);

    // // Clock Start
    // high_resolution_clock::time_point startTime = high_resolution_clock::now();

  // downloading the file
  std::pair<string, double> p = myCrawler.downloader(url);
//   string html = myCrawler.downloader(url);
    string html = p.first;
    double totaldTime = p.second;

  ofstream log("./thread_logs/"+to_string(rand())+".log");

  log << "file downloaded." << endl;
  cout << "file downloaded." << endl;
  cout << "total time taken to download = " << totaldTime << endl;

  // extracting all the links from it
//   set<string> linkedSites = getLinks(html, myCrawler.maxLinks);
  std::pair<set<string>, double> q = getLinks(html, myCrawler.maxLinks);
  set<string> linkedSites = q.first;
  double totalpTime = q.second;

  log << "links extracted." << endl;
  cout << "links extracted." << endl;
  cout << "total time taken to parse = " << totalpTime << endl;
	

  // updating the shared variables
  lock(&myCrawler.mainLock);
  for (auto i : linkedSites)
  {
    log << i << endl;
    myCrawler.ranker[getDomain(i)]++;
    if (!myCrawler.discoveredSites[i])
    {
      myCrawler.discoveredSites[i] = true;
      myCrawler.linkQueue.push(i);
    }
  }
  
  unlock(&myCrawler.mainLock);

  // cout << "shared variables updated." << endl;
  // log << "shared variables updated." << endl;
  // unlock()


  lock(&myCrawler.wT_lock);
  myCrawler.workingThreads--;
  unlock(&myCrawler.wT_lock);
  

  
	if(myCrawler.pagesLimitReached){

		lock(&myCrawler.wT_lock);
		if(myCrawler.workingThreads == 0){
			unlock(&myCrawler.wT_lock);

			// Awake the parent
			lock(&myCrawler.parent_lock);
				
			// myCrawler.done = true;
			// cout << "cond signal" << myCrawler.done << endl;
			pthread_cond_signal(&myCrawler.parent_cond);
			
			unlock(&myCrawler.parent_lock);
		}
		unlock(&myCrawler.wT_lock);

	}
	else {

		lock(&myCrawler.parent_lock);
		// myCrawler.done = true;
		// cout << "cond signal" << myCrawler.done << endl;
		pthread_cond_signal(&myCrawler.parent_cond);
		
		unlock(&myCrawler.parent_lock);
	}

  log.close();
  pthread_exit(NULL);
}
