//
// Created by Anıl Orhun Demiroğlu on 24.08.2024.
//

#ifndef TESTS_NETWORK_GENERATOR_HPP
#define TESTS_NETWORK_GENERATOR_HPP

#include <algorithm>
#include <vector>
#include <cmath>


#include "network.hpp"
#include "genome.hpp"

namespace ml::neat {

    struct NetworkGenerator {

        std::vector<std::uint32_t> m_idx_to_order{};

        NetworkGenerator() = default;

        auto generate(Genome &genome) -> Network {

            m_idx_to_order.resize(genome.m_info.get_node_count());
            Network network{};
            network.initialize(genome.m_info, static_cast<std::uint32_t>(genome.m_connections.size()));

            auto const order = get_order(genome);

            for (std::uint32_t i{0}; i < order.size(); ++i) {

                m_idx_to_order[order[i]] = i;
            }
            std::uint32_t node_idx{0};
            std::uint32_t connection_idx{0};

            for (std::uint32_t o: order) {

                auto const &node = genome.m_nodes[o];
                network.set_node(node_idx, node.m_activation, node.m_bias,
                                 genome.m_graph.m_nodes[o].get_out_connection_count());
                network.set_node_depth(node_idx, node.m_depth);

                for (auto const &c: genome.m_connections) {

                    if (c.m_from == o) {

                        std::uint32_t const target = m_idx_to_order[c.m_to];
                        assert(target > node_idx);
                        network.set_connection(connection_idx, target, c.m_weight);
                        ++connection_idx;
                    }
                }
                ++node_idx;
            }
            network.m_max_depth = network.get_node(node_idx - 1).m_depth;
            return network;
        }

        auto get_order(Genome &genome) const -> std::vector<std::uint32_t> {

            genome.compute_depth();
            std::vector<std::uint32_t> order(genome.m_nodes.size());
            for (std::uint32_t i{0}; i < genome.m_nodes.size(); i++) {

                order[i] = i;
            }
            return order;
        }
    };

}
#endif //TESTS_NETWORK_GENERATOR_HPP
