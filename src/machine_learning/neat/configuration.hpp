//
//  Copyright (c) 2024 STM Savunma Teknolojileri, Mühendislik ve Ticaret A.Ş. All rights reserved.
//
//  author : aorhun.demiroglu@stm.com.tr
//
#ifndef NEAT_CONFIGURATION_HPP
#define NEAT_CONFIGURATION_HPP

#include <cmath>
#include <cstdint>

namespace ml::neat {

    static constexpr std::float_t sc_new_node_proba{0.05f};
    static constexpr std::float_t sc_new_conn_proba{0.8f};

    static constexpr std::float_t sc_new_value_proba{0.2f};
    static constexpr std::double_t sc_weight_range{1.0};
    static constexpr std::double_t sc_weight_small_range{0.01};

    static constexpr std::uint32_t sc_mut_count{4};
    static constexpr std::uint32_t sc_max_hidden_nodes{30};
}  // namespace ml::neat
#endif  // NEAT_CONFIGURATION_HPP