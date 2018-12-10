#pragma once


#include <string>
#include <utility>


namespace crq
{

    class Response
    {
    public:

        Response(/*...*/);
        Response(Response &&res) noexcept;
        ~Response();

        Response& operator=(Response &&rhs) noexcept;
    };

    class Request
    {
    public:

        static Response get(::std::string url)
        {
            auto res = Response(/*...*/);
            return res;
        }
    };

}