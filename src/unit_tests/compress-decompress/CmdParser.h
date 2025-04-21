#pragma once

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

class CmdParser {
public:
    CmdParser(int argc, char** argv);

    std::vector<std::string> GetListByArg(const std::string& s);

    std::vector<std::string> GetAllArgs() const;

    int GetArgsNumber() const;

    bool IsExistsArg(const std::string& s) const;

    std::string GetById(int id) const;

    std::vector<std::string> GetAllString() const;

private:
    std::map<std::string, std::vector<std::string>> list_for_args_;
    std::vector<std::string> independent_strings_;
};
