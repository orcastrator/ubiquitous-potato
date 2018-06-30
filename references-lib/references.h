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

	static std::string correction(const std::vector<std::string> referenceList, const std::string inputBook);
	static const std::regex validReference;
	// TODO: check if reference format is valid
	static const std::regex singleVerse;
	static const std::regex wholeChapter;
	static const std::regex wholeMultiChapter;
	static const std::regex multiVerseSingleChapter;
	static const std::regex multiVerseMultiChapter;

	static const size_t maxVersesInChapter = 176;
	static const size_t maxChapterInBook = 150;

private:
	static std::vector<std::string> known(std::vector<std::string> referenceList, std::vector<std::string> inputList);
	static std::vector<std::string> edit(const std::string input);
};

#endif REFERENCES_H