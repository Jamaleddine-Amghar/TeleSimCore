#include "core/TransmissionPipeline.hpp"

namespace telecom {

std::vector<int> TransmissionPipeline::generateBits(int n, unsigned int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 1);
    std::vector<int> bits(n);
    // for (auto& b : bits) b = dist(rng);
    std::generate(bits.begin(), bits.end(), [&]() { return dist(rng); });  // suggestion de cppcheck
    return bits;
}

SimulationResult TransmissionPipeline::run(int num_bits, unsigned int seed) const {
    if (!isReady()) {
        throw std::runtime_error(
            "TransmissionPipeline::run() : pipeline incomplet. "
            "Appeler setModulator(), setChannel(), setDemodulator() avant run().");
    }

    // 1. Generer les bits aleatoires
    auto bits = generateBits(num_bits, seed);

    // 2. Modulation
    auto symbols = modulator_->modulate(bits);  // (*modulator_).modulate(bits); modulator_ est un pointeur

    // 3. Canal
    auto noisy = channel_->apply(symbols);

    // 4. Demodulation
    auto result = demodulator_->demodulate(noisy);

    if (!result.has_value()) {
        throw std::runtime_error("TransmissionPipeline::run() : demodulation echouee (symbole indecidable)");
    }

    // 5. Calcul BER
    int errors = 0;
    for (size_t i = 0; i < bits.size(); ++i) {
        if (bits[i] != (*result)[i]) ++errors;
    }

    const double ber = static_cast<double>(errors) / static_cast<double>(num_bits);

    SimulationResult res;  // c++17
    res.ber = ber;
    res.total_bits = num_bits;
    res.error_bits = errors;
    res.snr_db = 0.0;
    res.modulator_name = modulator_->name();
    res.channel_name = channel_->name();

    // return SimulationResult{ // C++20
    //     .ber            = ber,
    //     .total_bits     = num_bits,
    //     .error_bits     = errors,
    //     .snr_db         = 0.0,   // sera rempli par l'appelant si besoin
    //     .modulator_name = modulator_->name(),
    //     .channel_name   = channel_->name(),
    // };
    return res;
}

}  // namespace telecom
