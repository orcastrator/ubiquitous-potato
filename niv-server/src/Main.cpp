#include <iostream>
#include <vector>
#include <regex>
#include <tuple>

#include <restbed>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <docopt/docopt.h>

#include <references-lib/references.h>

boost::property_tree::ptree root;
std::vector<std::string> booksOfTheBible;

void hello_handler(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	size_t content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [request](const std::shared_ptr<restbed::Session> session, const restbed::Bytes & body)
	{
		session->close(restbed::OK, "Hello, World!", { { "Content-Length", "13" },{ "Connection", "close" } });
	});
}

void get_bible_verse_handler(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	size_t content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [request](const std::shared_ptr<restbed::Session> session, const restbed::Bytes & body)
	{
		std::string reference = request->get_query_parameter("reference", "");
		std::string content = root.get<std::string>(reference, "");

		session->close(restbed::OK, content, { { "Content-Length", std::to_string(content.length()) },{ "Connection", "close" } });
	});
}

void get_bible_verses_handler(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	size_t content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [request](const std::shared_ptr<restbed::Session> session, const restbed::Bytes & body)
	{
		std::string reference = request->get_query_parameter("reference", "");

		std::pair<bible_reference, bible_reference> referenceEnds = BibleReference::parseReference(reference);

		std::string content = "";
		for (int i = std::get<1>(referenceEnds.first); i <= std::get<1>(referenceEnds.second); i++) {
			int verseBegin, verseEnd;
			if (i == std::get<1>(referenceEnds.first))
				verseBegin = std::get<2>(referenceEnds.first); // first chapter does not always start at verse 1
			else
				verseBegin = 1;
			if (i == std::get<1>(referenceEnds.second))
				verseEnd = std::get<2>(referenceEnds.second); // last chapter does not always end at end of chapter
			else
				verseEnd = BibleReference::maxVersesInChapter; // magic number for number of verses in psalm 119
			for (int j = verseBegin; j <= verseEnd; j++) {
				std::string ref;
				try {
					ref = std::get<0>(referenceEnds.first) + "." + std::to_string(i) + "." + std::to_string(j);
					std::string testBookName = root.get<std::string>(std::get<0>(referenceEnds.first));
				}
				catch (std::exception& e) {
					std::cout << e.what() << std::endl;
					// Get possible corrections
					std::string correction = BibleReference::correction(booksOfTheBible, std::get<0>(referenceEnds.first));
					if (!correction.empty())
						content = "Unable to find " + std::get<0>(referenceEnds.first) + ". Did you mean " + correction + "?";
					if (content.empty())
						content = "Unable to find verse at the provided reference: " + reference;
					break;
				}
				try {
					std::string verse = root.get<std::string>(ref);
					content += verse + " ";
				}
				catch (std::exception& e) {
					std::cout << e.what() << std::endl;
					if (content.empty())
						content = "Unable to find verse at the provided reference: " + reference;
					break;
				}
			}
			content += "\r\n \r\n \r\n";
		}
		session->close(restbed::OK, content, { { "Content-Length", std::to_string(content.length()) },{ "Connection", "close" } });
	});
}

void get_bible_books_handler(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	size_t content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [request](const std::shared_ptr<restbed::Session> session, const restbed::Bytes & body)
	{
		std::string content = "";
		for (const auto &book : booksOfTheBible) {
			content += book;
			content += "\n";
		}

		session->close(restbed::OK, content, { { "Content-Length", std::to_string(content.length()) },{ "Connection", "close" } });
	});
}

void get_help_handler(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	size_t content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [request](const std::shared_ptr<restbed::Session> session, const restbed::Bytes & body)
	{
		std::string content = "To use this website, navigate to /verses?reference=<insert-reference-here>\nFor example: /verses?reference=Genesis 1:1\n\
To retrieve a list of the books of the bible, navigate to /books";

		session->close(restbed::OK, content, { { "Content-Length", std::to_string(content.length()) },{ "Connection", "close" } });
	});
}

std::shared_ptr<restbed::Resource> createHandler(std::string path, std::string method, const std::function<void(std::shared_ptr<restbed::Session>)>& callback) {
	auto resource = std::make_shared<restbed::Resource>();
	resource->set_path(path);
	resource->set_method_handler(method, callback);
	return resource;
}

int main(int argC, char* argV[]) {
	std::string usage = R"(NIV 1984 Server
	Usage:
		niv-server [--config-path=<path> --port=<port> --path-to-bible=<bib>]
		niv-server (-h | --help)
		niv-server --version

	Options:
		--config-path=<path>	Path to config file in json format
		--port=<port>			Port where REST API will be served [default: 4000]
		--path-to-bible=<bib>	Location of Bible file in json format
		-h --help				Show this screen.
		--version				Show version.
)";

	std::map<std::string, docopt::value> args = docopt::docopt(
		usage,
		{ argV + 1, argV + argC },
		true,	// show help if requested
		"1.0");	// version string

	std::string configPath = args["--config-path"] ? args["--config-path"].asString() : "";
	int port = args["--port"] ? args["--port"].asLong() : 4000;
	std::string pathToBible = args["--path-to-bible"] ? args["--path-to-bible"].asString() : "";

	std::cout << "Reading Bible from path: " << pathToBible << std::endl;
	try {
		boost::property_tree::read_json(pathToBible, root);
		for (const auto &book : root.get_child("")) {
			booksOfTheBible.push_back(book.first.data());
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Unable to parse bible json file: " << e.what() << std::endl;
		return 1;
	}

	std::vector<std::shared_ptr<restbed::Resource>> handlers;
	
	handlers.push_back(createHandler("/hello", "GET", hello_handler));
	handlers.push_back(createHandler("/verse", "GET", get_bible_verse_handler));
	handlers.push_back(createHandler("/verses", "GET", get_bible_verses_handler));
	handlers.push_back(createHandler("/books", "GET", get_bible_books_handler));
	handlers.push_back(createHandler("/help", "GET", get_help_handler));

	auto settings = std::make_shared<restbed::Settings>();
	settings->set_port(port);
	settings->set_default_header("Connection", "close");

	restbed::Service service;
	for (auto handler : handlers) {
		service.publish(handler);
	}
	std::cout << "Listening on port " << port << std::endl;
	service.start(settings);

	return 0;
}
