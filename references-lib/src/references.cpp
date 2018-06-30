#include <iostream>
#include <vector>
#include <regex>
#include <tuple>
#include <algorithm>

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

std::string BibleReference::correction(const std::vector<std::string> referenceList, const std::string inputBook) {
	std::string outputString;

	std::vector<std::string> oneEdit = edit(inputBook);
	std::vector<std::string> corrections = known(referenceList, edit(inputBook));

	if (corrections.size() > 1)
		outputString = corrections[0];

	/*import re
	from collections import Counter

	def words(text) : return re.findall(r'\w+', text.lower())

	WORDS = Counter(words(open('big.txt').read()))

	def P(word, N = sum(WORDS.values())) :
	"Probability of `word`."
	return WORDS[word] / N

	def correction(word) :
	"Most probable spelling correction for word."
	return max(candidates(word), key = P)

	def candidates(word) :
	"Generate possible spelling corrections for word."
	return (known([word]) or known(edits1(word)) or known(edits2(word)) or [word])

	def known(words) :
	"The subset of `words` that appear in the dictionary of WORDS."
	return set(w for w in words if w in WORDS)

	def edits1(word) :
	"All edits that are one edit away from `word`."
	letters = 'abcdefghijklmnopqrstuvwxyz'
	splits = [(word[:i], word[i:])    for i in range(len(word) + 1)]
	deletes = [L + R[1:]               for L, R in splits if R]
	transposes = [L + R[1] + R[0] + R[2:] for L, R in splits if len(R)>1]
	replaces = [L + c + R[1:]           for L, R in splits if R for c in letters]
	inserts = [L + c + R               for L, R in splits for c in letters]
	return set(deletes + transposes + replaces + inserts)

	def edits2(word) :
	"All edits that are two edits away from `word`."
	return (e2 for e1 in edits1(word) for e2 in edits1(e1))*/

	return outputString;
}

std::vector<std::string> BibleReference::known(std::vector<std::string> referenceList, std::vector<std::string> inputList) {
	std::sort(referenceList.begin(), referenceList.end());
	std::sort(inputList.begin(), inputList.end());

	std::vector<std::string> known(referenceList.size() + inputList.size());
	std::vector<std::string>::iterator it;
	it = std::set_intersection(referenceList.begin(), referenceList.end(), inputList.begin(), inputList.end(), known.begin());
	known.resize(it - known.begin());
	return known;
}

// All edits that are one edit away from `word`."
std::vector<std::string> BibleReference::edit(const std::string word) {
	std::string characters = "1234567890 abcdefghijklmnopqrstuvwxyz";
	std::vector<std::string> output;
	std::vector<std::string> deletes;
	std::vector<std::string> transposes;
	std::vector<std::string> replaces;
	std::vector<std::string> inserts;
	for (int i = 0; i <= word.size(); i++) {
		if (i != word.size()) {
			deletes.push_back(word.substr(0, i) + word.substr(i + 1, word.size() - i));
			for (int j = 0; j < word.size(); j++) {
				if (word[i] != word[j]) {
					std::string modifiedWord = word;
					char modified = modifiedWord[i];
					modifiedWord[i] = modifiedWord[j];
					modifiedWord[j] = modified;
					transposes.push_back(modifiedWord);
				}
			}
			for (int k = 0; k < characters.size(); k++) {
				replaces.push_back(word.substr(0, i) + characters[k] + word.substr(i + 1, word.size() - i - 1));
				inserts.push_back(word.substr(0, i) + characters[k] + word.substr(i, word.size() - i));
			}
		}
		for (int k = 0; k < characters.size(); k++) {
			inserts.push_back(word.substr(0, i) + characters[k] + word.substr(i, word.size() - i));
		}
	}

	//for (auto one : deletes)
	//	std::cout << one << std::endl;
	//for (auto one : transposes)
	//	std::cout << one << std::endl;
	//for (auto one : replaces)
	//	std::cout << one << std::endl;
	//for (auto one : inserts)
	//	std::cout << one << std::endl;

	output.reserve(deletes.size() + transposes.size() + replaces.size() + inserts.size());
	output.insert(output.end(), deletes.begin(), deletes.end());
	output.insert(output.end(), transposes.begin(), transposes.end());
	output.insert(output.end(), replaces.begin(), replaces.end());
	output.insert(output.end(), inserts.begin(), inserts.end());
	return output;
}
