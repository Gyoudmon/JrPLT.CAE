#include "digitama/game.hpp"
#include "digitama/graphics/text.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    class FontBrowser : public Universe {
        public:
            FontBrowser() : Universe("字体浏览器", 0) {}
        
        public:
            void construct(int argc, char* argv[]) {
                this->fontsize = 0;
                this->col_count = 0;

                if (argc > 1) {
                    this->fontsize = atoi(argv[1]);

                    if (argc > 2) {
                        this->col_count = atoi(argv[2]);
                    }
                }

                if (this->fontsize <= 0) {
                    this->fontsize = 12;
                }

                if (this->col_count <= 0) {
                    this->col_count = 5;
                }
            }

            void draw(SDL_Renderer* renderer, int x, int y, int width, int height) {
                int font_count;
                const std::string* fonts = game_font_list(&font_count);

                for (int i = 0; i < font_count; i++) {
                    TTF_Font* f = game_create_font(fonts[i].c_str(), this->fontsize);
                    std::string text = game_create_string("%s (字体陈列馆)", fonts[i].c_str());

                    game_draw_blended_text(f, renderer, this->get_foreground_color(), x, y, text.c_str());
                    game_font_destroy(f);

                    y = y + this->fontsize + 2;
                    if (y > height - this->fontsize) {
                        x = x + width / this->col_count;
                        y = 0;
                    }
                }
            }

        private:
            int fontsize;
            int col_count;
    };
}

/*************************************************************************************************/
int main(int argc, char* args[]) {
    FontBrowser* universe = new FontBrowser();

    universe->construct(argc, args);
    universe->big_bang();

    delete universe;
    return 0;
}
