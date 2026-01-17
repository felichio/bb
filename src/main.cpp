#include <iostream>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>

int main(int argc, char *argv[])
{
  std::cout << "bb" << std::endl;
  boost::asio::io_context io_context;
  boost::beast::flat_buffer buffer;
}