#include "path.h"
#include "file.h"
#include <sys/stat.h>
#include <ctime>
#include <dirent.h>

namespace file {
    path::path() = default;

    path::path(const std::string& p) : m_path(p) {}

    path::path(const char* p) : m_path(p ? p : "") {}

    const char* path::c_str () const {
        return this->m_path.c_str();
    }
    path::operator std::string() const {
        return m_path;
    }

    std::string path::string() const {
        return m_path;
    }

    path path::operator/(const path& other) const {
        if (m_path.empty()) return other;
        if (other.m_path.empty()) return *this;
        if (m_path.back() == '/' || m_path.back() == '\\') {
            return path(m_path + other.m_path);
        }
        return path(m_path + "/" + other.m_path);
    }

    path path::stem() const {
        // 1. Extraire le nom du fichier (après le dernier '/' ou '\')
        size_t slash_pos = m_path.find_last_of("/\\");
        std::string fname = (slash_pos == std::string::npos)
                            ? m_path
                            : m_path.substr(slash_pos + 1);

        if (fname == "." || fname == "..") {
            return path(fname);
        }

        // 2. Retirer l'extension (après le dernier '.')
        size_t dot_pos = fname.find_last_of('.');
        if (dot_pos == std::string::npos || dot_pos == 0) {
            return path(fname);
        }
        return path(fname.substr(0, dot_pos));
    }

    bool path::is_regular_file() const{
        struct stat info;
        if (stat(this->m_path.c_str(), &info) != 0) return false;
        return (info.st_mode & S_IFREG) != 0;
    }

    std::string path::extension() const {
        size_t slash_pos = m_path.find_last_of("/\\");
        std::string fname = (slash_pos == std::string::npos)
                            ? m_path
                            : m_path.substr(slash_pos + 1);

        if (fname == "." || fname == "..") {
            return path(fname);
        }

        // 2. Garder que l'extension (après le dernier '.')
        size_t dot_pos = fname.find_last_of('.');
        if (dot_pos == std::string::npos || dot_pos == 0) {
            return path(fname);
        }
        return path(fname.substr(dot_pos));
    }


    bool file::exists(const path& p) {
        struct stat info;
        return stat(p.c_str(), &info) == 0;
    }

    bool file::is_directory(const path& p) {
        struct stat info;
        if (stat(p.c_str(), &info) != 0) return false;
        return (info.st_mode & S_IFDIR) != 0;
    }

    bool file::is_regular_file(const path& p) {
        struct stat info;
        if (stat(p.c_str(), &info) != 0) return false;
        return (info.st_mode & S_IFREG) != 0;
    }

    std::string file::format_last_write_time(const path& p) {
        struct stat info;
        info.st_mtim;

        if (stat(p.c_str(), &info) != 0) {
            return "";
        }

        std::tm tm_info{};
        localtime_r(&info.st_mtim.tv_sec, &tm_info);
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d %02d:%02d:%02d",
                  tm_info.tm_mday,
                  tm_info.tm_mon + 1,
                  tm_info.tm_year + 1900,
                  tm_info.tm_hour,
                  tm_info.tm_min,
                  tm_info.tm_sec);

        return std::string(buffer);
    }

    int64_t file::last_write_time(const path& p) {
        struct stat info;
        if (stat(p.c_str(), &info) != 0) {
            return 0;
        }
        return info.st_mtim.tv_sec;
    }

    std::vector<path> file::directory_iterator(const path& p) {
        std::vector<path> entries;
        DIR* dir = opendir(p.c_str());
        if (!dir) {
            return entries;
        }

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string name(entry->d_name);
            if (name != "." && name != "..") {
                entries.push_back(p / path(name));
            }
        }
        closedir(dir);
        return entries;
    }

}