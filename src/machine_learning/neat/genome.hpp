//
// Created by Anıl Orhun Demiroğlu on 21.07.2024.
//

#ifndef WORSTBACKBREAKER_GENOME_HPP
#define WORSTBACKBREAKER_GENOME_HPP

#include <algorithm>
#include <iostream>
#include <ranges>

#include "binary_io.hpp"
#include "dag.hpp"
#include "network.hpp"

namespace ml::neat {

    struct Genome {

        Genome() = default;

        explicit Genome(std::uint32_t inputs, std::uint32_t outputs) : m_info{inputs, outputs} {

            for (std::uint32_t i{m_info.m_inputs}; i--;) {

                create_node(Activation::None, false);
            }
            for (std::uint32_t i{m_info.m_outputs}; i--;) {

                create_node(Activation::Tanh, false);
            }
        }

        struct Node {

            std::double_t m_bias{0.0f};
            Activation m_activation{Activation::Sigmoid};
            std::uint32_t m_depth{0};
        };

        struct Connection {

            std::uint32_t m_from{0};
            std::uint32_t m_to{0};
            std::double_t m_weight{0.0f};
        };

        Network::Info m_info{};
        std::vector<Node> m_nodes{};
        std::vector<Connection> m_connections{};
        data_structures::graphs::DAG m_graph{};

        auto create_node(Activation activation, bool hidden = true) -> std::uint32_t {
            m_nodes.emplace_back();
            m_nodes.back().m_activation = activation;
            m_nodes.back().m_bias = 0.0f;

            m_graph.create_node();
            // Update info if needed
            if (hidden) {
                ++m_info.m_hidden;
            }
            // Return index of new node
            return static_cast<uint32_t>(m_nodes.size() - 1);
        }

        auto try_create_connection(uint32_t from, uint32_t to, std::double_t weight) -> bool {

            if (m_graph.create_connection(from, to)) {

                m_connections.push_back({from, to, weight});
                return true;
            }
            return false;
        }

        auto create_connection(std::uint32_t from, std::uint32_t to, std::double_t weight) -> void {

            m_graph.create_connection(from, to);
            m_connections.push_back({from, to, weight});
        }

        auto split_connection(std::uint32_t i) -> void {

            if (i >= m_connections.size()) {

                std::cout << "Invalid connection " << i << std::endl;
                return;
            }

            Connection const& c = m_connections[i];
            std::uint32_t const from = c.m_from;
            std::uint32_t const to = c.m_to;
            std::double_t const weight = c.m_weight;
            remove_connection(i);

            uint32_t const node_idx{create_node(Activation::Relu)};
            create_connection(from, node_idx, weight);
            create_connection(node_idx, to, 1.0f);
        }

        auto remove_connection(std::uint32_t i) -> void {

            m_graph.remove_connection(m_connections[i].m_from, m_connections[i].m_to);
            std::swap(m_connections[i], m_connections.back());
            m_connections.pop_back();
        }

        [[nodiscard]] auto get_order() const -> std::vector<std::uint32_t> {

            std::vector<std::uint32_t> order(m_nodes.size());
            for (std::uint32_t i{0}; i < m_nodes.size(); ++i) {

                order[i] = i;
            }

            std::ranges::sort(order.begin(), order.end(), [this](std::uint32_t a, std::uint32_t b) {
                return m_nodes[a].m_depth < m_nodes[b].m_depth;
            });

            return order;
        }

        auto compute_depth() -> void {

            auto const node_count = static_cast<uint32_t>(m_nodes.size());

            // Compute order
            std::uint32_t max_depth = 0;
            m_graph.compute_depth();

            for (std::uint32_t i{0}; i < node_count; ++i) {

                m_nodes[i].m_depth = m_graph.m_nodes[i].depth;
                max_depth = std::max(m_nodes[i].m_depth, max_depth);
            }

            // Set outputs to the last "layer"
            std::uint32_t const output_depth = std::max(max_depth, 1u);
            for (std::uint32_t i{0}; i < m_info.m_outputs; ++i) {

                m_nodes[m_info.m_inputs + i].m_depth = output_depth;
            }
        }

        [[nodiscard]] auto is_input(std::uint32_t i) const -> bool {

            return i < m_info.m_inputs;
        }

        [[nodiscard]] auto is_output(std::uint32_t i) const -> bool {

            return (i >= m_info.m_inputs) && (i < m_info.m_inputs + m_info.m_outputs);
        }

        auto write_to_file(std::string const& filename) const -> void {

            worst_common::BinaryWriter writer(filename);
            writer.write(m_info);

            for (auto const& n : m_nodes) {

                writer.write(n);
            }
            writer.write(m_connections.size());
            for (auto const& c : m_connections) {

                writer.write(c);
            }
        }

        auto load_from_file(std::string const& filename) -> void {
            // Create the reader
            worst_common::BinaryReader reader(filename);
            if (!reader.is_valid()) {

                std::cout << "Cannot open file \"" << filename << "\"" << std::endl;
                return;
            }

            // Clear graph
            m_graph.m_nodes.clear();

            // Load info
            reader.read_into(m_info);
            m_nodes.resize(m_info.get_node_count());

            // Load nodes
            for (auto& n : m_nodes) {

                reader.read_into(n);
                m_graph.create_node();
            }

            // Load connections
            auto const connection_count = reader.read<std::size_t>();
            for (std::size_t i{0}; i < connection_count; ++i) {

                auto const c = reader.read<Connection>();
                create_connection(c.m_from, c.m_to, c.m_weight);
            }
            std::cout << "\"" << filename << "\" loaded." << std::endl;
        }
    };

}  // namespace ml::neat

#endif  //WORSTBACKBREAKER_GENOME_HPP
