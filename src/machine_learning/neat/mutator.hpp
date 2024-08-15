//
// Created by Anıl Orhun Demiroğlu on 21.07.2024.
//

#ifndef WORSTBACKBREAKER_MUTATOR_HPP
#define WORSTBACKBREAKER_MUTATOR_HPP

#include "configuration.hpp"
#include "genome.hpp"
#include "number_generator.hpp"

namespace ml::neat {

    struct Mutator {

        static auto mutate_genome(Genome &genome) -> void {

            for (std::uint32_t i{0}; i < sc_mut_count; i++) {

                if (worst_common::RNGf::proba(0.25f)) {

                    if (worst_common::RNGf::proba(0.5f)) {

                        mutate_biases(genome);
                    }
                    else {

                        mutate_weights(genome);
                    }
                }
            }
            if (worst_common::RNGf::proba(sc_new_conn_proba) && sc_max_hidden_nodes > genome.m_info.m_hidden) {

                new_node(genome);
            }

            if (worst_common::RNGf::proba(sc_new_conn_proba)) {

                new_connection(genome);
            }
        }

        static auto mutate_biases(Genome &genome) -> void {
            Genome::Node &n = pick_random(genome.m_nodes);
            if (worst_common::RNGf::proba(sc_new_value_proba)) {

                n.m_bias = worst_common::RNGf::get_full_range(sc_weight_range);
            }
            else {

                if (worst_common::RNGf::proba(0.25f)) {

                    n.m_bias += worst_common::RNGf::get_full_range(sc_weight_range);
                }
                else {

                    n.m_bias += sc_weight_small_range * worst_common::RNGf::get_full_range(sc_weight_range);
                }
            }
        }

        static auto mutate_weights(Genome &genome) -> void {
            // Nothing to do if no connections
            if (genome.m_connections.empty()) {

                return;
            }

            Genome::Connection &c = pick_random(genome.m_connections);
            if (worst_common::RNGf::proba(sc_new_value_proba)) {

                c.m_weight = worst_common::RNGf::get_full_range(sc_weight_range);
            }
            else {

                if (worst_common::RNGf::proba(0.75f)) {

                    c.m_weight += sc_weight_small_range * worst_common::RNGf::get_full_range(sc_weight_range);
                }
                else {

                    c.m_weight += worst_common::RNGf::get_full_range(sc_weight_range);
                }
            }
        }

        static auto new_node(Genome &genome) -> void {
            // Nothing to do if no connections
            if (genome.m_connections.empty()) {

                return;
            }

            std::uint32_t const connection_idx = get_rand_index(genome.m_connections.size());
            genome.split_connection(connection_idx);
        }

        static auto new_connection(Genome &genome) -> void {
            // Pick first random node, input + hidden
            std::uint32_t const count_1 = genome.m_info.m_inputs + genome.m_info.m_hidden;
            std::uint32_t idx_1 = get_rand_index(count_1);
            // If the picked node is an output, offset it by the number of outputs to land on hidden
            if (idx_1 >= genome.m_info.m_inputs && idx_1 < (genome.m_info.m_inputs + genome.m_info.m_outputs)) {

                idx_1 += genome.m_info.m_outputs;
            }
            // Pick second random node, hidden + output
            std::uint32_t const count_2 = genome.m_info.m_hidden + genome.m_info.m_outputs;
            // Skip inputs
            std::uint32_t idx_2 = get_rand_index(count_2) + genome.m_info.m_inputs;

            assert(!genome.is_output(idx_1));
            assert(!genome.is_input(idx_2));

            // Create the new connection
            if (!genome.try_create_connection(idx_1, idx_2, worst_common::RNGf::get_full_range(sc_weight_range))) {

                std::cout << "Cannot create connection " << idx_1 << " -> " << idx_2 << std::endl;
            }
        }

        static auto get_rand_index(std::uint64_t max_value) -> std::uint32_t {

            auto const max_value_f = static_cast<float>(max_value);
            return static_cast<uint32_t>(worst_common::RNGf::get_under(max_value_f));
        }

        template<typename TDataType>
        static auto pick_random(std::vector<TDataType> &container) -> TDataType & {

            std::uint32_t const idx = get_rand_index(container.size());
            return container[idx];
        }
    };

}  // namespace ml::neat

#endif  //WORSTBACKBREAKER_MUTATOR_HPP
