# AAF to Reaper Import Tool

Профессиональный инструмент для импорта AAF файлов в Reaper с поддержкой извлечения embedded аудио и модульной архитектурой.

## Архитектура

Проект построен на модульной архитектуре с четким разделением ответственности:

### Ядро системы
- **`aaf_proper_parser`** - координатор всех модулей, основной AAF парсер
- **`data_structures.h`** - единые структуры данных для всех компонентов

### Специализированные модули
- **`aaf_clip_processor`** - обработка клипов и цепочек Mob, рекурсивный анализ сегментов
- **`aaf_audio_properties`** - извлечение аудио характеристик (sample rate, channels, bit depth)
- **`aaf_essence_extractor`** - извлечение embedded essence, определение форматов, fallback-логика
- **`aaf_utils`** - общие утилиты для работы с AAF SDK
- **`media_utils`** - файловые операции и создание директорий

### Экспорт и интеграция
- **`csv_export`** - экспорт данных в CSV для быстрого импорта в Reaper
- **`debug_logger`** - подробное логирование процесса анализа

## Возможности

### C++ AAF Reader (`aaf_reader.exe`)
- **Полный анализ AAF структуры** с рекурсивной обработкой Composition
- **Извлечение embedded аудио** с автоматическим определением формата
- **Поддержка всех типов сегментов**: SourceClip, NestedScope, Sequence, Selector
- **Извлечение аудио свойств**: sample rate, каналы, разрядность
- **Fallback-логика** для файлов без расширения
- **Экспорт в CSV/JSON** с подробной информацией о клипах

### Lua Import Script для Reaper
- `aaf_import_csv.lua` - **РЕКОМЕНДУЕТСЯ** - простой CSV импорт
- JSON импорт с расширенными возможностями
- Автоматический поиск аудио файлов в нескольких локациях

## Использование

### Быстрый старт

1. **Простой способ - используйте batch файл:**
   ```cmd
   aaf_convert.bat "path\to\your\file.aaf"
   ```

2. **Ручной способ:**
   ```cmd
   cd "C:\Users\KSN_PC\C++Projects\AAF_Reaper\aaf_reader\build\Release"
   aaf_reader.exe "path\to\your\file.aaf"
   ```

### Процесс работы программы

1. **Загрузка AAF файла** - основной парсер инициализирует AAF SDK
2. **Анализ структуры** - clip processor рекурсивно обходит Composition
3. **Извлечение свойств** - audio properties извлекает характеристики треков
4. **Обработка essence** - essence extractor извлекает embedded аудио файлы
5. **Экспорт данных** - создание CSV/JSON с полной информацией

### Результаты работы

- **`aaf_export.csv`** - **РЕКОМЕНДУЕТСЯ** - готовый для импорта в Reaper
- **`extracted_media/`** - папка с извлеченными embedded аудио файлами
- **`output.txt`** - подробный лог с диагностикой процесса
- **`aaf_export.json`** - альтернативный JSON формат для расширенной интеграции

### Особенности извлечения embedded аудио

Программа автоматически:
- Определяет формат файлов по заголовкам (WAV, AIFF, MP3, FLAC)
- Создает правильные расширения файлов
- Применяет fallback-логику для неопознанных форматов
- Сохраняет оригинальные имена файлов когда возможно

### Импорт в Reaper

1. **Скопируйте Lua скрипт в Reaper:**
   ```
   Из: C:\Users\KSN_PC\C++Projects\AAF_Reaper\aaf_reader\aaf_import_csv.lua
   В: C:\Users\[Username]\AppData\Roaming\REAPER\Scripts\aaf_import_csv.lua
   ```

2. **В Reaper:**
   - `Actions` → `Load ReaScript...` → выберите `aaf_import_csv.lua`
   - Или `Actions` → `Show action list` → `Load...` → выберите скрипт
   - Можно назначить горячую клавишу для быстрого доступа

3. **Импорт:**
   - Запустите скрипт
   - Выберите CSV файл (`aaf_export.csv`)
   - Скрипт автоматически найдет extracted_media/ папку
   - Подтвердите создание треков и импорт клипов

4. **Автоматические действия скрипта:**
   - Создание аудио треков с оригинальными именами
   - Размещение клипов в точных временных позициях
   - Применение всех аудио параметров (gain, pan, volume)
   - Автоматический поиск аудио файлов в extracted_media/
   - Подробное логирование в консоль Reaper

## Модульная архитектура

### Основные классы

```cpp
// Координатор всех модулей
class AAFProperParser {
    std::unique_ptr<AAFClipProcessor> clipProcessor;
    std::unique_ptr<AAFAudioProperties> audioProperties;
    std::unique_ptr<AAFEssenceExtractor> essenceExtractor;
    // Инъекция зависимостей и координация
};

// Обработка клипов и навигация по Mob
class AAFClipProcessor {
    void processComposition(IAAFCompositionMob* comp);
    void processSequence(IAAFSequence* sequence);
    void processSourceClip(IAAFSourceClip* sourceClip);
    // Рекурсивная обработка всех типов сегментов
};

// Извлечение аудио характеристик
class AAFAudioProperties {
    void extractAudioProperties(IAAFSourceMob* sourceMob, AudioClipInfo& clipInfo);
    bool isAudioDataDef(IAAFDataDef* dataDef);
    // Определение sample rate, channels, bit depth
};

// Извлечение embedded essence
class AAFEssenceExtractor {
    bool extractEmbeddedEssence(IAAFSourceMob* sourceMob, const std::string& outputDir);
    std::string determineFileExtension(const std::vector<uint8_t>& data);
    // Определение формата, fallback-логика
};
```

### Структуры данных

```cpp
struct AudioClipInfo {
    std::string fileName;
    std::string trackName;
    double timelineStart;
    double length;
    double sourceStart;
    uint32_t sampleRate;
    uint16_t channels;
    uint16_t bitDepth;
    double gain;
    double volume;
    double pan;
    bool isEmbedded;
};

struct TrackInfo {
    uint32_t trackIndex;
    std::string trackName;
    double volume;
    double pan;
    std::vector<AudioClipInfo> clips;
};
```

## Поиск аудио файлов

Система поиска аудио файлов работает в следующем порядке:
1. **Extracted media** - папка `extracted_media/` с извлеченными embedded файлами
2. **Оригинальный путь** - путь, указанный в AAF файле
3. **Папка с CSV/JSON** - директория с экспортированными данными
4. **Папка проекта Reaper** - текущая рабочая директория
5. **Автоматический поиск** - поиск по расширениям (.wav, .aif, .mp3, .flac)

## Расширенные возможности

### Поддерживаемые форматы embedded essence
- **WAV** - автоматическое определение по RIFF заголовку
- **AIFF** - определение по FORM AIFF заголовку
- **MP3** - определение по ID3 или синхронизационным байтам
- **FLAC** - определение по fLaC заголовку
- **Неизвестные форматы** - сохранение как .dat с fallback по имени файла

### Поддерживаемые AAF сегменты
- **SourceClip** - прямые ссылки на медиа файлы
- **NestedScope** - вложенные композиции
- **Sequence** - последовательности клипов
- **Selector** - множественные варианты клипов
- **OperationGroup** - эффекты и параметры

### Извлекаемые аудио свойства
- **Sample Rate** - частота дискретизации из AAFSoundDescriptor
- **Channels** - количество каналов
- **Bit Depth** - разрядность аудио
- **Length** - длительность в samples и секундах
- **Audio DataDef** - проверка типа данных (Sound vs Picture)

## Сборка проекта

### Требования
- **AAF SDK** - размещен в `../AAF_SDK_devel/`
- **CMake 3.15+**
- **Visual Studio 2019+** или совместимый компилятор
- **Windows SDK** для COM интерфейсов

### Команды сборки
```cmd
cd aaf_reader
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Структура после сборки
- `build/Release/aaf_reader.exe` - основной исполняемый файл  
- `build/extracted_media/` - папка для extracted essence
- `build/aaf_export.csv` - экспортированные данные
- `build/output.txt` - лог процесса

## Диагностика и troubleshooting

### C++ Reader диагностика
- **Подробные логи** - все операции записываются в `output.txt`
- **Проверка AAF SDK** - убедитесь в наличии `AAFCOAPI.dll` рядом с .exe
- **Модульная диагностика** - каждый модуль логирует свою работу отдельно
- **Валидация данных** - проверка корректности extracted essence

### Типичные проблемы и решения

#### Проблемы с AAF файлами
```
Ошибка: "Cannot initialize AAF library"
Решение: Проверьте наличие AAFCOAPI.dll и правильность путей к AAF SDK
```

#### Проблемы с embedded essence
```
Ошибка: "Cannot extract embedded essence"
Решение: Проверьте права доступа к папке extracted_media/, освободите место на диске
```

#### Проблемы с аудио свойствами
```
Предупреждение: "Cannot determine audio properties"
Решение: Файл может быть не аудио или иметь нестандартный формат, проверьте DataDef
```

### Отладка Lua скриптов
- **Консоль Reaper** - все операции логируются подробно
- **Проверка файлов** - скрипт сообщает о отсутствующих аудио файлах
- **Валидация CSV** - автоматическая проверка структуры импортируемых данных

### Рекомендации по производительности
- **Большие файлы** - используйте SSD для папки extracted_media/
- **Множественные файлы** - запускайте batch обработку в отдельных процессах
- **Память** - для очень больших AAF файлов может потребоваться 64-bit версия

## Разработка и расширение

Подробная техническая документация по архитектуре, API и стратегиям расширения доступна в [`AGENTS.md`](AGENTS.md).

### Основные точки расширения
- **Новые форматы essence** - добавление в `AAFEssenceExtractor::determineFileExtension()`
- **Дополнительные свойства** - расширение структур в `data_structures.h`
- **Новые экспортеры** - создание модулей по аналогии с `csv_export`
- **Интеграции** - использование модульной архитектуры для других DAW

## Лицензия и поддержка

Этот проект использует AAF SDK и совместим с его лицензией. Для получения технической поддержки и отчетов об ошибках обращайтесь к документации в `AGENTS.md`.
