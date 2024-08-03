//
// Created by Anıl Orhun Demiroğlu on 21.07.2024.
//

#ifndef WORSTBACKBREAKER_DAG_HPP
#define WORSTBACKBREAKER_DAG_HPP

#include <cmath>
#include <vector>
#include <cassert>


namespace data_structures::graphs {

    struct DAG {

        struct Node {

            std::uint32_t incoming;
            std::uint32_t depth;
            std::vector<std::uint32_t> out;

            auto get_out_connection_count() const -> const std::uint32_t {

                return out.size();
            };
        };

        std::vector<Node> m_nodes;

        auto create_node() -> void {

            m_nodes.emplace_back();
        }

        auto is_valid(std::uint32_t i) const -> const bool {

            return i < m_nodes.size();
        }


        auto is_ancestor(uint32_t node_1, uint32_t node_2) const -> bool {

            auto &out = m_nodes[node_1].out;
            if (is_parent(node_1, node_2)) {

                return true;
            }
            for (auto const &o: out) {

                if (is_ancestor(o, node_2)) {

                    return true;
                }
            }
            return false;
        }


        [[nodiscard]] auto is_parent(uint32_t node_1, uint32_t node_2) const -> bool {

            auto &out = m_nodes[node_1].out;
            for (auto const &o: out) {

                if (o == node_2) {

                    return true;
                }
            }
            return false;
        }

        auto create_connection(std::uint32_t from, std::uint32_t to) -> bool {

            if (!is_valid(from)) {

                return false;
            }
            if (!is_valid(to)) {

                return false;
            }

            if (from == to) {

                return false;
            }

            if (is_ancestor(to, from)) {

                return false;
            }
            if (is_parent(from, to)) {

                return false;
            }
            m_nodes[from].out.push_back(to);
            m_nodes[to].incoming++;
            return true;
        }

        auto compute_depth() -> void {

            auto const node_count = m_nodes.size();
            std::vector<std::uint32_t> start_nodes;
            std::vector<std::uint32_t> incoming;
            incoming.reserve(node_count);

            for (auto const &n: m_nodes) {

                incoming.push_back(n.incoming);
            }
            std::uint32_t i{0};
            for (auto &n: m_nodes) {

                if (n.incoming == 0) {

                    n.depth = 0;
                    start_nodes.push_back(i);
                }
                i++;
            }

            while (!start_nodes.empty()) {

                std::uint32_t const idx = start_nodes.back();
                start_nodes.pop_back();

                Node const &n = m_nodes[idx];
                for (auto const o: n.out) {

                    incoming[o]--;
                    Node &connected = m_nodes[o];
                    connected.depth = std::max(connected.depth, n.depth + 1);
                    if (incoming[o] == 0) {

                        start_nodes.push_back(o);
                    }
                }
            }
        }

        auto get_order() const -> std::vector<std::uint32_t> {

            std::vector<std::uint32_t> order(m_nodes.size());

            for (std::uint32_t i = 0; i < m_nodes.size(); i++) {

                order[i] = i;
            }
            std::sort(order.begin(), order.end(), [this](std::uint32_t a, std::uint32_t b) {

                return m_nodes[a].depth < m_nodes[b].depth;
            });
            return order;
        }

        auto remove_connection(std::uint32_t from, std::uint32_t to) -> void {
            auto &connections = m_nodes[from].out;
            auto const count = static_cast<std::uint32_t>(connections.size());
            std::uint32_t found{0};

            for (std::uint32_t i{0}; i < count - found;) {

                if (connections[i] == to) {

                    std::swap(connections[i], connections.back());
                    connections.pop_back();
                    --m_nodes[to].incoming;
                    ++found;
                }
                else {

                    ++i;
                }
            }
        }
    };

}


#endif //WORSTBACKBREAKER_DAG_HPP
