#include "core/modulation/BpskModulator.hpp"

namespace telecom {

	std::vector<BpskModulator::Symbol>
	BpskModulator::modulate(const std::vector<int>& bits) const
	{
		// Si l'entree est vide, retourner un vecteur vide
		if (bits.empty()) {
			return {};
		}

		std::vector<Symbol> symbols;
		symbols.reserve(bits.size()); // Pre-allouer la memoire

		for (const int bit : bits) {
			
			// Valider que le bit est bien 0 ou 1
			if (bit != 0 && bit != 1) {
				throw std::invalid_argument(
					"BpskModulator::modulate() : bit invalide '" +
					std::to_string(bit) +
					"' — valeurs acceptees : 0 ou 1"
				);
			}
			// bit 0 -> -1.0 + 0.0j
			// bit 1 -> +1.0 + 0.0j
			
			const double real = (bit == 1) ? +1.0 : -1.0;
			symbols.emplace_back(real, 0.0);
		}

		return symbols;
	}

} // namespace telecom
