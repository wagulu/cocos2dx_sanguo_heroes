//
//  TestBattleConfigReader.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/17/15.
//
//

#include "TestBattleConfigReader.h"
#include "json/rapidjson.h"
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "json/prettywriter.h"

void TestBattleConfigReader::filenamesAtFolder(const std::string& folderPath, const std::string& fileExtension, FileNamesResult& result)
{
#if TEST_BATTLE_CONFIG_READER
    
    std::string fullPath = cocos2d::FileUtils::getInstance()->fullPathForFilename(folderPath);
    result.folderPath = fullPath;
    result.fileExtension = fileExtension;
    
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSURL* url = [NSURL URLWithString:[[NSString stringWithUTF8String:fullPath.c_str()] stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    NSArray* contents = [fileManager contentsOfDirectoryAtURL:url
                                   includingPropertiesForKeys:@[]
                                                      options:NSDirectoryEnumerationSkipsHiddenFiles
                                                        error:nil];
    
    NSPredicate* predicate = [NSPredicate predicateWithFormat:[NSString stringWithFormat:@"pathExtension ENDSWITH '%@'", [NSString stringWithUTF8String:fileExtension.c_str()]]];
    for (NSURL* path in [contents filteredArrayUsingPredicate:predicate])
    {
        result.filenames.push_back(new std::string([[[[path path] lastPathComponent] stringByDeletingPathExtension] UTF8String]));
    }

#endif
}

void TestBattleConfigReader::readTestConfig(const std::string& filename, const FileNamesResult& result, std::string& content)
{
    std::string fullpath = cocos2d::StringUtils::format("%s/%s.%s", result.folderPath.c_str(), filename.c_str(), result.fileExtension.c_str());
    content = cocos2d::FileUtils::getInstance()->getStringFromFile(fullpath);
}

void TestBattleConfigReader::autoGenerateSkillTestConfig()
{
#if TEST_BATTLE_CONFIG_READER
    std::string outputPath = cocos2d::FileUtils::getInstance()->getWritablePath() + "config/test/skill";
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSString* dirPath = [NSString stringWithUTF8String:outputPath.c_str()];
    
    if (![fileManager fileExistsAtPath:dirPath])
    {
        [fileManager createDirectoryAtPath:dirPath withIntermediateDirectories:YES attributes:nil error:nil];
    }
    
    FileNamesResult result;
    filenamesAtFolder("config/skill", "json", result);
    
    int autoId = 0;
    
    for (auto* pFileName : result.filenames)
    {
        std::string filePath = result.folderPath + "/" + *pFileName + "." + result.fileExtension;
        
        std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile(filePath);
        
        rapidjson::Document doc;
        doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
    
        int mpCost = 1;
        float energyStart = 1;
        float energyCost = 1;
        float energySpeed = 20;
        
        if (doc.HasMember("mpCost")) mpCost = doc["mpCost"].GetInt();
        if (doc.HasMember("energyStart")) energyStart = doc["energyStart"].GetDouble();
        if (doc.HasMember("energyCost")) energyCost = doc["energyCost"].GetDouble();
        
        rapidjson::Document docOutput;
        auto& allocator = docOutput.GetAllocator();
        
        rapidjson::Value root(rapidjson::kObjectType);
        root.AddMember("config", (*pFileName).c_str(), allocator);
        root.AddMember("id", ++autoId, allocator);
        root.AddMember("name", (*pFileName).c_str(), allocator);
        root.AddMember("level", 1, allocator);
        root.AddMember("mpCost", mpCost, allocator);
        root.AddMember("energyStart", energyStart, allocator);
        root.AddMember("energyCost", energyCost, allocator);
        root.AddMember("energySpeed", energySpeed, allocator);
        
        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        root.Accept(writer);
        
        NSData* outputData = [[NSString stringWithUTF8String:buffer.GetString()] dataUsingEncoding:NSUTF8StringEncoding];
        NSString *outputFilePath = [dirPath stringByAppendingPathComponent:[NSString stringWithUTF8String:(*pFileName + ".json").c_str()]];
        
        if (![fileManager fileExistsAtPath:outputFilePath])
        {
            [fileManager createFileAtPath:outputFilePath contents:outputData attributes:nil];
            
            CCLOG("create test skill %s", (*pFileName).c_str());
        }
    }

#endif
}

void TestBattleConfigReader::saveTempFile(const std::string& fileName, const std::string& content)
{
#if TEST_BATTLE_CONFIG_READER
    std::string outputPath = cocos2d::FileUtils::getInstance()->getWritablePath() + fileName;
    
    NSFileManager* fileManager = [NSFileManager defaultManager];
    
    NSData* outputData = [[NSString stringWithUTF8String:content.c_str()] dataUsingEncoding:NSUTF8StringEncoding];
    [fileManager createFileAtPath:[NSString stringWithUTF8String:outputPath.c_str()] contents:outputData attributes:nil];
#endif
}

void TestBattleConfigReader::readMonsterJSON(MonsterResult& result)
{
    std::string str = cocos2d::CCFileUtils::getInstance()->getStringFromFile("config/test/monster/monster.json");
    
    result.doc.Parse<rapidjson::kParseDefaultFlags>(str.c_str());
    
    if (result.doc.IsArray())
    {
        auto num = result.doc.Size();
        for (int i = 0; i < num; i++)
        {
            result.monsters.push_back(&result.doc[i]);
        }
        result.resetFilter();
    }
}

void TestBattleConfigReader::readHeroCSV(CSVHeroResult& result)
{
    std::string str = cocos2d::CCFileUtils::getInstance()->getStringFromFile("config/test/csv/hero.csv");
    
    std::vector<std::string> rows;
    split(str, "\n", rows);
    
    if (rows.size() >= 2)
    {
        std::vector<std::string> columnNames;
        split(rows[1], ",", columnNames);
        
        for (int i = 2; i < rows.size(); i++)
        {
            std::vector<std::string> columnValues;
            split(rows[i], ",", columnValues);
            
            CSVHeroData heroData;
            
            for (int j = 0; j < columnNames.size(); j++)
            {
                std::string key = columnNames[j];
                std::string value = columnValues[j];
                if (key == "id")
                {
                    heroData.heroId = value;
                }
                else if (key == "subType")
                {
                    heroData.subType = value;
                }
                
                heroData.columnValues.insert(std::pair<std::string, std::string>(key, value));
            }
            
            if (heroData.heroId.size() > 0)
            {
                result.heroDatas.push_back(heroData);
                result.heroSubTypes[heroData.heroId].push_back(heroData.subType);
            }
        }
        
        for (auto& iter : result.heroSubTypes)
        {
            result.heroIds.push_back(iter.first);
        }
        
        CCLOG("read hero csv done");
    }
}

void TestBattleConfigReader::readSoldierCSV(CSVSoldierResult& result)
{
    std::string str = cocos2d::CCFileUtils::getInstance()->getStringFromFile("config/test/csv/soldier.csv");
    
    std::vector<std::string> rows;
    split(str, "\n", rows);
    
    if (rows.size() >= 2)
    {
        std::vector<std::string> columnNames;
        split(rows[1], ",", columnNames);
        
        for (int i = 2; i < rows.size(); i++)
        {
            std::vector<std::string> columnValues;
            split(rows[i], ",", columnValues);
            
            CSVSoldierData soldierData;
            
            for (int j = 0; j < columnNames.size(); j++)
            {
                std::string key = columnNames[j];
                std::string value = columnValues[j];
                if (key == "id")
                {
                    soldierData.soldierId = value;
                }
                else if (key == "subType")
                {
                    soldierData.subType = value;
                }
                
                soldierData.columnValues.insert(std::pair<std::string, std::string>(key, value));
            }
            
            if (soldierData.soldierId.size() > 0)
            {
                result.soldierDatas.push_back(soldierData);
                result.soldierSubTypes[soldierData.soldierId].push_back(soldierData.subType);
            }
        }
        
        for (auto& iter : result.soldierSubTypes)
        {
            result.soldierIds.push_back(iter.first);
        }
        
        CCLOG("read soldier csv done");
    }
}

void TestBattleConfigReader::split(const std::string &str, const std::string& seperator, std::vector<std::string>& ret)
{
    ret.clear();
    
    size_t nStartPosFound = str.find(seperator, 0);
    size_t nFieldStart = 0;
    for (; nStartPosFound != -1; nStartPosFound = str.find(seperator, nStartPosFound))
    {
        std::string strSub = str.substr(nFieldStart, nStartPosFound - nFieldStart);
        nStartPosFound = nStartPosFound + seperator.size();
        nFieldStart = nStartPosFound;
        
        ret.push_back(strSub);
    }
    
    if (nFieldStart < str.size())
    {
        std::string strSub = str.substr(nFieldStart, str.size() - nFieldStart);
        ret.push_back(strSub);
    }
}
