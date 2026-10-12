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
            for (size_t i = 0; i < N; ++i)
                data[i] = str[i] ^ static_cast<CharT>(key + i * 37);
        }

        struct PlainView {
            CharT buffer[N];
            ~PlainView() {
                volatile CharT* p = buffer;
                for (size_t i = 0; i < N; ++i) p[i] = 0;
            }
            const CharT* c_str() const { return buffer; }
        };

        PlainView decrypt() const {
            PlainView view{};
            for (size_t i = 0; i < N; ++i) 
                view.buffer[i] = data[i] ^ static_cast<CharT>(key + i * 37);
            return view;
        }
    };

    template <typename CharT, size_t N>
    Encrypt(const CharT(&)[N], uint32_t) -> Encrypt<CharT, N>;
}

#define OBF_A(s) []() { \
    constexpr auto enc = ::Obfuscation::Encrypt(s, 0x1234 + __LINE__); \
    return enc.decrypt(); \
}().c_str()

#define OBF_W(s) []() { \
    constexpr auto enc = ::Obfuscation::Encrypt(s, 0x5678 + __LINE__); \
    return enc.decrypt(); \
}().c_str()

#define OBF(s) []() { \
    constexpr auto enc = ::Obfuscation::Encrypt(s, 0x90AB + __LINE__); \
    return enc.decrypt(); \
}().c_str()
