/*
 * FileEngine.cpp
 *
 *  Created on: 03.08.2020
 *      Author: Stefan_2
 */

#include "FileEngine.h"

FileEngine::FileEngine() {
    search_recursive = false;
    start_directory = ".";

}

FileEngine::~FileEngine() {
    // TODO Auto-generated destructor stub
}

void FileEngine::SetSearchRecursive(bool const b)
{
    search_recursive = b;
}

bool FileEngine::GetSearchRecursive() const
{
    return search_recursive;
}

void FileEngine::SetStartDirectory(std::string const sd)
{
    start_directory = sd;
}

void FileEngine::CreateFileTokenData(auto const &entry, std::map<std::string, DataType> &filetype_identifier, std::vector<FileTokenData> &result) const
{
    if (entry.is_regular_file()) {
        std::string ext = entry.path().extension();
        if(filetype_identifier.count(ext) > 0)
        {
            FileTokenData d(filetype_identifier[ext]);
            d.filepath = entry.path();

            result.push_back(d);
        }
    }
}

std::vector<FileTokenData> FileEngine::GetListOfFiles() const
{
    std::vector<FileTokenData> result;
    std::map<std::string, DataType> filetype_identifier;

    filetype_identifier[".rs"] = DataType::RequirementsSpecification;
    filetype_identifier[".rd"] = DataType::RequirementsData;

    if(GetSearchRecursive())
    {
        for(const auto& entry: std::filesystem::recursive_directory_iterator(start_directory))
        {
            CreateFileTokenData(entry, filetype_identifier, result);
        }
    }
    else
    {
        for(const auto& entry: std::filesystem::directory_iterator(start_directory))
        {
            CreateFileTokenData(entry, filetype_identifier, result);
        }
    }

    return result;
}
