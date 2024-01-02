#pragma once

#include <gydm/game.hpp>
#include <vector>

#include "../entity/discipline.hpp"

namespace WarGrey::IMS {
    class Gradelet : public GYDM::IGraphlet {
    public:
        Gradelet(const char* name, float width);
        virtual ~Gradelet() noexcept {}

        const char* name() override { return this->_name.c_str(); }

    public:
        void construct(SDL_Renderer* renderer) override;
        GYDM::Box get_bounding_box() override;
        void draw(SDL_Renderer* renderer, float x, float y, float width, float height) override;

    public:
        void set_title(const char* title, ...);
        void set_title(const std::string& title) { this->set_title(title, GYDM::MatterAnchor::CB); }
        void set_title(GYDM::MatterAnchor anchor, const char* title, ...);
        void set_title(const std::string& title, GYDM::MatterAnchor anchor);
        void set_disciplines(const std::vector<WarGrey::IMS::DisciplineType>& dis, GYDM::MatterAnchor anchor);
        void set_scores(uint64_t timestamp, const std::vector<double>& scores);
        void set_score_via_points(uint64_t timestamp, const std::vector<std::vector<double>>& score_pts);
        void set_score_diffs(const std::vector<double>& diffs);
        void clear(GYDM::MatterAnchor anchor = GYDM::MatterAnchor::CB);

    protected:
        virtual uint32_t score_color(const std::vector<double>& scores, double score);

    private:
        float score_line_height();
        void clear_score(SDL_Renderer* renderer, int idx);
        void set_total_score(SDL_Renderer* renderer, uint64_t timestamp, double score);

    private:
        GYDM::shared_texture_t title;
        GYDM::shared_texture_t total_score;
        GYDM::shared_texture_t timestamp;
        std::vector<GYDM::shared_texture_t> disciplines;
        std::vector<GYDM::shared_texture_t> scores;
        std::vector<GYDM::shared_texture_t> diffs;
        std::vector<std::vector<double>> percentages;

    private:
        std::string _name;
        float width;
    };
}
