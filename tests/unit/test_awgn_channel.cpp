#include <gtest/gtest.h>
#include "core/channel/AwgnChannel.hpp"
#include <cmath>
#include <numeric>

using namespace telecom;

class AwgnChannelTest : public ::testing::Test {
protected:
    // Seed fixe -> tests 100% reproductibles
    AwgnChannel channel_10db  { 10.0, 42 };
    AwgnChannel channel_100db { 100.0, 42 }; // SNR tres eleve ~ canal parfait
};

// ── Test 1 : entree vide -> sortie vide ──────────────────────────────────────
TEST_F(AwgnChannelTest, EmptyInputReturnsEmptyOutput) {
    auto out = channel_10db.apply({});
    EXPECT_TRUE(out.empty());
}

// ── Test 2 : taille entree = taille sortie ───────────────────────────────────
TEST_F(AwgnChannelTest, OutputSizeMatchesInputSize) {
    std::vector<std::complex<double>> signal = {{1,0},{-1,0},{1,0}};
    auto out = channel_10db.apply(signal);
    EXPECT_EQ(out.size(), signal.size());
}

// ── Test 3 : SNR tres eleve -> signal presque inchange ───────────────────────
TEST_F(AwgnChannelTest, VeryHighSnrSignalAlmostUnchanged) {
    std::vector<std::complex<double>> signal = {{1.0, 0.0}, {-1.0, 0.0}};
    auto out = channel_100db.apply(signal);

    // Avec SNR = 100dB, le bruit est negligeable
    EXPECT_NEAR(out[0].real(), 1.0, 0.01); // Passes if |out[0].real() - 1.0| <= 0.01 => |result - expected| <= 0.01
    EXPECT_NEAR(out[1].real(), -1.0, 0.01);
}

// ── Test 4 : seed fixe -> resultats reproductibles ───────────────────────────
TEST_F(AwgnChannelTest, SameSeedProducesSameOutput) {
    std::vector<std::complex<double>> signal = {{1.0, 0.0}, {-1.0, 0.0}};

    AwgnChannel ch1(10.0, 123);
    AwgnChannel ch2(10.0, 123);

    auto out1 = ch1.apply(signal);
    auto out2 = ch2.apply(signal);

    // Meme seed -> exactement le meme bruit
    EXPECT_DOUBLE_EQ(out1[0].real(), out2[0].real());
    EXPECT_DOUBLE_EQ(out1[1].real(), out2[1].real());
}

// ── Test 5 : seeds differents -> resultats differents ────────────────────────
TEST_F(AwgnChannelTest, DifferentSeedsProduceDifferentOutput) {
    std::vector<std::complex<double>> signal = {{1.0, 0.0}};

    AwgnChannel ch1(10.0, 1);
    AwgnChannel ch2(10.0, 2);

    auto out1 = ch1.apply(signal);
    auto out2 = ch2.apply(signal);

    // Seeds differents -> bruit different
    // EXPECT_NE(out1[0].real(), out2[0].real()); // checks whether two values are not equal
	EXPECT_GT(std::abs(out1[0].real() - out2[0].real()), 1e-12); // léger risque théorique (égalité par hasard)
}

// ── Test 6 : noisePower coherente avec SNR ───────────────────────────────────
TEST_F(AwgnChannelTest, NoisePowerCoherentWithSnr) {
    // SNR = 10 dB -> snr_linear = 10
    // sigma^2 = 1 / (2 * 10) = 0.05
    AwgnChannel ch(10.0, 42);
    EXPECT_NEAR(ch.noisePower(), 0.05, 1e-10);
}

// ── Test 7 : snrDb() retourne la valeur passee au constructeur ───────────────
TEST_F(AwgnChannelTest, SnrDbReturnsConstructorValue) {
    AwgnChannel ch(7.5, 42);
    EXPECT_DOUBLE_EQ(ch.snrDb(), 7.5);
}
