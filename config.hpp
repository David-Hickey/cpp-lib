#pragma once

#include <string>
#include <stdexcept>
#include <sys/stat.h>
#include <SimpleIni.h>
#include <sstream>

class Config {

public:
    int get_int(const std::string& section, const std::string& key) const {
        return get<int>(section, key, this->ini);
    }

    double get_double(const std::string& section, const std::string& key) const {
        return get<double>(section, key, this->ini);
    }

    std::string get_string(const std::string& section, const std::string& key) const {
        return get<std::string>(section, key, this->ini);
    }

    std::string get_file_path() const {
        return this->file_path;
    };

    Config(const std::string file_path): file_path(file_path) {

       if (!file_exists(file_path.c_str())) {
           throw std::runtime_error("Invalid config file supplied");
       }

       ini.SetUnicode();
       ini.LoadFile(file_path.c_str());
    }

private:
    const std::string file_path;
    CSimpleIniA ini;

    template <class T>
    T get(const std::string& section, const std::string& key, const CSimpleIniA& ini) const {
        const char* c_string_value = ini.GetValue(section.c_str(), key.c_str(), NULL);

        if (c_string_value == NULL) {
            std::stringstream error_ss;
            error_ss << "No such key: '" << section << "/" << key << "'";
            throw std::invalid_argument(error_ss.str());
        }

        const std::string string_value(c_string_value);
        const T value = convert_numeric<T>(string_value);

        return value;
    }

    inline bool file_exists(const std::string& file_path) const {
        struct stat buffer;
        return (stat (file_path.c_str(), &buffer) == 0);
    }

    template <class T>
    inline T convert_numeric(const std::string& string_value) const {
        T value;

        std::stringstream ss;
        ss << string_value;
        ss >> value;

        if (ss.fail()) {
            throw std::runtime_error("Could not convert string to desired numeric");
        }

        return value;
    }
};
