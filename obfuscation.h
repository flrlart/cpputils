#pragma once
#include <array>
#include <cstdint>

namespace Obfuscation {
    template <typename CharT, size_t N>
    struct Encrypt {
        uint32_t key;
        std::array<CharT, N> data;

        constexpr Encrypt(const CharT(&str)[N], const uint32_t k) : key(k), data{} 
        {
            for (size_t i = 0; i < N; ++i) {
                data[i] = str[i] ^ static_cast<CharT>((key + i * 37) & static_cast<uint32_t>(~CharT(0)));
            }
        }

        struct PlainView {
            CharT buffer[N];

            ~PlainView() {
                volatile CharT* p = buffer;
                for (size_t i = 0; i < N; ++i)
                    p[i] = 0;
            }

            const CharT* c_str() const { 
                return buffer; 
            }
        };

        PlainView decrypt() const {
            PlainView view{};
            for (size_t i = 0; i < N; ++i)
                view.buffer[i] = data[i] ^ static_cast<CharT>((key + i * 37) & static_cast<uint32_t>(~CharT(0)));
            return view;
        }
    };

    template <typename CharT, size_t N>
    Encrypt(const CharT(&)[N], uint32_t) -> Encrypt<CharT, N>;
}

#define OBF_SEED (0x9E3779B9u + (__LINE__ * 0x1000193u) ^ (__COUNTER__ * 0x5BD1E995u))

#define OBF_A(s) []() { \
    constexpr auto enc = ::Obfuscation::Encrypt(s, OBF_SEED); \
    return enc.decrypt(); \
}().c_str()

#define OBF_W(s) []() { \
    constexpr auto enc = ::Obfuscation::Encrypt(s, OBF_SEED); \
    return enc.decrypt(); \
}().c_str()

#define OBF(s) []() { \
    constexpr auto enc = ::Obfuscation::Encrypt(s, OBF_SEED); \
    return enc.decrypt(); \
}().c_str()
