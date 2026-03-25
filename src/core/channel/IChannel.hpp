#pragma once

#include <complex>
#include <string>
#include <vector>

namespace telecom {

/**
 * Interface abstraite pour tout modele de canal.
 * Tout canal (AWGN, Rayleigh, parfait...) doit implementer cette interface.
 */
class IChannel {
public:
    virtual ~IChannel() = default;

    /**
     * Applique les effets du canal sur le signal emis
     */
    [[nodiscard]]
    virtual std::vector<std::complex<double>> apply(
        const std::vector<std::complex<double>>& signal) const = 0;

    /**
     * Nom du canal
     */
    [[nodiscard]]
    virtual std::string name() const = 0;
};

}  // namespace telecom
