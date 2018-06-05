#ifndef HTTPD_H
#define HTTPD_H

using namespace std;

const string CR = "\r";
const string CRLF = "\r\n";
const string endSingal = "\r\n\r\n";

void start_httpd(unsigned short port, string doc_root);

#endif // HTTPD_H
