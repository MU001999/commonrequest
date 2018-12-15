#include "crq.hpp"
#include <iostream>


using namespace std;


int main(int argc, char *argv[])
{
    auto res = crq::Request::get("www.baidu.com");

    cout << res.raw << endl;

    return 0;
}