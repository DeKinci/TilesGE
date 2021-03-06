//
// Created by DeKinci on 5/3/2020.
//

#include "graphics/FontLoader.h"

#include <cmrc/cmrc.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include <string>
#include FT_FREETYPE_H
#include "utils/Log.h"

CMRC_DECLARE(fonts);

Font &FontLoader::load(const char *fontName) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        Log::error("ERROR::FREETYPE: Could not init FreeType Library");

    auto fs = cmrc::fonts::get_filesystem();
    auto textureResource = fs.open(std::string(fontName) + ".ttf");
    FT_Face face;
    if (FT_New_Memory_Face(ft, (FT_Byte *) textureResource.begin(), textureResource.size(), 0, &face))
        Log::error("ERROR::FREETYPE: Failed to load font");

    if (FT_Set_Pixel_Sizes(face, 48, 48))
        Log::error("ERROR::FREETYPE: Failed to set pixel size");

    std::map<char, Symbol> characters;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 33; c < 127; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            Log::error("ERROR::FREETYTPE: Failed to load Glyph {} ({})", c, (int) c);
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Symbol character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Symbol>(c, character));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    Font *font = new Font(characters);
    return *font;
}
