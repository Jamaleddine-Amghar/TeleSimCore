#pragma once

#include "core/modulation/IModulator.hpp"
// #include <vector>
// #include <complex>
// #include <string>
#include <stdexcept>

namespace telecom {

/**
 * Modulateur BPSK (Binary Phase Shift Keying)
 *
 * Mapping :
 *   bit 0  ->  (-1.0, 0.0)
 *   bit 1  ->  (+1.0, 0.0)
 */
class BpskModulator : public IModulator {
public:
    // Type du symbole de sortie : nombre complexe double precision
    // using Symbol = std::complex<double>;

    BpskModulator() = default;

    /**
     * Module une sequence de bits en symboles BPSK
     * @param bits  Vecteur de bits (valeurs 0 ou 1)
     * @return      Vecteur de symboles complexes
     * @throws std::invalid_argument si un bit n'est pas 0 ou 1
     */
    [[nodiscard]]  // tell the compiler that the return value of a function should not be ignored.
    std::vector<std::complex<double>> modulate(const std::vector<int>& bits) const override;

    /**
     * Nombre de bits par symbole (toujours 1 pour BPSK)
     */
    [[nodiscard]]
    int bitsPerSymbol() const noexcept override {
        return 1;
    }

    /**
     * Nom du modulateur
     */
    [[nodiscard]]
    std::string name() const override {
        return "BPSK";
    }
};

}  // namespace telecom
