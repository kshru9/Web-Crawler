<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-4-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

# Multi Threaded Web-Crawler

## Description
The goal of this project is to create a multi-threaded web crawler. A Web crawler is an Internet bot that systematically browses the World Wide Web, typically for the purpose of Web indexing. Any search engine uses these indexes, web graphs, and an appropriate algorithm ( such as PageRank ) to rank the pages. The main focus of the project would be to implement a multi-threaded downloader that can download multi websites at the same time. The plan is to implement this in C or C++.

## Deliverables
 - Part 1: Multi-threaded web-crawler
 - Part 2: Tool for Web scraping
 - Part 3: (Extended scope) Web Ranking

### Tools Used
 - Sockets
 - OpenSSL
 - Pthread library
    - For concurrency and synchronization techniques
       - Locks
         - Single locks
         - Reader Writer locks
       - Condition Variables


## Simple Crawler Flowchart
![flowchart](https://github.com/ShrutiKatpara/Web-Crawler/blob/main/flowchart.jpg?raw=true)

## [Table of contents](#table-of-contents)
- [Single Threaded Web Crawler](#single-threaded)
    - [Components](#single-threaded-components)
    - [How to run single threaded web crawler](#run-multi-threaded)
- [Multithreaded Web Crawler using single lock](#multi-threaded-singlelock)
    - [Components](#multi-threaded-components)
    - [How to run multi threaded web crawler using single lock](#run-multi-threaded-singlelock)


## [Single Threaded Web Crawler](#single-threaded)

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
    <pre>
    <code>
    ...
    while(!mainQueue.empty() && totalVisitedPages < pagesLimit)
    {
        currWebsite = mainQueue.pop()
        html = websiteDownloader(currWebsite)
        linkWebsite = htmlParser(html)
        update(discoveredWebsites, mainQueue, totalVisitedPages)
    }
    ...
    </pre></code>
- Website ranker
    - using a simple counter

### [How to run single threaded web crawler](#run-single-threaded)
- use `make` to compile the program
- `maxlinks`, `pagelimit` can be given as argument in with `make` command.
    - For e.g. `make maxlinks=1000 pagelimit=100`
    - Here the arguments are:
        - `maxlinks`: Maximum number of links to be extracted from a website
        - `pagelimit`: Maximum number of websites to be downloaded while crawling

## [Multithreaded Web Crawler using single lock](#multi-threaded-singlelock)

## [Components](#multi-threaded-components):
 - **Crawler** as a thread controller
 - **Child thread**
    - HTML downloader
    - Link extractor
    - Domain extractor
    - Ranker using counter

### Crawler loop code
<pre>
<code>
...
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
...
</pre></code>

### Child Thread code
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


### Tools Used
 - Sockets
 - OpenSSL
 - Pthread library
    - For concurrency and synchronization techniques

## [How to run Multithreaded Web Crawler using SINGLE_LOCK technique](#run-multi-threaded-singlelock)
- use `make` to compile the program
- `maxlinks`, `pagelimit`, `threads` can be given as argument in with `make` command.
    - For e.g. `make maxlinks=1000 pagelimit=100 threads=10`
    - Here the arguments are:
        - `maxlinks`: Maximum number of links to be extracted from a website
        - `pagelimit`: Maximum number of websites to be downloaded while crawling
        - `threads`: Maximum number of threads to be created

## [Multithreaded Web Crawler using THREAD_SAFE data structures](#multi-threaded-threadsafe)

## [Components](#multi-threaded-components-threadsafe):
- **Crawler** as a thread controller
- **Child thread**
    - HTML downloader
    - Link extractor
    - Domain extractor

### Crawler loop code
<pre>
<code>
...
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
...
</pre></code>

### Child Thread code
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

## [How to run Multithreaded Web Crawler using THREAD_SAFE data structures](#run-multi-threaded-threadsafe)
- use `make` to compile the program
- `maxlinks`, `pagelimit`, `threads` can be given as argument in with `make` command.
    - For e.g. `make maxlinks=1000 pagelimit=100 threads=10`
    - Here the arguments are:
        - `maxlinks`: Maximum number of links to be extracted from a website
        - `pagelimit`: Maximum number of websites to be downloaded while crawling
        - `threads`: Maximum number of threads to be created
        - `rankerFlag`: Flag to choose which ranking algorithm to be executed

## [Website domain name ranker](#ranker)
- [Simple counter based](#ranker-counter)
- [PageRank algorithm](https://cs50.harvard.edu/ai/2020/projects/2/pagerank/)
The intuition behind our pagerank algorithm is as follows. Suppose there is a random surfer which randomly chooses a starting website. After that it chooses next website from all the linked website with current chosen website with probability of 0.85 or it chooses next website from all available websites with a probability of 0.15. 

In this way, the importance of a website is measured by how repetitively a random surfer visits a website. Hence, a website is important if it is linked to more number of important websites.

There are two ways to implement this algorithm:
    - [Iteration based pagerank algorithm](#ranker-iterative)
    - [Sampling based pagerank algorithm](#ranker-sampling)

### [Simple counter based ranking algorithm](#ranker-counter)

<pre><code>
...
corpus = read(csv_file)
for website in corpus.keys():
    for x in corpus[website]:
        rank[website]+=1
...
</code></pre>

#### Demo run
<pre><code>------------------------------------------------
  Domain Name rankings using counter
------------------------------------------------

................................................
  Domain Name            Rank
................................................

1 .  mygov.in                                  43
2 .  main.ayush.gov.in                         36
3 .  tourism.gov.in                            24
4 .  digitalindia.gov.in                       19
5 .  asi.nic.in                                16
------------------------------------------------------------</pre></code>

### [Sampling based pagerank algorithm](#ranker-sampling)
In this approach, we randomly choose a website according to pagerank algorithm intuition 

In this way, the importance of website is measured by how many times the random surfer will be on a website.

<pre><code>
...
DAMPING = 0.85
SAMPLES = 10000

corpus = read(csv_file)

for i in range(1,SAMPLES):
    for x in corpus.keys():
        if (x in corpus[page]):
            model[x] = ((DAMPING * (1/number_of_linked_websites)) + ((1-DAMPING)* (1/total_websites)))
        else:
            model[x] = ((1-DAMPING)* (1/total_websites))
    x = random.choices(websites, weights=model.values())[0]
    pagerrank[x] += (1/n)
return pagerrank
...
</code></pre>

#### Demo run
<pre><code>-------------------------------------------------------------
  Domain Name ranking using PageRank: Sampling (n = 10000)
-------------------------------------------------------------

................................................
  Domain Name            Rank
................................................

1 .  haryana.mygov.in                          0.1290000000000021
2 .  chhattisgarh.mygov.in                     0.11000000000000212
3 .  blog.mygov.in                             0.07730000000000119
4 .  mygov.in                                  0.07260000000000105
5 .  aatmanirbharbharat.mygov.in               0.04840000000000036
------------------------------------------------------------</pre></code>

### [Iteration based pagerank algorithm](#ranker-iterative)
The intuition behind ranking using iterative pagerank algorithm is as follows.  We will update the probability of every website according to this. We will stop iterating when the difference between old and updated probabilities is less than certain threashold.

In this way, the importance of website is measured by what

<pre><code>
...
DAMPING = 0.85
THRESHOLD = 0.001
corpus = read(csv_file)

while(1):
    before = [pagerrank[v] for v in pagerrank.keys()]
    for x in corpus.keys():
        for v in linkedWebsites:
            pagerrank[x] += (DAMPING*(pagerank[v]/total_linkedWebsites))
        pagerank[x] += ((1-DAMPING)/number_of_websites)
        
    if (change(before, [pagerrank[v] for v in pagerrank.keys()]) <= THRESHOLD):
        break

return pagerrank
...
</code></pre>

#### Demo run
<pre><code>----------------------------------------------
  Domain Name ranking using PageRank: Iteration
----------------------------------------------

................................................
  Domain Name            Rank
................................................

1 .  chhattisgarh.mygov.in                     0.03622047763650005
2 .  haryana.mygov.in                          0.03622047763650005
3 .  mygov.in                                  0.015051816113982971
4 .  blog.mygov.in                             0.014124265632032305
5 .  india.gov.in                              0.011385432326608554
------------------------------------------------------------</pre></code>