#pragma once
#include <bits/stdc++.h>

#include "Color.h"

using namespace std;

class Sprite
{
public:
    Sprite() {}
    Sprite(int w, int h);
    Sprite(wstring sFile);

    void SetGlyph(int x, int y, short c);
    void SetColour(int x, int y, short c);
    short GetGlyph(int x, int y);
    short GetColour(int x, int y);
    short SampleGlyph(float x, float y);
    short SampleColour(float x, float y);
    bool Load(wstring sFile);

    int nWidth = 0;
    int nHeight = 0;
    void Create(int w, int h);

private:
    short* m_Glyphs = nullptr;
    short* m_Colours = nullptr;
};
