#include "Crawler.h"

#include "headers/downloaders.h"
#include "headers/getLinks.h"
#include "headers/getDomain.h"

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

void Crawler::runCrawler()
{
  while (!linkQueue.empty() && totalVisitedPages < pagesLimit)
  {
    // Creating a thread
    // lock(linkQueue)
    string currentSite = linkQueue.front();
    linkQueue.pop();
    // unlock()

    // childThread(currentSite);
    pthread_t th;
    int ret_val = pthread_create(
        &th, NULL, childThread, (void *)currentSite.c_str());

    pthread_join(th, NULL);
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

void *childThread(void *url)
{
  // downloading the file
  string html = myCrawler.downloader(string((char *)url));
  myCrawler.log << "file downloaded." << endl;
  cout << "file downloaded." << endl;

  // extracting all the links from it
  set<string> linkedSites = getLinks(html, myCrawler.maxLinks);
  myCrawler.log << "links extracted." << endl;
  cout << "links extracted." << endl;

  // updating the shared variables
  // lock(linkedQueue, linkedSites, discoveredSites, totalVisitedPages)
  for (auto i : linkedSites)
  {
    myCrawler.lout << i << endl;
    myCrawler.ranker[getDomain(i)]++;
    if (!myCrawler.discoveredSites[i])
    {
      myCrawler.discoveredSites[i] = true;
      myCrawler.linkQueue.push(i);
    }
  }
  myCrawler.totalVisitedPages++;
  cout << "shared variables updated." << endl;
  myCrawler.log << "shared variables updated." << endl;
  // unlock()

  pthread_exit(NULL);
}