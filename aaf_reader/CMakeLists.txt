cmake_minimum_required(VERSION 3.14)
project(aaf_reader)

set(CMAKE_CXX_STANDARD 17)

# Корневой путь к SDK
set(AAFSDK_ROOT "C:/Users/KSN_PC/C++Projects/AAF_Reaper/AAF_SDK_devel")

# Путь к заголовкам и библиотекам
include_directories("${AAFSDK_ROOT}/include")
link_directories("C:/Users/KSN_PC/C++Projects/AAF_Reaper/AAF_SDK_devel/x64/Release/RefImpl")

# Исходные файлы нового модульного проекта
set(SOURCES
    main_new.cpp
    aaf_utils.cpp
    aaf_parser.cpp
    media_utils.cpp
    csv_export.cpp
)

# Заголовочные файлы
set(HEADERS
    data_structures.h
    aaf_utils.h
    aaf_parser.h
    media_utils.h
    csv_export.h
)

add_executable(aaf_reader ${SOURCES} ${HEADERS})



# Линкуем необходимые библиотеки
target_link_libraries(aaf_reader AAF AAFIID ole32 uuid)

