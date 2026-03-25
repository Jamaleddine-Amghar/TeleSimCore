#pragma once

#include <vector>
#include <complex>
#include <string>

namespace telecom {

/**
 * Interface abstraite pour tout modulateur numerique.
 * Tout modulateur (BPSK, QPSK, QAM...) doit implementer cette interface.
 */
class IModulator {
public:
    // Destructeur virtuel obligatoire pour les classes de base
    virtual ~IModulator() = default;

    /**
     * Module une sequence de bits en symboles complexes
     */
    [[nodiscard]]
    virtual std::vector<std::complex<double>>
    modulate(const std::vector<int>& bits) const = 0;

    /**
     * Nombre de bits par symbole
     */
    [[nodiscard]]
    virtual int bitsPerSymbol() const noexcept = 0;

    /**
     * Nom du modulateur
     */
    [[nodiscard]]
    virtual std::string name() const = 0;
};

} // namespace telecom
