#include <gtest/gtest.h>
#include "core/modulation/BpskModulator.hpp"

using namespace telecom;

// Fixture : cree un BpskModulator reutilisable dans tous les tests
class BpskModulatorTest : public ::testing::Test {
protected:
    BpskModulator modulator;
};

// ── Test 1 : bit 0 donne le symbole (-1, 0) ──────────────────────────────────
TEST_F(BpskModulatorTest, ZeroBitMapsToMinusOne) {
    auto symbols = modulator.modulate({0});

    ASSERT_EQ(symbols.size(), 1u);          // Un bit -> un symbole
    EXPECT_DOUBLE_EQ(symbols[0].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(),  0.0);
}

// ── Test 2 : bit 1 donne le symbole (+1, 0) ──────────────────────────────────
TEST_F(BpskModulatorTest, OneBitMapsToPositiveOne) {
    auto symbols = modulator.modulate({1});

    ASSERT_EQ(symbols.size(), 1u);
    EXPECT_DOUBLE_EQ(symbols[0].real(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[0].imag(),  0.0);
}

// ── Test 3 : entree vide -> sortie vide ──────────────────────────────────────
TEST_F(BpskModulatorTest, EmptyInputReturnsEmptyOutput) {
    auto symbols = modulator.modulate({});
    EXPECT_TRUE(symbols.empty());
}

// ── Test 4 : N bits -> N symboles ────────────────────────────────────────────
TEST_F(BpskModulatorTest, OutputSizeMatchesInputSize) {
    std::vector<int> bits = {0, 1, 0, 1, 1, 0};
    auto symbols = modulator.modulate(bits);
    EXPECT_EQ(symbols.size(), bits.size());
}

// ── Test 5 : sequence connue -> symboles corrects ────────────────────────────
TEST_F(BpskModulatorTest, KnownSequenceProducesCorrectSymbols) {
    // 0  1  0  1
    // -1 +1 -1 +1
    auto symbols = modulator.modulate({0, 1, 0, 1});

    ASSERT_EQ(symbols.size(), 4u);
    EXPECT_DOUBLE_EQ(symbols[0].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[1].real(), +1.0);
    EXPECT_DOUBLE_EQ(symbols[2].real(), -1.0);
    EXPECT_DOUBLE_EQ(symbols[3].real(), +1.0);
}

// ── Test 6 : bitsPerSymbol vaut toujours 1 ───────────────────────────────────
TEST_F(BpskModulatorTest, BitsPerSymbolIsOne) {
    EXPECT_EQ(modulator.bitsPerSymbol(), 1);
}

// ── Test 7 : nom du modulateur ───────────────────────────────────────────────
TEST_F(BpskModulatorTest, NameIsBPSK) {
    EXPECT_EQ(modulator.name(), "BPSK");
}

// ── Test 8 : invalide argument ───────────────────────────────────────────────
TEST_F(BpskModulatorTest, InvalidArgument) {
    EXPECT_THROW(
	(void) modulator.modulate({2}), // (void) = on ignore volontairement le retour
	std::invalid_argument);
}