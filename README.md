# Multi Threaded Web-Crawler

### Description
The goal of this project is to create a multi-threaded web crawler. A Web crawler is an Internet bot that systematically browses the World Wide Web, typically for the purpose of Web indexing. Any search engine uses these indexes, web graphs, and an appropriate algorithm ( such as PageRank ) to rank the pages. The main focus of the project would be to implement a multi-threaded downloader that can download multi websites at the same time. The plan is to implement this in C or C++.

### Deliverables
 - Part 1: Multi-threaded web-crawler
 - Part 2: Tool for Web scraping
 - Part 3: (Extended scope) Web Ranking

### Main Components
 - **Crawler** as a thread controller
 - **Child thread**
    - HTML downloader
    - Link extractor
    - Domain extractor

### Tools Used
 - Sockets
 - OpenSSL
 - Pthread library
    - For concurrency and synchronization techniques

## How to run
 - use `make` to run the program
 - `maxlimit`, `pagelimit`, `threads` can be given as argument in with `make` command.
 For example `make maxlimit=1000 pagelimit=100 threads=10`