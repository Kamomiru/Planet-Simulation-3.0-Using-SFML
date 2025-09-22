#include "stringHelper.h"


//Converts double to a String in scientific format
std::string doubleToStringScientific(double value, int precision) {
	std::ostringstream oss;
	oss << std::scientific << std::setprecision(precision) << value;
	return oss.str();
}

char pop_front(std::string& s) {
	char chr = s.front(); //get first char
	s.erase(s.begin());
	return chr;
}

void eraseLeadingWhitespace(std::string& s) {
	while (s[0] == ' ') {
		s.erase(0, 1);
	}
}

void eraseTrailingWhitespace(std::string& s) {
	while (s.back() == ' ' && !s.empty()) {
		s.erase(s.size() - 1, 1);
	}
}

std::string pop_frontWord(std::string& s) {
	std::string poppedWord;

	eraseLeadingWhitespace(s); //remove leading whitespace

	int firstWhitespacePos = s.find(" ");//find first whitespace (without leading whitespace)
	if (firstWhitespacePos == std::string::npos) { //if no whitespace found, the string is one word
		poppedWord = s; //return the whole string
		s.clear(); //clear the original string
		return poppedWord;
	}

	poppedWord = s.substr(0, firstWhitespacePos); //get all chars until first whitespace and append them to our return value
	s.erase(0, firstWhitespacePos + 1); //erase word and whitespace from original string

	return poppedWord;
}

std::string pop_backWord(std::string& s) {
	std::string poppedWord;

	eraseTrailingWhitespace(s); //remove leading whitespace

	int lastWhitespacePos = s.find_last_of(' ');//find first whitespace (without leading whitespace)
	if (lastWhitespacePos == std::string::npos) { //if no whitespace found, the string is one word
		poppedWord = s; //return the whole string
		s.clear(); //clear the original string
		return poppedWord;
	}

	poppedWord = s.substr(lastWhitespacePos + 1, s.size()); //get all chars until first whitespace and append them to our return value
	s.erase(lastWhitespacePos + 1, s.size() - 1); //erase word and whitespace from original string

	return poppedWord;
}

std::vector<std::string> splitWords(std::string s) {
	std::vector<std::string> words;

	while (s.size() > 0) {
		words.push_back(pop_frontWord(s)); //append all words to output vector
	}

	return words;
}
