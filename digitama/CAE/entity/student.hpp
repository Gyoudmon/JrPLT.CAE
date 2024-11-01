#pragma once

#include "entity.hpp"

namespace WarGrey::CAE {
    enum class StudentGender { Girl, Boy, _ };

    class StudentEntity : public WarGrey::CAE::GMSEntity {
    public:
        static bool match(const std::string& line, int* offset);
        static const char* prompt();
        static const char* update_prompt();
        static const char* update_gender_prompt();

    public:
        StudentEntity(const char* s, int idx = 0) : StudentEntity(std::string(s), idx) {}
        StudentEntity(const std::string& s, int idx);
        virtual ~StudentEntity() {}

    public:
        bool update(const char* s, size_t end, size_t idx = 0);
        bool update_avatar_gender(const char* s, size_t end, size_t idx = 0);
        bool update_avatar(int aNo);
        void toggle_gender();

    public:
        uint8_t get_avatar() { return this->avatar; }
        std::string get_nickname() { return this->nickname; }

    public:
        uint64_t primary_key() override { return this->No; }
        std::string to_string() override;

    private:
        uint8_t avatar;
        uint64_t No;
        std::string nickname;
        StudentGender gender;
    };

    typedef std::shared_ptr<StudentEntity> shared_student_t;
}
