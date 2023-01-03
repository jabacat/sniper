# Use this only when changing the style spec. For local changes, use git clang-format
clang-format -i $(find src -path 'glad' -prune -o -name 'stb_*' -o -name '*.[ch]pp' -print)
