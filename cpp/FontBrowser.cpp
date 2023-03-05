#include "digitama/big_bang/game.hpp"
#include "digitama/big_bang/graphics/text.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
namespace {
    enum class CmdOpt { FontSize, ColumnCount, _ };

    class FontBrowser : public Universe {
        public:
            FontBrowser() : Universe("字体浏览器", 0) {}
        
        public:
            void construct(int argc, char* argv[]) override {
                for (int idx = 1; idx < argc; idx ++) {
                    if (this->option == CmdOpt::FontSize) {
                        this->fontsize = atoi(argv[idx]);
                        this->option = CmdOpt::_;
                    } else if (this->option == CmdOpt::ColumnCount) {
                        this->col_count = atoi(argv[idx]);
                        this->option = CmdOpt::_;
                    } else if (strncmp(argv[idx], "-s", 3) == 0) {
                        this->option = CmdOpt::FontSize;
                    } else if (strncmp(argv[idx], "-c", 3) == 0) {
                        this->option = CmdOpt::ColumnCount;
                    } else {
                        if (!this->text.empty()) {
                            this->text.push_back(' ');
                        }

                        this->text.append(argv[idx]);
                    }
                }

                if (this->fontsize <= 0) {
                    this->fontsize = 16;
                }

                if (this->col_count <= 0) {
                    this->col_count = 4;
                }

                if (this->text.empty()) {
                    this->text = "Sphinx [字体陈列馆]";
                }
            }

            void draw(SDL_Renderer* renderer, int x, int y, int width, int height) override {
                int font_count;
                const std::string* fonts = game_font_list(&font_count);

                for (int i = 0; i < font_count; i++) {
                    TTF_Font* f = game_create_font(fonts[i].c_str(), this->fontsize);

                    if (is_font_okay(f, this->text)) {
                        game_draw_blended_text(f, renderer, this->get_foreground_color(),
                            x, y, "%s: %s", fonts[i].c_str(), text.c_str());
                        
                        y = y + this->fontsize + 2;
                        if (y > height - this->fontsize) {
                            x = x + width / this->col_count;
                            y = 0;
                        }
                    }

                    game_font_destroy(f);
                }
            }

        private:
            CmdOpt option = CmdOpt::_;
            std::string text;
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
