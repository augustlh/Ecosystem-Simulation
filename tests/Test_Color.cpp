#include "UnitTests.h"
#include "Renderer.h"
#include <iostream>

using Color = Ecosim::Color;

void Color_Constructor()
{
    Color color1;
    FAIL_IF_FALSE(color1.r == 0 && color1.g == 0 && color1.b == 0, "Default constructor");

    Color color2(128);
    FAIL_IF_FALSE(color2.r == 128 && color2.g == 128 && color2.b == 128, "Grayscale constructor");

    Color color3(255, 0, 127);
    FAIL_IF_FALSE(color3.r == 255 && color3.g == 0 && color3.b == 127, "RGB constructor");
}

void Color_FromHex()
{
    std::string hex = "#FF8000";
    Color color = Color::FromHex(hex);
    FAIL_IF_FALSE(color.r == 255 && color.g == 128 && color.b == 0, "FromHex method");
}

int main(void)
{
    std::cout << "Testing Color" << std::endl;
    TEST_FUNCTION(Color_Constructor);
    TEST_FUNCTION(Color_FromHex);
    return 0;
}
