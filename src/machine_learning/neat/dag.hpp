//
// Created by Anıl Orhun Demiroğlu on 21.07.2024.
//

#ifndef WORSTBACKBREAKER_DAG_HPP
#define WORSTBACKBREAKER_DAG_HPP

#include <cmath>
#include <vector>
#include <cassert>


namespace ml::neat {

    struct DAG {

        struct Node {

            std::uint32_t incoming;
            std::uint32_t depth;
            std::vector<std::uint32_t> out;

            auto get_out_connection_count() -> const std::uint32_t {

                return out.size();
            };
        };

        std::vector<Node> nodes;

        auto create_node() -> void {

            nodes.emplace_back();
        }

        auto is_valid(std::uint32_t i) -> const bool {

            return i < nodes.size();
        }


        auto is_ancestor(uint32_t node_1, uint32_t node_2) const -> bool {

            auto &out = nodes[node_1].out;
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


        auto is_parent(uint32_t node_1, uint32_t node_2) const -> bool {

            auto &out = nodes[node_1].out;
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
            nodes[from].out.push_back(to);
            nodes[to].incoming++;
            return true;
        }

        auto compute_depth() -> void {

            auto const node_count = nodes.size();
            std::vector<std::uint32_t> start_nodes;
            std::vector<std::uint32_t> incoming;
            incoming.reserve(node_count);

            for (auto const &n: nodes) {

                incoming.push_back(n.incoming);
            }
            std::uint32_t i{0};
            for (auto &n: nodes) {

                if (n.incoming == 0) {

                    n.depth = 0;
                    start_nodes.push_back(i);
                }
                i++;
            }

            while (!start_nodes.empty()) {

                std::uint32_t const idx = start_nodes.back();
                start_nodes.pop_back();

                Node const &n = nodes[idx];
                for (auto const o: n.out) {

                    incoming[o]--;
                    Node &connected = nodes[o];
                    connected.depth = std::max(connected.depth, n.depth + 1);
                    if (incoming[o] == 0) {

                        start_nodes.push_back(o);
                    }
                }
            }
        }

        auto get_order() const -> std::vector<std::uint32_t> {

            std::vector<std::uint32_t> order(nodes.size());

            for(std::uint32_t i = 0; i < nodes.size(); i++) {

                order[i] = i;
            }
            std::sort(order.begin(), order.end(), [this](std::uint32_t a, std::uint32_t b) {

                return nodes[a].depth < nodes[b].depth;
            });
            return order;
        }
    };

}


#endif //WORSTBACKBREAKER_DAG_HPP
