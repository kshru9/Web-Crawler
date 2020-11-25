#include "Crawler.h"
#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"

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

	std::unique_lock<std::mutex> lk(cv_m);

	cv.wait(lk);

	cout << "Awaken now." << endl;
}

void Crawler::createThread()
{
	string currentSite = linkQueue.pop();
	totalVisitedPages.add(1);
	workingThreads.add(1);

	cout << "Creating a thread, total = " << workingThreads.value() << endl;

	thread th(childThread, currentSite);
	th.detach();
}

void Crawler::runCrawler()
{
	while (1)
	{
		if (pagesLimitReached || totalVisitedPages.value() >= pagesLimit)
		{
			// pagesLimit reached
			if (!pagesLimitReached && totalVisitedPages.value() >= pagesLimit)
			{
				pagesLimitReached = true;
				cout << "~!!!pagesLimit Reached here.!!!~" << endl;
			}

			if (workingThreads.value())
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
			 << "\t" << totalVisitedPages.value() << endl;

	cout << "" << endl;

	cout << "-----------------------------------------------------" << endl;
	cout << "Web rankings" << endl;
	cout << "-----------------------------------------------------" << endl;

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
		cout << r++ << "\t\t" << i.second << " : " << i.first << endl;
	}

	cout << "-----------------------------------------------------" << endl;
}

void childThread(string url)
{
	ofstream log("./thread_logs/" + to_string(rand()) + ".log");

	high_resolution_clock::time_point t1, t2;
	double totaldTime = 0;
	double d_Time, p_Time, u_Time;

	// downloading the file
	t1 = _now;
	string html = myCrawler.downloader(url);
	t2 = _now;
	d_Time = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

	log << "file downloaded." << endl;
	cout << "file downloaded." << endl;

	// for calculating time of downloading
	t1 = _now;
	// extracting all the links from it
	set<string> linkedSites = getLinks(html, myCrawler.maxLinks);
	t2 = _now;

	p_Time = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

	log << "links extracted." << endl;
	cout << "links extracted." << endl;

	// updating the shared variables
	t1 = _now;

	for (auto i : linkedSites)
	{
		log << i << endl;
		myCrawler.webRanking.inc(getDomain(i));

		if (!myCrawler.discoveredSites.get(i))
		{
			myCrawler.discoveredSites.inc(i);
			myCrawler.linkQueue.push(i);
		}
	}

	t2 = _now;
	u_Time = chrono::duration_cast<chrono::microseconds>(t2 - t1).count();

	cout << "shared variables updated." << endl;

	// saving time measurements for this thread
	myCrawler.timingLock.lock();
	myCrawler.threadTimings.push_back(vector<double>{d_Time, p_Time, u_Time});
	myCrawler.timingLock.unlock();
	log.close();

	myCrawler.workingThreads.add(-1);
	

	std::unique_lock<std::mutex> lk(myCrawler.cv_m);
	
	if (myCrawler.pagesLimitReached)
	{
		if (myCrawler.workingThreads.value() == 0)
		{
			myCrawler.cv.notify_all();
		}
	}
	else
	{
		myCrawler.cv.notify_all();
	}
}