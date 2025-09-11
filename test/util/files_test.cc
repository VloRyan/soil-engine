#include "util/files.h"

#include <filesystem>

#include "gtest/gtest.h"

namespace soil::util {
    class FilesTest : public testing::Test {};

    TEST_F(FilesTest, Exists) {
        const std::filesystem::path tempFilePath = std::filesystem::temp_directory_path() / std::tmpnam(nullptr);
        auto* file = fopen(tempFilePath.c_str(), "w");
        std::fputs("Hello, world", file);
        fclose(file);

        EXPECT_TRUE(Files::Exists(tempFilePath));
        std::remove(tempFilePath.c_str());
        EXPECT_FALSE(Files::Exists(tempFilePath));
    }

    TEST_F(FilesTest, GetDirectory) {
        // TODO
    }

    TEST_F(FilesTest, IsAbsolute) {
#ifdef _GLIBCXX_FILESYSTEM_IS_WINDOWS
        EXPECT_TRUE(Files::IsAbsolute("c:\\folder\\"));
#else
        EXPECT_TRUE(Files::IsAbsolute("/folder/"));
#endif

        EXPECT_FALSE(Files::IsAbsolute("folder/"));
        EXPECT_FALSE(Files::IsAbsolute("../folder/"));
        EXPECT_FALSE(Files::IsAbsolute("./folder/"));
    }

    TEST_F(FilesTest, GetExtension) {
        // TODO
    }

    TEST_F(FilesTest, MkDir) {
        // TODO
    }

    TEST_F(FilesTest, FolderExists) {
        // TODO
    }

    TEST_F(FilesTest, IsDirectory) {
        // TODO
    }

    TEST_F(FilesTest, IsFile) {
        // TODO
    }
} // namespace soil::util
