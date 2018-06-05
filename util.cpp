#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>  /* for perror() */
#include <stdlib.h> /* for exit() */

#include "util.h"

vector<string> split(const string &s, const string &delimiter) {
  std::vector<string> items;
  size_t pos = 0;
  while (pos < s.size()) {
    size_t next_pos = s.find(delimiter, pos);
    if (next_pos == string::npos) {
      break;
    }
    string token = s.substr(pos, next_pos - pos);
    if (token.size()) {
      items.push_back(token);
    }
    pos = next_pos + delimiter.size();
  }
  if (pos < s.size()) {
    items.push_back(s.substr(pos));
  }
  return items;
}

string strip(const string &s) {
  int i = 0, j = (int)s.size() - 1;
  while (i <= j && isspace(s[i])) ++i;
  while (i <= j && isspace(s[j])) --j;
  return s.substr(i, j - i + 1);
}

string read_content(const string &path) {
  std::ifstream fin(path, std::ios::binary);
  std::ostringstream oss;
  oss << fin.rdbuf();
  return oss.str();
}

string get_file_ext(const string &path) {
  vector<string> items = split(path, ".");
  if (items.size() > 1) {
    return items[items.size() - 1];
  } else {
    return "";
  }
}

bool is_exist_file(const string &path) {
  struct stat attr;
  int flag = ::stat(path.c_str(), &attr);
  return (flag == 0);
}

bool has_permission(const string &path) {
  struct stat attr;
  ::stat(path.c_str(), &attr);
  return (attr.st_mode & S_IROTH);
}

string get_abs_path(const string &path) {
  char *p = ::realpath(path.c_str(), nullptr);
  if (!p) {
    return "";
  }
  string abs_path(p);
  delete p;
  return abs_path;
}

bool escape_doc_root(const string &path, const string &doc_root) {
  string abs_path = get_abs_path(path);
  string abs_doc_root = get_abs_path(doc_root);
  return abs_path.size() < abs_doc_root.size() ||
         abs_path.substr(0, abs_doc_root.size()) != abs_doc_root;
}

string get_last_modified_time(const string &path) {
  struct stat attr;
  ::stat(path.c_str(), &attr);
  struct tm *tm = gmtime(&attr.st_mtime);
  char buf[256];
  int len = strftime(buf, 256, "%A, %d %m %Y %H:%M:%S GMT", tm);
  return string(buf, len);
}


string get_content_type(const string &path) {
  string ext = get_file_ext(path);
  if (ext == "html") {
    return "text/html";
  } else if (ext == "jpg" || ext == "jpeg") {
    return "image/jpeg";
  } else if (ext == "png") {
    return "image/png";
  } else {
    return "text/plain";
  }
}


void DieWithErr(const char *errorMessage)
{
    perror(errorMessage);
    exit(1);
}


