
#include "../../digitama/big_bang/wormhole/jargon/asn_der.hpp"

#include "../../digitama/big_bang/datum/fixnum.hpp"
#include "../../digitama/big_bang/datum/flonum.hpp"
#include "../../digitama/big_bang/datum/natural.hpp"
#include "../../digitama/big_bang/datum/enum.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
#define asn_tame_string(s, span, offset, basn, bsize, restored, rsize, type_span, type_to_octets, octets_to_type) \
	std::string str(s, rsize); \
	octets asn; \
	std::string rs; \
\
	(*offset) = 0; \
	(*span) = asn_span(type_span, str); \
	asn = type_to_octets(str); \
	rs = octets_to_type(asn, offset); \
\
	memcpy(basn, asn.c_str(), bsize); \
	memcpy(restored, rs.c_str(), rsize)

/*************************************************************************************************/
extern "C" {
	__ffi__ size_t asn_tame_length(size_t size, size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		asn_length_into_octets(size, basn);

		(*span) = asn_length_span(size);
		(*offset) = 0;
		
		return asn_octets_to_length(basn, offset);
	}

	__ffi__ int64_t asn_tame_fixnum(int64_t fixnum, size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		asn_fixnum_into_octets(fixnum, basn);
		
		(*span) = asn_span(asn_fixnum_span, fixnum);
		(*offset) = 0;
		
		return asn_octets_to_fixnum(basn, offset);
	}

	__ffi__ void asn_tame_natural(const char* nstr, size_t* span, size_t* offset, uint8_t* basn, size_t bsize,
			uint8_t* restored, size_t rsize, char ten) {
		Natural n(16U, std::string(nstr));

		asn_natural_into_octets(n, basn);
		
		(*span) = asn_span(asn_natural_span, n);
		(*offset) = 0;
		
		memcpy(restored, asn_octets_to_natural(basn, offset).to_hexstring(ten).c_str(), rsize);
	}
	
	__ffi__ double asn_tame_flonum(double real, size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		octets flonum = asn_flonum_to_octets(real);

		memcpy(basn, flonum.c_str(), bsize);
		(*span) = asn_span(asn_flonum_span, real);
		(*offset) = 0;
			
		return asn_octets_to_flonum(basn, offset);
	}

	__ffi__ bool asn_tame_boolean(bool b, size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		asn_boolean_into_octets(b, basn);
		
		(*span) = asn_span(asn_boolean_span, b);
		(*offset) = 0;
		
		return asn_octets_to_boolean(basn, offset);
	}

	__ffi__ void asn_tame_null(size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		asn_null_into_octets(nullptr, basn);
		
		(*span) = asn_span(asn_null_span, nullptr);
		(*offset) = 0;
		
		asn_octets_to_null(basn, offset);
	}

	__ffi__ void asn_tame_ia5_string(const char* s, size_t* span, size_t* offset, uint8_t* basn, size_t bsize, uint8_t* restored, size_t rsize) {
		asn_tame_string(s, span, offset, basn, bsize, restored, rsize,
			asn_ia5_span, asn_ia5_to_octets, asn_octets_to_ia5);
	}

	__ffi__ void asn_tame_utf8_string(const char* s, size_t* span, size_t* offset, uint8_t* basn, size_t bsize, uint8_t* restored, size_t rsize) {
		asn_tame_string(s, span, offset, basn, bsize, restored, rsize,
			asn_utf8_span, asn_utf8_to_octets, asn_octets_to_utf8);
	}
}

/*************************************************************************************************/
define_asn_enum(log, Log,
	Debug, Info, Notice, Warning,
	Error, Critical, Alarm, Panic,
	_
);

struct LogMessage : public IASNSequence {
public:
	LogMessage(Log level, const char* message, int64_t timestamp, const char* topic)
		: IASNSequence(4), level(level), message(message), timestamp(timestamp), topic(topic) {}

	LogMessage(const uint8_t* basn, size_t* offset = nullptr) : IASNSequence(4) {
		this->from_octets(basn, offset);
	}

	LogMessage(const octets& basn, size_t* offset = nullptr) : IASNSequence(4) {
		this->from_octets(basn, offset);
	}

public:
	Log level;
	std::string message;
	int64_t timestamp;
	std::string topic;

public:
	bool equals(LogMessage* target) {
		return (target != nullptr)
			&& (this->level== target->level)
			&& (this->timestamp == target->timestamp)
			&& (strcmp(this->message.c_str(), target->message.c_str()) == 0)
			&& (strcmp(this->topic.c_str(), target->topic.c_str()) == 0);
	}

protected:
	size_t field_payload_span(size_t idx) override {
		size_t span = 0;

		switch (idx) {
		case 0: span = asn_log_span(this->level); break;
		case 1: span = asn_utf8_span(this->message); break;
		case 2: span = asn_fixnum_span(this->timestamp); break;
		case 3: span = asn_ia5_span(this->topic); break;
		}

		return span;
	}

	size_t fill_field(size_t idx, uint8_t* octets, size_t offset) override {
		switch (idx) {
		case 0: offset = asn_log_into_octets(this->level, octets, offset); break;
		case 1: offset = asn_utf8_into_octets(this->message, octets, offset); break;
		case 2: offset = asn_fixnum_into_octets(this->timestamp, octets, offset); break;
		case 3: offset = asn_ia5_into_octets(this->topic, octets, offset); break;
		}

		return offset;
	}

	void extract_field(size_t idx, const uint8_t* basn, size_t* offset) override {
		switch (idx) {
		case 0: this->level = asn_octets_to_log(basn, offset); break;
		case 1: this->message = asn_octets_to_utf8(basn, offset); break;
		case 2: this->timestamp = asn_octets_to_fixnum(basn, offset); break;
		case 3: this->topic = asn_octets_to_ia5(basn, offset); break;
		}
	}
};

/*************************************************************************************************/
extern "C" {
	__ffi__ uint16_t asn_tame_enum(uint16_t e, size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		Log log = _E(Log, e);

		asn_log_into_octets(log, basn);

		(*span) = asn_span(asn_log_span, log);
		(*offset) = 0;
		
		return _S(asn_octets_to_log(basn, offset));
	}

	__ffi__ bool asn_tame_sequence(uint16_t level, const char* desc, int64_t ts, const char* topic, size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		LogMessage* message = nullptr;
		LogMessage* restored = nullptr;
		bool okay = false;

		message = new LogMessage(_E(Log, level), desc, ts, topic);
		message->into_octets(basn);
		(*span) = asn_span(message);

		(*offset) = 0;
		restored = new LogMessage(basn, offset);
		okay = message->equals(restored);

		delete message;
		delete restored;

		return okay;
	}
}
