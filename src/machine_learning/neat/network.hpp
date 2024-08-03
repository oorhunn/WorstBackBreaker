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

        auto set_node_depth(std::uint32_t i, std::uint32_t depth) -> void {

            m_slots[i].m_node.m_depth = depth;
        }

        auto set_connection(std::uint32_t i, std::uint32_t to, std::double_t weight) -> void {

            Connection &connection
        }
    };

}
#endif //WORSTBACKBREAKER_NETWORK_HPP
