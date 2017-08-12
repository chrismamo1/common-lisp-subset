/**
 * credit for these 3 quick 'n dirty utility functions goes to StackOverflow
 * user Evan Teran in the thread
 * <a href="http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring">here</a>.
 */

#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>

#ifndef _UTIL_HPP
#define _UTIL_HPP

// trim from start
void ltrim(std::string &s);

// trim from end
void rtrim(std::string &s);

// trim from both ends
void trim(std::string &s);

#endif
