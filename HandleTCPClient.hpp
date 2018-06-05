#ifndef HANDLETCPCLIENT_HPP
#define HANDLETCPCLIENT_HPP

#include <string>

using namespace std;


void HandleTCPClient(int clntSocket, const string &doc_root);
void sendHeader(string resMessage, int clntSocket);

#endif // HANDLETCPCLIENT_HPP
