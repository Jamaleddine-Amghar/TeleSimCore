#include "core/modulation/QpskDemodulator.hpp"

namespace telecom {

std::optional<std::vector<int>> QpskDemodulator::demodulate(const std::vector<std::complex<double>>& symbols) const {
    if (symbols.empty()) {
        return std::vector<int>{};  // Entree vide -> sortie vide (pas nullopt)
    }

    std::vector<int> bits;
    bits.reserve(2 * symbols.size());

    for (const auto& symbol : symbols) {
        const double real = symbol.real();
        const double imag = symbol.imag();

        int b1;
        int b2;

        if (real > 0.0) {
            b1 = 1;
        } else if (real < 0.0) {
            b1 = 0;
        } else {
            return std::nullopt;
        }

        if (imag > 0.0) {
            b2 = 1;
        } else if (imag < 0.0) {
            b2 = 0;
        } else {
            return std::nullopt;
        }
        bits.push_back(b1);
        bits.push_back(b2);
    }

    return bits;
}

}  // namespace telecom
