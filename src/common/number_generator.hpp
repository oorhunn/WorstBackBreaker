//
// Created by Anıl Orhun Demiroğlu on 15.08.2024.
//

#ifndef NUMBER_GENERATOR_HPP
#define NUMBER_GENERATOR_HPP

#include <random>

namespace worst_common {

    class NumberGenerator {

    public:
        NumberGenerator() : m_gen{0} {}

        std::random_device m_rd{};
        std::mt19937 m_gen{};

        auto set_seed(std::uint32_t seed) -> void {

            m_gen.seed(seed);
        }
    };

    template<typename T>
    class RealNumberGenerator : public NumberGenerator {
    public:
        RealNumberGenerator() : NumberGenerator(), m_dist{0.0f, 1.0f} {}

        RealNumberGenerator(const RealNumberGenerator<T> &right) : NumberGenerator(), m_dist(right.m_dist) {}

        auto get() -> std::float_t {

            return m_dist(m_gen);
        }

        auto get_under(T max) -> std::float_t {

            return m_dist(m_gen) * max;
        }

        auto get_range(T min, T max) -> std::float_t {

            return min + get() * (max - min);
        }

        auto get_range(T width) -> std::float_t {

            return get_range(-width * 0.5f, width * 0.5f);
        }

    private:
        std::uniform_real_distribution<T> m_dist{};
    };

    template<typename T>
    class RNG {

    public:
        static auto get() -> T {

            return m_gen.get();
        }

        static auto get_under(T max) -> std::float_t {

            return m_gen.get_under(max);
        }

        static auto get_u_int_under(std::uint64_t max) -> std::uint64_t {

            return static_cast<uint64_t>(m_gen.get_under(static_cast<float>(max) + 1.0f));
        }

        static auto get_range(T min, T max) -> std::float_t {

            return m_gen.get_range(min, max);
        }

        static auto get_range(T width) -> std::float_t {

            return m_gen.get_range(width);
        }

        static auto get_full_range(T width) -> std::float_t {

            return m_gen.get_range(static_cast<T>(2.0f) * width);
        }

        static auto proba(std::float_t threshold) -> bool {

            return get() < threshold;
        }

        static void set_seed(std::uint32_t seed) {

            m_gen.set_seed(seed);
        }

    private:
        static RealNumberGenerator<T> m_gen;
    };

    using RNGf = RNG<std::float_t>;

    template<typename T>
    RealNumberGenerator<T> RNG<T>::m_gen = RealNumberGenerator<T>();

    template<typename T>
    class IntegerNumberGenerator : public NumberGenerator {
    public:
        IntegerNumberGenerator() : NumberGenerator() {}

        // random_device is not copyable
        IntegerNumberGenerator(const IntegerNumberGenerator<T> &right) : NumberGenerator() {}

        auto get_under(T max) -> T {

            std::uniform_int_distribution<std::mt19937::result_type> dist(0, max);
            return dist(m_gen);
        }

        auto get_range(T min, T max) -> T {

            std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
            return dist(m_gen);
        }
    };

    template<typename T>
    class RNGi {

    public:
        static auto get_under(T max) -> T {

            return m_gen.get_under(max);
        }

        static auto get_range(T min, T max) -> T {

            return m_gen.get_range(min, max);
        }

    private:
        static IntegerNumberGenerator<T> m_gen;
    };

    template<typename T>
    IntegerNumberGenerator<T> RNGi<T>::m_gen;

    using RNGi32 = RNGi<int32_t>;
    using RNGi64 = RNGi<int64_t>;
    using RNGu32 = RNGi<uint32_t>;
    using RNGu64 = RNGi<uint64_t>;
}  // namespace worst_common

#endif