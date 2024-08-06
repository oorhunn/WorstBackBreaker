//
// Created by Anıl Orhun Demiroğlu on 3.08.2024.
//

#ifndef WORSTBACKBREAKER_NETWORK_HPP
#define WORSTBACKBREAKER_NETWORK_HPP

#include <cmath>
#include <vector>

#include "activation.hpp"

namespace ml::neat {


    struct Network {

        struct Info {

            Info() = default;

            Info(std::uint32_t inputs, std::uint32_t outputs) :
                    m_inputs{inputs}, m_outputs{outputs}, m_hidden{0} {}


            auto get_node_count() const -> std::uint32_t {

                return m_inputs + m_outputs + m_hidden;
            }

            std::uint32_t m_inputs{0};
            std::uint32_t m_outputs{0};
            std::uint32_t m_hidden{0};
        };

        struct Node {

            auto get_value() const -> std::double_t {

                return m_activation(m_sum + m_bias);
            }

            ActivationPtr m_activation = ActivationFunction::none;
            std::double_t m_sum{0.0};
            std::double_t m_bias{0.0};
            std::uint32_t m_connection_count{0};
            std::uint32_t m_depth{0};
        };

        struct Connection {

            std::uint32_t m_to{0};
            std::double_t m_weight{0.0};
            std::double_t m_value{0.0};
        };

        union Slot {

            Node m_node{};
            Connection m_connection;

            Slot() : m_node{} {}
        };

        std::vector<Slot> m_slots;
        std::vector<std::double_t> m_output;
        Info m_info;
        std::uint32_t m_max_depth{0};
        std::uint32_t m_connection_count{0};

        Network() = default;

        auto initialize(Info const &info, std::uint32_t connection_count) -> void {

            m_info = info;
            m_connection_count = connection_count;

            m_slots.resize(info.get_node_count() + m_connection_count);
            m_output.resize(m_info.m_outputs);
        }

        auto
        set_node(std::uint32_t i, Activation activation, std::double_t bias, std::uint32_t connection_count) -> void {

            auto node = get_node(i);
            node.m_activation = ActivationFunction::get_function(activation);
            node.m_bias = bias;
            node.m_connection_count = connection_count;
        }

        auto get_node(uint32_t i) -> Node & {

            return m_slots[i].m_node;
        }

        auto get_connection(std::uint32_t i) -> Connection & {

            return m_slots[m_info.get_node_count() + i].m_connection;
        }

        auto get_connection(std::uint32_t i) const -> const Connection & {

            return m_slots[m_info.get_node_count() + i].m_connection;
        }

        auto set_node_depth(std::uint32_t i, std::uint32_t depth) -> void {

            m_slots[i].m_node.m_depth = depth;
        }

        auto set_connection(std::uint32_t i, std::uint32_t to, std::double_t weight) -> void {

            Connection &connection = get_connection(i);
            connection.m_to = to;
            connection.m_weight = weight;
        }

        auto get_output(std::uint32_t i) -> Node & {

            return m_slots[m_info.m_inputs + m_info.m_hidden + i].m_node;
        }

        auto execute(std::vector<std::double_t> const &input) -> bool {

            if (input.size() != m_info.m_inputs) {

                return false;
            }
            foreach_node([](Node &n, std::uint32_t) {

                n.m_sum = 0.0;
            });

            for (std::uint32_t i{0}; i < m_info.m_inputs; ++i) {

                m_slots[i].m_node.m_sum = input[i];
            }

            std::uint32_t current_connection{0};
            std::uint32_t const node_count = m_info.get_node_count();
            // execute network
            for (std::uint32_t i{0}; i < node_count; i++) {

                Node const &node = m_slots[i].m_node;

                std::double_t const value = node.get_value();

                for (std::uint32_t o{0}; o < node.m_connection_count; o++) {

                    Connection &c = get_connection(current_connection++);
                    c.m_value = value * c.m_weight;
                    get_node(c.m_to).m_sum += c.m_value;
                }
            }

            // update output
            for (std::uint32_t i{0}; i < m_info.m_outputs; i++) {

                m_output[i] = get_output(i).get_value();
            }
            return true;
        }

        template<typename TCallback>
        auto foreach_node(TCallback &&callback) -> void {

            std::uint32_t const node_count = m_info.get_node_count();
            for (uint32_t i{0}; i < node_count; ++i) {

                callback(m_slots[i].m_node, i);
            }
        }

        template<typename TCallback>
        auto foreach_node(TCallback &&callback) const -> void {

            std::uint32_t const node_count = m_info.get_node_count();
            for (std::uint32_t i{0}; i < node_count; ++i) {

                callback(m_slots[i].m_node, i);
            }
        }

        template<typename TCallback>
        auto foreach_connection(TCallback &&callback) const -> void {

            for (std::uint32_t i{0}; i < m_connection_count; ++i) {

                callback(get_connection(i), i);
            }
        }

        auto get_result() const -> std::vector<std::double_t> const & {

            return m_output;
        }

        auto get_depth() const -> std::uint32_t {

            return m_max_depth;
        }
    };
}
#endif //WORSTBACKBREAKER_NETWORK_HPP
