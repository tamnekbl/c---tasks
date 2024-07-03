/*#include <filesystem>
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <future>

namespace fs = std::filesystem;

std::atomic_int lines_count{0};

// Пул потоков
class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    template<class F>
    auto enqueue(F&& f) -> std::future<typename std::result_of<F()>::type>;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            for (;;) {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty())
                        return;
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers)
        worker.join();
}

template<class F>
auto ThreadPool::enqueue(F&& f) -> std::future<typename std::result_of<F()>::type> {
    using return_type = typename std::result_of<F()>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(std::forward<F>(f));

    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task]() { (*task)(); });
    }
    condition.notify_one();
    return res;
}

std::vector<fs::path> GetAllFiles(const fs::path& dirName) {
    std::vector<fs::path> result;
    fs::directory_entry entry{dirName};
    if (entry.exists()) {
        for (const auto& dir_entry : fs::recursive_directory_iterator(dirName)) {
            if (!dir_entry.is_directory()) {
                // Игнорируем архивы по расширению
                if (dir_entry.path().extension() == ".zip" || dir_entry.path().extension() == ".rar" ||
                    dir_entry.path().extension() == ".7z" || dir_entry.path().extension() == ".tar") {
                    continue;
                }
                result.push_back(dir_entry.path());
            }
        }
    }
    return result;
}

void ProcessChunk(const std::vector<char>& data, size_t start, size_t end) {
    size_t lineCount = 0;
    std::string tmp;

    for (size_t i = start; i < end; ++i) {
        if (data[i] != '\n') {
            tmp += data[i];
        } else {
            ++lineCount;
            tmp.clear();
        }
    }

    // Обрабатываем последнюю строку, если она не завершена новой строкой
    if (!tmp.empty()) {
        ++lineCount;
    }

    lines_count += lineCount;
}

void LineCounter(const fs::path& filePath) {
    const size_t chunkSize = 4096;
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }

    std::vector<char> buffer(chunkSize);
    while (file) {
        file.read(buffer.data(), chunkSize);
        std::streamsize bytesRead = file.gcount();
        if (bytesRead > 0) {
            ProcessChunk(buffer, 0, static_cast<size_t>(bytesRead));
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    fs::path dirName = "C:\\log";
    auto start = std::chrono::high_resolution_clock::now();

    auto files = GetAllFiles(dirName);

    const size_t numThreads = std::thread::hardware_concurrency();
    ThreadPool pool(numThreads);

    std::vector<std::future<void>> results;
    for (const auto& file : files) {
        results.emplace_back(pool.enqueue([file] { LineCounter(file); }));
    }

    for (auto &&result : results) {
        result.get();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;
    std::cout << "Общее количество строк: " << lines_count.load() << std::endl;

    return 0;
}*/

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

namespace fs = std::filesystem;

std::atomic<int> count(0);
std::mutex mtx;

std::vector<fs::path> GetAllFiles(const fs::path& dirName) {
    std::vector<fs::path> fileList;
    if (fs::exists(dirName) && fs::is_directory(dirName)) {
        for (const auto& entry : fs::recursive_directory_iterator(dirName)) {
            if (fs::is_regular_file(entry.path())) {
                fileList.push_back(entry.path());
            }
        }
    }
    return fileList;
}

void LinesCounter(const fs::path& filePath) {
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        count++;
    }
}

int main() {
    const std::string dirName = "C:\\Log";
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<fs::path> files = GetAllFiles(dirName);
    for (const auto& file : files) {
        threads.emplace_back(LinesCounter, file);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Время выполнения: " << elapsed.count() << " секунд" << std::endl;
    std::cout << "Общее количество строк: " << count.load() << std::endl;

    return 0;
}
