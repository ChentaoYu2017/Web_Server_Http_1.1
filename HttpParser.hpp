#ifndef HTTPPARSER_HPP
#define HTTPPARSER_HPP

#include <string>
#include <stdint.h>

#include <unordered_map>

using namespace std;


typedef struct httpRequest_t {
	// header

	// initial line
	string method;
	string url;
	string version;

	//key-value pairs
	unordered_map<string, string> keyValues;

	// // body
	// string body;

	// // connection status
	// bool flag;
} httpRequest;


typedef struct httpResponse_t {
	// header

	// initial line
	string version;
	int status;
	string description;

	// key-value pairs
	unordered_map<string, string> keyValues;

	// body
	string body;
} httpResponse;

/*
 * Alternatively:
 * class CalcInstruction {
 *   // DEFINE YOUR CLASS HERE
 * };
 *
 */


class HttpParser {
public:
	static httpRequest parseRequest(string httpMessage);
	static string parseResponse(httpResponse response);
};

#endif // parser_HPP
