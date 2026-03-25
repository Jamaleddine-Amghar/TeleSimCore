#include <gtest/gtest.h>

#include "core/modulation/QpskModulator.hpp"

using namespace telecom;

// Fixture : cree un QpskModulator reutilisable dans tous les tests
class QpskModulatorTest : public ::testing::Test {
protected:
    QpskModulator modulator;
};

// 00      →   (-1, -1)
// 01      →   (-1, +1)
// 10      →   (+1, -1)
// 11      →   (+1, +1)

// ── Test 1 : les bits 0 0 donne le symbole (-1, -1) ──────────────────────────────────
TEST_F(QpskModulatorTest, ZeroZeroMapsToMinusOneMinusOneSymbol) {
    auto symbols = modulator.modulate({0, 0});

    ASSERT_EQ(symbols.size(), 1u);  // Un bit -> un symbole
    EXPECT_DOUBLE_EQ(symbols[0].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(), -1.0);
}

// ── Test 2 : les bits 0 1 donne le symbole (-1, +1) ──────────────────────────────────
TEST_F(QpskModulatorTest, ZeroOneMapsToMinusOnePlusOneSymbol) {
    auto symbols = modulator.modulate({0, 1});

    ASSERT_EQ(symbols.size(), 1u);  // Un bit -> un symbole
    EXPECT_DOUBLE_EQ(symbols[0].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(), +1.0);
}

// ── Test 3 : les bits 1 0 donne le symbole (+1, -1) ──────────────────────────────────
TEST_F(QpskModulatorTest, OneZeroMapsToPlusOneMinusOneSymbol) {
    auto symbols = modulator.modulate({1, 0});

    ASSERT_EQ(symbols.size(), 1u);  // Un bit -> un symbole
    EXPECT_DOUBLE_EQ(symbols[0].real(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(), -1.0);
}

// ── Test 4 : les bits 1 1 donne le symbole (+1, +1) ──────────────────────────────────
TEST_F(QpskModulatorTest, OneOneMapsToPlusOnePlusOneSymbol) {
    auto symbols = modulator.modulate({1, 1});

    ASSERT_EQ(symbols.size(), 1u);  // Un bit -> un symbole
    EXPECT_DOUBLE_EQ(symbols[0].real(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(), +1.0);
}

// ── Test 5 : entree vide -> sortie vide ──────────────────────────────────────
TEST_F(QpskModulatorTest, EmptyInputReturnsEmptyOutput) {
    auto symbols = modulator.modulate({});
    EXPECT_TRUE(symbols.empty());
}

// ── Test 6 : N (pair) bits -> N/2 symboles ────────────────────────────────────────────
TEST_F(QpskModulatorTest, OutputSizeMatchesHalfInputSize_Even_N) {
    std::vector<int> bits = {0, 1, 0, 1, 1, 0};
    auto symbols = modulator.modulate(bits);
    EXPECT_EQ(symbols.size(), (bits.size() + 1) / 2);
}

// ── Test 7 : N (impair) bits -> (N+1)/2 symboles ────────────────────────────────────────────
TEST_F(QpskModulatorTest, OutputSizeMatchesHalfInputSize_Odd_N) {
    std::vector<int> bits = {0, 1, 0, 1, 1, 0, 1};
    auto symbols = modulator.modulate(bits);
    EXPECT_EQ(symbols.size(), (bits.size() + 1) / 2);
}

// ── Test 8 : sequence connue -> symboles corrects ────────────────────────────
TEST_F(QpskModulatorTest, KnownSequenceProducesCorrectSymbols_Even_N) {
    // 0  1   1  1   1  0
    // ( ) ( ) ( )
    auto symbols = modulator.modulate({0, 1, 1, 1, 1, 0});

    ASSERT_EQ(symbols.size(), 3u);

    EXPECT_DOUBLE_EQ(symbols[0].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[1].real(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[1].imag(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[2].real(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[2].imag(), -1.0);
}

// ── Test 9 : sequence connue -> symboles corrects ────────────────────────────
TEST_F(QpskModulatorTest, KnownSequenceProducesCorrectSymbols_Odd_N) {
    // 0  1   1  1   0   => 0
    // ( ) ( ) ( )
    auto symbols = modulator.modulate({0, 1, 1, 1, 0});

    ASSERT_EQ(symbols.size(), 3u);

    EXPECT_DOUBLE_EQ(symbols[0].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[1].real(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[1].imag(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[2].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[2].imag(), -1.0);
}

// ── Test 10 : bitsPerSymbol vaut toujours 2 ───────────────────────────────────
TEST_F(QpskModulatorTest, BitsPerSymbolIsTwo) { EXPECT_EQ(modulator.bitsPerSymbol(), 2); }

// ── Test 11 : nom du modulateur ───────────────────────────────────────────────
TEST_F(QpskModulatorTest, NameIsQPSK) { EXPECT_EQ(modulator.name(), "QPSK"); }

// ── Test 12 : invalide argument ───────────────────────────────────────────────
TEST_F(QpskModulatorTest, InvalidArgument) {
    EXPECT_THROW(
        (void)modulator.modulate({1, 0, 2}),  // (void) = on ignore volontairement le retour
        std::invalid_argument);
}