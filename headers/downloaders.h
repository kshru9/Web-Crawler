// https://stackoverflow.com/questions/16255323/make-an-https-request-using-sockets-on-linux
// https://stackoverflow.com/questions/41229601/openssl-in-c-socket-connection-https-client
// https://stackoverflow.com/questions/36847456/openssl-ssl-read-malfunction

#include <iostream>
#include <string.h>
#include <fstream>

#include <netdb.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>

using namespace std;

#define MAX_FILE_SIZE 4096 * 8192

string httpDownloader(string);
string httpsDownloader(string);

string getHostnameFromUrl(string);
string getHostPathFromUrl(string);

string httpDownloader(string url)
{
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
		return "Socket Connection failed.";
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
			if (Response.size() > MAX_FILE_SIZE)
			{
				Response = "";
				break;
			}
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

string httpsDownloader(string url)
{
	// first connect to the remote as usual, but use the port 443 instead of 80
	int sock, port = 443;
	string path = getHostPathFromUrl(url);
	string hostname = getHostnameFromUrl(url);
	cout << "hostname: " << hostname << endl;
	cout << "path: " << path << endl;

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
		return "Socket Connection failed.";
	}

	// initialize OpenSSL - do this once and stash ssl_ctx in a global var
	SSL_load_error_strings();
	SSL_CTX *ssl_ctx = SSL_CTX_new(SSLv23_client_method());
	SSL_library_init();

	// create an SSL connection and attach it to the socket
	SSL *conn = SSL_new(ssl_ctx);
	SSL_set_fd(conn, sock);

	// perform the SSL/TLS handshake with the server - when on the
	// server side, this would use SSL_accept()
	int err = SSL_connect(conn);
	if (err != 1)
	{
		// facing this error with https://aave.com/, some certification failing...
		return "SSL_connect() failed.";
	}

	// now proceed with HTTP traffic,
	//     using SSL_read instead of recv() and
	//     SSL_write instead of send(),
	string getRequest = "GET " + path + " HTTP/1.1\r\n" + "Host: " + hostname + "\r\n" +
											+"Connection: close\r\n\r\n";

	char tempBuff[10000];
	strcpy(tempBuff, getRequest.c_str());

	int writeRet = SSL_write(conn, tempBuff, getRequest.size());
	if (writeRet < 0)
	{
		return "Send Request Failed.";
	}

	string httpResponse = "";
	char ptr[1024];
	int totalBytesRead = 0;
	int bytesRead;

	do
	{
		bytesRead = SSL_read(conn, ptr, sizeof(ptr) / sizeof(ptr[0]));
		totalBytesRead += bytesRead;
		httpResponse += string(ptr);
		if (httpResponse.size() > MAX_FILE_SIZE)
		{
			httpResponse = "";
			break;
		}
	} while (bytesRead);

	//and SSL_shutdown/SSL_free before close()
	SSL_shutdown(conn);
	SSL_free(conn);
	close(sock);


	return httpResponse;
}