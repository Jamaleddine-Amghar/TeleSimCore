#pragma once

#include "core/modulation/IModulator.hpp"
#include "core/modulation/IDemodulator.hpp"
#include "core/channel/IChannel.hpp"
#include <vector>
#include <memory>
#include <random>
#include <stdexcept>

namespace telecom {

/**
 * Resultat d'une simulation
 */
struct SimulationResult {
    double ber;           // Bit Error Rate mesure
    int    total_bits;    // Nombre total de bits transmis
    int    error_bits;    // Nombre de bits errones
    double snr_db;        // SNR utilise
    std::string modulator_name;
    std::string channel_name;
};

/**
 * Pipeline de transmission numerique.
 * Assemble modulateur, canal, et demodulateur.
 *
 * Usage :
 *   TransmissionPipeline pipeline;
 *   pipeline.setModulator(std::make_unique<BpskModulator>());
 *   pipeline.setChannel(std::make_unique<AwgnChannel>(10.0));
 *   pipeline.setDemodulator(std::make_unique<BpskDemodulator>());
 *   auto result = pipeline.run(1000);
 */
class TransmissionPipeline {
public:
    TransmissionPipeline() = default;

    // Setters — injection des blocs via interfaces
    void setModulator  (std::unique_ptr<IModulator>   mod) { modulator_   = std::move(mod); }
    void setChannel    (std::unique_ptr<IChannel>     ch)  { channel_     = std::move(ch);  }
    void setDemodulator(std::unique_ptr<IDemodulator> dem) { demodulator_ = std::move(dem); }

    /**
     * Lance une simulation
     * @param num_bits  Nombre de bits a transmettre
     * @param seed      Seed pour la generation des bits aleatoires
     * @return          Resultats de la simulation
     */
    [[nodiscard]]
    SimulationResult run(int num_bits, unsigned int seed = 0) const;

    // Verificiation que le pipeline est complet
    [[nodiscard]]
    bool isReady() const noexcept {
        return modulator_ && channel_ && demodulator_;
    }

private:
    std::unique_ptr<IModulator>   modulator_;
    std::unique_ptr<IChannel>     channel_;
    std::unique_ptr<IDemodulator> demodulator_;

    // Genere des bits aleatoires
    [[nodiscard]]
    std::vector<int> generateBits(int n, unsigned int seed) const;
};

} // namespace telecom
