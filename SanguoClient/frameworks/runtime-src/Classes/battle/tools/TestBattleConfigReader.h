//
//  TestBattleConfigReader.h
//  sanguoClient
//
//  Created by fuchenhao on 5/17/15.
//
//

#ifndef __sanguoClient__TestBattleConfigReader__
#define __sanguoClient__TestBattleConfigReader__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#define TEST_BATTLE_CONFIG_READER  (true)
#else
#define TEST_BATTLE_CONFIG_READER  (false)
#endif /* CC_TARGET_PLATFORM */

#include "cocos2d.h"
#include "json/document.h"

class TestBattleConfigReader
{
public:
    struct FileNamesResult
    {
        std::string folderPath;
        std::string fileExtension;
        std::vector<std::string*> filenames;
        
        ~FileNamesResult()
        {
            for (auto* pName : filenames)
            {
                CC_SAFE_DELETE(pName);
            }
            filenames.clear();
        }
    };
    
    struct MonsterResult
    {
        std::vector<rapidjson::Value*> monsters;
        std::vector<rapidjson::Value*> monstersFiltered;
        
        rapidjson::Document doc;
        
        static bool compareValue(const std::string& t1, const std::string& t2)
        {
            int lv1 = atoi(t1.c_str());
            int lv2 = atoi(t2.c_str());
            return lv1 < lv2;
        }
        
        void resetFilter()
        {
            monstersFiltered.clear();
            monstersFiltered = monsters;
        }
        
        void enumValues(const std::string& key, std::vector<std::string>& values)
        {
            std::map<std::string, bool> valueMaps;
            
            for (auto* pData : monstersFiltered)
            {
                if (pData->HasMember(key.c_str()))
                {
                    auto& data = *pData;
                    if (!data[key.c_str()].IsNull())
                    {
                        valueMaps[data[key.c_str()].GetString()] = true;
                    }
                }
            }
            
            values.clear();
            for (auto& iter : valueMaps)
            {
                values.push_back(iter.first);
            }
            
            if (key == "level")
            {
                std::sort(values.begin(), values.end(), MonsterResult::compareValue);
            }
        }
        
        void filterMonsters(const std::string& key, const std::string& value)
        {
            if (value.length() > 0)
            {
                std::vector<rapidjson::Value*> monstersCopy = monstersFiltered;
                
                monstersFiltered.clear();
                
                for (auto* pData : monstersCopy)
                {
                    if (pData->HasMember(key.c_str()))
                    {
                        if (std::string((*pData)[key.c_str()].GetString()) == value)
                        {
                            monstersFiltered.push_back(pData);
                        }
                    }
                }
            }
        }
    };
    
    struct CSVHeroData
    {
        std::string heroId;
        std::string subType;
        std::map<std::string, std::string> columnValues;
    };
    
    struct CSVHeroResult
    {
        std::vector<std::string> heroIds;
        std::map<std::string, std::vector<std::string>> heroSubTypes;
        std::vector<CSVHeroData> heroDatas;
        
        CSVHeroData* findFirst(const std::string& heroId)
        {
            for (auto& heroData : heroDatas)
            {
                if (heroData.heroId == heroId)
                {
                    return &heroData;
                }
            }
            return nullptr;
        }
        
        CSVHeroData* findByIdAndSubTypes(const std::string& heroId, const std::string& heroSubType)
        {
            for (auto& heroData : heroDatas)
            {
                if (heroData.heroId == heroId && heroData.subType == heroSubType)
                {
                    return &heroData;
                }
            }
            return nullptr;
        }
    };
    
    struct CSVSoldierData
    {
        std::string soldierId;
        std::string subType;
        std::map<std::string, std::string> columnValues;
    };
    
    struct CSVSoldierResult
    {
        std::vector<std::string> soldierIds;
        std::map<std::string, std::vector<std::string>> soldierSubTypes;
        std::vector<CSVSoldierData> soldierDatas;
        
        CSVSoldierData* findFirst(const std::string& soldierId)
        {
            for (auto& soldierData : soldierDatas)
            {
                if (soldierData.soldierId == soldierId)
                {
                    return &soldierData;
                }
            }
            return nullptr;
        }
        
        CSVSoldierData* findByIdAndSubTypes(const std::string& soldierId, const std::string& soldierSubType)
        {
            for (auto& soldierData : soldierDatas)
            {
                if (soldierData.soldierId == soldierId && soldierData.subType == soldierSubType)
                {
                    return &soldierData;
                }
            }
            return nullptr;
        }
    };
    
    static void filenamesAtFolder(const std::string& folderPath, const std::string& fileExtension, FileNamesResult& result);
    static void readTestConfig(const std::string& filename, const FileNamesResult& result, std::string& content);
    static void autoGenerateSkillTestConfig();
    
    static void readHeroCSV(CSVHeroResult& result);
    static void readSoldierCSV(CSVSoldierResult& result);
    
    static void readMonsterJSON(MonsterResult& result);
    static void saveTempFile(const std::string& fileName, const std::string& content);

private:
    static void split(const std::string &str, const std::string& seperator, std::vector<std::string>& ret);
};



#endif /* defined(__sanguoClient__TestBattleConfigReader__) */
