#pragma once


#include <string>
#include <utility>
#include <algorithm>

#include <cstring>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>


namespace crq
{

    class Response
    {
    public:

        ::std::string url;

        int           status_code;
        ::std::string reason;
        
        ::std::string charset;
        ::std::string text;

        ::std::string raw;


        Response() {}

        Response(::std::string &data)
        {
            raw = data;
        }

        Response(Response &&res) noexcept = default;
        ~Response() = default;


        Response& operator=(Response &rhs)
        {
            url         = rhs.url;
            status_code = rhs.status_code;
            reason      = rhs.reason;
            charset     = rhs.charset;
            text        = rhs.text;
        }

        Response& operator=(Response &&rhs) noexcept
        {
            ::std::swap(url,         rhs.url);
            ::std::swap(status_code, rhs.status_code);
            ::std::swap(reason,      rhs.reason);
            ::std::swap(charset,     rhs.charset);
            ::std::swap(text,        rhs.text);
        }
    };


    class Request
    {
    private:

        static ::std::string gen_req(::std::string url)
        {

        }

        static Response request(const ::std::string url, const ::std::string method)
        {
            ::std::string response_msg;

            // init server_addr
            addrinfo hints = {}, *result;
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            getaddrinfo(url.c_str(), "80", &hints, &result);

            // connect to server
            auto socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            for (auto addr = result; addr != NULL; addr = addr->ai_next)
            {
                if (connect(socket_fd, addr->ai_addr, addr->ai_addrlen) == 0)
                {
                    break;
                }
            }

            // send request message to server
            {
                ::std::string request_msg = " HTTP/1.1\r\n"
                    "Connection: keep-alive\r\n"
                    "Accept-Encoding: gzip, deflate, br\r\n"
                    "Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2\r\n"
                    "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:63.0) Gecko/20100101 Firefox/63.0\r\n";
                request_msg = method + gen_req(url) + request_msg;

                send(socket_fd, request_msg.c_str(), request_msg.length(), 0);
            }

            // receive response message from server
            {
                char tmp[4096] = { 0 };

                while (recv(socket_fd, tmp, 4096, 0) > 0)
                {
                    response_msg += tmp;
                }
            }

            freeaddrinfo(result);

            close(socket_fd);

            return Response(response_msg);
        }

    public:

        static Response get(::std::string url)
        {
            return request(url, "GET ");
        }
        
    };

}