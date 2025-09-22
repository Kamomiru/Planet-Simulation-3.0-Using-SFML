#pragma once
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

std::string doubleToStringScientific(double value, int precision);
char pop_front(std::string& s);
void eraseLeadingWhitespace(std::string& s);
void eraseTrailingWhitespace(std::string& s);
std::string pop_frontWord(std::string& s);
std::string pop_backWord(std::string& s);
std::vector<std::string> splitWords(std::string s);
