#include "crq.hpp"
#include <iostream>


using namespace std;


int main(int argc, char *argv[])
{
    auto res = crq::Request::get("http://mil.sohu.com/");
    //auto res = crq::Request::get("www.baidu.com");
    //if (res.status_code == 302) res = crq::Request::get(res.headers["Location"], {
    //    {"Cookie", res.headers["Set-Cookie"]},
    //    });

    cout << res.status_code() << endl;
    cout << res.reason() << endl;
    cout << res.body() << endl;

    return 0;
}