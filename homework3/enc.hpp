#ifdef _WIN32
#define initialize_system_encoding() system("chcp 65001")
#else
#define initialize_system_encoding()
#endif