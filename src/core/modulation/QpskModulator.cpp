#include "core/modulation/QpskModulator.hpp"

#include <cmath>

namespace telecom {
using Symbol = std::complex<double>;
std::vector<Symbol> QpskModulator::modulate(const std::vector<int>& bits) const {
    // Si l'entree est vide, retourner un vecteur vide
    if (bits.empty()) {
        return {};
    }

    std::vector<Symbol> symbols;
    symbols.reserve((bits.size() + 1) / 2);  // Pre-allouer la memoire

    for (size_t i = 0; i < bits.size(); i += 2) {
        auto b1 = bits[i];
        auto b2 = (i + 1 < bits.size()) ? bits[i + 1] : 0;

        if ((b1 != 0 && b1 != 1) || (b2 != 0 && b2 != 1)) {
            throw std::invalid_argument("QpskModulator::modulate() : bits invalide '" +
                                        std::to_string(b1) + " or " + std::to_string(b2) +
                                        "' — valeurs acceptees : 0 ou 1");
        }
        // 00      →   (-1, -1)
        // 01      →   (-1, +1)
        // 10      →   (+1, -1)
        // 11      →   (+1, +1)

        const double real = (b1 == 1) ? +1.0 : -1.0;
        const double imag = (b2 == 1) ? +1.0 : -1.0;
        symbols.emplace_back(real, imag);
    }
    return symbols;
}

}  // namespace telecom
