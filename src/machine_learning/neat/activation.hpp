//
// Created by Anıl Orhun Demiroğlu on 21.07.2024.
//

#ifndef WORSTBACKBREAKER_ACTIVATION_HPP
#define WORSTBACKBREAKER_ACTIVATION_HPP

#include <cmath>

namespace ml::neat {


    enum class Activation : std::uint8_t {

        None,
        Sigmoid,
        Relu,
        Tanh
    };

    struct ActivationFunction {

        auto apply(std::double_t x, Activation activation) -> std::double_t {

            switch (activation) {

                case Activation::None:

                    return none(x);
                case Activation::Sigmoid:

                    return sigmoid(x);
                case Activation::Relu:

                    return relu(x);
                case Activation::Tanh:

                    return tanh(x);
                default:
                    return none(x);
            }
        }

        auto none(std::double_t x) -> std::double_t {

            return x;
        }

        auto sigmoid(std::double_t x) -> std::double_t {

            return 1 / (1 + std::exp(-x));
        }

        auto relu(std::double_t x) -> std::double_t {

            return x > 0 ? x : 0;
        }

        auto tanh(std::double_t x) -> std::double_t {

            return std::tanh(x);
        }
    };

}

#endif //WORSTBACKBREAKER_ACTIVATION_HPP
