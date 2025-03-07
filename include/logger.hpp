#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class Logger {
public:
    enum class Level { DEBUG, INFO, WARNING, ERROR, FATAL };

    Logger(const std::string& log_dir = "./log", const std::string& program_name = "log",
           bool should_clean = true, int retain_count = 3)
        : log_directory(log_dir), program_name(program_name), retain_count(retain_count) {
        if (!fs::exists(log_directory)) {
            fs::create_directories(log_directory);
        }
        clean_old_logs(should_clean);
        open_new_log();
    }

    ~Logger() {
        if (log_file.is_open()) {
            log_file.close();
        }
    }

    void log(Level level, const std::string& message) {
        std::string timestamp = get_timestamp();
        std::string level_str = get_level_string(level);
        std::string color = get_level_color(level);
        std::string reset_color = "\033[0m";

        // 输出到终端
        std::cout << color << "[" << timestamp << "] [" << program_name << "] [" << level_str << "] " 
                  << message << reset_color << std::endl;

        // 写入日志文件
        if (log_file.is_open()) {
            log_file << "[" << timestamp << "] [" << program_name << "] [" << level_str << "] " 
                     << message << std::endl;
        }
    }

    void debug(const std::string& message) { log(Level::DEBUG, message); }
    void info(const std::string& message) { log(Level::INFO, message); }
    void warning(const std::string& message) { log(Level::WARNING, message); }
    void error(const std::string& message) { log(Level::ERROR, message); }
    void fatal(const std::string& message) { log(Level::FATAL, message); }

private:
    std::string log_directory;
    std::string program_name;
    int retain_count;
    std::ofstream log_file;
    std::string log_filename;

    void open_new_log() {
        std::string timestamp = get_timestamp("%Y%m%d_%H%M%S");
        log_filename = log_directory + "/" + program_name + "_" + timestamp + ".log";
        log_file.open(log_filename, std::ios::out);
    }

    void clean_old_logs(bool should_clean) {
        if (!should_clean) return;

        std::vector<fs::path> log_files;
        for (const auto& entry : fs::directory_iterator(log_directory)) {
            if (entry.is_regular_file() && entry.path().filename().string().find(program_name) != std::string::npos) {
                log_files.push_back(entry.path());
            }
        }

        if (log_files.size() > retain_count) {
            std::sort(log_files.begin(), log_files.end(),
                      [](const fs::path& a, const fs::path& b) { return fs::last_write_time(a) < fs::last_write_time(b); });

            while (log_files.size() > retain_count) {
                fs::remove(log_files.front());
                log_files.erase(log_files.begin());
            }
        }
    }

    std::string get_timestamp(const std::string& format = "%Y-%m-%d %H:%M:%S") {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, format.c_str());
        return oss.str();
    }

    std::string get_level_string(Level level) {
        switch (level) {
            case Level::DEBUG: return "DEBUG";
            case Level::INFO: return "INFO";
            case Level::WARNING: return "WARNING";
            case Level::ERROR: return "ERROR";
            case Level::FATAL: return "FATAL";
        }
        return "UNKNOWN";
    }

    std::string get_level_color(Level level) {
        switch (level) {
            case Level::DEBUG: return "\033[36m";  // 青色
            case Level::INFO: return "\033[32m";   // 绿色
            case Level::WARNING: return "\033[33m"; // 黄色
            case Level::ERROR: return "\033[31m";   // 红色
            case Level::FATAL: return "\033[35m";   // 紫色
        }
        return "\033[0m";  // 默认颜色
    }
};

#endif // LOGGER_HPP
