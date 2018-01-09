#include <iostream>
#include <vector>
#include <regex>
#include <tuple>

#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include "references.h"

const std::regex BibleReference::validReference = std::regex("(\\d{0,1}[\\w\\s]+) (\\d+)[:]?(-)?(\\d+)?(-)?(\\d+)?[:]?(\\d+)?");
const std::regex BibleReference::singleVerse = std::regex("(\\d{0,1}[\\w\\s]+) (\\d+)[:](\\d+)");
const std::regex BibleReference::multiVerseSingleChapter = std::regex("(\\d{0,1}[\\w\\s]+) (\\d+)[:](\\d+)[-](\\d+)");
const std::regex BibleReference::multiVerseMultiChapter = std::regex("(\\d{0,1}[\\w\\s]+) (\\d+)[:](\\d+)[-](\\d+)[:](\\d+)");

/**
Possible variations of reference input
1 john 1 (could be either an entire chapter or just one verse depending on book)
1 john 1:1 supported
1 john 1:1-2 supported
1 john 1-2
1 john 1:1-1:2 supported
*/
std::pair<bible_reference, bible_reference> BibleReference::parseReference(const std::string inputReference) {
	std::smatch matches;
	if (std::regex_search(inputReference.begin(), inputReference.end(), matches, multiVerseMultiChapter)) {
		std::string book = matches[1];
		int chapterBegin = std::stoi(matches[2]);
		int verseBegin = std::stoi(matches[3]);
		int chapterEnd = std::stoi(matches[4]);
		int verseEnd = std::stoi(matches[5]);
		bible_reference begin = std::make_tuple(book, chapterBegin, verseBegin);
		bible_reference end = std::make_tuple(book, chapterEnd, verseEnd);
		return std::make_pair(begin, end);
	}
	else if (std::regex_search(inputReference.begin(), inputReference.end(), matches, multiVerseSingleChapter)){
		std::string book = matches[1];
		int chapter = std::stoi(matches[2]);
		int verseBegin = std::stoi(matches[3]);
		int verseEnd = std::stoi(matches[4]);
		bible_reference begin = std::make_tuple(book, chapter, verseBegin);
		bible_reference end = std::make_tuple(book, chapter, verseEnd);
		return std::make_pair(begin, end);
	}
	else if (std::regex_search(inputReference.begin(), inputReference.end(), matches, singleVerse)) {
		std::string book = matches[1];
		int chapter = std::stoi(matches[2]);
		int verse = std::stoi(matches[3]);
		bible_reference begin = std::make_tuple(book, chapter, verse);
		bible_reference end = std::make_tuple(book, chapter, verse);
		return std::make_pair(begin, end);
	}
	return std::make_pair(std::make_tuple("", 0, 0), std::make_tuple("", 0, 0)); // properly handle this error
}