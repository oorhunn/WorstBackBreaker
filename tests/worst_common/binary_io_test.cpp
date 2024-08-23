//
// Created by Anıl Orhun Demiroğlu on 23.08.2024.
//

#include <gtest/gtest.h>
#include "binary_io.hpp"
#include <cstdio>  // For std::remove

namespace worst_common {

    class BinaryIOTest : public ::testing::Test {
    protected:
        const std::string test_filename = "test_binary_io.dat";

        void SetUp() override {
            // This function will be run before each test case
            // Remove any existing test files to start with a clean state
            std::remove(test_filename.c_str());
        }

        void TearDown() override {
            // This function will be run after each test case
            // Clean up any test files created
            std::remove(test_filename.c_str());
        }
    };

    TEST_F(BinaryIOTest, TestBinaryWriterWrite) {
        BinaryWriter writer(test_filename);
        int value_to_write = 42;

        writer.write(value_to_write);

        // Verify the file has been created
        std::ifstream infile(test_filename, std::ios::binary);
        ASSERT_TRUE(infile.is_open()) << "File should be created and opened for reading.";
        infile.close();
    }

    TEST_F(BinaryIOTest, TestBinaryReaderRead) {
        // Write data to file
        {
            BinaryWriter writer(test_filename);
            double value_to_write = 3.14159;
            writer.write(value_to_write);
        }

        // Read the data back and verify it
        {
            BinaryReader reader(test_filename);
            ASSERT_TRUE(reader.is_valid()) << "Reader should be valid after opening an existing file.";

            double read_value = reader.read<double>();
            EXPECT_DOUBLE_EQ(read_value, 3.14159) << "Read value should match the written value.";
        }
    }

    TEST_F(BinaryIOTest, TestBinaryReaderReadInto) {
        // Write data to file
        {
            BinaryWriter writer(test_filename);
            int value_to_write = 100;
            writer.write(value_to_write);
        }

        // Read the data back into a variable
        {
            BinaryReader reader(test_filename);
            ASSERT_TRUE(reader.is_valid()) << "Reader should be valid after opening an existing file.";

            int read_value;
            reader.read_into(read_value);
            EXPECT_EQ(read_value, 100) << "Read value should match the written value.";
        }
    }

    TEST_F(BinaryIOTest, TestInvalidFile) {
        BinaryReader reader("non_existent_file.dat");
        EXPECT_FALSE(reader.is_valid()) << "Reader should be invalid when opening a non-existent file.";
    }

}  // namespace worst_common