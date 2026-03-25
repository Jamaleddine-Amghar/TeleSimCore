#include <gtest/gtest.h>

#include "core/TransmissionPipeline.hpp"
#include "core/channel/AwgnChannel.hpp"
#include "core/modulation/BpskDemodulator.hpp"
#include "core/modulation/BpskModulator.hpp"
#include "mocks/PerfectChannel.hpp"

using namespace telecom;

class TransmissionPipelineTest : public ::testing::Test {
protected:
    // Construit un pipeline complet avec PerfectChannel
    TransmissionPipeline buildPerfectPipeline() {
        TransmissionPipeline pipeline;
        pipeline.setModulator(std::make_unique<BpskModulator>());
        pipeline.setChannel(std::make_unique<PerfectChannel>());
        pipeline.setDemodulator(std::make_unique<BpskDemodulator>());
        return pipeline;
    }
};

// ── Test 1 : pipeline vide -> isReady() retourne false ───────────────────────
TEST_F(TransmissionPipelineTest, EmptyPipelineIsNotReady) {
    TransmissionPipeline pipeline;
    EXPECT_FALSE(pipeline.isReady());
}

// ── Test 2 : pipeline incomplet -> isReady() retourne false ──────────────────
TEST_F(TransmissionPipelineTest, IncompletePipelineIsNotReady) {
    TransmissionPipeline pipeline;
    pipeline.setModulator(std::make_unique<BpskModulator>());
    // Pas de canal ni demodulateur
    EXPECT_FALSE(pipeline.isReady());
}

// ── Test 3 : pipeline complet -> isReady() retourne true ─────────────────────
TEST_F(TransmissionPipelineTest, CompletePipelineIsReady) {
    auto pipeline = buildPerfectPipeline();
    EXPECT_TRUE(pipeline.isReady());
}

// ── Test 4 : pipeline incomplet -> run() lance une exception ─────────────────
TEST_F(TransmissionPipelineTest, RunOnIncompletePipelineThrows) {
    TransmissionPipeline pipeline;
    pipeline.setModulator(std::make_unique<BpskModulator>());
    EXPECT_THROW((void)pipeline.run(100), std::runtime_error);
}

// ── Test 5 : canal parfait -> BER = 0 ────────────────────────────────────────
TEST_F(TransmissionPipelineTest, PerfectChannelYieldsZeroBer) {
    auto pipeline = buildPerfectPipeline();
    auto result = pipeline.run(1000, 42);

    EXPECT_DOUBLE_EQ(result.ber, 0.0);
    EXPECT_EQ(result.error_bits, 0);
    EXPECT_EQ(result.total_bits, 1000);
}

// ── Test 6 : nombre de bits correct dans le resultat ─────────────────────────
TEST_F(TransmissionPipelineTest, ResultContainsCorrectBitCount) {
    auto pipeline = buildPerfectPipeline();
    auto result = pipeline.run(5000, 42);
    EXPECT_EQ(result.total_bits, 5000);
}

// ── Test 7 : noms des blocs dans le resultat ─────────────────────────────────
TEST_F(TransmissionPipelineTest, ResultContainsBlockNames) {
    auto pipeline = buildPerfectPipeline();
    auto result = pipeline.run(100, 42);

    EXPECT_EQ(result.modulator_name, "BPSK");
    EXPECT_EQ(result.channel_name, "Perfect");
}

// ── Test 8 : meme seed -> meme resultat ──────────────────────────────────────
TEST_F(TransmissionPipelineTest, SameSeedProducesSameResult) {
    auto p1 = buildPerfectPipeline();
    auto p2 = buildPerfectPipeline();

    auto r1 = p1.run(1000, 42);
    auto r2 = p2.run(1000, 42);

    EXPECT_DOUBLE_EQ(r1.ber, r2.ber);
    EXPECT_EQ(r1.error_bits, r2.error_bits);
}
