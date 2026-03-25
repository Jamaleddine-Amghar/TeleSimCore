#include "core/TransmissionPipeline.hpp"
#include "core/modulation/BpskModulator.hpp"
#include "core/modulation/BpskDemodulator.hpp"
#include "core/modulation/QpskModulator.hpp"
#include "core/modulation/QpskDemodulator.hpp"
#include "core/channel/AwgnChannel.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace telecom;

// Calcul BER theorique BPSK : 0.5 * erfc(sqrt(Eb/N0))
double theoreticalBer(double snr_db) {
    const double snr_linear = std::pow(10.0, snr_db / 10.0);
	return 0.5 * std::erfc(std::sqrt(snr_linear));
}

double theoreticalSer(double snr_db, const std::string& mod) {
	double theo_ber = theoreticalBer(snr_db);

	if (mod == "BPSK"){ 
		return theo_ber; 
	} else if (mod == "QPSK"){
	return (2 * theo_ber) - std::pow(theo_ber,2);
	} else {
		throw std::invalid_argument("Modulation inconnue : " + mod);
	}
}

int main() {
    std::cout << "\n";
    std::cout << "================================================\n";
    std::cout << "    TeleSimCore : Simulation BER BPSK + AWGN    \n";
    std::cout << "================================================\n\n";

    // Plage de SNR a simuler
    const std::vector<double> snr_range = {
        -50.0, -20.0, -15.0, -2.0, 0.0, 2.0, 4.0, 6.0, 8.0, 10.0, 15.0
    };
    const int num_bits = 500000;

    // En-tete du tableau
	constexpr int W1 = 12;
	constexpr int W2 = 18;
    std::cout << std::left
              << std::setw(W1) << "BNR (dB)"
              << std::setw(W2) << "BER theorique"
              << std::setw(W2) << "SER theorique"
              << std::setw(W1) << "Bits errones"
              << "\n";
    std::cout << std::string(W1 + 2*W2 + W1, '-') << "\n";

    for (const double snr_db : snr_range) {
        // Construire le pipeline pour ce SNR
        TransmissionPipeline pipeline;
        pipeline.setModulator  (std::make_unique<BpskModulator>());
        pipeline.setChannel    (std::make_unique<AwgnChannel>(snr_db, 42));
        pipeline.setDemodulator(std::make_unique<BpskDemodulator>());

        // Lancer la simulation
        auto result = pipeline.run(num_bits, 99);
        result.snr_db = snr_db;

        // Calcul BER theorique
        // const double ber_theory = theoreticalBer(snr_db);

        // Affichage ligne du tableau
        std::cout << std::left 
				  << std::setw(W1)
				  << std::fixed << std::setprecision(1) << snr_db
                  << std::setw(W2)
                  // << std::scientific 
				  << std::setprecision(8) << theoreticalBer(snr_db)
                  << std::setw(W2) 
				  << theoreticalSer(snr_db, "BPSK")
                  << std::setw(W1) 
				  << result.error_bits
                  << "\n";
    }

    std::cout << std::string(W1 + 2*W2 + W1, '-') << "\n";
    std::cout << "\nModulateur : BPSK  |  Canal : AWGN";
    std::cout << "  |  Bits/simulation : " << num_bits << "\n\n";
	
	
	
	
	std::cout << "\n";
    std::cout << "================================================\n";
    std::cout << "    TeleSimCore : Simulation BER QPSK + AWGN    \n";
    std::cout << "================================================\n\n";

    std::cout << std::left
              << std::setw(W1) << "SNR (dB)"
              << std::setw(W2) << "BER theorique"
              << std::setw(W2) << "SER theorique"
              << std::setw(W1) << "Bits errones"
              << "\n";
    std::cout << std::string(W1 + 2*W2 + W1, '-') << "\n";

    for (const double snr_db : snr_range) {
        // Construire le pipeline pour ce SNR
        TransmissionPipeline pipeline;
        pipeline.setModulator  (std::make_unique<QpskModulator>());
        pipeline.setChannel    (std::make_unique<AwgnChannel>(snr_db, 42));
        pipeline.setDemodulator(std::make_unique<QpskDemodulator>());

        // Lancer la simulation
        auto result = pipeline.run(num_bits, 99);
        result.snr_db = snr_db;

        // Calcul BER theorique
        //const double ber_theory = theoreticalBer(snr_db);

        // Affichage ligne du tableau
        std::cout << std::left 
				  << std::setw(W1)
				  << std::fixed << std::setprecision(1) << snr_db
                  << std::setw(W2)
                  // << std::scientific 
				  << std::setprecision(8) << theoreticalBer(snr_db)
                  << std::setw(W2) 
				  << theoreticalSer(snr_db, "QPSK")
                  << std::setw(W1) 
				  << result.error_bits
                  << "\n";
    }

    std::cout << std::string(W1 + 2*W2 + W1, '-') << "\n";
    std::cout << "\nModulateur : QPSK  |  Canal : AWGN";
    std::cout << "  |  Bits/simulation : " << num_bits << ". => le debit double." << "\n\n"; 

    return 0;
}
