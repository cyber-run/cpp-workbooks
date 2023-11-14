#include <iostream>
#include <fstream>

using namespace std;

int main() {

    ofstream image;
    image.open("image.ppm");

    if (image.is_open())
    {
        image << "P3\n";
        image << "3 2\n";
        image << "255\n";
    }
    

}