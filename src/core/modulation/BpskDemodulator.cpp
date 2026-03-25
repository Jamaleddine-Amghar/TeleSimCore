#include "core/modulation/BpskDemodulator.hpp"

namespace telecom {

std::optional<std::vector<int>> BpskDemodulator::demodulate(
    const std::vector<std::complex<double>>& symbols) const {
    if (symbols.empty()) {
        return std::vector<int>{};  // Entree vide -> sortie vide (pas nullopt)
    }

    std::vector<int> bits;
    bits.reserve(symbols.size());

    for (const auto& symbol : symbols) {
        const double real = symbol.real();

        if (real > 0.0) {
            bits.push_back(1);
        } else if (real < 0.0) {
            bits.push_back(0);
        } else {
            // real == 0.0 : exactement sur la frontiere de decision
            // Cas theoriquement improbable mais a gerer proprement
            return std::nullopt;
        }
    }

    return bits;
}

}  // namespace telecom
