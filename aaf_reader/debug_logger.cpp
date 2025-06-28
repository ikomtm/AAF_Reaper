#include "debug_logger.h"
#include <iomanip>
#include <ctime>
#include <algorithm>

DebugLogger::DebugLogger(std::ofstream& outStream, LogLevel level)
    : m_out(outStream), m_logLevel(level), m_sectionDepth(0) {
    // По умолчанию включаем все категории
    m_enabledCategories = {
        LogCategory::GENERAL,
        LogCategory::PARSER,
        LogCategory::EXTRACTION,
        LogCategory::MAPPING,
        LogCategory::ESSENCE,
        LogCategory::TRACKS,
        LogCategory::CLIPS,
        LogCategory::CSV_EXPORT
    };
}

void DebugLogger::enableCategory(LogCategory category) {
    if (std::find(m_enabledCategories.begin(), m_enabledCategories.end(), category) == m_enabledCategories.end()) {
        m_enabledCategories.push_back(category);
    }
}

void DebugLogger::disableCategory(LogCategory category) {
    m_enabledCategories.erase(
        std::remove(m_enabledCategories.begin(), m_enabledCategories.end(), category),
        m_enabledCategories.end());
}

void DebugLogger::setEnabledCategories(const std::vector<LogCategory>& categories) {
    m_enabledCategories = categories;
}

bool DebugLogger::isCategoryEnabled(LogCategory category) const {
    return std::find(m_enabledCategories.begin(), m_enabledCategories.end(), category) != m_enabledCategories.end();
}

bool DebugLogger::shouldLog(LogLevel level, LogCategory category) const {
    return static_cast<int>(level) <= static_cast<int>(m_logLevel) && isCategoryEnabled(category);
}

void DebugLogger::error(LogCategory category, const std::string& message) {
    log(LogLevel::LOG_ERROR, category, message);
}

void DebugLogger::warn(LogCategory category, const std::string& message) {
    log(LogLevel::LOG_WARN, category, message);
}

void DebugLogger::info(LogCategory category, const std::string& message) {
    log(LogLevel::LOG_INFO, category, message);
}

void DebugLogger::debug(LogCategory category, const std::string& message) {
    log(LogLevel::LOG_DEBUG, category, message);
}

void DebugLogger::trace(LogCategory category, const std::string& message) {
    log(LogLevel::LOG_TRACE, category, message);
}

void DebugLogger::logProgress(LogCategory category, const std::string& operation, int current, int total) {
    if (!shouldLog(LogLevel::LOG_INFO, category)) return;
    
    std::ostringstream oss;
    oss << operation << ": " << current << "/" << total;
    if (total > 0) {
        int percent = (current * 100) / total;
        oss << " (" << percent << "%)";
    }
    log(LogLevel::LOG_INFO, category, oss.str());
}

void DebugLogger::logResult(LogCategory category, const std::string& operation, bool success, const std::string& details) {
    LogLevel level = success ? LogLevel::LOG_INFO : LogLevel::LOG_WARN;
    if (!shouldLog(level, category)) return;
    
    std::string status = success ? "✅ SUCCESS" : "⚠️ FAILED";
    
    std::ostringstream oss;
    oss << operation << ": " << status;
    if (!details.empty()) {
        oss << " - " << details;
    }
    log(level, category, oss.str());
}

void DebugLogger::logMobIDMapping(const std::string& mobID, const std::string& fileName) {
    if (!shouldLog(LogLevel::LOG_DEBUG, LogCategory::MAPPING)) return;
    
    std::ostringstream oss;
    oss << "MobID " << mobID.substr(0, 8) << "... -> \"" << fileName << "\"";
    log(LogLevel::LOG_DEBUG, LogCategory::MAPPING, oss.str());
}

void DebugLogger::logFileExtraction(const std::string& fileName, bool success, size_t bytesExtracted) {
    LogLevel level = success ? LogLevel::LOG_INFO : LogLevel::LOG_WARN;
    std::string status = success ? "✅ EXTRACTED" : "❌ FAILED";
    
    std::ostringstream oss;
    oss << status << ": \"" << fileName << "\"";
    if (success && bytesExtracted > 0) {
        oss << " (" << bytesExtracted << " bytes)";
    }
    log(level, LogCategory::EXTRACTION, oss.str());
}

void DebugLogger::beginSection(const std::string& sectionName) {
    std::ostringstream oss;
    oss << "🔷 === " << sectionName << " ===";
    log(LogLevel::LOG_INFO, LogCategory::GENERAL, oss.str());
    m_sectionDepth++;
}

void DebugLogger::endSection(const std::string& sectionName) {
    if (m_sectionDepth > 0) {
        m_sectionDepth--;
    }
    
    if (!sectionName.empty()) {
        std::ostringstream oss;
        oss << "🔶 === END " << sectionName << " ===";
        log(LogLevel::LOG_INFO, LogCategory::GENERAL, oss.str());
    }
}

void DebugLogger::logStatistics(const std::string& title, const std::vector<std::pair<std::string, int>>& stats) {
    if (!shouldLog(LogLevel::LOG_INFO, LogCategory::GENERAL)) return;
    
    std::ostringstream oss;
    oss << "📊 " << title;
    log(LogLevel::LOG_INFO, LogCategory::GENERAL, oss.str());
    
    for (const auto& stat : stats) {
        std::ostringstream statOss;
        statOss << "  " << stat.first << ": " << stat.second;
        log(LogLevel::LOG_INFO, LogCategory::GENERAL, statOss.str());
    }
}

void DebugLogger::logSummary(const std::string& title, const std::map<std::string, std::string>& summary) {
    if (!shouldLog(LogLevel::LOG_INFO, LogCategory::GENERAL)) return;
    
    m_out << "\n" << getIndent() << "=== " << title << " ===" << std::endl;
    for (const auto& pair : summary) {
        m_out << getIndent() << "  " << pair.first << ": " << pair.second << std::endl;
    }
    m_out << getIndent() << "========================" << std::endl;
}

void DebugLogger::logPerformanceMetric(const std::string& operation, double timeMs) {
    if (!shouldLog(LogLevel::LOG_INFO, LogCategory::GENERAL)) return;
    
    std::ostringstream oss;
    oss << "⏱️ " << operation << " took " << std::fixed << std::setprecision(2) << timeMs << " ms";
    log(LogLevel::LOG_INFO, LogCategory::GENERAL, oss.str());
}

std::string DebugLogger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::LOG_ERROR: return "ERROR";
        case LogLevel::LOG_WARN:  return "WARN ";
        case LogLevel::LOG_INFO:  return "INFO ";
        case LogLevel::LOG_DEBUG: return "DEBUG";
        case LogLevel::LOG_TRACE: return "TRACE";
        default: return "?????";
    }
}

std::string DebugLogger::categoryToString(LogCategory category) {
    switch (category) {
        case LogCategory::GENERAL:    return "GEN";
        case LogCategory::PARSER:     return "PAR";
        case LogCategory::EXTRACTION: return "EXT";
        case LogCategory::MAPPING:    return "MAP";
        case LogCategory::ESSENCE:    return "ESS";
        case LogCategory::TRACKS:     return "TRK";
        case LogCategory::CLIPS:      return "CLP";
        case LogCategory::CSV_EXPORT: return "CSV";
        default: return "???";
    }
}

std::string DebugLogger::getIndent() {
    return std::string(m_sectionDepth * 2, ' ');
}

void DebugLogger::log(LogLevel level, LogCategory category, const std::string& message) {
    if (!shouldLog(level, category)) {
        return; // Не выводим сообщения выше текущего уровня или отключенных категорий
    }
    
    m_out << "[" << levelToString(level) << "|" << categoryToString(category) << "] "
          << getIndent() << message << std::endl;
}
