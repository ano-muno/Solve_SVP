#pragma once
#include "StringFormat.h"
#include "FileLogger.h"
#include <iostream>
#include <string>
#include <mutex>

#include "Util.hpp"
/* Loggerクラス宣言 */
class Logger
{
public:
    /* 書式指定Informationログ */
    template<typename ... Args>
    static void Trivial(const std::string& format, Args&& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのInformationログ出力呼び出し */
        Logger::GetInstance().LogTrivial(StringFormat(format, std::forward<Args>(args) ...));
    }
    /* 書式指定Informationログ */
    template<typename ... Args>
    static void Info(const std::string& format, Args&& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのInformationログ出力呼び出し */
        Logger::GetInstance().LogInfo(StringFormat(format, std::forward<Args>(args) ...));
    }
    /* 書式指定Informationログ */
    template<typename ... Args>
    static void Fatal(const std::string& format, Args&& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのInformationログ出力呼び出し */
        Logger::GetInstance().LogFatal(StringFormat(format, std::forward<Args>(args) ...));
    }
    /* 書式指定Warningログ */
    template<typename ... Args>
    static void Warn(const std::string& format, Args&& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのWarningログ出力呼び出し */
        Logger::GetInstance().LogWarn(StringFormat(format, std::forward<Args>(args) ...));
    }
    /* 書式指定Errorログ */
    template<typename ... Args>
    static void Error(const std::string& format, Args&& ... args)
    {
        /* 書式フォーマットしてシングルトンインスタンスのErrorログ出力呼び出し */
        Logger::GetInstance().LogError(StringFormat(format, std::forward<Args>(args) ...));
    }
private:
    /* シングルトンインスタンス取得 */
    static Logger& GetInstance();
private:
    /* コンストラクタ */
    Logger();
    /* デストラクタ */
    ~Logger();
public:
    /* コピーコンストラクタを削除 */
    Logger(const Logger&) = delete;
    /* ムーブコンストラクタを削除 */
    Logger(Logger&&) = delete;
    /* コピー代入オペレータを削除 */
    Logger& operator=(const Logger&) = delete;
    /* ムーブ代入オペレータを削除 */
    Logger& operator=(Logger&&) = delete;
private:
    /* Trivialログ出力 */
    void LogTrivial(const std::string& message);
    /* Informationログ出力 */
    void LogInfo(const std::string& message);
    /* Fatalログ出力 */
    void LogFatal(const std::string& message);
    /* Warningログ出力 */
    void LogWarn(const std::string& message);
    /* Errorログ出力 */
    void LogError(const std::string& message);
private:
    /* ミューテックス */
    std::mutex m_Mutex;
    /* File Loggerクラスインスタンス */
    FileLogger m_FileLogger;
};

/* コンソール出力文字色制御コード */
constexpr const char* ConsoleColorReset     = "\x1b[0m";
constexpr const char* ConsoleColorBlack     = "\x1b[30m";
constexpr const char* ConsoleColorRed       = "\x1b[31m";
constexpr const char* ConsoleColorGreen     = "\x1b[32m";
constexpr const char* ConsoleColorYellow    = "\x1b[33m";
constexpr const char* ConsoleColorBlue      = "\x1b[34m";
constexpr const char* ConsoleColorMagenta   = "\x1b[35m";
constexpr const char* ConsoleColorCyan      = "\x1b[36m";
constexpr const char* ConsoleColorWhite     = "\x1b[37m";
/* シングルトンインスタンス取得 */
Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}
/* コンストラクタ */
Logger::Logger()
    : m_Mutex()
    , m_FileLogger()
{
    /* ログファイルオープン */
    this->m_FileLogger.Open("Log/log.txt");
}
/* デストラクタ */
Logger::~Logger()
{
    /* ログファイルクローズ */
    this->m_FileLogger.Close();
}
/* Informationログ出力 */
void Logger::LogTrivial(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Informationログを生成 */
    std::string log_message = StringFormat(Getnowtime() + "[Trivial] %s", message);
    /* Informationログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}
/* Informationログ出力 */
void Logger::LogInfo(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Informationログを生成 */
    std::string log_message = StringFormat(Getnowtime() + "[INFO] %s", message);
    /* Informationログを標準出力に出力(シアン) */
    std::cout << ConsoleColorCyan << log_message << ConsoleColorReset << std::endl;
    /* Informationログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}
/* Fatalログ出力 */
void Logger::LogFatal(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Informationログを生成 */
    std::string log_message = StringFormat(Getnowtime() + "[Fatal] %s", message);
    /* Informationログを標準出力に出力(シアン) */
    std::cout << ConsoleColorMagenta << log_message << ConsoleColorReset << std::endl;
    /* Informationログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}
/* Warningログ出力 */
void Logger::LogWarn(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Warningログを生成 */
    std::string log_message = StringFormat(Getnowtime() + "[WARN] %s", message);
    /* Warningログを標準出力に出力(黄色) */
    std::cerr << ConsoleColorYellow << log_message << ConsoleColorReset << std::endl;
    /* Warningログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}
/* Errorログ出力 */
void Logger::LogError(const std::string& message)
{
    /* ミューテックスによる排他処理 */
    std::lock_guard<std::mutex> lock(this->m_Mutex);
    /* Errorログを生成 */
    std::string log_message = StringFormat(Getnowtime() + "[ERROR] %s", message);
    /* Errorログを標準出力に出力(赤色) */
    std::cerr << ConsoleColorRed << log_message << ConsoleColorReset << std::endl;
    /* Errorログをファイルに出力 */
    this->m_FileLogger.Write(log_message);
}