#pragma once

#include <cstring>
#include <map>
#include <string>

namespace Utils {

constexpr unsigned int AliceBlue = 0xF0F8FF;
constexpr unsigned int AntiqueWhite = 0xFAEBD7;
constexpr unsigned int Aqua = 0x00FFFF;
constexpr unsigned int Aquamarine = 0x7FFFD4;
constexpr unsigned int Azure = 0xF0FFFF;
constexpr unsigned int Beige = 0xF5F5DC;
constexpr unsigned int Bisque = 0xFFE4C4;
constexpr unsigned int Black = 0x000000;
constexpr unsigned int BlanchedAlmond = 0xFFEBCD;
constexpr unsigned int Blue = 0x0000FF;
constexpr unsigned int BlueViolet = 0x8A2BE2;
constexpr unsigned int Brown = 0xA52A2A;
constexpr unsigned int BurlyWood = 0xDEB887;
constexpr unsigned int CadetBlue = 0x5F9EA0;
constexpr unsigned int Chartreuse = 0x7FFF00;
constexpr unsigned int Chocolate = 0xD2691E;
constexpr unsigned int Coral = 0xFF7F50;
constexpr unsigned int CornflowerBlue = 0x6495ED;
constexpr unsigned int Cornsilk = 0xFFF8DC;
constexpr unsigned int Crimson = 0xDC143C;
constexpr unsigned int Cyan = 0x00FFFF;
constexpr unsigned int DarkBlue = 0x00008B;
constexpr unsigned int DarkCyan = 0x008B8B;
constexpr unsigned int DarkGoldenRod = 0xB8860B;
constexpr unsigned int DarkGray = 0xA9A9A9;
constexpr unsigned int DarkGrey = 0xA9A9A9;
constexpr unsigned int DarkGreen = 0x006400;
constexpr unsigned int DarkKhaki = 0xBDB76B;
constexpr unsigned int DarkMagenta = 0x8B008B;
constexpr unsigned int DarkOliveGreen = 0x556B2F;
constexpr unsigned int DarkOrange = 0xFF8C00;
constexpr unsigned int DarkOrchid = 0x9932CC;
constexpr unsigned int DarkRed = 0x8B0000;
constexpr unsigned int DarkSalmon = 0xE9967A;
constexpr unsigned int DarkSeaGreen = 0x8FBC8F;
constexpr unsigned int DarkSlateBlue = 0x483D8B;
constexpr unsigned int DarkSlateGray = 0x2F4F4F;
constexpr unsigned int DarkSlateGrey = 0x2F4F4F;
constexpr unsigned int DarkTurquoise = 0x00CED1;
constexpr unsigned int DarkViolet = 0x9400D3;
constexpr unsigned int DeepPink = 0xFF1493;
constexpr unsigned int DeepSkyBlue = 0x00BFFF;
constexpr unsigned int DimGray = 0x696969;
constexpr unsigned int DimGrey = 0x696969;
constexpr unsigned int DodgerBlue = 0x1E90FF;
constexpr unsigned int FireBrick = 0xB22222;
constexpr unsigned int FloralWhite = 0xFFFAF0;
constexpr unsigned int ForestGreen = 0x228B22;
constexpr unsigned int Fuchsia = 0xFF00FF;
constexpr unsigned int Gainsboro = 0xDCDCDC;
constexpr unsigned int GhostWhite = 0xF8F8FF;
constexpr unsigned int Gold = 0xFFD700;
constexpr unsigned int GoldenRod = 0xDAA520;
constexpr unsigned int Gray = 0x808080;
constexpr unsigned int Grey = 0x808080;
constexpr unsigned int Green = 0x008000;
constexpr unsigned int GreenYellow = 0xADFF2F;
constexpr unsigned int HoneyDew = 0xF0FFF0;
constexpr unsigned int HotPink = 0xFF69B4;
constexpr unsigned int IndianRed = 0xCD5C5C;
constexpr unsigned int Indigo = 0x4B0082;
constexpr unsigned int Ivory = 0xFFFFF0;
constexpr unsigned int Khaki = 0xF0E68C;
constexpr unsigned int Lavender = 0xE6E6FA;
constexpr unsigned int LavenderBlush = 0xFFF0F5;
constexpr unsigned int LawnGreen = 0x7CFC00;
constexpr unsigned int LemonChiffon = 0xFFFACD;
constexpr unsigned int LightBlue = 0xADD8E6;
constexpr unsigned int LightCoral = 0xF08080;
constexpr unsigned int LightCyan = 0xE0FFFF;
constexpr unsigned int LightGoldenRodYellow = 0xFAFAD2;
constexpr unsigned int LightGray = 0xD3D3D3;
constexpr unsigned int LightGrey = 0xD3D3D3;
constexpr unsigned int LightGreen = 0x90EE90;
constexpr unsigned int LightPink = 0xFFB6C1;
constexpr unsigned int LightSalmon = 0xFFA07A;
constexpr unsigned int LightSeaGreen = 0x20B2AA;
constexpr unsigned int LightSkyBlue = 0x87CEFA;
constexpr unsigned int LightSlateGray = 0x778899;
constexpr unsigned int LightSlateGrey = 0x778899;
constexpr unsigned int LightSteelBlue = 0xB0C4DE;
constexpr unsigned int LightYellow = 0xFFFFE0;
constexpr unsigned int Lime = 0x00FF00;
constexpr unsigned int LimeGreen = 0x32CD32;
constexpr unsigned int Linen = 0xFAF0E6;
constexpr unsigned int Magenta = 0xFF00FF;
constexpr unsigned int Maroon = 0x800000;
constexpr unsigned int MediumAquaMarine = 0x66CDAA;
constexpr unsigned int MediumBlue = 0x0000CD;
constexpr unsigned int MediumOrchid = 0xBA55D3;
constexpr unsigned int MediumPurple = 0x9370DB;
constexpr unsigned int MediumSeaGreen = 0x3CB371;
constexpr unsigned int MediumSlateBlue = 0x7B68EE;
constexpr unsigned int MediumSpringGreen = 0x00FA9A;
constexpr unsigned int MediumTurquoise = 0x48D1CC;
constexpr unsigned int MediumVioletRed = 0xC71585;
constexpr unsigned int MidnightBlue = 0x191970;
constexpr unsigned int MintCream = 0xF5FFFA;
constexpr unsigned int MistyRose = 0xFFE4E1;
constexpr unsigned int Moccasin = 0xFFE4B5;
constexpr unsigned int NavajoWhite = 0xFFDEAD;
constexpr unsigned int Navy = 0x000080;
constexpr unsigned int OldLace = 0xFDF5E6;
constexpr unsigned int Olive = 0x808000;
constexpr unsigned int OliveDrab = 0x6B8E23;
constexpr unsigned int Orange = 0xFFA500;
constexpr unsigned int OrangeRed = 0xFF4500;
constexpr unsigned int Orchid = 0xDA70D6;
constexpr unsigned int PaleGoldenRod = 0xEEE8AA;
constexpr unsigned int PaleGreen = 0x98FB98;
constexpr unsigned int PaleTurquoise = 0xAFEEEE;
constexpr unsigned int PaleVioletRed = 0xDB7093;
constexpr unsigned int PapayaWhip = 0xFFEFD5;
constexpr unsigned int PeachPuff = 0xFFDAB9;
constexpr unsigned int Peru = 0xCD853F;
constexpr unsigned int Pink = 0xFFC0CB;
constexpr unsigned int Plum = 0xDDA0DD;
constexpr unsigned int PowderBlue = 0xB0E0E6;
constexpr unsigned int Purple = 0x800080;
constexpr unsigned int RebeccaPurple = 0x663399;
constexpr unsigned int Red = 0xFF0000;
constexpr unsigned int RosyBrown = 0xBC8F8F;
constexpr unsigned int RoyalBlue = 0x4169E1;
constexpr unsigned int SaddleBrown = 0x8B4513;
constexpr unsigned int Salmon = 0xFA8072;
constexpr unsigned int SandyBrown = 0xF4A460;
constexpr unsigned int SeaGreen = 0x2E8B57;
constexpr unsigned int SeaShell = 0xFFF5EE;
constexpr unsigned int Sienna = 0xA0522D;
constexpr unsigned int Silver = 0xC0C0C0;
constexpr unsigned int SkyBlue = 0x87CEEB;
constexpr unsigned int SlateBlue = 0x6A5ACD;
constexpr unsigned int SlateGray = 0x708090;
constexpr unsigned int SlateGrey = 0x708090;
constexpr unsigned int Snow = 0xFFFAFA;
constexpr unsigned int SpringGreen = 0x00FF7F;
constexpr unsigned int SteelBlue = 0x4682B4;
constexpr unsigned int Tan = 0xD2B48C;
constexpr unsigned int Teal = 0x008080;
constexpr unsigned int Thistle = 0xD8BFD8;
constexpr unsigned int Tomato = 0xFF6347;
constexpr unsigned int Turquoise = 0x40E0D0;
constexpr unsigned int Violet = 0xEE82EE;
constexpr unsigned int Wheat = 0xF5DEB3;
constexpr unsigned int White = 0xFFFFFF;
constexpr unsigned int WhiteSmoke = 0xF5F5F5;
constexpr unsigned int Yellow = 0xFFFF00;
constexpr unsigned int YellowGreen = 0x9ACD32;

struct Color {
    unsigned char r, g, b;

    Color() : r(0), g(0), b(0) {
    }

    Color(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {
    }

    explicit Color(std::string hex) {
        if (hex[0] == '#') {
            hex = hex.substr(1);
        }

        if (hex.length() == 6) {
            r = std::stoi(hex.substr(0, 2), nullptr, 16);
            g = std::stoi(hex.substr(2, 2), nullptr, 16);
            b = std::stoi(hex.substr(4, 2), nullptr, 16);
        } else if (hex.length() == 3) {
            r = std::stoi(hex.substr(0, 1), nullptr, 16);
            g = std::stoi(hex.substr(1, 1), nullptr, 16);
            b = std::stoi(hex.substr(2, 1), nullptr, 16);
        } else {
            r = 0;
            g = 0;
            b = 0;
        }
    }

    explicit Color(unsigned int color) : r((color >> 16) & 0xFF), g((color >> 8) & 0xFF), b(color & 0xFF) {
    }

    explicit operator unsigned int() const {
        return (r << 16) | (g << 8) | b;
    }

    explicit operator std::string() const {
        char buf[9];
        snprintf(buf, 9, "#%02X%02X%02X", r, g, b);
        return buf;
    }

    bool operator==(const Color &other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    bool operator!=(const Color &other) const {
        return !(*this == other);
    }

    bool operator==(const std::string &other) const {
        return (std::string) * this == other;
    }

    bool operator!=(const std::string &other) const {
        return !(*this == other);
    }

    bool operator==(const unsigned int &other) const {
        return (unsigned int)*this == other;
    }

    bool operator!=(const unsigned int &other) const {
        return !(*this == other);
    }

    Color operator+(const Color &other) const {
        return {(unsigned char)(r + other.r), (unsigned char)(g + other.g), (unsigned char)(b + other.b)};
    }

    Color operator-(const Color &other) const {
        return {(unsigned char)(r - other.r), (unsigned char)(g - other.g), (unsigned char)(b - other.b)};
    }

    Color &operator+=(const Color &other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    Color &operator-=(const Color &other) {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        return *this;
    }

    Color &operator*=(float f) {
        r *= f;
        g *= f;
        b *= f;
        return *this;
    }

    Color &operator/=(float f) {
        r /= f;
        g /= f;
        b /= f;
        return *this;
    }

    [[nodiscard]] unsigned int toInt() const {
        return (r << 16) | (g << 8) | b;
    }

    static Color getColor(const std::string &name);

    using ColorMap = std::map<std::string, unsigned int>;

  private:
    static ColorMap HtmlColorMap;
};
} // namespace Utils