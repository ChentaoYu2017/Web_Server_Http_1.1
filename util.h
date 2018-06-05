#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using std::string;
using std::vector;


// split input string into a series of substrings by delimiter.
vector<string> split(const string &s, const string &delimiter);

// remove leading and trailing spaces of input string.
string strip(const string &s);

// read whole file as a string.
string read_content(const string &path);

// get file extension.
string get_file_ext(const string &path);

// check if given path corresponds to a existent file.
bool is_exist_file(const string &path);

// check if the file is "world" readable.
bool has_permission(const string &path);

// convert relative path to absolute path.
string get_abs_path(const string &path);

// check if the file is not under directory doc_root.
bool escape_doc_root(const string &path, const string &doc_root);

// get last time the file is modified (in GMT timezone).
string get_last_modified_time(const string &path);

// httpResponse genResponse(const httpRequest &request, const string &doc_root);

string get_content_type(const string &path);

void DieWithErr(const char *errorMessage);

#endif // UTIL_H
