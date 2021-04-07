/* SPDX-License-Identifier: MIT */
/* Copyright (c) 2020-present Stefan Schlichthärle */

#include "FileEngine.h"

bool ftd_cmp(FileTokenData const &a, FileTokenData const &b) {
    if (a.filepath.compare(b.filepath) < 0)
        return true;
    else
        return false;
};

FileEngine::FileEngine(Logger &logger) : l(logger) { search_recursive = false; }

FileEngine::~FileEngine() {}

void FileEngine::SetSearchRecursive(bool const b) { search_recursive = b; }

bool FileEngine::GetSearchRecursive() const { return search_recursive; }

void FileEngine::SetDirectory(std::string const sd) {
    directories.push_back(sd);
}

template <typename T>
void FileEngine::CreateFileTokenData(
    T const &entry, std::map<std::string, DataType> &filetype_identifier,
    std::set<FileTokenData, decltype(ftd_cmp) *> &files_sorted) const {
    if (entry.is_regular_file()) {
        std::string ext = entry.path().extension();
        if (filetype_identifier.count(ext) > 0) {
            l.LOG(LogLevel::DBUG, "relevant file found: ");
            l.LOG(LogLevel::DBUG, entry.path());
            FileTokenData d(filetype_identifier[ext]);
            d.filepath = entry.path();

            files_sorted.insert(d);
        }
    }
}

std::vector<FileTokenData> FileEngine::GetListOfFiles() const {
    // Set contains the list of files identified in the file system, sorted by
    // name
    std::set<FileTokenData, decltype(ftd_cmp) *> files_sorted(ftd_cmp);
    std::map<std::string, DataType> filetype_identifier;

    filetype_identifier[".rs"] = DataType::RequirementsSpecification;
    filetype_identifier[".rd"] = DataType::RequirementsData;

    std::for_each(
        directories.begin(), directories.end(), [&](std::string const &dir) {
            if (GetSearchRecursive()) {
                for (const auto &entry :
                     std::filesystem::recursive_directory_iterator(dir)) {
                    CreateFileTokenData(entry, filetype_identifier,
                                        files_sorted);
                }
            } else {
                for (const auto &entry :
                     std::filesystem::directory_iterator(dir)) {
                    CreateFileTokenData(entry, filetype_identifier,
                                        files_sorted);
                }
            }
        });

    if (directories.size() == 0)
        l.LOG(LogLevel::WARNING, "No input path specified.");

    // now transfer from set into a vector
    std::vector<FileTokenData> result;
    for (auto &e : files_sorted) result.push_back(e);

    return result;
}
