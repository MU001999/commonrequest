#pragma once


#include <string>
#include <utility>
#include <algorithm>

#include <cstring>

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>


#ifdef DEBUG
#include <iostream>
#endif // DEBUG


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
            
            return *this;
        }

        Response& operator=(Response &&rhs) noexcept
        {
            ::std::swap(url,         rhs.url);
            ::std::swap(status_code, rhs.status_code);
            ::std::swap(reason,      rhs.reason);
            ::std::swap(charset,     rhs.charset);
            ::std::swap(text,        rhs.text);
        
            return *this;
        }
    };


    class Request
    {
    private:

        static ::std::string gen_host(::std::string url)
        {
            auto pos = url.find("/");
            return pos == url.npos ? url : url.substr(0, pos);
        }

        static ::std::string gen_req(::std::string url)
        {
            auto pos = url.find('/');
            return pos == url.npos ? "/" : url.substr(pos);
        }

        static Response request(const ::std::string url, const ::std::string method)
        {
            ::std::string response_msg;


            // init server_addr
            addrinfo hints, *result;
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;
            getaddrinfo(gen_host(url).c_str(), "80", &hints, &result);


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
            ::std::string request_msg = " HTTP/1.1\r\n"
                "Host: " + gen_host(url) + "\r\n"
                "Accept-Encoding: gzip, deflate, br\r\n"
                "Accept-Language: zh-CN,zh,en-US\r\n"
                "Connection: keep-alive\r\n"
                "User-Agent: Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)\r\n\r\n";
            request_msg = method + gen_req(url) + request_msg;
            
#ifdef DEBUG
            std::cout << request_msg << std::endl;
#endif // DEBUG

            send(socket_fd, request_msg.c_str(), request_msg.length(), 0);


            // receive response message from server
            char tmp[65536] = { 0 };

            while (recv(socket_fd, tmp, 65536, 0) > 0)
            {
                response_msg += tmp;
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