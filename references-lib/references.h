#ifndef REFERENCES_H
#define REFERENCES_H

#include <iostream>
#include <vector>
#include <regex>
#include <tuple>

#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

//boost::property_tree::ptree root;
typedef std::tuple<std::string, int, int> bible_reference;

class BibleReference {
public:
	static std::pair<bible_reference, bible_reference> parseReference(const std::string inputReference);
	static const std::regex validReference;
	// TODO: check if reference format is valid
	static const std::regex singleVerse;
	static const std::regex multiVerseSingleChapter;
	static const std::regex multiVerseMultiChapter;
private:

};

#endif REFERENCES_H