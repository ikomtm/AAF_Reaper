# AAF Reader - Агентная архитектура и модульная структура

## Обзор архитектуры

AAF Reader представляет собой модульную систему для извлечения embedded аудио из AAF-файлов. Система построена по принципу разделения ответственности, где каждый модуль отвечает за конкретную область функциональности.

## Основные принципы

### 1. Модульность
- Каждый модуль имеет четко определенную область ответственности
- Модули взаимодействуют через четко определенные интерфейсы
- Возможность независимого тестирования и модификации модулей

### 2. Инверсия зависимостей
- Высокоуровневые модули не зависят от низкоуровневых
- Зависимости инжектируются через конструкторы
- Легкость замены компонентов

### 3. Единая система логирования
- Все модули используют единый DebugLogger
- Категоризированное логирование по типам операций
- Контролируемый уровень детализации логов

## Архитектура модулей

### Координатор: AAFProperParser

**Файлы:** `aaf_proper_parser.cpp/h`

**Назначение:** Главный координатор системы, управляет процессом парсинга AAF-файлов.

**Ответственность:**
- Поиск и идентификация TopLevel CompositionMob
- Поиск аудио TimelineMobSlot
- Координация работы специализированных модулей
- Управление жизненным циклом парсинга

**Зависимости:**
- `AAFClipProcessor` - для обработки клипов
- `AAFAudioProperties` - для извлечения аудио свойств
- `AAFEssenceExtractor` - для извлечения embedded аудио

**Ключевые методы:**
```cpp
bool parseAAFFile()                                    // Главный метод парсинга
IAAFMob* findTopLevelCompositionMob()                 // Поиск главного CompositionMob
std::vector<IAAFMobSlot*> findAudioTimelineSlots()   // Поиск аудио слотов
bool extractAllEmbeddedAudio()                        // Извлечение всего embedded аудио
```

### Процессор клипов: AAFClipProcessor

**Файлы:** `aaf_clip_processor.cpp/h`

**Назначение:** Обработка клипов и навигация по цепочкам SourceClip → MasterMob → FileSourceMob.

**Ответственность:**
- Рекурсивная обработка Sequence и SourceClip
- Поиск и связывание MasterMob с FileSourceMob
- Расчёт временных параметров клипов
- Обработка nested структур (Sequence в Sequence)

**Зависимости:**
- `AAFAudioProperties` - для получения аудио свойств
- `AAFEssenceExtractor` - для извлечения essence данных

**Ключевые методы:**
```cpp
void processSegmentForClips()                         // Обработка сегмента на клипы
void processSequence()                                // Обработка Sequence
void processSourceClip()                              // Обработка SourceClip
IAAFSourceMob* findFileSourceMobFromMaster()         // Поиск FileSourceMob от MasterMob
```

### Анализатор аудио свойств: AAFAudioProperties

**Файлы:** `aaf_audio_properties.cpp/h`

**Назначение:** Извлечение аудио свойств из EssenceDescriptor.

**Ответственность:**
- Извлечение параметров аудио (sample rate, channels, bit depth)
- Определение типа аудио данных (PCM, Compressed)
- Анализ EssenceDescriptor и связанных объектов
- Работа с различными типами EssenceDescriptor

**Ключевые методы:**
```cpp
void getAudioProperties()                             // Получение всех аудио свойств
bool isAudioDataDef()                                // Проверка на аудио DataDef
bool extractAudioInfoFromDescriptor()                // Извлечение из EssenceDescriptor
```

### Экстрактор Essence: AAFEssenceExtractor

**Файлы:** `aaf_essence_extractor.cpp/h`

**Назначение:** Извлечение embedded аудио, определение формата, fallback-методы.

**Ответственность:**
- Извлечение embedded essence данных
- Определение формата аудио файлов
- Fallback-логика для имён файлов и расширений
- Работа с различными типами Locator

**Ключевые методы:**
```cpp
bool extractAllEmbeddedAudio()                        // Извлечение всего embedded аудио
bool checkIfEmbedded()                                // Проверка на embedded статус
std::string getFileNameFromLocator()                 // Извлечение имени из Locator
std::string determineAudioFormat()                   // Определение формата аудио
std::string getFileNameFallbackFromSourceMob()      // Fallback для имён файлов
```

### Структуры данных: data_structures.h

**Назначение:** Определение всех структур данных для работы с AAF.

**Основные структуры:**
```cpp
struct AAFClipInfo {
    std::string mobID;
    std::string clipName;
    std::string originalFileName;
    aafPosition_t startTime;
    aafLength_t length;
    // ... другие поля
};

struct AAFAudioTrackInfo {
    aafSlotID_t slotID;
    std::string trackName;
    aafRational_t editRate;
    std::vector<AAFClipInfo> clips;
    // ... другие поля
};
```

## Вспомогательные модули

### Утилиты: media_utils.cpp/h

**Назначение:** Вспомогательные функции для работы с файловой системой.

**Функции:**
- `createDirectoryRecursive()` - создание директорий
- Очищен от legacy-кода, содержит только актуальные утилиты

### Логирование: debug_logger.cpp/h

**Назначение:** Единая система логирования.

**Возможности:**
- Категоризированное логирование
- Контроль уровня детализации
- Форматированный вывод
- Сводные отчёты

## Взаимодействие модулей

### Диаграмма зависимостей

```
AAFProperParser (Координатор)
    ├── AAFClipProcessor
    │   ├── AAFAudioProperties
    │   └── AAFEssenceExtractor
    ├── AAFAudioProperties
    └── AAFEssenceExtractor

Вспомогательные:
    ├── DebugLogger (используется всеми)
    ├── media_utils (создание директорий)
    └── aaf_utils (общие утилиты)
```

### Поток данных

1. **Инициализация:** AAFProperParser создаёт и настраивает все модули
2. **Парсинг:** Поиск TopLevel CompositionMob и аудио слотов
3. **Обработка клипов:** AAFClipProcessor обрабатывает каждый сегмент
4. **Извлечение свойств:** AAFAudioProperties анализирует EssenceDescriptor
5. **Извлечение аудио:** AAFEssenceExtractor извлекает embedded данные

## Примеры использования

### Основное использование

```cpp
// Создание парсера
AAFProperParser parser(pHeader, logFile, LogLevel::INFO);

// Парсинг AAF файла
if (parser.parseAAFFile()) {
    // Извлечение embedded аудио
    parser.extractAllEmbeddedAudio("./extracted_media/");
}

// Получение результатов
const auto& tracks = parser.getAudioTracks();
for (const auto& track : tracks) {
    for (const auto& clip : track.clips) {
        std::cout << "Clip: " << clip.clipName 
                  << " (" << clip.sourceType << ")" << std::endl;
    }
}
```

### Расширение функциональности

Для добавления нового типа обработки:

1. Создайте новый модуль (например, `AAFVideoProcessor`)
2. Добавьте зависимость в `AAFProperParser`
3. Инжектируйте зависимости в конструкторе
4. Используйте модуль в методе `parseAAFFile()`

## Тестирование

### Модульное тестирование

Каждый модуль может быть протестирован независимо:

```cpp
// Тест AAFAudioProperties
TEST(AAFAudioPropertiesTest, ExtractsSampleRate) {
    DebugLogger logger(std::cout, LogLevel::DEBUG);
    AAFAudioProperties props(logger);
    
    // Создание мок-объектов
    // Тестирование функции
    // Проверка результатов
}
```

### Интеграционное тестирование

Тестирование взаимодействия модулей:

```cpp
// Тест полного процесса
TEST(AAFParserIntegrationTest, ParsesCompleteAAF) {
    // Создание тестового AAF файла
    // Запуск парсера
    // Проверка результатов
}
```

## Производительность

### Оптимизации

1. **Кэширование:** Результаты поиска Mob кэшируются
2. **Ленивая загрузка:** Essence данные загружаются по требованию
3. **Пул объектов:** Переиспользование AAF объектов

### Метрики

- Время парсинга AAF файла
- Объём извлечённого аудио
- Использование памяти

## Расширяемость

### Добавление новых форматов

1. Расширьте `AAFEssenceExtractor::determineAudioFormat()`
2. Добавьте новые magic bytes в `detectFormatFromData()`
3. Создайте соответствующие fallback-методы

### Добавление новых типов Mob

1. Расширьте `AAFClipProcessor::processSegmentForClips()`
2. Добавьте обработку нового типа в switch-case
3. Создайте специализированные методы обработки

### Добавление новых свойств

1. Расширьте структуры в `data_structures.h`
2. Добавьте методы извлечения в соответствующий модуль
3. Обновите логирование и вывод

## Обработка ошибок

### Стратегии

1. **Graceful degradation:** Продолжение работы при частичных сбоях
2. **Fallback-методы:** Альтернативные способы получения данных
3. **Подробное логирование:** Детальная информация об ошибках

### Типичные ошибки

- Отсутствие TopLevel CompositionMob → Использование первого найденного
- Недоступность EssenceDescriptor → Fallback на имя файла
- Отсутствие Locator → Генерация имени по MobID

## Конфигурация

### Уровни логирования

- `TRACE`: Максимальная детализация
- `DEBUG`: Отладочная информация
- `INFO`: Основные операции
- `WARN`: Предупреждения
- `ERROR`: Ошибки

### Настройки извлечения

- Директория для extracted медиа
- Формат имён файлов
- Обработка дубликатов

## Заключение

Модульная архитектура AAF Reader обеспечивает:

- **Поддерживаемость:** Лёгкость модификации и расширения
- **Тестируемость:** Независимое тестирование компонентов
- **Читаемость:** Чёткое разделение ответственности
- **Надёжность:** Robustness через fallback-методы
- **Производительность:** Оптимизированная обработка данных

Система готова к дальнейшему развитию и может быть легко адаптирована для новых требований и форматов AAF файлов.
