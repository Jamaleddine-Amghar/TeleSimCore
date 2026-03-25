#pragma once

#include <vector>
#include <complex>
#include <optional>
#include <string>

namespace telecom {

/**
 * Interface abstraite pour tout demodulateur numerique.
 */
class IDemodulator {
public:
    virtual ~IDemodulator() = default;

    /**
     * Demodule des symboles complexes en bits
     */
    [[nodiscard]]
    virtual std::optional<std::vector<int>>
    demodulate(const std::vector<std::complex<double>>& symbols) const = 0;

    [[nodiscard]]
    virtual int bitsPerSymbol() const noexcept = 0;

    [[nodiscard]]
    virtual std::string name() const = 0;
};

} // namespace telecom
