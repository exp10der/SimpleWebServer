#include <cpprest/http_listener.h>
#include <cpprest/http_client.h>
#include <future>
#include <chrono>

using namespace web::http::experimental::listener;
using namespace web::http::client;
using namespace web::http;
using namespace web;
using namespace utility;

void handle(http_request request) {
	request.reply(status_codes::OK,
		"<html><head></head><body><h1>Simple web server</h1></body></html>", "text/html; charset=UTF-8");
}

int main()
{
	http_listener listener(U("http://localhost:3000"));
	listener
		.open()
		.then([&listener]()
		{
			listener.support(methods::GET, std::bind(&handle, std::placeholders::_1));
		})
		.wait();


	std::thread task([]() {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(5));

			http_client client(U("http://localhost:3000"));
			client
				.request(methods::GET)
				.then([](http_response response) { return response.extract_string(); })
				.then([](string_t content) { std::wcout << U("Content: ") << content << std::endl; });
		}
	});


	std::promise<void>().get_future().wait();

	return 1;
}