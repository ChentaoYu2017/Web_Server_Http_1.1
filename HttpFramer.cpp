#include <assert.h>
#include "HttpFramer.hpp"
#include "httpd.h"

using namespace std;

void HttpFramer::append(string chars)
{
	// PUT YOUR CODE HERE
	this->buffer.append(chars);
}

bool HttpFramer::hasMessage() const
{
	// PUT YOUR CODE HERE
	return this->buffer.find(endSingal) != string::npos;
}

string HttpFramer::topMessage() const
{
	// PUT YOUR CODE HERE
	return this->buffer.substr(0, this->buffer.find(endSingal));
}

void HttpFramer::popMessage()
{
	// PUT YOUR CODE HERE
	this->buffer = this->buffer.substr(this->buffer.find(endSingal) + 4);
}

// void HttpFramer::printToStream(ostream& stream) const
// {
// 	// PUT YOUR CODE HERE
// }
