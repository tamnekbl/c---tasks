//
#include <iostream>
#include <regex>
#include <cstdlib>

int main() {
    std::string log = "~[19640]; 2024-03-13; 20:08:01.038; ERROR; 0; Change log level to DEBUG";
    std::regex r_number("(\\d+)");
    std::smatch number_match;
    if (std::regex_search(log, number_match, r_number))
    {
        std::cout<<number_match.str();
    }
    

	//std::cout<<"Number:19640 Date:2024-03-13 Type:ERROR Message: Change log level to DEBUG";
}