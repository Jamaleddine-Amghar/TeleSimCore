#include <gtest/gtest.h>

#include "core/modulation/QpskDemodulator.hpp"

using namespace telecom;

class QpskDemodulatorTest : public ::testing::Test {
protected:
    QpskDemodulator demodulator;
};

// ── Test 1 : symbole (+1,+1) -> bits 1 1 ────────────────────────────────────────
TEST_F(QpskDemodulatorTest, PlusOnePlusOneSymbolGivesBitsOneOne) {
    auto result = demodulator.demodulate({{+1.0, +1.0}});

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2u);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 1);
}

// ── Test 2 : symbole (+1,-1) -> bits 1 0 ────────────────────────────────────────
TEST_F(QpskDemodulatorTest, PlusOneMinusOneSymbolGivesBitsOneZero) {
    auto result = demodulator.demodulate({{+1.0, -1.0}});

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2u);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 0);
}

// ── Test 3 : symbole (-1,+1) -> bits 0 1 ────────────────────────────────────────
TEST_F(QpskDemodulatorTest, MinusOnePlusOneSymbolGivesBitsZeroOne) {
    auto result = demodulator.demodulate({{-1.0, +1.0}});

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2u);
    EXPECT_EQ((*result)[0], 0);
    EXPECT_EQ((*result)[1], 1);
}

// ── Test 4 : symbole (-1,-1) -> bits 0 0 ────────────────────────────────────────
TEST_F(QpskDemodulatorTest, PlusOnePlusOneSymbolGivesBitsZeroZero) {
    auto result = demodulator.demodulate({{-1.0, -1.0}});

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 2u);
    EXPECT_EQ((*result)[0], 0);
    EXPECT_EQ((*result)[1], 0);
}

// ── Test 5 : entree vide -> sortie vide (pas nullopt) ────────────────────────
TEST_F(QpskDemodulatorTest, EmptyInputReturnsEmptyVector) {
    auto result = demodulator.demodulate({});

    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(result->empty());
}

// ── Test 6 : partie reel du symbole exactement zero -> nullopt ───────────────────────────────
TEST_F(QpskDemodulatorTest, ZeroRealReturnsNullopt) {
    auto result = demodulator.demodulate({{0.0, 0.6}});
    EXPECT_FALSE(result.has_value());
}

// ── Test 7 : partie imaginaire du symbole exactement zero -> nullopt
// ───────────────────────────────
TEST_F(QpskDemodulatorTest, ZeroImagReturnsNullopt) {
    auto result = demodulator.demodulate({{0.4, 0.0}});
    EXPECT_FALSE(result.has_value());
}

// ── Test 8 : sequence connue -> bits corrects ─────────────────────────────────
TEST_F(QpskDemodulatorTest, KnownSequenceProducesCorrectBits) {
    // (+1,-1) (-1,-1) (+1,+1) (-1,+1)  ->  1 0 0 0 1 1 0 1
    std::vector<std::complex<double>> symbols = {{+1.0, -1.0}, {-1.0, -1.0}, {+1.0, +1.0}, {-1.0, +1.0}};
    auto result = demodulator.demodulate(symbols);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 8u);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 0);
    EXPECT_EQ((*result)[2], 0);
    EXPECT_EQ((*result)[3], 0);
    EXPECT_EQ((*result)[4], 1);
    EXPECT_EQ((*result)[5], 1);
    EXPECT_EQ((*result)[6], 0);
    EXPECT_EQ((*result)[7], 1);
}

// ── Test 9 : symboles bruites mais decidables ─────────────────────────────────
TEST_F(QpskDemodulatorTest, NoisyButDecidableSymbols) {
    // Symboles legerement bruites mais toujours du bon cote
    std::vector<std::complex<double>> symbols = {{+0.7, +0.3}, {-0.8, -0.1}, {-0.5, +0.9}};
    auto result = demodulator.demodulate(symbols);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), 6u);
    EXPECT_EQ((*result)[0], 1);
    EXPECT_EQ((*result)[1], 1);
    EXPECT_EQ((*result)[2], 0);
    EXPECT_EQ((*result)[3], 0);
    EXPECT_EQ((*result)[4], 0);
    EXPECT_EQ((*result)[5], 1);
}

// ── Test 10 : bitsPerSymbol vaut 2 ────────────────────────────────────────────
TEST_F(QpskDemodulatorTest, BitsPerSymbolIsTwo) { EXPECT_EQ(demodulator.bitsPerSymbol(), 2); }

// ── Test 8 : nom du demodulateur ─────────────────────────────────────────────
TEST_F(QpskDemodulatorTest, NameIsQPSK) { EXPECT_EQ(demodulator.name(), "QPSK-DEMOD"); }
