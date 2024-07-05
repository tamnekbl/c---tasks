#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string log_line = "~[19640]; 2024-03-13; 20:08:01.038; ERROR; 0; Change log level to DEBUG";

    // Регулярное выражение для разбора строки
    std::regex log_pattern(R"~(\~#?\[\s*(\d+)\]; (\d{4}-\d{2}-\d{2}); \d{2}:\d{2}:\d{2}\.\d{3}; (TRACE|INFO|DEBUG|WARN|ERROR); \d; (.+))~");

    std::smatch match;
    if (std::regex_match(log_line, match, log_pattern)) {
        std::string number = match[1];
        std::string date = match[2];
        std::string log_type = match[3];
        std::string message = match[4];

        std::cout << "Number:" << number << " Date:" << date << " Type:" << log_type << " Message:" << message << std::endl;
    } else {
        std::cout << "Строка не соответствует ожидаемому формату" << std::endl;
    }

    return 0;
}
