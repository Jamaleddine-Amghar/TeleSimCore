#pragma once

// #include <vector>
// #include <complex>
#include <cmath>
#include <random>

#include "core/channel/IChannel.hpp"

namespace telecom {

/**
 * Canal AWGN (Additive White Gaussian Noise)
 *
 * Ajoute un bruit gaussien centre au signal transmis.
 * Le niveau de bruit est controle par le rapport signal/bruit (SNR) en dB.
 *
 * Modele : y = x + n
 *   x = signal emis
 *   n = bruit gaussien de variance sigma^2
 *   y = signal recu
 */
class AwgnChannel : public IChannel {
public:
    /**
     * @param snr_db  Rapport signal/bruit en dB
     * @param seed    Seed du generateur aleatoire (reproductibilite des tests)
     */
    explicit AwgnChannel(double snr_db, unsigned int seed = 42);
    // explicit empêche les conversions implicites.
    // AwgnChannel channel = 10.0;  // ❌ interdit  AwgnChannel channel(10.0);   // ✅ OK
    // seed reproduire exactement le même bruit

    /**
     * Applique le bruit AWGN sur le signal entrant
     * @param signal  Signal emis (vecteur de symboles complexes)
     * @return        Signal bruite
     */
    [[nodiscard]]
    std::vector<std::complex<double>> apply(
        const std::vector<std::complex<double>>& signal) const override;

    [[nodiscard]]
    std::string name() const override {
        return "AWGN";
    }

    /**
     * Retourne la puissance du bruit (sigma^2)
     */
    [[maybe_unused]] [[nodiscard]]
    double noisePower() const noexcept {
        return sigma_ * sigma_;
    }
    // noexcept veut dire : “Cette fonction ne lance jamais d’exception
    // Pourquoi ? simple calcul (sigma_ * sigma_) aucun risque

    /**
     * Retourne le SNR en dB
     */
    [[maybe_unused]] [[nodiscard]]
    double snrDb() const noexcept {
        return snr_db_;
    }

private:
    double snr_db_;
    double sigma_;  // Ecart-type du bruit

    mutable std::mt19937 rng_;  // Generateur Mersenne Twister
    mutable std::normal_distribution<double>
        dist_;  // Distribution gaussienne
                // mutable => Modifying Data Members in const Methods
};

}  // namespace telecom
