//
// Created by Anıl Orhun Demiroğlu on 24.08.2024.
//

#include <gtest/gtest.h>
#include "number_generator.hpp"

namespace worst_common {

    class NumberGeneratorTest : public ::testing::Test {
    protected:
        NumberGenerator generator;

        void SetUp() override {
            // You can set up any necessary test environment here.
        }

        void TearDown() override {
            // Clean up any test environment here.
        }
    };

    TEST_F(NumberGeneratorTest, SeedSettingTest) {
        generator.set_seed(1234);
        ASSERT_NO_THROW(generator.m_gen.seed(1234));
    }

    TEST(RealNumberGeneratorTest, DefaultRangeTest) {
        RealNumberGenerator<float> realGen;
        auto number = realGen.get();
        ASSERT_GE(number, 0.0f);
        ASSERT_LE(number, 1.0f);
    }

    TEST(RealNumberGeneratorTest, GetUnderTest) {
        RealNumberGenerator<float> realGen;
        auto number = realGen.get_under(5.0f);
        ASSERT_GE(number, 0.0f);
        ASSERT_LT(number, 5.0f);
    }

    TEST(RealNumberGeneratorTest, GetRangeTest) {
        RealNumberGenerator<float> realGen;
        auto number = realGen.get_range(-1.0f, 1.0f);
        ASSERT_GE(number, -1.0f);
        ASSERT_LE(number, 1.0f);
    }

    TEST(RealNumberGeneratorTest, GetRangeWidthTest) {
        RealNumberGenerator<float> realGen;
        auto number = realGen.get_range(2.0f);
        ASSERT_GE(number, -1.0f);
        ASSERT_LE(number, 1.0f);
    }

    TEST(RNGTest, StaticRealGeneratorTest) {
        RNGf::set_seed(1234);
        auto number = RNGf::get();
        ASSERT_GE(number, 0.0f);
        ASSERT_LE(number, 1.0f);

        number = RNGf::get_under(10.0f);
        ASSERT_GE(number, 0.0f);
        ASSERT_LT(number, 10.0f);

        number = RNGf::get_range(-5.0f, 5.0f);
        ASSERT_GE(number, -5.0f);
        ASSERT_LE(number, 5.0f);

        number = RNGf::get_range(4.0f);
        ASSERT_GE(number, -2.0f);
        ASSERT_LE(number, 2.0f);
    }

    TEST(RNGTest, StaticProbaTest) {
        RNGf::set_seed(1234);
        ASSERT_TRUE(RNGf::proba(1.0f));  // Always true when threshold is 1.0
        ASSERT_FALSE(RNGf::proba(0.0f)); // Always false when threshold is 0.0
    }

    TEST(IntegerNumberGeneratorTest, GetUnderTest) {
        IntegerNumberGenerator<int> intGen;
        auto number = intGen.get_under(10);
        ASSERT_GE(number, 0);
        ASSERT_LT(number, 10);
    }

    TEST(IntegerNumberGeneratorTest, GetRangeTest) {
        IntegerNumberGenerator<int> intGen;
        auto number = intGen.get_range(1, 5);
        ASSERT_GE(number, 1);
        ASSERT_LE(number, 5);
    }

    TEST(RNGiTest, StaticIntegerGeneratorTest) {
        auto number = RNGi32::get_under(100);
        ASSERT_GE(number, 0);
        ASSERT_LT(number, 100);

        number = RNGi32::get_range(50, 100);
        ASSERT_GE(number, 50);
        ASSERT_LE(number, 100);
    }

}  // namespace worst_common
