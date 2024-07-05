#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <atomic>
#include <thread>
#include <regex>
#include <mutex>

namespace fs = std::filesystem;

class ParserStatistics {
    private:
        std::string first_date = "9999-00-00 14:42:10.222",
                    last_date = "0000-00-00 14:42:10.222";
        size_t count_lines = 0,
               count_trace = 0,
               count_info = 0,
               count_debug = 0,
               count_warn = 0,
               count_error = 0;
        std::mutex M;
    public:
        void Parsing(const std::vector<std::string>& lines);
        std::string GetPeriod() { return first_date + " --- " + last_date; };
        size_t GetLines() { return count_lines; };
        size_t GetTrace() { return count_trace; };
        size_t GetInfo()  { return count_info;  };
        size_t GetDebug() { return count_debug; };
        size_t GetWarn()  { return count_warn;  };
        size_t GetError() { return count_error; };
        
};

void ParserStatistics::Parsing(const std::vector<std::string> & lines)
{
    std::regex reg("(~#?\\[\\s*\\d+\\]); (\\d{4}-\\d{2}-\\d{2}); (\\d{2}:\\d{2}:\\d{2}\\.\\d{3}); (TRACE|INFO|DEBUG|WARN|ERROR); (\\d); (.*)");

    for (std::string line : lines)
    {
        line = line.length() > 6000? line.substr(0, 6800):line;
        std::smatch results;
        if (std::regex_match(line, results, reg))
        {
            std::lock_guard<std::mutex> lock(M);
            std::string date = results[2].str() + " " + results[3].str();
            if (first_date > date) first_date = date;
            if (last_date < date) last_date = date;
                 if (results[4] == "INFO" ) count_info++;
            else if (results[4] == "DEBUG") count_debug++;
            else if (results[4] == "TRACE") count_trace++;
            else if (results[4] == "WARN" ) count_warn++;
            else if (results[4] == "ERROR") count_error++;
            count_lines++;
        }
    }
}

void PrintStatistics(ParserStatistics &stat) {
    std::cout << "=========================STATISTICS=========================\n";
    std::cout << "\tPERIOD:         \t" << stat.GetPeriod() << "\n";
    std::cout << "\tNUMBER OF LINES:\t" << stat.GetLines()  << "\n";
    std::cout << "\tNUMBER OF TRACE:\t" << stat.GetTrace()  << "\n"; 
    std::cout << "\tNUMBER OF INFO: \t" << stat.GetInfo()   << "\n";
    std::cout << "\tNUMBER OF DEBUG:\t" << stat.GetDebug()  << "\n";
    std::cout << "\tNUMBER OF WARN: \t" << stat.GetWarn()   << "\n";
    std::cout << "\tNUMBER OF ERROR:\t" << stat.GetError()  << "\n"; 
    std::cout << "============================================================\n";
}

std::vector<fs::path> GetAllFiles(const fs::path& dirName)
{
    std::vector<fs::path> result;
    fs::directory_entry entry{dirName};
    if (entry.exists())
    {
        for (auto const& dir_entry : fs::recursive_directory_iterator(dirName))
            if ((dir_entry.path().extension() == ".log"))
            {
                result.push_back(dir_entry.path());
            } 
        return result;
    }
    return std::vector<fs::path>();
}

std::vector<std::string> read_file(const std::string& filename)
{
    std::vector<std::string> res;
    std::string line;
    std::ifstream infile(filename);
    while(std::getline(infile, line)){ 
        res.push_back(line);
    }
    infile.close();
    return res;
}

void LineCounter(const fs::path& f, ParserStatistics& pst)
{
    auto data = read_file(f.string());
    pst.Parsing(data);
}

int main()
{

    std::vector<std::thread> pool;
    ParserStatistics stats;
    SetConsoleOutputCP(CP_UTF8);
    fs::path dirName = "c:\\log";
    auto start = std::chrono::high_resolution_clock::now();
    auto files = GetAllFiles(dirName);
    
    for (auto const& f:files){
        std::cout<<f<<"\n";
        pool.emplace_back(LineCounter, f, std::ref(stats));
    }
    for (auto& thread: pool)
        thread.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    PrintStatistics(stats);
    std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;
    
       
    return 0;
}