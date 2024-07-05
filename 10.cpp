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
            if (dir_entry.is_regular_file())
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
        lines_count++; 
        res.push_back(line);
    }
    infile.close();
    return res;
}

void LineCounter(const fs::path& f)
{
    read_file(f.string());
}

int main()
{

    std::vector<std::thread> pool;

    SetConsoleOutputCP(CP_UTF8);
    fs::path dirName = "C:\\Log";
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