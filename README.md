# CommonRequest

Simple package of HTTP request

### Interface

#### Global Functions

Function Name | Description
------------- | -----------
get           | make an HTTP GET request
post          | make an HTTP POST request

#### Request Class

##### Member Functions

Function Name | Description
------------- | -----------
get           | make an HTTP GET request
post          | make an HTTP POST request

#### Response Class

##### Member Functions

Function Name | Description
------------- | -----------
status_code   | returns the status code
raw           | returns the raw response message
reason        | returns the reason
body          | returns the body of response message
headers       | returns headers


### Usage
```cpp
// An example of GET method

#include <iostream>
#include "crq.hpp"

int main(int argc, char *argv[])
{
    auto response = crq::get("http://mil.sohu.com/");

    std::cout << response.raw() << std::endl;
    std::cout << response.status_code() << std::endl;
    std::cout << response.reason() << std::endl;
    std::cout << response.body() << std::endl;

    std::cout << "## HEADERS ##" << std::endl;

    for (auto &kv : response.headers())
        std::cout << kv.first << " : " << kv.second << std::endl;

    return 0;
}

```
