//
// Created by Anıl Orhun Demiroğlu on 6.08.2024.
//

#ifndef WORSTBACKBREAKER_BINARY_IO_HPP
#define WORSTBACKBREAKER_BINARY_IO_HPP

#include <fstream>
#include <string>

namespace worst_common {

    struct BinaryWriter {

        const std::string m_filename;
        std::ofstream m_outfile;

        BinaryWriter() = delete;

        explicit BinaryWriter(const std::string &filename_) : m_filename{filename_},
                                                              m_outfile{m_filename,
                                                                        std::istream::out | std::ios::binary} {}

        ~BinaryWriter() {

            if (m_outfile) {

                m_outfile.close();
            }
        }

        template<typename TValue>
        auto write(const TValue &value) -> void {

            m_outfile.write(reinterpret_cast<const char *>(&value), sizeof(value));
        }
    };

    struct BinaryReader {

        const std::string m_filename;
        std::ifstream m_infile;

        BinaryReader() = delete;

        explicit BinaryReader(const std::string &filename_)
                : m_filename{filename_}, m_infile{m_filename, std::ios_base::binary} {}

        ~BinaryReader() {

            if (m_infile) {

                m_infile.close();
            }
        }

        [[nodiscard]]
        auto is_valid() const -> bool {

            return m_infile.operator bool();
        }

        template<typename TValue>
        auto read() -> TValue {

            TValue result = {};
            m_infile.read(reinterpret_cast<char *>(&result), sizeof(TValue));
            return result;
        }

        template<typename TValue>
        auto read_into(TValue &value) -> void {

            m_infile.read(reinterpret_cast<char *>(&value), sizeof(TValue));
        }
    };
}
#endif //WORSTBACKBREAKER_BINARY_IO_HPP
