#include <core/wsclient/WebSocketClient.hpp>
#include <spdlog/spdlog.h>
#include <core/events/TradeEvent.hpp>
#include <iostream>

namespace bb
{
  template <typename T>
  WebSocketClient<T>::WebSocketClient() : m_ioc(),
                                       m_ctx{net::ssl::context::tlsv12},
                                       m_wss{net::make_strand(m_ioc), m_ctx},
                                       m_host{EnvReader::getReader()->getKey("WEBSOCKET_STREAM_HOST")},
                                       m_port{EnvReader::getReader()->getKey("WEBSOCKET_STREAM_PORT")}
  {
    spdlog::info("Connecting to websocket host: {} on port {}", m_host, m_port);
  }

  template <typename T>
  void WebSocketClient<T>::connect()
  {
    net::ip::tcp::resolver resolver(m_ioc);
    get_lowest_layer(m_wss).connect(resolver.resolve(m_host, m_port));

    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(m_wss.next_layer().native_handle(), m_host.c_str()))
    {
      throw beast::system_error(
          static_cast<int>(::ERR_get_error()),
          net::error::get_ssl_category());
    }

    // Set the expected hostname in the peer certificate for verification
    m_wss.next_layer().set_verify_callback(net::ssl::host_name_verification(m_host));

    // Perform the SSL handshake
    m_wss.next_layer().handshake(net::ssl::stream_base::client);

    m_wss.handshake(
        m_host,   // The Host field
        "/stream" // The request-target
    );
  }

  template <typename T>
  void WebSocketClient<T>::subscribe(nlohmann::json &intent)
  {
    m_wbuffer = intent.dump();
    m_wss.async_write(net::buffer(m_wbuffer), [this](auto ec, auto)
                      { if (ec) spdlog::error("write failed: {}", ec.message()); });
    read();
  }

  template <typename T>
  void WebSocketClient<T>::read()
  {
    m_wss.async_read(m_rbuffer, [this](beast::error_code ec, std::size_t)
                     {
                      if (ec) return;

                      std::string message{
                        static_cast<const char*>(m_rbuffer.data().data()),
                        m_rbuffer.size()
                      };

                      nlohmann::json response = nlohmann::json::parse(message);
                      if (!response["stream"].is_null())
                      {
                        T event {response["data"]};
                        m_producer->produce(event);
                        spdlog::info("{}", message);
                      }
                      
                      m_rbuffer.consume(m_rbuffer.size());  
                      this->read(); });
  }

  template <typename T>
  void WebSocketClient<T>::run()
  {
    m_ioc.run();
  }

  template <typename T>
  void WebSocketClient<T>::registerProducer(IProducer<T> *producer)
  {
    m_producer = producer;
  }

  template class WebSocketClient<TradeEvent>;
} // bb