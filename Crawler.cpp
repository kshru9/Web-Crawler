#include "Crawler.h"

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

	ready = false;							 // because main thread is now not ready to process any data
	unique_lock<mutex> lk(cv_m); // unique_lock for conditional variable

	// written in a while loop, if spuriously woken up
	while (!ready)
		cv.wait(lk); // Waiting until not ready

	cout << "Awaken now." << endl;
}

void Crawler::awake()
{
	unique_lock<mutex> lk(cv_m); // unique_lock for conditional variable
	ready = true;								 // because now main thread needs to be awaken
	cv.notify_all();						 // notifying the main thread which is sleeping
}

void Crawler::createThread()
{
	string currentSite = linkQueue.pop();
	discoveredSites.inc(currentSite);
	totalVisitedPages.add(1);
	workingThreads.add(1);

	log << currentSite << endl;

	cout << GREEN << "Creating a thread, total: " << workingThreads.value() << C_END << endl;

	thread th(childThread, currentSite);
	th.detach();
}

void Crawler::runCrawler()
{
	while (true)
	{
		if (pagesLimitReached || totalVisitedPages.value() >= pagesLimit)
		{
			// pagesLimit reached
			if (!pagesLimitReached && totalVisitedPages.value() >= pagesLimit)
			{
				pagesLimitReached = true;
				cout << RED << "~!!!pagesLimit Reached here.!!!~" << C_END << endl;
			}

			if (workingThreads.value())
			{
				// sleep
				gotosleep();
			}
			else
			{
				// exiting

				cout << RED << "Exiting as all threads are finished & pagelimit reached.\033[0m\n"
						 << C_END << endl;

				break;
			}
		}
		else
		{
			// pagesLimit not reached
			if (workingThreads.value() < maxThreads && linkQueue.size() > 0)
			{
				// create thread
				createThread();
			}
			else if (workingThreads.value() == 0)
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

/*
*/
void Crawler::showResults()
{
	// system("clear");
	string dashline = "-----------------------------------------------------";
	cout << endl;
	cout << "Parameters:" << endl;
	cout << dashline << endl;
	cout << "Max Links from a website:"
			 << "\t" << maxLinks << endl;
	cout << "Max pages downloaded:"
			 << "\t" << pagesLimit << endl;
	cout << "Max threads working:"
			 << "\t" << maxThreads << endl;
	cout << "Total visited pages:"
			 << "\t" << totalVisitedPages.value() << endl;

	cout << "" << endl;

	cout << dashline << endl;
	cout << "Web rankings" << endl;
	cout << dashline << endl;

	map<int, string, greater<int>> mm;

	for (auto it : webRanking.value())
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
		cout << RED << r++ << "\t\t" << GREEN << i.second << " : " << i.first << C_END <<  endl;
	}

	cout << "-----------------------------------------------------" << endl;
}

void childThread(string url)
{
	string name = to_string(rand() % 1000);
	cout << CYAN << "Thread " << name << " has started working." << C_END << endl;

	high_resolution_clock::time_point t1, t2;
	double totaldTime = 0;
	double d_Time, p_Time, u_Time;

	// downloading the file
	t1 = _now;
	string html = myCrawler.downloader(url);
	t2 = _now;
	d_Time = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

	cout << CYAN << "Thread " << name << " has downloaded files." << C_END << endl;

	// for calculating time of downloading
	t1 = _now;
	set<string> linkedSites = getLinks(html, myCrawler.maxLinks);
	t2 = _now;
	p_Time = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << CYAN << "Thread " << name << " has extracted links." << C_END << endl;

	// updating the shared variables
	t1 = _now;
	for (auto i : linkedSites)
	{
		myCrawler.webRanking.inc(getDomain(i));

		if (!myCrawler.discoveredSites.get(i))
		{
			myCrawler.discoveredSites.inc(i);
			myCrawler.linkQueue.push(i);
		}
	}
	t2 = _now;
	u_Time = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();
	cout << CYAN << "Thread " << name << " has updated shared var." << C_END << endl;

	// saving time measurements for this thread
	myCrawler.timingLock.lock();
	myCrawler.threadTimings.push_back(vector<double>{d_Time, p_Time, u_Time});
	myCrawler.timingLock.unlock();

	cout << BLUE << "Thread " << name << " finished, total: " << myCrawler.workingThreads.add(-1) << C_END << endl;

	if (myCrawler.pagesLimitReached)
	{
		if (myCrawler.workingThreads.value() == 0)
		{
			myCrawler.awake();
		}
	}
	else
	{
		myCrawler.awake();
	}

	// EXIT NOW
}