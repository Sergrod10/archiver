#include "CmdParser.h"

#include <iostream>

CmdParser::CmdParser(int argc, char **argv) {
    std::string cur_arg;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            cur_arg = argv[i];
            cur_arg = cur_arg.substr(1, cur_arg.size() - 1);
            if (list_for_args_.find(cur_arg) == list_for_args_.end()) {
                list_for_args_[cur_arg] = {};
            }
            if (cur_arg == "h") {
                cur_arg = "";
            }
        } else if (!cur_arg.empty()) {
            list_for_args_[cur_arg].push_back(argv[i]);
        } else {
            independent_strings_.push_back(argv[i]);
        }
    }
    for (auto &u : list_for_args_) {
        u.second.erase(std::unique(u.second.begin(), u.second.end()), u.second.end());
    }
}

std::vector<std::string> CmdParser::GetListByArg(const std::string &s) {
    if (!IsExistsArg(s)) {
        return {};
    }
    return list_for_args_[s];
}

std::vector<std::string> CmdParser::GetAllArgs() const {
    std::vector<std::string> ans(GetArgsNumber());
    int id = 0;
    for (auto &u : list_for_args_) {
        ans[id++] = u.first;
    }
    return ans;
}

int CmdParser::GetArgsNumber() const {
    return static_cast<int>(list_for_args_.size());
}

bool CmdParser::IsExistsArg(const std::string &s) const {
    return (list_for_args_.find(s) != list_for_args_.end());
}

std::string CmdParser::GetById(int id) const {
    if (id >= independent_strings_.size()) {
        throw std::runtime_error("cmd_parser: tried to get not exists string");
    }
    return independent_strings_[id];
}

std::vector<std::string> CmdParser::GetAllString() const {
    return independent_strings_;
}
