#include <iostream>
#include <regex>
#include <string>

int main() {
    std::string log_line = "~#[1]; 2024-02-26; 14:42:10.222; WARN; 0; Service pack string: ";

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
