#include "gradelet.hpp"

#include <plteen/datum/string.hpp>
#include <plteen/datum/vector.hpp>

#include <plteen/graphics/image.hpp>

using namespace Plteen;
using namespace WarGrey::CAE;

/*************************************************************************************************/
static const float horizontal_inset = 8.0F;

static const uint32_t head_color = GHOSTWHITE;
static const uint32_t body_color = SNOW;
static const uint32_t tail_color = WHITESMOKE;
static const uint32_t line_color = LIGHTGRAY;

#define HEAD_FONT GameFont::serif(FontSize::medium)
#define BODY_FONT GameFont::monospace()
#define MATH_FONT GameFont::math()
#define DIFF_FONT GameFont::math(FontSize::xx_small)

/*************************************************************************************************/
WarGrey::CAE::Gradelet::Gradelet(const char* name, float width) : _name(name), width(width) {
    this->camouflage(true);
    this->enable_resize(false);
}

void WarGrey::CAE::Gradelet::construct(dc_t* dc) {
    this->title = std::make_shared<Texture>(dc->create_blended_text(this->_name, HEAD_FONT, head_color));
    this->timestamp = std::make_shared<Texture>(dc->create_blended_text(" ", MATH_FONT, tail_color));
    this->total_score = std::make_shared<Texture>(dc->create_blended_text("-", MATH_FONT, tail_color));
}

Box WarGrey::CAE::Gradelet::get_bounding_box() {
    float title_height, total_score_height;
    float line_count = float(this->disciplines.size());

    this->title->feed_extent(nullptr, &title_height);
    this->total_score->feed_extent(nullptr, &total_score_height);

    return { flabs(this->width), title_height + total_score_height + this->score_line_height() * line_count };
}

void WarGrey::CAE::Gradelet::draw(dc_t* dc, float x, float y, float width, float height) {
    float dis_height = this->score_line_height();
    float rx = x + width;
    float by = y + height;
    float dis_lx = x + horizontal_inset;
    float dis_rx = rx - horizontal_inset;    
    float cw, ch;
    
    /* background decoration */ {
        dc->fill_rect(x, y, width, height, RGBA(DIMGRAY, 0.5));
        dc->draw_rect(x, y, width, height, RGBA(ROYALBLUE, 1.0));
    }

    /* draw title */ {
        this->title->feed_extent(&cw, &ch);
        dc->stamp(this->title->self(), x + (width - cw) * 0.5F, y);
        dc->draw_line(x, y + ch, rx, y + ch, line_color);
    }

    if (dis_height > 0.0F) {
        float dis_row_by = y + ch + dis_height;
        
        for (size_t idx = 0; idx < this->disciplines.size(); idx ++) {
            float dff_w = 0.0F;
        
            this->disciplines[idx]->feed_extent(&cw, &ch);
            dc->stamp(this->disciplines[idx]->self(), dis_lx, dis_row_by - ch, cw, ch);
        
            if (this->diffs[idx].use_count() > 0) {
                this->diffs[idx]->feed_extent(&dff_w, &ch);
                dc->stamp(this->diffs[idx]->self(), dis_rx - dff_w, dis_row_by - ch, dff_w, ch);
            }
        
            this->scores[idx]->feed_extent(&cw, &ch);
            dc->stamp(this->scores[idx]->self(), dis_rx - cw - dff_w, dis_row_by - ch, cw, ch);
            
            dis_row_by += dis_height;
        }
    }

    /* draw tail */ {
        this->total_score->feed_extent(&cw, &ch);
        dc->stamp(this->timestamp->self(), dis_lx, by - ch);
        dc->stamp(this->total_score->self(), dis_rx - cw, by - ch);
        dc->draw_line(x, by - ch, rx, by - ch, line_color);
    }
}

void WarGrey::CAE::Gradelet::set_title(const std::string& title, Plteen::MatterPort anchor) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        this->moor(anchor);
        this->title.reset(new Texture(dc->create_blended_text(title, HEAD_FONT, head_color)));
        this->notify_updated();
        this->clear_moor();
    }
}

void WarGrey::CAE::Gradelet::set_title(const char* title, ...) {
    VSNPRINT(t, title);
    this->set_title(t);
}

void WarGrey::CAE::Gradelet::set_title(Plteen::MatterPort anchor, const char* title, ...) {
    VSNPRINT(t, title);
    this->set_title(t, anchor);
}

void WarGrey::CAE::Gradelet::set_disciplines(const std::vector<DisciplineType>& dis, Plteen::MatterPort anchor) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        size_t total = dis.size();

        this->moor(anchor);

        this->disciplines.resize(total);
        this->scores.resize(total);
        this->diffs.resize(total);
        this->percentages.resize(total);

        for (size_t idx = 0; idx < total; idx ++) {
            std::string name = make_nstring("%s", DisciplineEntity::type_to_name(dis[idx]));

            this->disciplines[idx].reset(new Texture(dc->create_blended_text(name, BODY_FONT, body_color)));
            this->scores[idx].reset(new Texture(dc->create_blended_text("-", MATH_FONT, body_color)));
            this->diffs[idx].reset();
            this->percentages[idx].clear();
        }

        this->notify_updated();
        this->clear_moor();
    }
}

void WarGrey::CAE::Gradelet::set_scores(uint64_t timestamp, const std::vector<double>& scores) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        size_t having = this->scores.size();
        size_t given = scores.size();
        double total = flnan;
        
        for (size_t idx = 0; idx < having; idx ++) {
            if (idx < given) {
                double s = scores[idx];

                if (s >= 0.0) {
                    this->scores[idx].reset(new Texture(dc->create_blended_text(
                        make_nstring("%.2f", scores[idx]), MATH_FONT, this->score_color(scores, s))));
                    total = flisnan(total) ? s : total + s; 
                } else {
                    this->clear_score(dc, idx);
                }
            } else {
                this->clear_score(dc, idx);
            }
        }

        this->set_total_score(dc, timestamp, total);
        this->notify_updated();
    }
}

void WarGrey::CAE::Gradelet::set_score_via_points(uint64_t timestamp, const std::vector<std::vector<double>>& score_pts) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        size_t having = this->scores.size();
        size_t given = scores.size();
        double total = flnan;
        
        for (size_t idx = 0; idx < having; idx ++) {
            if (idx < given) {
                std::vector<double> pts = score_pts[idx];

                if (pts.size() > 0) {
                    double s = vector_sum(pts);

                    if (s > 0.0) {
                        this->scores[idx].reset(new Texture(dc->create_blended_text(
                            make_nstring("%.1f", s), MATH_FONT, this->score_color(pts, s))));
                        total = flisnan(total) ? s : total + s; 
                    } else {
                        this->clear_score(dc, idx);
                    }
                } else {
                    this->clear_score(dc, idx);
                }
            } else {
                this->clear_score(dc, idx);
            }
        }

        this->set_total_score(dc, timestamp, total);
        this->notify_updated();
    }
}

void WarGrey::CAE::Gradelet::set_score_diffs(const std::vector<double>& diffs) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        size_t total = this->scores.size();
        size_t given = scores.size();
        
        for (size_t idx = 0; idx < total; idx ++) {
            if (idx < given) {
                double diff = diffs[idx];

                if (diff > 0.0) {
                    this->diffs[idx].reset(new Texture(dc->create_blended_text(
                        make_nstring("+%.1f", diff), DIFF_FONT, LIME)));
                } else if (diff < 0.0) {
                    this->diffs[idx].reset(new Texture(dc->create_blended_text(
                        make_nstring("%.1f", diff), DIFF_FONT, DARKRED)));
                } else if (diff == 0.0) {
                    this->diffs[idx].reset(new Texture(dc->create_blended_text(
                        "=", DIFF_FONT, LIGHTGRAY)));
                } else { // NaN
                    this->diffs[idx].reset();
                }
            } else {
                this->diffs[idx].reset();
            }
        }

        this->notify_updated();
    }
}

void WarGrey::CAE::Gradelet::set_total_score(dc_t* dc, uint64_t timestamp, double score) {
    this->timestamp.reset(new Texture(dc->create_blended_text(
        (timestamp > 0U) ? make_nstring("%llu", timestamp) : "-",
        MATH_FONT, tail_color)));

    this->total_score.reset(new Texture(dc->create_blended_text(
        (score >= 0.0) ? make_nstring("%.1f", score) : "-",
        MATH_FONT, tail_color)));
}

void WarGrey::CAE::Gradelet::clear(MatterPort anchor) {
    dc_t* dc = this->master()->drawing_context();

    if (dc != nullptr) {
        this->moor(anchor);

        this->set_title(" ");

        for (size_t idx = 0; idx < this->scores.size(); idx ++) {
            this->clear_score(dc, idx);
        }

        this->set_total_score(dc, 0U, flnan);

        this->notify_updated();
        this->clear_moor();
    }
}

void WarGrey::CAE::Gradelet::clear_score(dc_t* dc, int idx) {
    this->scores[idx].reset(new Texture(dc->create_blended_text("-", MATH_FONT, body_color)));
    this->diffs[idx].reset();
}

/**************************************************************************************************/
float WarGrey::CAE::Gradelet::score_line_height() {
    float score_height = 0.0F;

    if (this->disciplines.size() > 0) {
        float dh, sh;

        this->disciplines[0]->feed_extent(nullptr, &dh);
        this->scores[0]->feed_extent(nullptr, &sh);
        score_height = flmax(dh, sh);
    }

    return score_height;
}

uint32_t WarGrey::CAE::Gradelet::score_color(const std::vector<double>& scores, double score) {
    return (score >= 60.0) ? GOLD : ANTIQUEWHITE;
}
