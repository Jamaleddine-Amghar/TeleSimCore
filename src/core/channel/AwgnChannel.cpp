#include "core/channel/AwgnChannel.hpp"

namespace telecom {

AwgnChannel::AwgnChannel(double snr_db, unsigned int seed)
    : snr_db_(snr_db),
      rng_(seed),
      dist_(0.0, 1.0)  // Gaussienne centree, ecart-type = 1
{
    // Conversion SNR dB -> lineaire
    // snr_linear = 10^(snr_db / 10)
    const double snr_linear = std::pow(10.0, snr_db / 10.0);

    // Calcul de sigma pour bande de base complexe
    // Puissance signal = 1.0 (symboles BPSK normalises)
    // sigma = sqrt(1 / (2 * snr_linear))
    // Le facteur 2 vient des deux composantes I et Q independantes
    sigma_ = std::sqrt(1.0 / (2.0 * snr_linear));
}

std::vector<std::complex<double>> AwgnChannel::apply(
    const std::vector<std::complex<double>>& signal) const {
    if (signal.empty()) {
        return {};
    }

    std::vector<std::complex<double>> noisy;
    noisy.reserve(signal.size());

    for (const auto& symbol : signal) {
        // Generer bruit independant sur I et Q
        const double noise_i = sigma_ * dist_(rng_);
        const double noise_q = sigma_ * dist_(rng_);

        noisy.emplace_back(symbol.real() + noise_i, symbol.imag() + noise_q);
    }

    return noisy;
}

}  // namespace telecom
