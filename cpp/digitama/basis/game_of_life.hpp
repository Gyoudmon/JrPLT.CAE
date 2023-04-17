#pragma once // 确保只被 include 一次

#include "../big_bang/bang.hpp"

#include <map>

namespace WarGrey::STEM {
    /*********************************************************************************************/
    /** 声明游戏物体 **/
    class GameOfLifelet : public WarGrey::STEM::IGraphlet {
    public:
        GameOfLifelet(int n, float gridsize) : GameOfLifelet(n, n, gridsize) {}
        GameOfLifelet(int row, int col, float gridsize) : row(row), col(col), gridsize(gridsize) {}
        virtual ~GameOfLifelet();

        void construct(SDL_Renderer* renderer) override;

    public:
        void feed_extent(float x, float y, float* width = nullptr, float* height = nullptr) override;
        void draw(SDL_Renderer* renderer, float x, float y, float Width, float Height) override;

    public:
        void show_grid(bool yes);
        void set_color(uint32_t hex);
        void modify_life_at_location(float x, float y);
        int current_generation() { return this->generation; }

    public:
        void construct_random_world();
        bool pace_forward(int repeats);
        void reset();

    public:
        void load(const std::string& life_world, std::ifstream& golin);
        void save(const std::string& life_world, std::ofstream& golout);

    protected: // 演化策略, 默认留给子类实现
        virtual void evolve(int** world, int* shadow, int row, int col) = 0;

    private:
        int row;
        int col;
        int generation;
        int** world = nullptr;
        int* shadow = nullptr;

    private:
        uint32_t color = BLACK;
        bool hide_grid;

    private:
        float gridsize;
    };

    /*********************************************************************************************/
    /** 声明游戏宇宙 **/
    
    enum class GameState { Auto, Stop, Edit, _ };

    class GameOfLifeWorld : public WarGrey::STEM::TheBigBang {
        public:
            GameOfLifeWorld(float gridsize = 8.0F) : GameOfLifeWorld(nullptr, gridsize) {}
            GameOfLifeWorld(const std::string& life_demo, float gridsize = 8.0F)
                : TheBigBang("生命游戏"), demo_path(life_demo), gridsize(gridsize) {}
            virtual ~GameOfLifeWorld() {}

        public:    // 覆盖游戏基本方法
            void load(float width, float height) override;
            void reflow(float width, float height) override;
            void update(uint64_t count, uint32_t interval, uint64_t uptime) override;
            void on_mission_start(float width, float height) override;

        public:
            bool can_select(WarGrey::STEM::IMatter* m) override;
            
        protected: // 覆盖输入事件处理方法
            void on_char(char key, uint16_t modifiers, uint8_t repeats, bool pressed) override; // 处理键盘事件
            void on_tap(WarGrey::STEM::IMatter* m, float x, float y) override;                  // 处理鼠标事件

        protected: // 处理保存事件
            const char* usrdata_extension() override { return ".gol"; }
            void on_save(const std::string& life_world, std::ofstream& golout) override;

        private:
            void switch_game_state(WarGrey::STEM::GameState new_state);
            void update_instructions_state(const uint32_t* colors);
            void pace_forward(int repeats = 1);
            void load_demos();
            
        private: // 游戏物体
            WarGrey::STEM::GameOfLifelet* gameboard;
            WarGrey::STEM::Labellet* generation;
            std::map<char, WarGrey::STEM::Labellet*> instructions;

        private: // 游戏状态
            WarGrey::STEM::GameState state = GameState::_;

        private:
            std::string demo_path;
            float gridsize;
    };
}
