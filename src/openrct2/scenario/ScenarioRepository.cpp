#pragma region Copyright (c) 2014-2017 OpenRCT2 Developers
/*****************************************************************************
 * OpenRCT2, an open source clone of Roller Coaster Tycoon 2.
 *
 * OpenRCT2 is the work of many authors, a full list can be found in contributors.md
 * For more information, visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * A full copy of the GNU General Public License can be found in licence.txt
 *****************************************************************************/
#pragma endregion

#include <algorithm>
#include <memory>
#include <vector>
#include "../core/Console.hpp"
#include "../core/File.h"
#include "../core/FileIndex.hpp"
#include "../core/FileStream.hpp"
#include "../core/Math.hpp"
#include "../core/Path.hpp"
#include "../core/String.hpp"
#include "../core/Util.hpp"
#include "../ParkImporter.h"
#include "../PlatformEnvironment.h"
#include "../rct12/SawyerChunkReader.h"
#include "ScenarioRepository.h"
#include "ScenarioSources.h"
#include <openrct2/Speedrunning.h>

#include "../config/Config.h"
#include "../localisation/Localisation.h"
#include "../platform/platform.h"
#include "Scenario.h"

#define HIGHSCORE_FILE_VERSION 1
#define SPEEDRUN_DAYS_RECORD_FILE_VERSION 2
#define SPEEDRUN_TIME_RECORD_FILE_VERSION 3

using namespace OpenRCT2;

static sint32 ScenarioCategoryCompare(sint32 categoryA, sint32 categoryB)
{
    if (categoryA == categoryB) return 0;
    if (categoryA == SCENARIO_CATEGORY_DLC) return -1;
    if (categoryB == SCENARIO_CATEGORY_DLC) return 1;
    if (categoryA == SCENARIO_CATEGORY_BUILD_YOUR_OWN) return -1;
    if (categoryB == SCENARIO_CATEGORY_BUILD_YOUR_OWN) return 1;
    return Math::Sign(categoryA - categoryB);
}

static sint32 scenario_index_entry_CompareByCategory(const scenario_index_entry &entryA,
                                                  const scenario_index_entry &entryB)
{
    // Order by category
    if (entryA.category != entryB.category)
    {
        return ScenarioCategoryCompare(entryA.category, entryB.category);
    }

    // Then by source game / name
    switch (entryA.category) {
    default:
        if (entryA.source_game != entryB.source_game)
        {
            return entryA.source_game - entryB.source_game;
        }
        return strcmp(entryA.name, entryB.name);
    case SCENARIO_CATEGORY_REAL:
    case SCENARIO_CATEGORY_OTHER:
        return strcmp(entryA.name, entryB.name);
    }
}

static sint32 scenario_index_entry_CompareByIndex(const scenario_index_entry &entryA,
                                               const scenario_index_entry &entryB)
{
    // Order by source game
    if (entryA.source_game != entryB.source_game)
    {
        return entryA.source_game - entryB.source_game;
    }

    // Then by index / category / name
    uint8 sourceGame = entryA.source_game;
    switch (sourceGame) {
    default:
        if (entryA.source_index == -1 && entryB.source_index == -1)
        {
            if (entryA.category == entryB.category)
            {
                return scenario_index_entry_CompareByCategory(entryA, entryB);
            }
            else
            {
                return ScenarioCategoryCompare(entryA.category, entryB.category);
            }
        }
        else if (entryA.source_index == -1)
        {
            return 1;
        }
        else if (entryB.source_index == -1)
        {
            return -1;
        }
        else
        {
            return entryA.source_index - entryB.source_index;
        }
    case SCENARIO_SOURCE_REAL:
        return scenario_index_entry_CompareByCategory(entryA, entryB);
    }
}

static void scenario_highscore_free(scenario_highscore_entry * highscore)
{
    SafeFree(highscore->fileName);
    SafeFree(highscore->name);
    SafeDelete(highscore);
}

static void scenario_speedrun_highscores_days_free(scenario_speedrun_days_record_entry * days)
{
    SafeFree(days->fileName);
    SafeDelete(days);
}

static void scenario_speedrun_highscores_time_free(scenario_speedrun_time_record_entry * time)
{
    SafeFree(time->fileName);
    SafeDelete(time);
}

class ScenarioFileIndex final : public FileIndex<scenario_index_entry>
{
private:
    static constexpr uint32 MAGIC_NUMBER = 0x58444953; // SIDX
    static constexpr uint16 VERSION = 3;
    static constexpr auto PATTERN = "*.sc4;*.sc6";
    
public:
    explicit ScenarioFileIndex(IPlatformEnvironment * env) :
        FileIndex("scenario index",
                  MAGIC_NUMBER,
                  VERSION,
                  env->GetFilePath(PATHID::CACHE_SCENARIOS),
                  std::string(PATTERN),
                  std::vector<std::string>({
                      env->GetDirectoryPath(DIRBASE::RCT1, DIRID::SCENARIO),
                      env->GetDirectoryPath(DIRBASE::RCT2, DIRID::SCENARIO),
                      env->GetDirectoryPath(DIRBASE::USER, DIRID::SCENARIO) }))
    {
    }

protected:
    std::tuple<bool, scenario_index_entry> Create(const std::string &path) const override
    {
        scenario_index_entry entry;
        auto timestamp = File::GetLastModified(path);
        if (GetScenarioInfo(path, timestamp, &entry))
        {
            return std::make_tuple(true, entry);
        }
        else
        {
            return std::make_tuple(true, scenario_index_entry());
        }
    }

    void Serialise(IStream * stream, const scenario_index_entry &item) const override
    {
        stream->Write(item.path, sizeof(item.path));
        stream->WriteValue(item.timestamp);

        stream->WriteValue(item.category);
        stream->WriteValue(item.source_game);
        stream->WriteValue(item.source_index);
        stream->WriteValue(item.sc_id);

        stream->WriteValue(item.objective_type);
        stream->WriteValue(item.objective_arg_1);
        stream->WriteValue(item.objective_arg_2);
        stream->WriteValue(item.objective_arg_3);

        stream->Write(item.internal_name, sizeof(item.internal_name));
        stream->Write(item.name, sizeof(item.name));
        stream->Write(item.details, sizeof(item.details));
    }

    scenario_index_entry Deserialise(IStream * stream) const override
    {
        scenario_index_entry item;

        stream->Read(item.path, sizeof(item.path));
        item.timestamp = stream->ReadValue<uint64>();

        item.category = stream->ReadValue<uint8>();
        item.source_game = stream->ReadValue<uint8>();
        item.source_index = stream->ReadValue<sint16>();
        item.sc_id = stream->ReadValue<uint16>();

        item.objective_type = stream->ReadValue<uint8>();
        item.objective_arg_1 = stream->ReadValue<uint8>();
        item.objective_arg_2 = stream->ReadValue<sint32>();
        item.objective_arg_3 = stream->ReadValue<sint16>();
        item.highscore = nullptr;

        stream->Read(item.internal_name, sizeof(item.internal_name));
        stream->Read(item.name, sizeof(item.name));
        stream->Read(item.details, sizeof(item.details));

        return item;
    }

private:
    /**
     * Reads basic information from a scenario file.
     */
    static bool GetScenarioInfo(const std::string &path, uint64 timestamp, scenario_index_entry * entry)
    {
        log_verbose("GetScenarioInfo(%s, %d, ...)", path.c_str(), timestamp);
        try
        {
            std::string extension = Path::GetExtension(path);
            if (String::Equals(extension, ".sc4", true))
            {
                // RCT1 scenario
                bool result = false;
                try
                {
                    auto s4Importer = std::unique_ptr<IParkImporter>(ParkImporter::CreateS4());
                    s4Importer->LoadScenario(path.c_str(), true);
                    if (s4Importer->GetDetails(entry))
                    {
                        String::Set(entry->path, sizeof(entry->path), path.c_str());
                        entry->timestamp = timestamp;
                        result = true;
                    }
                }
                catch (const std::exception &)
                {
                }
                return result;
            }
            else
            {
                // RCT2 scenario
                auto fs = FileStream(path, FILE_MODE_OPEN);
                auto chunkReader = SawyerChunkReader(&fs);
 
                rct_s6_header header = chunkReader.ReadChunkAs<rct_s6_header>();
                if (header.type == S6_TYPE_SCENARIO)
                {
                    rct_s6_info info = chunkReader.ReadChunkAs<rct_s6_info>();
                    *entry = CreateNewScenarioEntry(path, timestamp, &info);
                    return true;
                }
                else
                {
                    log_verbose("%s is not a scenario", path.c_str());
                }
            }
        }
        catch (const std::exception &)
        {
            Console::Error::WriteLine("Unable to read scenario: '%s'", path.c_str());
        }
        return false;
    }

    static scenario_index_entry CreateNewScenarioEntry(const std::string &path, uint64 timestamp, rct_s6_info * s6Info)
    {
        scenario_index_entry entry = { 0 };

        // Set new entry
        String::Set(entry.path, sizeof(entry.path), path.c_str());
        entry.timestamp = timestamp;
        entry.category = s6Info->category;
        entry.objective_type = s6Info->objective_type;
        entry.objective_arg_1 = s6Info->objective_arg_1;
        entry.objective_arg_2 = s6Info->objective_arg_2;
        entry.objective_arg_3 = s6Info->objective_arg_3;
        entry.highscore = nullptr;
        if (String::IsNullOrEmpty(s6Info->name))
        {
            // If the scenario doesn't have a name, set it to the filename
            String::Set(entry.name, sizeof(entry.name), Path::GetFileNameWithoutExtension(entry.path));
        }
        else
        {
            String::Set(entry.name, sizeof(entry.name), s6Info->name);
            // Normalise the name to make the scenario as recognisable as possible.
            ScenarioSources::NormaliseName(entry.name, sizeof(entry.name), entry.name);
        }

        // entry.name will be translated later so keep the untranslated name here
        String::Set(entry.internal_name, sizeof(entry.internal_name), entry.name);

        String::Set(entry.details, sizeof(entry.details), s6Info->details);

        // Look up and store information regarding the origins of this scenario.
        source_desc desc;
        if (ScenarioSources::TryGetByName(entry.name, &desc))
        {
            entry.sc_id = desc.id;
            entry.source_index = desc.index;
            entry.source_game = desc.source;
            entry.category = desc.category;
        }
        else
        {
            entry.sc_id = SC_UNIDENTIFIED;
            entry.source_index = -1;
            if (entry.category == SCENARIO_CATEGORY_REAL)
            {
                entry.source_game = SCENARIO_SOURCE_REAL;
            }
            else
            {
                entry.source_game = SCENARIO_SOURCE_OTHER;
            }
        }

        scenario_translate(&entry, &s6Info->entry);
        return entry;
    }
};

class ScenarioRepository final : public IScenarioRepository
{
private:
    static constexpr uint32 HighscoreFileVersion = 1;
    static constexpr uint32 SpeedrunDaysRecordFileVersion = 2;
    static constexpr uint32 SpeedrunTimeRecordFileVersion = 3;

    IPlatformEnvironment * const _env;
    ScenarioFileIndex const _fileIndex;
    std::vector<scenario_index_entry> _scenarios;
    std::vector<scenario_highscore_entry*> _highscores;
    std::vector<scenario_speedrun_days_record_entry*> _speedrunDaysHighscores;
    std::vector<scenario_speedrun_time_record_entry*> _speedrunTimesHighscores;

public:
    explicit ScenarioRepository(IPlatformEnvironment * env)
        : _env(env),
          _fileIndex(env)
    {
    }

    virtual ~ScenarioRepository()
    {
        ClearHighscores();
    }

    void Scan() override
    {
        ImportMegaPark();

        // Reload scenarios from index
        _scenarios.clear();
        auto scenarios = _fileIndex.LoadOrBuild();
        for (auto scenario : scenarios)
        {
            AddScenario(scenario);
        }

        // Sort the scenarios and load the highscores
        Sort();
        LoadScores();
        LoadLegacyScores();
        AttachHighscores();
    }

    size_t GetCount() const override
    {
        return _scenarios.size();
    }

    const scenario_index_entry * GetByIndex(size_t index) const override
    {
        const scenario_index_entry * result = nullptr;
        if (index < _scenarios.size())
        {
            result = &_scenarios[index];
        }
        return result;
    }

    const scenario_index_entry * GetByFilename(const utf8 * filename) const override
    {
        for (const auto &scenario : _scenarios)
        {
            const utf8 * scenarioFilename = Path::GetFileName(scenario.path);

            // Note: this is always case insensitive search for cross platform consistency
            if (String::Equals(filename, scenarioFilename, true))
            {
                return &scenario;
            }
        }
        return nullptr;
    }

    const scenario_index_entry * GetByInternalName(const utf8 * name) const override 
    {
        for (size_t i = 0; i < _scenarios.size(); i++) {
            const scenario_index_entry * scenario = &_scenarios[i];

			if (scenario->source_game == SCENARIO_SOURCE_OTHER && scenario->sc_id == SC_UNIDENTIFIED)
				continue;

            // Note: this is always case insensitive search for cross platform consistency
            if (String::Equals(name, scenario->internal_name, true)) {
                return &_scenarios[i];
            }
        }
        return nullptr;
    }

    const scenario_index_entry * GetByPath(const utf8 * path) const override
    {
        for (const auto &scenario : _scenarios)
        {
            if (Path::Equals(path, scenario.path))
            {
                return &scenario;
            }
        }
        return nullptr;
    }

    bool TryRecordHighscore(const utf8 * scenarioFileName, money32 companyValue, const utf8 * name) override
    {
        // Scan the scenarios so we have a fresh list to query. This is to prevent the issue of scenario completions
        // not getting recorded, see #4951.
        Scan();

        scenario_index_entry * scenario = GetByFilename(scenarioFileName);
        if (scenario != nullptr)
        {
            // Check if record company value has been broken or the highscore is the same but no name is registered
            scenario_highscore_entry * highscore = scenario->highscore;
            if (highscore == nullptr || companyValue > highscore->company_value ||
                (String::IsNullOrEmpty(highscore->name) && companyValue == highscore->company_value))
            {
                if (highscore == nullptr)
                {
                    highscore = InsertHighscore();
                    highscore->timestamp = platform_get_datetime_now_utc();
                    scenario->highscore = highscore;
                }
                else
                {
                    if (!String::IsNullOrEmpty(highscore->name))
                    {
                        highscore->timestamp = platform_get_datetime_now_utc();
                    }
                    SafeFree(highscore->fileName);
                    SafeFree(highscore->name);
                }
                highscore->fileName = String::Duplicate(Path::GetFileName(scenario->path));
                highscore->name = String::Duplicate(name);
                highscore->company_value = companyValue;
                SaveHighscores();
                return true;
            }
        }
        return false;
    }

    bool TryRecordSpeedrunRecords(const utf8 * scenarioFileName, uint32 daysValue) override
    {
        // Scan the scenarios so we have a fresh list to query. This is to prevent the issue of scenario completions
        // not getting recorded, see #4951.
        Scan();

        scenario_index_entry * scenario = GetByFilename(scenarioFileName);
        if (scenario != nullptr)
        {
            // Check if time record value has been broken
            scenario_speedrun_time_record_entry * highscore_time = scenario->time_record;
            if (highscore_time == nullptr || gSpeedrunningState.speedrun_finished_time < highscore_time->time_record)
            {
                if (highscore_time == nullptr)
                {
                    highscore_time = InsertSpeedrunTimeHighscore();
                    scenario->time_record = highscore_time;
                }
                else
                {
                    SafeFree(highscore_time->fileName);
                }
                highscore_time->fileName = String::Duplicate(Path::GetFileName(scenario->path));
                highscore_time->time_record = gSpeedrunningState.speedrun_finished_time;
                SaveSpeedrunTimeHighscores();
            }

            // Check if days record value has been broken
            scenario_speedrun_days_record_entry * highscore_days = scenario->days_record;
            if (highscore_days == nullptr || daysValue < highscore_days->days_record)
            {
                if (highscore_days == nullptr)
                {
                    highscore_days = InsertSpeedrunDaysHighscore();
                    scenario->days_record = highscore_days;
                }
                else
                {
                    SafeFree(highscore_days->fileName);
                }
                highscore_days->fileName = String::Duplicate(Path::GetFileName(scenario->path));
                highscore_days->days_record = daysValue;
                SaveSpeedrunDaysHighscores();
                return true;
            }
        }
        return false;
    }

private:
    scenario_index_entry * GetByFilename(const utf8 * filename)
    {
        const ScenarioRepository * repo = this;
        return (scenario_index_entry *)repo->GetByFilename(filename);
    }

    scenario_index_entry * GetByPath(const utf8 * path)
    {
        const ScenarioRepository * repo = this;
        return (scenario_index_entry *)repo->GetByPath(path);
    }

    /**
     * Mega Park from RollerCoaster Tycoon 1 is stored in an encrypted hidden file: mp.dat.
     * Decrypt the file and save it as sc21.sc4 in the user's scenario directory.
     */
    void ImportMegaPark()
    {
        auto mpdatPath = _env->GetFilePath(PATHID::MP_DAT);
        auto scenarioDirectory = _env->GetDirectoryPath(DIRBASE::USER, DIRID::SCENARIO);
        auto sc21Path = Path::Combine(scenarioDirectory, "sc21.sc4");
        if (File::Exists(mpdatPath) && !File::Exists(sc21Path))
        {
            ConvertMegaPark(mpdatPath, sc21Path);
        }
    }

    /**
     * Converts Mega Park to normalised file location (mp.dat to sc21.sc4)
     * @param Full path to mp.dat
     * @param Full path to sc21.dat
     */
    void ConvertMegaPark(const std::string &srcPath, const std::string &dstPath)
    {
        auto directory = Path::GetDirectory(dstPath);
        platform_ensure_directory_exists(directory.c_str());

        size_t length;
        auto mpdat = (uint8 *)(File::ReadAllBytes(srcPath, &length));

        // Rotate each byte of mp.dat left by 4 bits to convert
        for (size_t i = 0; i < length; i++)
        {
            mpdat[i] = rol8(mpdat[i], 4);
        }

        File::WriteAllBytes(dstPath, mpdat, length);
        Memory::FreeArray(mpdat, length);
    }

    void AddScenario(const scenario_index_entry &entry)
    {
        auto filename = Path::GetFileName(entry.path);

        if (!String::Equals(filename, ""))
        {
            auto existingEntry = GetByFilename(filename);
            if (existingEntry != nullptr)
            {
                std::string conflictPath;
                if (existingEntry->timestamp > entry.timestamp)
                {
                    // Existing entry is more recent
                    conflictPath = String::ToStd(existingEntry->path);

                    // Overwrite existing entry with this one
                    *existingEntry = entry;
                }
                else
                {
                    // This entry is more recent
                    conflictPath = entry.path;
                }
                Console::WriteLine("Scenario conflict: '%s' ignored because it is newer.", conflictPath.c_str());
            }
            else
            {
                _scenarios.push_back(entry);
            }
        }
        else
        {
            log_error("Tried to add scenario with an empty filename!");
        }
    }

    void Sort()
    {
        if (gConfigGeneral.scenario_select_mode == SCENARIO_SELECT_MODE_ORIGIN)
        {
            std::sort(_scenarios.begin(), _scenarios.end(), [](const scenario_index_entry &a,
                                                               const scenario_index_entry &b) -> bool
            {
                return scenario_index_entry_CompareByIndex(a, b) < 0;
            });
        }
        else
        {
            std::sort(_scenarios.begin(), _scenarios.end(), [](const scenario_index_entry &a,
                                                               const scenario_index_entry &b) -> bool
            {
                return scenario_index_entry_CompareByCategory(a, b) < 0;
            });
        }
    }

    void LoadScores()
    {
        std::string path = _env->GetFilePath(PATHID::SCORES);
        if (!platform_file_exists(path.c_str()))
        {
            return;
        }

        uint32 numHighscores = 0;
        uint32 fileVersion = 0;

        // Load scenario scores
        try
        {
            auto fs = FileStream(path, FILE_MODE_OPEN);
            fileVersion = fs.ReadValue<uint32>();
            if (fileVersion != HIGHSCORE_FILE_VERSION)
            {
                Console::Error::WriteLine("Invalid or incompatible highscores file.");
                return;
            }

            ClearHighscores();

            numHighscores = fs.ReadValue<uint32>();
            for (uint32 i = 0; i < numHighscores; i++)
            {
                scenario_highscore_entry * highscore = InsertHighscore();
                highscore->fileName = fs.ReadString();
                highscore->name = fs.ReadString();
                highscore->company_value = fs.ReadValue<money32>();
                highscore->timestamp = fs.ReadValue<datetime64>();
            }
        }
        catch (const std::exception &)
        {
            Console::Error::WriteLine("Error reading highscores.");
        }

        if (gConfigGeneral.enable_speedrunning_mode) {
            std::string path_speedrun_days = _env->GetFilePath(PATHID::SCORES_SPEEDRUN_DAYS);
            std::string path_speedrun_time = _env->GetFilePath(PATHID::SCORES_SPEEDRUN_TIME);

            // Load speedrunning scores (Days)
            try
            {
                auto fs = FileStream(path_speedrun_days, FILE_MODE_OPEN);
                fileVersion = fs.ReadValue<uint32>();
                if (fileVersion != SPEEDRUN_DAYS_RECORD_FILE_VERSION)
                {
                    Console::Error::WriteLine("Invalid or incompatible highscores file.");
                    return;
                }

                ClearSpeedrunTimeHighscores();

                numHighscores = fs.ReadValue<uint32>();
                for (uint32 i = 0; i < numHighscores; i++)
                {
                    scenario_speedrun_days_record_entry * highscore = InsertSpeedrunDaysHighscore();
                    highscore->fileName = fs.ReadString();
                    highscore->days_record = fs.ReadValue<uint32>();
                }
            }
            catch (const std::exception &)
            {
                Console::Error::WriteLine("Error reading highscores.");
            }

            // Load speedrunning scores (Time)
            try
            {
                auto fs = FileStream(path_speedrun_time, FILE_MODE_OPEN);
                fileVersion = fs.ReadValue<uint32>();
                if (fileVersion != SPEEDRUN_TIME_RECORD_FILE_VERSION)
                {
                    Console::Error::WriteLine("Invalid or incompatible highscores file.");
                    return;
                }

                ClearSpeedrunDaysHighscores();

                numHighscores = fs.ReadValue<uint32>();
                for (uint32 i = 0; i < numHighscores; i++)
                {
                    scenario_speedrun_time_record_entry * highscore = InsertSpeedrunTimeHighscore();
                    highscore->fileName = fs.ReadString();
                    highscore->time_record = fs.ReadValue<datetime64>();
                }
            }
            catch (const std::exception &)
            {
                Console::Error::WriteLine("Error reading highscores.");
            }
        }
    }

    /**
     * Loads the original scores.dat file and replaces any highscores that
     * are better for matching scenarios.
     */
    void LoadLegacyScores()
    {
        std::string rct2Path = _env->GetFilePath(PATHID::SCORES_RCT2);
        std::string legacyPath = _env->GetFilePath(PATHID::SCORES_LEGACY);
        LoadLegacyScores(legacyPath);
        LoadLegacyScores(rct2Path);
    }

    void LoadLegacyScores(const std::string &path)
    {
        if (!platform_file_exists(path.c_str()))
        {
            return;
        }

        bool highscoresDirty = false;
        try
        {
            auto fs = FileStream(path, FILE_MODE_OPEN);
            if (fs.GetLength() <= 4)
            {
                // Initial value of scores for RCT2, just ignore
                return;
            }

            // Load header
            auto header = fs.ReadValue<rct_scores_header>();
            for (uint32 i = 0; i < header.ScenarioCount; i++)
            {
                // Read legacy entry
                auto scBasic = fs.ReadValue<rct_scores_entry>();

                // Ignore non-completed scenarios
                if (scBasic.Flags & SCENARIO_FLAGS_COMPLETED)
                {
                    bool notFound = true;
                    for (auto &highscore : _highscores)
                    {
                        if (String::Equals(scBasic.Path, highscore->fileName, true))
                        {
                            notFound = false;

                            // Check if legacy highscore is better
                            if (scBasic.CompanyValue > highscore->company_value)
                            {
                                SafeFree(highscore->name);
                                highscore->name = win1252_to_utf8_alloc(scBasic.CompletedBy, Util::CountOf(scBasic.CompletedBy));
                                highscore->company_value = scBasic.CompanyValue;
                                highscore->timestamp = DATETIME64_MIN;
                                break;
                            }
                        }
                    }
                    if (notFound)
                    {
                        scenario_highscore_entry * highscore = InsertHighscore();
                        highscore->fileName = String::Duplicate(scBasic.Path);
                        highscore->name = win1252_to_utf8_alloc(scBasic.CompletedBy, Util::CountOf(scBasic.CompletedBy));
                        highscore->company_value = scBasic.CompanyValue;
                        highscore->timestamp = DATETIME64_MIN;
                    }
                }
            }
        }
        catch (const std::exception &)
        {
            Console::Error::WriteLine("Error reading legacy scenario scores file: '%s'", path.c_str());
        }

        if (highscoresDirty)
        {
            SaveHighscores();
        }
    }

    void ClearHighscores()
    {
        for (auto highscore : _highscores)
        {
            scenario_highscore_free(highscore);
        }
        _highscores.clear();
    }

    void ClearSpeedrunDaysHighscores()
    {
        for (auto highscore : _speedrunDaysHighscores)
        {
            scenario_speedrun_highscores_days_free(highscore);
        }
        _speedrunDaysHighscores.clear();
    }

    void ClearSpeedrunTimeHighscores()
    {
        for (auto highscore : _speedrunTimesHighscores)
        {
            scenario_speedrun_highscores_time_free(highscore);
        }
        _speedrunTimesHighscores.clear();
    }

    scenario_highscore_entry * InsertHighscore()
    {
        auto highscore = new scenario_highscore_entry();
        memset(highscore, 0, sizeof(scenario_highscore_entry));
        _highscores.push_back(highscore);
        return highscore;
    }

    scenario_speedrun_days_record_entry * InsertSpeedrunDaysHighscore()
    {
        auto highscore = new scenario_speedrun_days_record_entry();
        memset(highscore, 0, sizeof(scenario_speedrun_days_record_entry));
        _speedrunDaysHighscores.push_back(highscore);
        return highscore;
    }

    scenario_speedrun_time_record_entry * InsertSpeedrunTimeHighscore()
    {
        auto highscore = new scenario_speedrun_time_record_entry();
        memset(highscore, 0, sizeof(scenario_speedrun_time_record_entry));
        _speedrunTimesHighscores.push_back(highscore);
        return highscore;
    }

    void AttachHighscores()
    {
        for (auto &highscore : _highscores)
        {
            scenario_index_entry * scenario = GetByFilename(highscore->fileName);
            if (scenario != nullptr)
            {
                scenario->highscore = highscore;
            }
        }
        for (auto &highscore : _speedrunDaysHighscores)
        {
            scenario_index_entry * scenario = GetByFilename(highscore->fileName);
            if (scenario != nullptr)
            {
                scenario->days_record = highscore;
            }
        }
        for (auto &highscore : _speedrunTimesHighscores)
        {
            scenario_index_entry * scenario = GetByFilename(highscore->fileName);
            if (scenario != nullptr)
            {
                scenario->time_record = highscore;
            }
        }
    }

    void SaveHighscores()
    {
        std::string path = _env->GetFilePath(PATHID::SCORES);
        try
        {
            auto fs = FileStream(path, FILE_MODE_WRITE);
            fs.WriteValue<uint32>(HighscoreFileVersion);
            fs.WriteValue<uint32>((uint32)_highscores.size());
            for (size_t i = 0; i < _highscores.size(); i++)
            {
                const scenario_highscore_entry * highscore = _highscores[i];
                fs.WriteString(highscore->fileName);
                fs.WriteString(highscore->name);
                fs.WriteValue(highscore->company_value);
                fs.WriteValue(highscore->timestamp);
            }
        }
        catch (const std::exception &)
        {
            Console::Error::WriteLine("Unable to save highscores to '%s'", path.c_str());
        }
    }

    void SaveSpeedrunDaysHighscores()
    {
        std::string path = _env->GetFilePath(PATHID::SCORES_SPEEDRUN_DAYS);
        try
        {
            auto fs = FileStream(path, FILE_MODE_WRITE);
            fs.WriteValue<uint32>(SpeedrunDaysRecordFileVersion);
            fs.WriteValue<uint32>((uint32)_speedrunDaysHighscores.size());
            for (size_t i = 0; i < _speedrunDaysHighscores.size(); i++)
            {
                const scenario_speedrun_days_record_entry * highscore = _speedrunDaysHighscores[i];
                fs.WriteString(highscore->fileName);
                fs.WriteValue(highscore->days_record);
            }
        }
        catch (const std::exception &)
        {
            Console::Error::WriteLine("Unable to save speedrun days highscores to '%s'", path.c_str());
        }
    }

    void SaveSpeedrunTimeHighscores()
    {
        std::string path = _env->GetFilePath(PATHID::SCORES_SPEEDRUN_TIME);
        try
        {
            auto fs = FileStream(path, FILE_MODE_WRITE);
            fs.WriteValue<uint32>(SpeedrunTimeRecordFileVersion);
            fs.WriteValue<uint32>((uint32)_speedrunTimesHighscores.size());
            for (size_t i = 0; i < _speedrunTimesHighscores.size(); i++)
            {
                const scenario_speedrun_time_record_entry * highscore = _speedrunTimesHighscores[i];
                fs.WriteString(highscore->fileName);
                fs.WriteValue(highscore->time_record);
            }
        }
        catch (const std::exception &)
        {
            Console::Error::WriteLine("Unable to save speedrun time highscores to '%s'", path.c_str());
        }
    }
};

static ScenarioRepository * _scenarioRepository;

IScenarioRepository * CreateScenarioRepository(IPlatformEnvironment * env)
{
    _scenarioRepository = new ScenarioRepository(env);
    return _scenarioRepository;
}

IScenarioRepository * GetScenarioRepository()
{
    return _scenarioRepository;
}

void scenario_repository_scan()
{
    IScenarioRepository * repo = GetScenarioRepository();
    repo->Scan();
}

size_t scenario_repository_get_count()
{
    IScenarioRepository * repo = GetScenarioRepository();
    return repo->GetCount();
}

const scenario_index_entry *scenario_repository_get_by_index(size_t index)
{
    IScenarioRepository * repo = GetScenarioRepository();
    return repo->GetByIndex(index);
}

    bool scenario_repository_try_record_highscore(const utf8 * scenarioFileName, money32 companyValue, const utf8 * name)
    {
        IScenarioRepository * repo = GetScenarioRepository();
        return repo->TryRecordHighscore(scenarioFileName, companyValue, name);
    }

    bool scenario_repository_try_record_speedrun_highscore(const utf8 * scenarioFileName, uint32 daysValue)
    {
        IScenarioRepository * repo = GetScenarioRepository();
        return repo->TryRecordSpeedrunRecords(scenarioFileName, daysValue);
    }
}

