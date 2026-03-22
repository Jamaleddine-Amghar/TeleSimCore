#pragma once

#include <vector>
#include <complex>
#include <string>
#include <optional>

namespace telecom {

/**
 * Demodulateur BPSK (Binary Phase Shift Keying)
 *
 * Decision dure sur la partie reelle du symbole recu :
 *   real > 0  ->  bit 1
 *   real < 0  ->  bit 0
 *   real = 0  ->  indecidable (retourne std::nullopt)
 */
class BpskDemodulator {
public:
    BpskDemodulator() = default;

    /**
     * Demodule un vecteur de symboles complexes en bits
     * @param symbols  Symboles recus (potentiellement bruites)
     * @return         std::nullopt si un symbole est indecidable (real == 0)
     *                 sinon le vecteur de bits decides
     */
    [[nodiscard]]
    std::optional<std::vector<int>>
    demodulate(const std::vector<std::complex<double>>& symbols) const;

    /**
     * Nombre de bits par symbole (toujours 1 pour BPSK)
     */
    [[nodiscard]]
    int bitsPerSymbol() const noexcept { return 1; }

    /**
     * Nom du demodulateur
     */
    [[nodiscard]]
    std::string name() const { return "BPSK"; }
};

} // namespace telecom
