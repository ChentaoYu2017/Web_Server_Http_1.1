#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <assert.h>
#include "HandleTCPClient.hpp"
#include "HttpFramer.hpp"
#include "HttpParser.hpp"
#include "util.h"
#include <string.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>


using namespace std;

char* charPath;

void HandleTCPClient(int clntSocket, const string &doc_root)
{
	std::cout << "start handling clntSocket" << std::endl;
	
	char buffer[513];
	ssize_t bytesReceived = recv(clntSocket, buffer, 512, 0);

	std::cout << buffer << std::endl;

	if (bytesReceived < 0 && errno == EWOULDBLOCK) {
		DieWithErr("recv() failed");
		close(clntSocket);
		return;
	}

	if (bytesReceived < 0) {
		DieWithErr("recv() failed");
	}

	HttpFramer framer;
	HttpParser parser;

	std::cout << "aaa" << std::endl;

	while (bytesReceived > 0) {

		buffer[bytesReceived] = '\0';
		string s(buffer);
		framer.append(s);

		std::cout << "bbb" << std::endl;

		while (framer.hasMessage()) {
			std::cout << "hhh" << std::endl;


			string httpMessage = framer.topMessage();
			framer.popMessage();

			std::cout << "ccc" << std::endl;
			// parse the request
			httpRequest request = parser.parseRequest(httpMessage);
			// generate response
			// httpResponse response = genResponse(request, doc_root);



			  httpResponse response;
			  response.version = "HTTP/1.1";
			  response.keyValues["Server"] = "localhost"; // default setting  

			  string url = request.url;
			  // default page
			  if (url == "/") {
			    url = "/index.html";
			  }

			  string path = doc_root + url;

			 

			  std::cout << path << std::endl;
			  // request kv pairs must contain Host

			if (!request.keyValues.count("Host")) {
			    response.status = 400;
			    response.description = "Client Error";
			    response.body = "<html><body><h1>400 Client Error</h1></body></html>";
			    response.keyValues["Content-Type"] = "text/html";
			    response.keyValues["Content-Length"] = to_string(response.body.size());
			    string resMessage = parser.parseResponse(response);
			    sendHeader(resMessage, clntSocket);
			} 


			else if (!is_exist_file(path) || escape_doc_root(path, doc_root)) {
			      response.status = 404;
			      response.description = "Not Found";
			      response.body = "<html><body><h1>404 Not Found</h1></body></html>";
			      response.keyValues["Content-Type"] = "text/html";
			      response.keyValues["Content-Length"] = to_string(response.body.size());
			      string resMessage = parser.parseResponse(response);
			      sendHeader(resMessage, clntSocket);			
			  } 


			else if (!has_permission(path)) {
			      response.status = 403;
			      response.description = "Forbidden";
			      response.body = "<html><body><h1>403 Forbidden</h1></body></html>";
			      response.keyValues["Content-Type"] = "text/html";
			      response.keyValues["Content-Length"] = to_string(response.body.size());	
			      string resMessage = parser.parseResponse(response);
			      sendHeader(resMessage, clntSocket);	
			    } 



			else {
			      response.status = 200;
			      response.description = "OK";
			      // response.body = read_content(path);
			      response.keyValues["Content-Type"] = get_content_type(path);
			      response.keyValues["Last-Modified"] = get_last_modified_time(path);

    struct stat sb;
    char pathBuf[1024];
    realpath(path.c_str(), pathBuf);
    string resPath(pathBuf);

    if (stat(resPath.c_str(), &sb) == -1) {
        printf("\tExiting from file not exist\n");
		}

size_t length;

length = sb.st_size;


        response.keyValues["Content-Length"] = to_string(length);	

        string resMessage = parser.parseResponse(response);
			      sendHeader(resMessage, clntSocket);	


            int in_fd = open(resPath.c_str(), O_RDONLY);
        if (in_fd < 0)
            DieWithErr("Open failed");



			      off_t* offset = 0;

ssize_t sentfile_cnt = 0;

		while (sentfile_cnt < (ssize_t)length) {
            ssize_t numBytesSent = sendfile(clntSocket, in_fd, offset, length);
            if (numBytesSent < 0)
                DieWithErr("sendfile() failed");
       
            sentfile_cnt += numBytesSent;
        }




				// charPath = (char*) malloc(path.size());
  		// 		strcpy(charPath,path.c_str());

  		// 		char newpath[8192];

  		// 		realpath(charPath, newpath);
			}

			






			// check if client want to close
			if (request.keyValues.count("Connection") && request.keyValues["Connection"] == "close") {
				close(clntSocket);
				break;
			}
		}

		std::cout << "ddd" << std::endl;

		bytesReceived = recv(clntSocket, buffer, 512, 0);

		std::cout << "eee" << std::endl;

		std::cout << buffer << std::endl;

		if (bytesReceived < 0 && errno == EWOULDBLOCK) {
			DieWithErr("recv() failed");
			close(clntSocket);
			return;
		}

		if (bytesReceived < 0) {
			DieWithErr("recv() failed");
		}
	}
	close(clntSocket);	
}


// httpResponse genResponse(const httpRequest &request, const string &doc_root)
// {
//   httpResponse response;
//   response.version = "HTTP/1.1";
//   response.keyValues["Server"] = "localhost"; // default setting  

//   string url = request.url;
//   // default page
//   if (url == "/") {
//     url = "/index.html";
//   }

//   string path = doc_root + url;

//   // request kv pairs must contain Host
//   if (!request.keyValues.count("Host")) {
//     response.status = 400;
//     response.description = "Client Error";
//     response.body = "<html><body><h1>404 Not Found</h1></body></html>";
//     response.keyValues["Content-Type"] = "text/html";
//   } 
//   else if (!is_exist_file(path) || escape_doc_root(path, doc_root)) {
//       response.status = 404;
//       response.description = "Not Found";
//       response.body = "<html><body><h1>404 Not Found</h1></body></html>";
//       response.keyValues["Content-Type"] = "text/html";
//     } 
//     else if (!has_permission(path)) {
//       response.status = 403;
//       response.description = "Forbidden";
//       response.body = "<html><body><h1>403 Forbidden</h1></body></html>";
//       response.keyValues["Content-Type"] = "text/html";
//     } 
//     else {
//       response.status = 200;
//       response.description = "OK";
//       response.body = read_content(path);
//       response.keyValues["Content-Type"] = get_content_type(path);
//       response.keyValues["Last-Modified"] = get_last_modified_time(path);
//     }

//   response.keyValues["Content-Length"] = to_string(response.body.size());
//   return response;
// }


			// parse response to string
			
void sendHeader(string resMessage, int clntSocket) {

			int len = resMessage.length();

			char* resArray = new char[len + 2];
			strcpy(resArray, resMessage.c_str());
			resArray[len] = '\r';			
			resArray[len+1] = '\n';

			// send to client
			ssize_t numBytesSent = send(clntSocket, resArray, len+2, 0);

			if (numBytesSent < 0) {
				DieWithErr("send() failed");
			}
}

