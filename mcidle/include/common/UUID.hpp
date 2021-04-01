#pragma once

#include <common/Typedef.hpp>

#include <iosfwd>
#include <string>

// Source: mclib

namespace mcidle {
namespace lib {

    class DataBuffer;

    class UUID
    {
    public:
        UUID()
        noexcept
            : m_MostSigBits(0)
            , m_LeastSigBits(0)
        {
        }
        UUID(u64 most, u64 least)
        noexcept
            : m_MostSigBits(most)
            , m_LeastSigBits(least)
        {
        }

        std::string ToString(bool dashes = true) const;

        u64 GetUpperBits() const noexcept
        {
            return m_MostSigBits;
        }
        u64 GetLowerBits() const noexcept
        {
            return m_LeastSigBits;
        }

        static UUID FromString(const std::string &str, bool dashes = true);
        static UUID FromString(const std::wstring &str, bool dashes = true);

        bool operator<(const UUID &r) const
        {
            const UUID &l = *this;
            if (l.GetUpperBits() < r.GetUpperBits())
                return true;
            if (l.GetUpperBits() > r.GetUpperBits())
                return false;
            if (l.GetLowerBits() < r.GetLowerBits())
                return true;
            if (l.GetLowerBits() > r.GetLowerBits())
                return false;
            return false;
        }

    private:
        u64 m_MostSigBits;
        u64 m_LeastSigBits;
    };

    DataBuffer &operator<<(DataBuffer &out, const UUID &uuid);
    DataBuffer &operator>>(DataBuffer &in, UUID &uuid);

    std::ostream &operator<<(std::ostream &out, const UUID &uuid);
    std::wostream &operator<<(std::wostream &out, const UUID &uuid);

}  // namespace lib
}  // namespace mcidle
