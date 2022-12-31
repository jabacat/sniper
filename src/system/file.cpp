#include "file.hpp"
#include "string.hpp"

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#if defined(__unix__)
#include <sys/mman.h>
#include <sys/stat.h>
#endif

template <typename T>
LoadResult<T> LoadResult<T>::last_errno() {
    return LoadResult::with_status(errno);
}

struct FileHandle {
    FILE * fp;

    FileHandle(FILE * fp) : fp(fp) {}
    ~FileHandle() {
        if (fp)
            std::fclose(fp);
    }

    operator FILE * () const {
        return fp;
    }

    void close() {
        if (fp) {
            std::fclose(fp);
            fp = nullptr;
        }
    }

    #if defined(__unix__)
    int fd() const {
        return fileno(fp);
    }
    #endif
};

template <typename T>
static LoadResult<T> load_file(ZStringView filename) {
    FileHandle fp = std::fopen(filename, "r");
    if (!fp)
        return LoadResult<T>::last_errno();

    // Strategy 1. Try mmap
    #if defined (__unix__)
    do {
        struct stat stat;
        int fd = fp.fd();
        if (fstat(fd, &stat) < -1) {
            break;
        }
        if (stat.st_size == 0) {
            // If file actually has 0 size, we'll catch that quickly
            // Skip the ftell method
            goto _S_read_in_loop;
        }
        size_t size = stat.st_size;
        void* mem = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (mem == MAP_FAILED) {
            break;
        }
        fp.close();
        const unsigned char* bytes = reinterpret_cast<const unsigned char *>(mem);
        T buf(bytes, bytes + size);
        munmap(mem, size);
        return std::move(buf);
    } while (false);
    #endif
    // Strategy 2. Try seeking to the end & reading
    do {
        if (std::fseek(fp, 0, SEEK_END) < 0) {
            break;
        }
        auto len = std::ftell(fp);
        if (len <= 0) {
            // If len is 0, we may still try to read it in a loop.
            if (std::fseek(fp, 0, SEEK_SET) < 0) {
                // Something went very wrong!
                return LoadResult<T>::last_errno();
            }
            break;
        }
        T buf(len, static_cast<unsigned char>(0));
        if (std::fseek(fp, 0, SEEK_SET) < 0) {
            // Something went very wrong!
            return LoadResult<T>::last_errno();
        }
        for (size_t left = len; left; ) {
            size_t read = std::fread(buf.data() + (len - left), 1, left, fp);
            if (read == 0) {
                // We handled left == 0 already
                return LoadResult<T>::last_errno();
            }
            left -= read;
        }
        return std::move(buf);
    } while (false);
    // Strategy 3. Just read it in a loop
    _S_read_in_loop:
    {
        T buf;
        while (true) {
            buf.reserve(buf.capacity() + 0x100);
            size_t cur = buf.size();
            buf.resize(buf.capacity());
            size_t cap = buf.capacity() - cur;
            size_t read = std::fread(buf.data() + cur, 1, cap, fp);
            if (read == 0) {
                if (std::feof(fp)) {
                    buf.resize(cur);
                    break;
                } else
                    return LoadResult<T>::last_errno();
            }
            buf.resize(cur + read);
        }
        return std::move(buf);
    }
}

LoadResult<std::vector<unsigned char>> load_file_as_bytes(ZStringView filename) {
    return load_file<std::vector<unsigned char>>(filename);
}

LoadResult<std::string> load_file_as_string(ZStringView filename) {
    return load_file<std::string>(filename);
}

[[noreturn]] void load_failed(int status) {
    std::fputs("FATAL: Failed to load file: ", stderr);
    std::fputs(strerror(status), stderr);
    std::fputs("\nAborting.\n", stderr);
    std::abort();
}
