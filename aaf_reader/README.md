# AAF to Reaper Import Tool

Набор инструментов для импорта AAF файлов в Reaper через JSON.

## Компоненты

### 1. C++ AAF Reader (`aaf_reader.exe`)
- Парсит AAF файлы с помощью AAF SDK
- Извлекает информацию об аудио треках и клипах
- Экспортирует данные в JSON формат

### 2. Lua Import Script для Reaper
- `aaf_import_csv.lua` - **РЕКОМЕНДУЕТСЯ** - простой CSV импорт
- `aaf_import_simple.lua` - JSON импорт (более сложный)
- Не требует внешних зависимостей

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

### Шаг 1: Конвертация AAF в JSON

Результат работы программы:
- `aaf_export.csv` - **РЕКОМЕНДУЕТСЯ** - простой CSV для импорта в Reaper
- `aaf_export.json` - альтернативный JSON формат  
- `output.txt` - подробный лог анализа

### Шаг 2: Импорт в Reaper

1. **Скопируйте Lua скрипт в Reaper:**
   ```
   Из: C:\Users\KSN_PC\C++Projects\AAF_Reaper\aaf_reader\aaf_import_simple.lua
   В: C:\Users\[Username]\AppData\Roaming\REAPER\Scripts\aaf_import_simple.lua
   ```

2. **В Reaper:**
   - `Actions` → `Load ReaScript...` → выберите `aaf_import_simple.lua`
   - Или `Actions` → `Show action list` → `Load...` → выберите скрипт
   - Можно назначить горячую клавишу для быстрого доступа

3. **Импорт:**
   - Запустите скрипт
   - Выберите JSON файл (`aaf_export.json`)
   - Подтвердите очистку проекта (по желанию)

4. **Автоматические действия скрипта:**
   - Создание аудио треков с правильными именами
   - Размещение клипов по таймлинии
   - Применение параметров (gain, pan, volume)
   - Поиск аудио файлов в нескольких местах
   - Подробное логирование в консоль Reaper

## Структура JSON

```json
{
  "project": {
    "name": "Project Name",
    "sampleRate": 48000.0,
    "totalLength": 120.5,
    "sessionStartTimecode": 0.0
  },
  "tracks": [
    {
      "trackIndex": 0,
      "trackName": "Audio Track 1",
      "volume": 1.0,
      "pan": 0.0,
      "clips": [
        {
          "fileName": "audio.wav",
          "timelineStart": 0.0,
          "length": 30.0,
          "sourceStart": 0.0,
          "gain": 0.0,
          "volume": 1.0,
          "pan": 0.0
        }
      ]
    }
  ]
}
```

## Поиск аудио файлов

Скрипт Lua ищет аудио файлы в следующем порядке:
1. Оригинальный путь из AAF
2. Папка с JSON файлом
3. Папка текущего проекта Reaper
4. Автоматический поиск по расширениям (.wav, .aif, .mp3, .flac)

## Особенности

### C++ Reader
- Обрабатывает только аудио треки (DataType = "Sound")
- Извлекает параметры эффектов (Gain, Volume, Pan)
- Учитывает стартовый timecode сессии
- Корректно обрабатывает SourceClip, OperationGroup, Transition

### Lua Import Script
- Простой парсер JSON без внешних зависимостей
- Автоматическое создание треков и клипов
- Применение аудио параметров
- Поиск отсутствующих файлов

## Тестирование

Для тестирования используйте `test_aaf_export.json` - пример правильной структуры данных.

## Требования

- **C++**: AAF SDK, CMake, Visual Studio
- **Reaper**: Версия 6.0+ с поддержкой ReaScript
- **Аудио файлы**: В доступной папке (рядом с JSON или в проекте)

## Troubleshooting

### Проблемы C++
- Убедитесь, что AAF SDK правильно подключен
- Проверьте наличие `AAFCOAPI.dll` рядом с `aaf_reader.exe`

### Проблемы Lua
- Проверьте консоль Reaper для подробных логов
- Убедитесь, что аудио файлы доступны
- Проверьте структуру JSON файла

### Отсутствующие файлы
- Скопируйте аудио файлы в папку с JSON
- Или укажите правильные пути в AAF проекте
