#pragma once

#include "core/channel/IChannel.hpp"

namespace telecom {

/**
 * Mock de canal : ne fait rien, retourne le signal intact.
 * Utilise uniquement dans les tests pour isoler le pipeline du bruit.
 * Avec ce canal, BER doit etre exactement 0.
 */
class PerfectChannel : public IChannel {
public:
    [[nodiscard]]
    std::vector<std::complex<double>> apply(const std::vector<std::complex<double>>& signal) const override {
        return signal;  // Signal inchange
    }

    [[nodiscard]]
    std::string name() const override {
        return "Perfect";
    }
};

}  // namespace telecom
