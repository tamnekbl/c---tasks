#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <atomic>
#include <thread>


namespace fs = std::filesystem;

std::atomic_int lines_count;

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

std::string read_file(const std::string& filename)
{
    std::string s, line;
    std::ifstream infile(filename, std::ios::binary);
    while(std::getline(infile, line)){ 
        lines_count++; 
        s+=line;
    }
    infile.close();
    return s;
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

void LineCounter(const fs::path& f)
{
    read_file(f.string());
}

int main()
{

    std::vector<std::thread> pool;

    SetConsoleOutputCP(CP_UTF8);
    fs::path dirName = "C:\\Logs";
    auto start = std::chrono::high_resolution_clock::now();
    auto files = GetAllFiles(dirName);


    for (auto const& f:files){
        std::cout<<f<<"\n";
        pool.emplace_back(LineCounter, f);
    }
    for (auto& thread: pool)
        thread.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;
    std::cout << "Общее количество строк: " << lines_count << std::endl;
       
    return 0;
}