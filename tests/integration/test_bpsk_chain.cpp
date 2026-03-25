#include <gtest/gtest.h>

#include <cmath>
#include <numeric>
#include <random>

#include "core/channel/AwgnChannel.hpp"
#include "core/modulation/BpskDemodulator.hpp"
#include "core/modulation/BpskModulator.hpp"

using namespace telecom;

// ─── Fonction utilitaire : genere N bits aleatoires ──────────────────────────
std::vector<int> generateRandomBits(int n, unsigned int seed = 0) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<int> bits(n);
    for (auto& b : bits) b = dist(rng);
    return bits;
}

// ─── Fonction utilitaire : calcule le BER ────────────────────────────────────
double computeBer(const std::vector<int>& sent, const std::vector<int>& received) {
    int errors = 0;
    for (size_t i = 0; i < sent.size(); ++i) {
        if (sent[i] != received[i]) ++errors;
    }
    return static_cast<double>(errors) / static_cast<double>(sent.size());
}

// ─── Fixture ─────────────────────────────────────────────────────────────────
class BpskChainTest : public ::testing::Test {
protected:
    BpskModulator modulator;
    BpskDemodulator demodulator;
};

// ── Test 1 : canal parfait -> BER = 0 ────────────────────────────────────────
TEST_F(BpskChainTest, PerfectChannelYieldsZeroBer) {
    // SNR = 100 dB ~ canal parfait
    AwgnChannel channel(100.0, 42);

    auto bits = generateRandomBits(1000, 1);
    auto symbols = modulator.modulate(bits);
    auto noisy = channel.apply(symbols);
    auto result = demodulator.demodulate(noisy);

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->size(), bits.size());

    double ber = computeBer(bits, *result);
    EXPECT_DOUBLE_EQ(ber, 0.0);
}

// ── Test 2 : BER diminue quand SNR augmente ───────────────────────────────────
TEST_F(BpskChainTest, BerDecreasesAsSnrIncreases) {
    const int num_bits = 50000;
    auto bits = generateRandomBits(num_bits, 42);

    double ber_low_snr = 0.0;   // SNR =  0 dB
    double ber_high_snr = 0.0;  // SNR = 10 dB

    // Simulation SNR = 0 dB
    {
        AwgnChannel channel(0.0, 42);
        auto symbols = modulator.modulate(bits);
        auto noisy = channel.apply(symbols);
        auto result = demodulator.demodulate(noisy);
        ASSERT_TRUE(result.has_value());
        ber_low_snr = computeBer(bits, *result);
    }

    // Simulation SNR = 10 dB
    {
        AwgnChannel channel(10.0, 42);
        auto symbols = modulator.modulate(bits);
        auto noisy = channel.apply(symbols);
        auto result = demodulator.demodulate(noisy);
        ASSERT_TRUE(result.has_value());
        ber_high_snr = computeBer(bits, *result);
    }

    // BER doit etre strictement plus faible a SNR plus eleve
    EXPECT_GT(ber_low_snr, ber_high_snr);
}

// ── Test 3 : BER a SNR=10dB coherent avec la theorie BPSK ────────────────────
TEST_F(BpskChainTest, BerAt10dBCoherentWithTheory) {
    // BER theorique BPSK a 10 dB ~ 3.87e-6 => BER = Q(√(2 * Eb/N0))
    // Avec 100 000 bits on ne peut pas mesurer ca precisement
    // On verifie juste que BER < 0.001 (seuil raisonnable)
    const int num_bits = 100000;
    AwgnChannel channel(10.0, 42);

    auto bits = generateRandomBits(num_bits, 99);
    auto symbols = modulator.modulate(bits);
    auto noisy = channel.apply(symbols);
    auto result = demodulator.demodulate(noisy);

    ASSERT_TRUE(result.has_value());
    double ber = computeBer(bits, *result);

    EXPECT_LT(ber, 0.001);
}

// ── Test 4 : BER a SNR=0dB autour de 0.08 ────────────────────────────────────
TEST_F(BpskChainTest, BerAt0dBAroundExpectedValue) {
    // BER theorique BPSK a 0 dB ~ 0.0786 => BER = Q(√(2 * Eb/N0))
    // On verifie que BER est dans [0.05, 0.15] — fourchette large
    // pour tenir compte de la variance statistique
    const int num_bits = 100000;
    AwgnChannel channel(0.0, 42);

    auto bits = generateRandomBits(num_bits, 77);
    auto symbols = modulator.modulate(bits);
    auto noisy = channel.apply(symbols);
    auto result = demodulator.demodulate(noisy);

    ASSERT_TRUE(result.has_value());
    double ber = computeBer(bits, *result);

    EXPECT_GT(ber, 0.05);
    EXPECT_LT(ber, 0.15);
}

// ── Test 5 : 8 bits envoyes = 8 bits recus sur canal parfait ─────────────────────
TEST_F(BpskChainTest, Sent8BitsEqualReceivedBitsOnPerfectChannel) {
    AwgnChannel channel(100.0, 42);

    std::vector<int> bits = {0, 1, 1, 0, 1, 0, 0, 1};
    auto symbols = modulator.modulate(bits);
    auto noisy = channel.apply(symbols);
    auto result = demodulator.demodulate(noisy);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, bits);
}

// ── Test 6 : 100000 bits envoyes = 100000 bits recus sur canal parfait ─────────────────────
TEST_F(BpskChainTest, Sent100000BitsEqualReceivedBitsOnPerfectChannel) {
    AwgnChannel channel(100.0, 42);

    auto bits = generateRandomBits(100000, 123);
    auto symbols = modulator.modulate(bits);
    auto noisy = channel.apply(symbols);
    auto result = demodulator.demodulate(noisy);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, bits);
}