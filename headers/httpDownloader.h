#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
using namespace std;

string httpDownloader(string url);
string getHostPathFromUrl(string url);
string getHostnameFromUrl(string url);



string httpDownloader(string url){
    int sock, port = 80;

    string path = getHostPathFromUrl(url);
    string hostname = getHostnameFromUrl(url);

    struct hostent *host;
    struct sockaddr_in server_addr;

    // Get host address by hostname
    host = gethostbyname(hostname.c_str());
    
    if (host == NULL || host->h_addr == NULL)
    {
        return "Error getting DNS info!";
    }

    // Create Socket structure
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return "Cannot create socket!";
    }
    
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero), 8);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        cout << "Socket Connection failed." << endl;
        exit(0);
    }


    string send_data = "GET " + path + " HTTP/1.1\r\nHOST:" + hostname + "\r\nConnection: Close\r\n\r\n";
    if (send(sock, send_data.c_str(), strlen(send_data.c_str()), 0) < 0)
    {
        return "Send Request Failed.";
    }

    char recv_data[1024];
    int totalBytesRead = 0;
    string Response = "";

    while (true)
    {
        bzero(recv_data, sizeof(recv_data));
        int bytesRead = recv(sock, recv_data, sizeof(recv_data), 0);
        
        // Check Data Received.
        if (bytesRead > 0)
        {
            Response += string(recv_data);
            totalBytesRead += bytesRead;
        }
        else
        {
            break;
        }
    }
    
    close(sock);

    return Response;
}

string getHostnameFromUrl(string url)
{
    int offset = 0;
    offset = offset == 0 && url.compare(0, 8, "https://") == 0 ? 8 : offset;
    offset = offset == 0 && url.compare(0, 7, "http://") == 0 ? 7 : offset;

    size_t pos = url.find("/", offset);
    string domain = url.substr(offset, (pos == string::npos ? url.length() : pos) - offset);

    return domain;
}

string getHostPathFromUrl(string url)
{
    int offset = 0;
    offset = offset == 0 && url.compare(0, 8, "https://") == 0 ? 8 : offset;
    offset = offset == 0 && url.compare(0, 7, "http://") == 0 ? 7 : offset;

    size_t pos = url.find("/", offset);
    string path = pos == string::npos ? "/" : url.substr(pos);

    // Remove extra slashes
    pos = path.find_first_not_of('/');
    if (pos == string::npos)
        path = "/";
    else
        path.erase(0, pos - 1);
    return path;
}