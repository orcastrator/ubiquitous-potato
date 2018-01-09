#include <iostream>
#include <vector>
#include <regex>
#include <tuple>

#include <restbed>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <references-lib/references.h>

boost::property_tree::ptree root;

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
				verseEnd = 176; // magic number for number of verses in psalm 119
			for (int j = verseBegin; j <= verseEnd; j++) {
				try {
					std::string ref = std::get<0>(referenceEnds.first) + "." + std::to_string(i) + "." + std::to_string(j);
					std::string verse = root.get<std::string>(ref);
					content += verse + " ";
				}
				catch (std::exception& e) {
					std::cout << e.what() << std::endl;
					break;
				}
			}
			content += "\r\n \r\n \r\n";
		}
		if (content.empty())
			content = "Unable to find verse at the provided reference";
		session->close(restbed::OK, content, { { "Content-Length", std::to_string(content.length()) },{ "Connection", "close" } });
	});
}

void get_help_handler(const std::shared_ptr<restbed::Session>& session) {
	const auto request = session->get_request();

	size_t content_length = request->get_header("Content-Length", 0);

	session->fetch(content_length, [request](const std::shared_ptr<restbed::Session> session, const restbed::Bytes & body)
	{
		std::string content = "To use this website, navigate to /verses?reference=<insert-reference-here>\nFor example: /verses?reference=Genesis 1:1\n";

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
	int port;
	std::cin >> port;
	std::string pathToBible;
	std::cin >> pathToBible;
	std::cout << "Reading Bible from path: " << pathToBible << std::endl;
	boost::property_tree::read_json(pathToBible, root);

	std::vector<std::shared_ptr<restbed::Resource>> handlers;
	
	handlers.push_back(createHandler("/hello", "GET", hello_handler));
	handlers.push_back(createHandler("/verse", "GET", get_bible_verse_handler));
	handlers.push_back(createHandler("/verses", "GET", get_bible_verses_handler));
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
