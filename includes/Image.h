#ifndef IMAGE_H_
#define IMAGE_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

struct pixel
{
    unsigned char r, g, b;
};

class Image
{
private:
    string type;
    string width;
    string height;
    string RGB;
    int pixelsize;
    int rgbsize;
    vector<pixel> pixels;
    char *rawData;

public:
    Image() : type(""), width(""), height(""), RGB(""), pixelsize(0), rgbsize(0){};
    Image(string imgPath) { open(imgPath); };
    ~Image(){};
    Image &gammaCorrection(const double &gamma);
    Image &alphaComp(Image &img, const double &alpha);
    pixel &operator[](const unsigned int &i);
    friend Image &operator+=(Image &img1, Image &img2);
    friend Image &operator+(Image &img1, Image &img2);
    friend Image &operator-(Image &img1, Image &img2);
    friend Image &operator-=(Image &img1, Image &img2);
    Image &operator*(const double &scalar);
    string getType() { return type; };
    string getWidth() { return width; };
    string getHeight() { return height; };
    string getRGB() { return RGB; };
    vector<pixel> getPixels() { return pixels; };
    char *getRawData() { return rawData; };
    int open(string imgPath);
    int save(string imgPath);
};

pixel &Image::operator[](const unsigned int &i)
{
    return pixels[i];
}

Image &operator+(Image &img1, Image &img2)
{
    static Image base = img1;

    if (base.type == img2.type && base.width == img2.width && base.height == img2.height)
    {
        if (base.type == "P2" || base.type == "P3")
        {
            for (int i = 0; i < base.pixelsize; ++i)
            {
                base[i].r += img2[i].r;
                base[i].g += img2[i].g;
                base[i].b += img2[i].b;
            }
        }
        else if (base.type == "P5" || base.type == "P6")
        {
            for (int i = 0; i < base.rgbsize; ++i)
            {
                int hold;
                hold = (int)base[i].r + (int)img2[i].r;
                base[i].r = (unsigned char)hold;
                hold = (int)base[i].g + (int)img2[i].g;
                base[i].g = (unsigned char)hold;
                hold = (int)base[i].b + (int)img2[i].b;
                base[i].b = (unsigned char)hold;
            }
        };
    }
    else
    {
        cout << "Formats Are Not The Same!" << endl;
    }

    return base;
}

Image &operator+=(Image &img1, Image &img2)
{
    static Image base = img1;

    if (base.type == img2.type && base.width == img2.width && base.height == img2.height)
    {
        if (base.type == "P2" || base.type == "P3")
        {
            for (int i = 0; i < base.pixelsize; ++i)
            {
                base[i].r = (base[i].r + img2[i].r) / 2;
                base[i].g = (base[i].g + img2[i].g) / 2;
                base[i].b = (base[i].b + img2[i].b) / 2;
            };
        }
        else if (base.type == "P5" || base.type == "P6")
        {
            for (int i = 0; i < base.rgbsize; ++i)
            {
                int hold;
                hold = ((int)base[i].r + (int)img2[i].r) / 2;
                base[i].r = (unsigned char)hold;
                hold = ((int)base[i].g + (int)img2[i].g) / 2;
                base[i].g = (unsigned char)hold;
                hold = ((int)base[i].b + (int)img2[i].b) / 2;
                base[i].b = (unsigned char)hold;
            }
        };
        img1 = base;
    }
    else
    {
        cout << "Formats Are Not The Same!" << endl;
    }

    return base;
}

Image &operator-(Image &img1, Image &img2)
{
    static Image base = img1;

    if (base.type == img2.type && base.width == img2.width && base.height == img2.height)
    {
        if (base.type == "P2" || base.type == "P3")
        {
            for (int i = 0; i < base.pixelsize; ++i)
            {
                base[i].r = ((int)base[i].r - (int)img2[i].r) < 0 ? 0 : base[i].r;
                base[i].g = ((int)base[i].g - (int)img2[i].g) < 0 ? 0 : base[i].g;
                base[i].b = ((int)base[i].b - (int)img2[i].b) < 0 ? 0 : base[i].b;
            };
        }
        else if (base.type == "P5" || base.type == "P6")
        {
            for (int i = 0; i < base.rgbsize; ++i)
            {
                base[i].r = ((int)base[i].r - (int)img2[i].r) < 0 ? (unsigned char)0 : base[i].r;
                base[i].g = ((int)base[i].g - (int)img2[i].g) < 0 ? (unsigned char)0 : base[i].g;
                base[i].b = ((int)base[i].b - (int)img2[i].b) < 0 ? (unsigned char)0 : base[i].b;
            }
        }
    }
    else
    {
        cout << "Formats Are Not The Same!" << endl;
    }

    return base;
}

Image &operator-=(Image &img1, Image &img2)
{
    static Image base = img1;

    if (base.type == img2.type && base.width == img2.width && base.height == img2.height)
    {
        if (base.type == "P2" || base.type == "P3")
        {
            for (int i = 0; i < base.pixelsize; ++i)
            {
                base[i].r = ((int)base[i].r - (int)img2[i].r) < 0 ? 0 : base[i].r;
                base[i].g = ((int)base[i].g - (int)img2[i].g) < 0 ? 0 : base[i].g;
                base[i].b = ((int)base[i].b - (int)img2[i].b) < 0 ? 0 : base[i].b;
            };
        }
        else if (base.type == "P5" || base.type == "P6")
        {
            for (int i = 0; i < base.rgbsize; ++i)
            {
                base[i].r = ((int)base[i].r - (int)img2[i].r) < 0 ? (unsigned char)0 : base[i].r;
                base[i].g = ((int)base[i].g - (int)img2[i].g) < 0 ? (unsigned char)0 : base[i].g;
                base[i].b = ((int)base[i].b - (int)img2[i].b) < 0 ? (unsigned char)0 : base[i].b;
            }
        }

        img1 = base;
    }
    else
    {
        cout << "Formats Are Not The Same!" << endl;
    }

    return base;
}

Image &Image::operator*(const double &scalar)
{
    static Image base = *this;

    int size;

    if (base.type == "P2" || base.type == "P3")
    {
        size = this->pixelsize;
    }
    else if (base.type == "P5" || base.type == "P6")
    {
        size = this->rgbsize;
    }

    for (int i = 0; i < size; ++i)
    {
        if ((double)base[i].r * scalar < 0)
        {
            base[i].r = 0;
        }
        else if ((double)base[i].r * scalar > 255)
        {
            base[i].r = 255;
        }
        else
        {
            base[i].r = (double)base[i].r * scalar;
        }

        if ((double)base[i].g * scalar < 0)
        {
            base[i].g = 0;
        }
        else if ((double)base[i].g * scalar > 255)
        {
            base[i].g = 255;
        }
        else
        {
            base[i].g = (double)base[i].g * scalar;
        }

        if ((double)base[i].b * scalar < 0)
        {
            base[i].b = 0;
        }
        else if ((double)base[i].b * scalar > 255)
        {
            base[i].b = 255;
        }
        else
        {
            base[i].b = (double)base[i].b * scalar;
        }
    };

    return base;
}

Image &Image::gammaCorrection(const double &gamma)
{
    static Image base = *this;

    double correction = 1 / gamma;

    int size;

    if (base.type == "P2" || base.type == "P3")
    {
        size = this->pixelsize;
    }
    else if (base.type == "P5" || base.type == "P6")
    {
        size = this->rgbsize;
    }

    for (int i = 0; i < size; ++i)
    {
        if ((255 * pow(((double)base[i].r / 255), correction)) < 0)
        {
            base[i].r = 0;
        }
        else if ((255 * pow(((double)base[i].r / 255), correction)) > 255)
        {
            base[i].r = 255;
        }
        else
        {
            base[i].r = (255 * pow(((double)base[i].r / 255), correction));
        }

        if ((255 * pow(((double)base[i].g / 255), correction)) < 0)
        {
            base[i].g = 0;
        }
        else if ((255 * pow(((double)base[i].g / 255), correction)) > 255)
        {
            base[i].g = 255;
        }
        else
        {
            base[i].g = (255 * pow(((double)base[i].g / 255), correction));
        }

        if ((255 * pow(((double)base[i].b / 255), correction)) < 0)
        {
            base[i].b = 0;
        }
        else if ((255 * pow(((double)base[i].b / 255), correction)) > 255)
        {
            base[i].b = 255;
        }
        else
        {
            base[i].b = (255 * pow(((double)base[i].b / 255), correction));
        }
    };

    return base;
}

Image &Image::alphaComp(Image &img, const double &alpha)
{
    static Image base = *this;
    if (base.type == img.type && base.width == img.width && base.height == img.height)
    {
        int size;

        if (base.type == "P2" || base.type == "P3")
        {
            size = this->pixelsize;
        }
        else if (base.type == "P5" || base.type == "P6")
        {
            size = this->rgbsize;
        }

        for (int i = 0; i < size; ++i)
        {
            base[i].r = (double)base[i].r * (1 - alpha) + (double)img[i].r * alpha;
            base[i].g = (double)base[i].g * (1 - alpha) + (double)img[i].g * alpha;
            base[i].b = (double)base[i].b * (1 - alpha) + (double)img[i].b * alpha;
        };
    }
    else
    {
        cout << "Formats Are Not The Same!" << endl;
    }

    return base;
}

int Image::open(string imgPath)
{
    ifstream image;

    image.open(imgPath, ios::in | ios::binary);

    if (image.is_open())
    {
        image >> type;

        image.seekg(1, image.cur);
        char comment;
        image.get(comment);

        if (comment == '#')
        {
            while (comment != '\n')
            {
                image.get(comment);
            }
        }
        else
        {
            image.seekg(-1, image.cur);
        }

        image >> width;
        image >> height;
        image >> RGB;

        if (stoi(RGB) > 255)
        {
            cout << "Error: Max Size Is Not Acceptable " << imgPath << endl;
            return 0;
        }

        pixelsize = stoi(width) * stoi(height);
        rgbsize = stoi(width) * stoi(height) * 3;

        if (type == "P5" || type == "P6")
        {
            image.seekg(1, image.cur);

            char *buffer = new char[rgbsize];

            pixels.resize(rgbsize);

            image.read(reinterpret_cast<char *>(buffer), rgbsize);
            rawData = buffer;

            int count = 0;
            for (int j = 0; j < rgbsize; j += 3)
            {
                pixels.push_back(pixel());
                pixels[j].r = buffer[j];
                pixels[j].g = buffer[j + 1];
                pixels[j].b = buffer[j + 2];
                count++;
            }

            delete[] buffer;
        }
        else if (type == "P3" || type == "P2")
        {
            for (int i = 0; i < pixelsize; i++)
            {
                int hold;
                pixels.push_back(pixel());
                image >> hold;
                pixels[i].r = hold;
                image >> hold;
                pixels[i].g = hold;
                image >> hold;
                pixels[i].b = hold;
            }
        }
    }
    else
    {
        cout << "Error: Check Path " << imgPath << endl;
        return 0;
    }

    image.close();

    return 1;
}

int Image::save(string imgPath)
{
    ofstream newimage;
    newimage.open(imgPath, ios::out | ios::binary);

    int r = 0, g = 0, b = 0;

    newimage << type << endl;
    newimage << width << " " << height << endl;
    newimage << RGB << endl;

    if (type == "P5" || type == "P6")
    {
        char *buffer = new char[rgbsize];

        for (int j = 0; j < rgbsize; j += 3)
        {
            buffer[j] = pixels[j].r;
            buffer[j + 1] = pixels[j].g;
            buffer[j + 2] = pixels[j].b;
        }

        newimage.write(reinterpret_cast<char *>(buffer), rgbsize);

        delete[] buffer;
    }
    else if (type == "P3" || type == "P2")
    {
        for (int i = 0; i < pixelsize; i++)
        {
            newimage << (int)pixels[i].r << " " << (int)pixels[i].g << " " << (int)pixels[i].b << endl;
        }
    }
    newimage.close();

    return 1;
}

#endif