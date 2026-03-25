#include <gtest/gtest.h>

#include "core/modulation/BpskDemodulator.hpp"

using namespace telecom;

class BpskDemodulatorTest : public ::testing::Test {
protected:
    BpskDemodulator demodulator;
};

// ── Test 1 : symbole positif -> bit 1 ────────────────────────────────────────
TEST_F(BpskDemodulatorTest, PositiveSymbolGivesBitOne) {
    auto result = demodulator.demodulate({{+1.0, 0.0}});

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 1u);
    EXPECT_EQ((*result)[0], 1);
}

// ── Test 2 : symbole negatif -> bit 0 ────────────────────────────────────────
TEST_F(BpskDemodulatorTest, NegativeSymbolGivesBitZero) {
    auto result = demodulator.demodulate({{-1.0, 0.0}});

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 1u);
    EXPECT_EQ((*result)[0], 0);
}

// ── Test 3 : entree vide -> sortie vide (pas nullopt) ────────────────────────
TEST_F(BpskDemodulatorTest, EmptyInputReturnsEmptyVector) {
    auto result = demodulator.demodulate({});

    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->empty());
}

// ── Test 4 : symbole exactement zero -> nullopt ───────────────────────────────
TEST_F(BpskDemodulatorTest, ZeroRealReturnsNullopt) {
    auto result = demodulator.demodulate({{0.0, 0.0}});
    EXPECT_FALSE(result.has_value());
}

// ── Test 5 : sequence connue -> bits corrects ─────────────────────────────────
TEST_F(BpskDemodulatorTest, KnownSequenceProducesCorrectBits) {
    // +1 -1 +1 -1  ->  1 0 1 0
    std::vector<std::complex<double>> symbols = {
        {+1.0, 0.0}, {-1.0, 0.0}, {+1.0, 0.0}, {-1.0, 0.0}};
    auto result = demodulator.demodulate(symbols);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 4u);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 0);
    EXPECT_EQ((*result)[2], 1);
    EXPECT_EQ((*result)[3], 0);
}

// ── Test 6 : symboles bruites mais decidables ─────────────────────────────────
TEST_F(BpskDemodulatorTest, NoisyButDecidableSymbols) {
    // Symboles legerement bruites mais toujours du bon cote
    std::vector<std::complex<double>> symbols = {{+0.7, 0.3}, {-0.8, -0.1}, {+0.5, 0.9}};
    auto result = demodulator.demodulate(symbols);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ((*result)[0], 1);  // +0.7 > 0 -> 1
    EXPECT_EQ((*result)[1], 0);  // -0.8 < 0 -> 0
    EXPECT_EQ((*result)[2], 1);  // +0.5 > 0 -> 1
}

// ── Test 7 : bitsPerSymbol vaut 1 ────────────────────────────────────────────
TEST_F(BpskDemodulatorTest, BitsPerSymbolIsOne) { EXPECT_EQ(demodulator.bitsPerSymbol(), 1); }

// ── Test 8 : nom du demodulateur ─────────────────────────────────────────────
TEST_F(BpskDemodulatorTest, NameIsBPSK) { EXPECT_EQ(demodulator.name(), "BPSK"); }
