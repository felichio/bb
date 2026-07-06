#pragma once

#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <spdlog/spdlog.h>
#include <util/EnvReader.hpp>
#include <nlohmann/json.hpp>
#include <core/producers/IProducer.hpp>

namespace bb
{

  namespace net = boost::asio;
  namespace beast = boost::beast;

  template <typename T>
  class WebSocketClient
  {
    net::io_context m_ioc;
    net::ssl::context m_ctx;
    beast::websocket::stream<net::ssl::stream<beast::tcp_stream>> m_wss;
    IProducer<T> *m_producer;

    std::string m_host;
    std::string m_port;
    beast::flat_buffer m_rbuffer;
    std::string m_wbuffer;

  public:
    WebSocketClient();
    void connect();
    void subscribe(nlohmann::json &intent);
    void read();
    void run();
    void registerProducer(IProducer<T> *producer);
  };

} // bb