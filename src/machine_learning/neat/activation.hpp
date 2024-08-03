//
// Created by Anıl Orhun Demiroğlu on 21.07.2024.
//

#ifndef WORSTBACKBREAKER_ACTIVATION_HPP
#define WORSTBACKBREAKER_ACTIVATION_HPP

#include <cmath>


namespace ml::neat {

    using ActivationPtr = std::double_t (*)(std::double_t);

    enum class Activation : std::uint8_t {

        None,
        Sigmoid,
        Relu,
        Tanh
    };

    struct ActivationFunction {

        auto static  get_function(Activation activation) -> ActivationPtr {

            switch (activation) {

                case Activation::None:

                    return none;
                case Activation::Sigmoid:

                    return sigmoid;
                case Activation::Relu:

                    return relu;
                case Activation::Tanh:

                    return tanh;

                default:

                    return none;
            }
        }

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

        auto static none(std::double_t x) -> std::double_t {

            return x;
        }

        auto static sigmoid(std::double_t x) -> std::double_t {

            return 1 / (1 + std::exp(-x));
        }

        auto static relu(std::double_t x) -> std::double_t {

            return x > 0 ? x : 0;
        }

        auto static tanh(std::double_t x) -> std::double_t {

            return std::tanh(x);
        }
    };

}

#endif //WORSTBACKBREAKER_ACTIVATION_HPP
