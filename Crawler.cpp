#include "Crawler.h"
#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"

#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock
#define _now high_resolution_clock::now()

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
		// Unable to read starting links from input file
		cout << "Error reading file: \"initialLinks.txt\"" << endl;
		linkQueue.push("https://www.google.com");
	}
}

string Crawler::downloader(string url)
{
	// check if the downloaded website is http,
	// then use appropriate HTML downloader
	string html;
	if (url.substr(0, 5) == "https")
	{
		html = httpsDownloader(url);
	}
	else
	{
		html = httpDownloader(url);
	}

	return html;
}

void Crawler::gotosleep()
{
	cout << "Going to sleep now" << endl;

	lock(&parent_lock);
	// while(done==0){
	pthread_cond_wait(&parent_cond, &parent_lock);
	// }
	unlock(&parent_lock);

	cout << "Awaken now." << endl;
}

void Crawler::createThread()
{
	lock(&mainLock);
	string currentSite = linkQueue.front();
	linkQueue.pop();
	totalVisitedPages++;
	workingThreads++;
	unlock(&mainLock);

	cout << "Creating a thread, total = " << workingThreads << endl;
	pthread_t th;
	char *ch = (char *)malloc(1 + currentSite.size() * sizeof(char));
	strcpy(ch, currentSite.c_str());

	int ret_val = pthread_create(
			&th, NULL, childThread, (void *)ch);
}

void Crawler::runCrawler()
{
	while (1)
	{
		lock(&mainLock);
		int _workingThreads = workingThreads;
		int _linkQueueSize = linkQueue.size();
		int _totalVisitedPages = totalVisitedPages;
		unlock(&mainLock);

		if (pagesLimitReached || _totalVisitedPages >= pagesLimit)
		{
			// pagesLimit reached
			if(!pagesLimitReached && _totalVisitedPages>=pagesLimit)
			{
        pagesLimitReached = true;
				cout << "~!!!pagesLimit Reached here.!!!~" << endl;
			}
			
			if (_workingThreads)
			{
				// sleep
				gotosleep();
			}
			else
			{
				// exiting
				cout << "EXITING AS ALL THREADS ARE COMPLETED & pagelimit reached." << endl;
				break;
			}
		}
		else
		{
			// pagesLimit not reached
			if (_workingThreads < maxThreads && _linkQueueSize > 0)
			{
				// create thread
				createThread();
			}
			else if (_workingThreads == 0)
			{
				// exiting
				cout << "EXITING AS NO THREADS WORKING. & pagelimit not reached" << endl;
				break;
			}
			else
			{
				// sleep
				gotosleep();
			}
		}

		// End of crawler loop
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
	cout << "Max Links from a website:"
			 << "\t" << maxLinks << endl;
	cout << "Max pages downloaded:"
			 << "\t" << pagesLimit << endl;
	cout << "Max threads working:"
			 << "\t" << maxThreads << endl;
	cout << "Total visited pages:"
			 << "\t" << totalVisitedPages << endl;

	cout << "" << endl;

	cout << "-----------------------------------------------------" << endl;
	cout << "Web rankings" << endl;
	cout << "-----------------------------------------------------" << endl;

	map<int, string, greater<int>> mm;

	for (auto &it : webRanking)
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

void *childThread(void *_url)
{
	string url((char *)_url);
	ofstream log("./thread_logs/" + to_string(rand()) + ".log");

	high_resolution_clock::time_point t1, t2;
	double totaldTime = 0;
	double d_Time, p_Time, u_Time;

	// downloading the file
	t1 = _now;
	string html = myCrawler.downloader(url);
	t2 = _now;
	d_Time = duration<double>(t2 - t1).count();

	log << "file downloaded." << endl;
	cout << "file downloaded." << endl;

	
  // for calculating time of downloading
	t1 = _now;
  // extracting all the links from it
  set<string> linkedSites = getLinks(html, myCrawler.maxLinks);
  t2 = _now;

	p_Time = duration<double>(t2 - t1).count();

	log << "links extracted." << endl;
	cout << "links extracted." << endl;

	// updating the shared variables
	t1 = _now;
	lock(&myCrawler.mainLock);
	for (auto i : linkedSites)
	{
		log << i << endl;
		myCrawler.webRanking[getDomain(i)]++;
		if (!myCrawler.discoveredSites[i])
		{
			myCrawler.discoveredSites[i] = true;
			myCrawler.linkQueue.push(i);
		}
	}
	myCrawler.workingThreads--;
	int _workingThreads = myCrawler.workingThreads;
	unlock(&myCrawler.mainLock);
	t2 = _now;
	u_Time = duration<double>(t2 - t1).count();

	cout << "shared variables updated." << endl;

	// saving time measurements for this thread
	lock(&myCrawler.timingLock);
	myCrawler.threadTimings.push_back(vector<double>{d_Time, p_Time, u_Time});
	unlock(&myCrawler.timingLock);

	if (myCrawler.pagesLimitReached)
	{
		if (_workingThreads == 0)
		{
			// Awake the parent
			lock(&myCrawler.parent_lock);
			// myCrawler.done = true;
			// cout << "cond signal" << myCrawler.done << endl;
			pthread_cond_signal(&myCrawler.parent_cond);

			unlock(&myCrawler.parent_lock);
		}
	}
	else
	{
		lock(&myCrawler.parent_lock);
		// myCrawler.done = true;
		pthread_cond_signal(&myCrawler.parent_cond);

		unlock(&myCrawler.parent_lock);
	}

	log.close();

	cout << "working threads now: " << myCrawler.workingThreads << endl;
	pthread_exit(NULL);
}