#include "utils/HtmlColor.h"

#include "utils/StringUtil.h"
#include <algorithm>
#include <map>

namespace Utils
{
Color::ColorMap Color::HtmlColorMap = []{
    return Color::ColorMap{
        {"aliceblue", AliceBlue},
        {"antiquewhite", AntiqueWhite},
        {"aqua", Aqua},
        {"aquamarine", Aquamarine},
        {"azure", Azure},
        {"beige", Beige},
        {"bisque", Bisque},
        {"black", Black},
        {"blanchedalmond", BlanchedAlmond},
        {"blue", Blue},
        {"blueviolet", BlueViolet},
        {"brown", Brown},
        {"burlywood", BurlyWood},
        {"cadetblue", CadetBlue},
        {"chartreuse", Chartreuse},
        {"chocolate", Chocolate},
        {"coral", Coral},
        {"cornflowerblue", CornflowerBlue},
        {"cornsilk", Cornsilk},
        {"crimson", Crimson},
        {"cyan", Cyan},
        {"darkblue", DarkBlue},
        {"darkcyan", DarkCyan},
        {"darkgoldenrod", DarkGoldenRod},
        {"darkgray", DarkGray},
        {"darkgrey", DarkGrey},
        {"darkgreen", DarkGreen},
        {"darkkhaki", DarkKhaki},
        {"darkmagenta", DarkMagenta},
        {"darkolivegreen", DarkOliveGreen},
        {"darkorange", DarkOrange},
        {"darkorchid", DarkOrchid},
        {"darkred", DarkRed},
        {"darksalmon", DarkSalmon},
        {"darkseagreen", DarkSeaGreen},
        {"darkslateblue", DarkSlateBlue},
        {"darkslategray", DarkSlateGray},
        {"darkslategrey", DarkSlateGrey},
        {"darkturquoise", DarkTurquoise},
        {"darkviolet", DarkViolet},
        {"deeppink", DeepPink},
        {"deepskyblue", DeepSkyBlue},
        {"dimgray", DimGray},
        {"dimgrey", DimGrey},
        {"dodgerblue", DodgerBlue},
        {"firebrick", FireBrick},
        {"floralwhite", FloralWhite},
        {"forestgreen", ForestGreen},
        {"fuchsia", Fuchsia},
        {"gainsboro", Gainsboro},
        {"ghostwhite", GhostWhite},
        {"gold", Gold},
        {"goldenrod", GoldenRod},
        {"gray", Gray},
        {"grey", Grey},
        {"green", Green},
        {"greenyellow", GreenYellow},
        {"honeydew", HoneyDew},
        {"hotpink", HotPink},
        {"indianred", IndianRed},
        {"indigo", Indigo},
        {"ivory", Ivory},
        {"khaki", Khaki},
        {"lavender", Lavender},
        {"lavenderblush", LavenderBlush},
        {"lawngreen", LawnGreen},
        {"lemonchiffon", LemonChiffon},
        {"lightblue", LightBlue},
        {"lightcoral", LightCoral},
        {"lightcyan", LightCyan},
        {"lightgoldenrodyellow", LightGoldenRodYellow},
        {"lightgray", LightGray},
        {"lightgrey", LightGrey},
        {"lightgreen", LightGreen},
        {"lightpink", LightPink},
        {"lightsalmon", LightSalmon},
        {"lightseagreen", LightSeaGreen},
        {"lightskyblue", LightSkyBlue},
        {"lightslategray", LightSlateGray},
        {"lightslategrey", LightSlateGrey},
        {"lightsteelblue", LightSteelBlue},
        {"lightyellow", LightYellow},
        {"lime", Lime},
        {"limegreen", LimeGreen},
        {"linen", Linen},
        {"magenta", Magenta},
        {"maroon", Maroon},
        {"mediumaquamarine", MediumAquaMarine},
        {"mediumblue", MediumBlue},
        {"mediumorchid", MediumOrchid},
        {"mediumpurple", MediumPurple},
        {"mediumseagreen", MediumSeaGreen},
        {"mediumslateblue", MediumSlateBlue},
        {"mediumspringgreen", MediumSpringGreen},
        {"mediumturquoise", MediumTurquoise},
        {"mediumvioletred", MediumVioletRed},
        {"midnightblue", MidnightBlue},
        {"mintcream", MintCream},
        {"mistyrose", MistyRose},
        {"moccasin", Moccasin},
        {"navajowhite", NavajoWhite},
        {"navy", Navy},
        {"oldlace", OldLace},
        {"olive", Olive},
        {"olivedrab", OliveDrab},
        {"orange", Orange},
        {"orangered", OrangeRed},
        {"orchid", Orchid},
        {"palegoldenrod", PaleGoldenRod},
        {"palegreen", PaleGreen},
        {"paleturquoise", PaleTurquoise},
        {"palevioletred", PaleVioletRed},
        {"papayawhip", PapayaWhip},
        {"peachpuff", PeachPuff},
        {"peru", Peru},
        {"pink", Pink},
        {"plum", Plum},
        {"powderblue", PowderBlue},
        {"purple", Purple},
        {"rebeccapurple", RebeccaPurple},
        {"red", Red},
        {"rosybrown", RosyBrown},
        {"royalblue", RoyalBlue},
        {"saddlebrown", SaddleBrown},
        {"salmon", Salmon},
        {"sandybrown", SandyBrown},
        {"seagreen", SeaGreen},
        {"seashell", SeaShell},
        {"sienna", Sienna},
        {"silver", Silver},
        {"skyblue", SkyBlue},
        {"slateblue", SlateBlue},
        {"slategray", SlateGray},
        {"slategrey", SlateGrey},
        {"snow", Snow},
        {"springgreen", SpringGreen},
        {"steelblue", SteelBlue},
        {"tan", Tan},
        {"teal", Teal},
        {"thistle", Thistle},
        {"tomato", Tomato},
        {"turquoise", Turquoise},
        {"violet", Violet},
        {"wheat", Wheat},
        {"white", White},
        {"whitesmoke", WhiteSmoke},
        {"yellow", Yellow},
        {"yellowgreen", YellowGreen},
    };
}();
}

Utils::Color Utils::Color::getColor(const std::string &str) {
    if (str.empty()) {
        return {};
    }

    auto lowerCase = str;
    std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);

    auto color = HtmlColorMap.find(lowerCase);
    if (color != HtmlColorMap.cend()) {
        return Utils::Color(color->second);
    }

    try {
        unsigned int const val = std::stoul(str);
        return Utils::Color(val);
    } catch (const std::exception &e) {
    }

    if (lowerCase.starts_with("#")) {
        lowerCase = lowerCase.substr(1);
    }

    return Utils::Color(lowerCase);
}