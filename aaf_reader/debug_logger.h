#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <iomanip>

enum class LogLevel {
    LOG_ERROR = 0,    // Только критические ошибки
    LOG_WARN = 1,     // Предупреждения
    LOG_INFO = 2,     // Основная информация
    LOG_DEBUG = 3,    // Подробная отладка
    LOG_TRACE = 4     // Максимально подробно
};

enum class LogCategory {
    GENERAL,      // Общая информация
    PARSER,       // Парсинг AAF
    EXTRACTION,   // Извлечение файлов
    MAPPING,      // Маппинг MobID -> имена файлов
    ESSENCE,      // Работа с EssenceData/EssenceAccess
    TRACKS,       // Обработка треков
    CLIPS,        // Обработка клипов
    CSV_EXPORT    // Экспорт CSV
};

class DebugLogger {
public:
    DebugLogger(std::ofstream& outStream, LogLevel level = LogLevel::LOG_INFO);
    
    void setLogLevel(LogLevel level) { m_logLevel = level; }
    LogLevel getLogLevel() const { return m_logLevel; }
    
    // Фильтрация по категориям
    void enableCategory(LogCategory category);
    void disableCategory(LogCategory category);
    void setEnabledCategories(const std::vector<LogCategory>& categories);
    bool isCategoryEnabled(LogCategory category) const;
    
    // Основные методы логирования
    void error(LogCategory category, const std::string& message);
    void warn(LogCategory category, const std::string& message);
    void info(LogCategory category, const std::string& message);
    void debug(LogCategory category, const std::string& message);
    void trace(LogCategory category, const std::string& message);
    
    // Специальные методы для частых случаев
    void logProgress(LogCategory category, const std::string& operation, int current, int total);
    void logResult(LogCategory category, const std::string& operation, bool success, const std::string& details = "");
    void logMobIDMapping(const std::string& mobID, const std::string& fileName);
    void logFileExtraction(const std::string& fileName, bool success, size_t bytesExtracted = 0);
    
    // Секции для группировки связанной информации
    void beginSection(const std::string& sectionName);
    void endSection(const std::string& sectionName = "");
    
    // Статистика
    void logStatistics(const std::string& title, const std::vector<std::pair<std::string, int>>& stats);
    
    // Методы для вывода сводной информации
    void logSummary(const std::string& title, const std::map<std::string, std::string>& summary);
    void logPerformanceMetric(const std::string& operation, double timeMs);

private:
    std::ofstream& m_out;
    LogLevel m_logLevel;
    int m_sectionDepth;
    std::vector<LogCategory> m_enabledCategories;
    
    std::string levelToString(LogLevel level);
    std::string categoryToString(LogCategory category);
    std::string getIndent();
    void log(LogLevel level, LogCategory category, const std::string& message);
    bool shouldLog(LogLevel level, LogCategory category) const;
};
