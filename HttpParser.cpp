#include <assert.h>
#include "HttpParser.hpp"
#include "httpd.h"
#include "util.h"


using namespace std;

httpRequest HttpParser::parseRequest(string httpMessage)
{
	httpRequest ret;
	vector<string> lines = split(httpMessage, CRLF);

	// parse initial line
	vector<string> inits = split(strip(lines[0]), " ");
	ret.method = inits[0];
	ret.url = inits[1];
	ret.version = inits[2];

	// parse key-value pairs
	for (int i = 1; i < (int)lines.size(); ++i) {
		vector<string> maps = split(lines[i], ":");
		string key = strip(maps[0]);
		string value = strip(maps[1]);
		ret.keyValues[key] = value;		
	}

	return ret;
}

string HttpParser::parseResponse(httpResponse response)
{
	string ret;
	// initial line
	ret += response.version + " " + to_string(response.status) + " " + response.description + CRLF;
	// key-value pairs
	// for (auto kv : response.keyValues) {
	// 	ret += kv.first + " : " + kv.second + CRLF;
	// }

	unordered_map<string, string>::iterator it;
	for (it = response.keyValues.begin(); it != response.keyValues.end(); ++it) {
		ret += it->first + " : " + it->second + CRLF;
	}

	// body
	ret += CRLF + response.body;

	return ret;
}
