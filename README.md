<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-4-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

# Multi Threaded Web-Crawler

## [Table of contents](#table-of-contents)
- [Single Threaded Web Crawler](#single-threaded)
    - [Components](#single-threaded-components)
    - [How to run single threaded web crawler](#run-multi-threaded)
- [Multithreaded Web Crawler using single lock](#multi-threaded-singlelock)
    - [Components](#multi-threaded-components)
    - [How to run multi threaded web crawler using single lock](#run-multi-threaded-singlelock)

## Description
The goal of this project is to create a multi-threaded web crawler. A Web crawler is an Internet bot that systematically browses the World Wide Web, typically for the purpose of Web indexing. Any search engine uses these indexes, web graphs, and an appropriate algorithm ( such as PageRank ) to rank the pages. The main focus of the project would be to implement a multi-threaded downloader that can download multi websites at the same time. The plan is to implement this in C or C++.

## Features/Deliverables
 - Part 1: Multi-threaded web-crawler
 - Part 2: Tool for Web scraping
 - Part 3: (Extended scope) Web Ranking
 
## Simple Crawler Flowchart
![flowchart](https://github.com/ShrutiKatpara/Web-Crawler/blob/main/flowchart.jpg?raw=true)




## [SINGLE THREADED Web Crawler](#single-threaded)

### [Components](#single-threaded-components):
- HTTP website downloader
    - using socket library
- HTTPs websites downloader
    - using openssl library
- HTML file parser
    - using regex
- Domain extractor
    - using regex
- Crawler loop
- Website ranker
    - using a simple counter
    
### [Pseudo Code]
<pre><code>    ...
    while(!mainQueue.empty() && totalVisitedPages < pagesLimit)
    {
        currWebsite = mainQueue.pop()
        html = websiteDownloader(currWebsite)
        linkWebsite = htmlParser(html)
        update(discoveredWebsites, mainQueue, totalVisitedPages)
    }
    ...</code></pre>
    
### [How to run SINGLE THREADED web crawler](#run-single-threaded)
- use `make` to compile the program
- `maxlinks`, `pagelimit` can be given as argument in with `make` command.
    - For e.g. `make maxlinks=1000 pagelimit=100`
    - Here the arguments are:
        - `maxlinks`: Maximum number of links to be extracted from a website
        - `pagelimit`: Maximum number of websites to be downloaded while crawling

## [MULTITHREADED Web Crawler](#multi-threaded)

### [Components](#multi-threaded-components):
 - **Crawler** as a thread controller
 - **Child thread**
    - HTML downloader
    - Link extractor
    - Domain extractor
    - Ranker using counter

### [Crawler loop code]
<pre><code>...
while(1){
    if(pagesLimitReached || visitedpages>=pagesLimit){
        pagesLimitReached = true;
        if(w_threads){
            <b>gotosleep()</b>;
        }
        else {
            break;
        }
    }
    else{
        if (w_threads < maxThreads && queue_size>0){
            createThread();
        }
        else if(w_threads == 0){
            break;
        }
        else{
            <b>gotosleep()</b>;
        }
    }
}
...</pre></code>

### [Child Thread code]
<pre>
<code>
...
download(url);
parse(url);
update(queue, visitedLinks, ranking);
if(pagesLimitReached){
    if(workingThreads == 0){
        <b>wake_parent()</b>;
    }
}
else{
    <b>wake_parent()</b>;
}
...
</pre></code>

### [Variations with locking techniques]
#### [using SINGLE_LOCK technique]
#### [using THREAD-SAFE data structures]
#### [How to run MULTITHREADED Web Crawler with SINGLE_LOCK technique](#run-multi-threaded-singlelock)
- use `make` to compile the program
- `maxlinks`, `pagelimit`, `threads` can be given as argument in with `make` command.
    - For e.g. `make maxlinks=1000 pagelimit=100 threads=10`
    - Here the arguments are:
        - `maxlinks`: Maximum number of links to be extracted from a website
        - `pagelimit`: Maximum number of websites to be downloaded while crawling
        - `threads`: Maximum number of threads to be created
#### [How to run MULTITHREADED Web Crawler with THREAD-SAFE data structures](#run-multi-threaded-singlelock)

## [Web Ranking Algorithms]
### [Simple counter based ranking system]
### [Sampling based ranking algorithms]
#### [Using random surfer model]
#### [Using iterative algorithm]


## [Demo Output]
<pre>./_crawler 100 10 4
<font color="#00CD00">Creating a thread, total: 1</font>
<font color="#00CD00">Creating a thread, total: 2</font>
<font color="#00CD00">Creating a thread, total: 3</font>
hostname: www.tajmahal.gov.in
path: /
Going to sleep now
hostname: whc.unesco.org
path: /en/list/252/
<font color="#00CDCD">Thread 1 has downloaded files.</font>
<font color="#00CDCD">Thread 1 has extracted links.</font>
<font color="#00CDCD">Thread 1 has updated shared var.</font>
<font color="#0000EE">Thread 1 finished, total: 2</font>
Awaken now.
Going to sleep now
<font color="#00CDCD">Thread 2 has downloaded files.</font>
<font color="#00CDCD">Thread 2 has extracted links.</font>
<font color="#00CDCD">Thread 2 has updated shared var.</font>
<font color="#0000EE">Thread 2 finished, total: 1</font>
Awaken now.
<font color="#00CD00">Creating a thread, total: 2</font>
<font color="#00CD00">Creating a thread, total: 3</font>
hostname: agra.nic.in
path: /
<font color="#00CD00">Creating a thread, total: 4</font>
Going to sleep now
hostname: asi.nic.in
path: /
hostname: tourism.gov.in
path: /
<font color="#00CDCD">Thread 4 has downloaded files.</font>
<font color="#00CDCD">Thread 6 has downloaded files.</font>
<font color="#00CDCD">Thread 4 has extracted links.</font>
<font color="#00CDCD">Thread 4 has updated shared var.</font>
<font color="#0000EE">Thread 4 finished, total: 3</font>
Awaken now.
<font color="#00CD00">Creating a thread, total: 4</font>
Going to sleep now
hostname: www.adysoftindia.com
path: /
<font color="#00CDCD">Thread 6 has extracted links.</font>
<font color="#00CDCD">Thread 6 has updated shared var.</font>
<font color="#0000EE">Thread 6 finished, total: 3</font>
Awaken now.
<font color="#00CD00">Creating a thread, total: 4</font>
Going to sleep now
hostname: www.asiagracircle.in
path: /
<font color="#00CDCD">Thread 8 has downloaded files.</font>
<font color="#00CDCD">Thread 8 has extracted links.</font>
<font color="#00CDCD">Thread 8 has updated shared var.</font>
<font color="#0000EE">Thread 8 finished, total: 3</font>
Awaken now.
<font color="#00CD00">Creating a thread, total: 4</font>
Going to sleep now
hostname: www.colonialwalkagra.com
path: /
<font color="#00CDCD">Thread 7 has downloaded files.</font>
<font color="#00CDCD">Thread 7 has extracted links.</font>
<font color="#00CDCD">Thread 7 has updated shared var.</font>
<font color="#0000EE">Thread 7 finished, total: 3</font>
Awaken now.
<font color="#00CD00">Creating a thread, total: 4</font>
<font color="#CD0000">~!!!pagesLimit Reached here.!!!~</font>
Going to sleep now
hostname: www.kalakrititheatre.com
path: /
<font color="#00CDCD">Thread 9 has downloaded files.</font>
<font color="#00CDCD">Thread 9 has extracted links.</font>
<font color="#00CDCD">Thread 9 has updated shared var.</font>
<font color="#0000EE">Thread 9 finished, total: 3</font>
<font color="#00CDCD">Thread 3 has downloaded files.</font>
<font color="#00CDCD">Thread 3 has extracted links.</font>
<font color="#00CDCD">Thread 3 has updated shared var.</font>
<font color="#0000EE">Thread 3 finished, total: 2</font>
<font color="#00CDCD">Thread 5 has downloaded files.</font>
<font color="#00CDCD">Thread 5 has extracted links.</font>
<font color="#00CDCD">Thread 5 has updated shared var.</font>
<font color="#0000EE">Thread 5 finished, total: 1</font>
<font color="#00CDCD">Thread 10 has downloaded files.</font>
<font color="#00CDCD">Thread 10 has extracted links.</font>
<font color="#00CDCD">Thread 10 has updated shared var.</font>
<font color="#0000EE">Thread 10 finished, total: 0</font>
Awaken now.
<font color="#CD0000">Exiting as all threads are finished &amp; pagelimit reached.</font>

Parameters:
-----------------------------------------------------
Max Links from a website:	100
Max pages downloaded:	10
Max threads working:	4
Total visited pages:	10
-----------------------------------------------------
FINISHED.
<font color="#CD0000">Elapsed time in milliseconds : 6988</font>
python3 th_timings.py 4
python3 ranker.py -sp
-------------------------------------------------------------
  Domain Name ranking using PageRank: Sampling (n = 10000)
-------------------------------------------------------------

................................................
  Domain Name 			 Rank
................................................

1 .  agra.nic.in                               0.20319999999999394
2 .  tourism.gov.in                            0.1717999999999974
3 .  whc.unesco.org                            0.15599999999999914
4 .  asi.nic.in                                0.14710000000000012
5 .  adysoftindia.com                          0.11370000000000223
6 .  kalakrititheatre.com                      0.10470000000000197
7 .  tajmahal.gov.in                           0.10350000000000194
------------------------------------------------------------
</pre>

## Tech/Framework used
 - Sockets
 - OpenSSL
 - Pthread library
    - For concurrency and synchronization techniques
       - Locks
         - Single locks
         - Reader Writer locks
       - Condition Variables

## How to use

## Credits

## License
