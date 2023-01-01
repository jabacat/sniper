#pragma once

#include <vector>
#include <string>
#include "string.hpp"

[[noreturn]] void load_failed(int status);

template<typename T>
struct LoadResult {
    int status;
    T result;

    LoadResult(T&& result) : status(0), result(std::move(result)) {}
    static LoadResult with_status(int status) { return LoadResult(nullptr, status); }
    static LoadResult last_errno();

    operator T() && {
        if (!is_ok()) {
            load_failed(status);
        }
        return std::move(result);
    }

    bool is_ok() const {
        return status == 0;
    }

    private:
    LoadResult(std::nullptr_t, int status) : status(status) {}
};

LoadResult<std::vector<unsigned char>> load_file_as_bytes(ZStringView filename);
LoadResult<std::string> load_file_as_string(ZStringView filename);
