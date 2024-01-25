#pragma once
#include <fstream>
/* File Loggerクラス宣言 */
class FileLogger
{
public:
    /* コンストラクタ */
    FileLogger();
    /* コンストラクタ */
    FileLogger(const std::string& file_path);
    /* デストラクタ */
    ~FileLogger();
    /* ファイルオープン */
    void Open(const std::string& file_path);
    /* ファイルクローズ */
    void Close();
    /* ファイルオープン確認 */
    bool IsOpened();
    /* ファイル書き込み */
    void Write(const std::string& log);
private:
    /* ログファイルパス */
    std::string m_FilePath;
    /* ファイル出力ストリーム */
    std::ofstream m_Stream;
};

/* コンストラクタ */
FileLogger::FileLogger()
    : m_FilePath("")
    , m_Stream()
{
    /* Nothing to do */
}
/* コンストラクタ */
FileLogger::FileLogger(const std::string& file_path)
    : m_FilePath(file_path)
    , m_Stream()
{
    /* ファイルオープン */
    this->Open(file_path);
}
/* デストラクタ */
FileLogger::~FileLogger()
{
    /* ファイルクローズ */
    this->Close();
}
/* ファイルオープン */
void FileLogger::Open(const std::string& file_path)
{
    /* ファイルが開いていなかったら */
    if (this->IsOpened() == false)
    {
        this->m_Stream.open(file_path, std::ios::app);
    }
}
/* ファイルクローズ */
void FileLogger::Close()
{
    /* ファイルが開いていたら */
    if (this->IsOpened() == true)
    {
        this->m_Stream.close();
    }
}
/* ファイルオープン確認 */
bool FileLogger::IsOpened()
{
    return this->m_Stream.is_open();
}
/* ファイル書き込み */
void FileLogger::Write(const std::string& log)
{
    /* ファイルが開いていたら */
    if (this->IsOpened() == true)
    {
        /* ファイルストリームに書き込み */
        this->m_Stream << log << std::endl;
    }
}