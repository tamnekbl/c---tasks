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
        std::string period = "";
        std::atomic_size_t count_lines = 0,
               count_trace = 0,
               count_info = 0,
               count_debug = 0,
               count_warn = 0,
               count_error = 0;
    public:
        void Parsing(const std::vector<std::string>& lines);
        std::string GetPeriod() { return period; };
        size_t GetLines() { return count_lines; };
        size_t GetTrace() { return count_trace; };
        size_t GetInfo()  { return count_info;  };
        size_t GetDebug() { return count_debug; };
        size_t GetWarn()  { return count_warn;  };
        size_t GetError() { return count_error; };
        
};

void ParserStatistics::Parsing(const std::vector<std::string> & lines)
{
    std::regex reg("(~#?\\[\\s*\\d+\\]); (\\d{4}-\\d{2}-\\d{2}); (\\d{2}:\\d{2}:\\d{2}\\.\\d{3}); (TRACE|INFO|DEBUG|WARN|ERROR); (\\d); (.*)\\r");
    std::string first_date = "";
    std::string last_date = "";

    for (std::string line : lines)
    {
        line = line.length() > 6000? line.substr(0, 6800):line;
        std::smatch results;
        if (std::regex_match(line, results, reg))
        {
            if (first_date == "") first_date = results[2].str() + " " + results[3].str();
            last_date = results[2].str() + " " + results[3].str();
                 if (results[4] == "INFO" ) count_info++;
            else if (results[4] == "DEBUG") count_debug++;
            else if (results[4] == "TRACE") count_trace++;
            else if (results[4] == "WARN" ) count_warn++;
            else if (results[4] == "ERROR") count_error++;
            count_lines++;
        }
    }
    period = first_date + " --- " + last_date;
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

std::vector<char> read_file_chunk(const std::string& filename)
{
    std::ifstream infile(filename, std::ios::binary);
    if (!infile) {
        throw std::runtime_error("Failed to open file");
    }
    infile.seekg(0, std::ios::end);
    const size_t file_size_in_byte = infile.tellg();
    std::vector<char> data(file_size_in_byte);
    infile.seekg(0, std::ios::beg);

    const short int chunk_size = 4096;
    size_t chunks = file_size_in_byte / chunk_size;
    size_t chunk_rest = file_size_in_byte % chunk_size;
    for (size_t i = 0; i < chunks; ++i)
    {
        infile.seekg(i*chunk_size, std::ios::beg);
        infile.read(data.data()+(i * chunk_size), chunk_size);
    }
    
    infile.read(data.data()+(chunks * chunk_size), chunk_rest);
    return data;
}

std::vector<std::string> make_lines(const std::vector<char>& data)
{
    std::vector<std::string> stringList;
    stringList.reserve(std::count(data.begin(), data.end(), '\n') + 1);

    std::string tmp;
    for (char ch : data)
    {
        if (ch != '\n')
        {
            tmp += ch;
        }
        else
        {
            stringList.push_back(tmp);
            tmp.clear();
        }
    }
    if (!tmp.empty())
    {
        stringList.emplace_back(tmp);
    }

    return stringList; 
}

void LineCounter(const fs::path& f, ParserStatistics& pst)
{
    auto data = make_lines(read_file_chunk(f.string()));
    pst.Parsing(data);
}

int main()
{

    std::vector<std::thread> pool;
    ParserStatistics stats;
    SetConsoleOutputCP(CP_UTF8);
    fs::path dirName = "C:\\log";
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