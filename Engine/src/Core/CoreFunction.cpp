#include "Core/Kareeem.h"
#include "CoreFunction.h"

namespace Karem {

    // ini sepertinya sebaiknya cuma sementara karena di khawatirkan akan mempengaruhi performance.
    // Mungkin agar lebih baik dibuat system caching 
    // yang mungkin bisa di lakukan di dalam compile time.
    glm::vec4 HexToVec4(const std::string& hexColor)
    {
        std::regex hexPattern("^#?([0-9a-fA-F]{6}|[0-9a-fA-F]{8})$");
        if (!std::regex_match(hexColor, hexPattern))
        {
            ENGINE_WARN("Invalid hex color format");
            return glm::vec4(0.0f);
        }

        // Hapus "#" jika ada
        std::string hexValue = hexColor;
        if (hexValue[0] == '#') {
            hexValue = hexValue.substr(1);
        }

        unsigned int intValue;
        std::istringstream(hexValue) >> std::hex >> intValue;

        float red = static_cast<float>((intValue >> 16) & 0xFF) / 255.0f;
        float green = static_cast<float>((intValue >> 8) & 0xFF) / 255.0f;
        float blue = static_cast<float>(intValue & 0xFF) / 255.0f;
        float alpha = 1.0f;

        // Jika ada 8 digit (termasuk alpha), atur alpha sesuai
        if (hexValue.length() == 8) {
            alpha = static_cast<float>((intValue >> 24) & 0xFF) / 255.0f;
        }

        return glm::vec4(red, green, blue, alpha);
    }


}