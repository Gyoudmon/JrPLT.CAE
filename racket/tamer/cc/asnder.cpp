
#include "../../digitama/big_bang/wormhole/jargon/asn_der.hpp"

#include "../../digitama/big_bang/datum/fixnum.hpp"
#include "../../digitama/big_bang/datum/flonum.hpp"
#include "../../digitama/big_bang/datum/natural.hpp"
#include "../../digitama/big_bang/datum/enum.hpp"

using namespace WarGrey::STEM;

/*************************************************************************************************/
define_asn_enum(log, Log, Debug, Info, Notice, Warning, Error, Critical, Alarm, Panic, _ );

/*
static size_t asn_utf_8_span(std::wstring& str) {
	return asn_utf8_span(str);
}

static octets asn_utf_8_to_octets(std::wstring& str) {
	return asn_utf8_to_octets(str);
}

static std::wstring asn_bytes_to_utf8(octets& bint, size_t* offset = nullptr) {
	return asn_octets_to_utf8(bint, offset);
}

static int64_t asn_bytes_to_fixnum(octets& bint, size_t* offset = nullptr) {
	return asn_octets_to_fixnum(bint, offset);
}

static bool asn_bytes_to_boolean(octets& bint, size_t* offset = nullptr) {
	return asn_octets_to_boolean(bint, offset);
}

static std::string asn_bytes_to_ia5(octets& bint, size_t* offset = nullptr) {
	return asn_octets_to_ia5(bint, offset);
}

static Log asn_bytes_to_log(octets& bint, size_t* offset = nullptr) {
	return asn_octets_to_log(bint, offset);
}
*/

struct LogMessage : public IASNSequence {
public:
	LogMessage(Log level, const char* message, int64_t timestamp, const char* topic)
		: IASNSequence(4), level(level), message(message), timestamp(timestamp), topic(topic) {}

	LogMessage(const octets& basn, size_t* offset = nullptr) : IASNSequence(4) {
		this->from_octets(basn, offset);
	}

public:
	Log level;
	std::string message;
	int64_t timestamp;
	std::string topic;

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

	__ffi__ const uint8_t* asn_tame_natural(const char* nstr, size_t* span, size_t* offset, uint8_t* basn, size_t bsize) {
		static Natural nat;

		Natural n(16U, std::string(nstr));
		asn_natural_into_octets(n, basn);
		
		(*span) = asn_span(asn_natural_span, n);
		(*offset) = 0;
		nat = asn_octets_to_natural(basn, offset);

		return nat.to_bytes().c_str();
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
}

	/*********************************************************************************************/
/*
	static bool bytes_eq(const char* b10, const uint8* b2, size_t size, Platform::String^ message) {
		uint8* b1 = (uint8*)b10;
		bool eq = true;

		for (size_t i = 0; i < size; i++) {
			if (b1[i] != b2[i]) {
				eq = false;
				break;
			}
		}

		if (!eq) {
			syslog(::Log::Error, L"BytesEq: %s:", message->Data());

			for (size_t i = 0; i < size; i++) {
				syslog(::Log::Error, L"  [%02d] exptected: %02X, given: %02X", i, b1[i], b2[i]);
			}
		}

		return eq;
	}

	static void assert(octets& n, const char* control, Platform::String^ message) {
		Assert::IsTrue(bytes_eq(control, n.c_str(), n.size(), message), message->Data());
	}
*/

	/*********************************************************************************************/
/*
	private class DERPrimitive : public TestClass<DERPrimitive> {
	public:
		TEST_METHOD(Natural) {
			test_natural("807fbc", "paded zero is an embedded null");
			test_natural("7fbc8ce9af7a9eb54c817fc7c1c796d1b1c80bddbcbacb15942480f5aa4ee120d27f93ebcf43275d01", "17^80");
		}

		TEST_METHOD(Enumerated) {
			const wchar_t* msgfmt = L"Enumerated Log [%s]";

			test_enum(Log::Debug,  asn_log_to_octets, asn_bytes_to_log, "\x0A\x01\x00", msgfmt);
			test_enum(Log::Error, asn_log_to_octets, asn_bytes_to_log, "\x0A\x01\x04", msgfmt);
		}

		TEST_METHOD(String) {
			test_string(std::string("6.0.5361.2"), asn_ia5_span, asn_ia5_to_octets, asn_bytes_to_ia5,
				"\x16\x0A\x36\x2E\x30\x2E\x35\x33\x36\x31\x2E\x32", L"String IA5[%S]");

			// WARNING: std::string can contain embedded '\0',  but be careful when making it with char-array literal.
			test_string(std::wstring(L"λsh\x0\nssh", 8), asn_utf_8_span, asn_utf_8_to_octets, asn_bytes_to_utf8,
				"\x0C\x09\xCE\xBB\x73\x68\x00\x0A\x73\x73\x68", L"String UTF8[%s]");
		}

	private:
		void test_natural(Platform::String^ representation, const char* readable_name) {
			Platform::String^ message = make_wstring(L"Natural[%S]", readable_name);
			::Natural nat((uint8)16U, (const uint16*)representation->Data(), 0, representation->Length());
			octets bnat = asn_natural_to_octets(nat);
			::Natural restored = asn_octets_to_natural(bnat);
			
			Assert::IsTrue(nat == restored, message->Data());
			Assert::AreEqual(bnat.size(), asn_span(asn_natural_span, nat), message->Data());

			asn_natural_into_octets(nat, (uint8*)bnat.c_str(), 0);
			Assert::IsTrue(nat == asn_octets_to_natural(bnat), message->Data());
		}

		template<typename T, typename T2O, typename O2T>
		void test_enum(T datum, T2O asn_to_octets, O2T octets_to_asn, const char* representation, const wchar_t* msgfmt) {
			Platform::String^ message = make_wstring(msgfmt, datum.ToString()->Data());
			octets basn = asn_to_octets(datum);

			Assert::IsTrue(bytes_eq(representation, basn.c_str(), basn.size(), message), message->Data());

			{ // decode
				T restored = octets_to_asn(basn, nullptr);

				Assert::IsTrue(datum == restored, message->Data());
			}
		}

		template<typename T, typename Span, typename T2O, typename O2T> 
		void test_string(T& datum, Span span, T2O asn_to_octets, O2T octets_to_asn, const char* representation, const wchar_t* msgfmt) {
			Platform::String^ message = make_wstring(msgfmt, datum.c_str());
			octets basn = asn_to_octets(datum);

			Assert::IsTrue(bytes_eq(representation, basn.c_str(), basn.size(), message), message->Data());
			Assert::AreEqual(basn.size(), asn_span(span, datum), message->Data());

			{ // decode
				T restored = octets_to_asn(basn, nullptr);

				Assert::AreEqual(datum.c_str(), restored.c_str(), message->Data());
			}
		}
	};

	private class DERSequence : public TestClass<DERSequence> {
	public:
		TEST_METHOD(PlainSequence) {
			LogMessage log_msg(Log::Debug, L"测试", 1585280242148LL, "tamer");

			test_sequence(log_msg, "\x30\x1a\x0a\x01\x00\x0c\x06\xe6\xb5\x8b\xe8\xaf\x95\x02\x06\x01\x71\x1a\x10\xd1\xe4\x16\x05\x74\x61\x6d\x65\x72");
		}

	private:
		void test_sequence(LogMessage& m, const char* representation) {
			Platform::String^ name = make_wstring(m.topic.c_str());
			octets basn = m.to_octets();
			
			Assert::IsTrue(bytes_eq(representation, basn.c_str(), basn.size(), name), name->Data());
			Assert::AreEqual(asn_span(&m), basn.size(), make_wstring(L"%s span", name->Data())->Data());

			{ // decode
				size_t offset = 0;
				LogMessage restored(basn, &offset);

				Assert::IsTrue(m.level == restored.level, make_wstring(L"%s topic", name->Data())->Data());
				Assert::IsTrue(m.message->Equals(restored.message), make_wstring(L"%s message", name->Data())->Data());
				Assert::AreEqual(m.timestamp, restored.timestamp, make_wstring(L"%s timestamp", name->Data())->Data());
				Assert::AreEqual(m.topic.c_str(), restored.topic.c_str(), make_wstring(L"%s topic", name->Data())->Data());
			}
		}
	};
}
*/
