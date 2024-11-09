#pragma once

#include <cstdint>
namespace Leaper
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;
        inline uint64_t& GetBase()
        {
            return m_uuid;
        }

        operator uint64_t() const
        {
            return m_uuid;
        }
        operator uintptr_t()
        {
            return (uintptr_t)m_uuid;
        }

    private:
        uint64_t m_uuid;
    };

}  // namespace Leaper

namespace std
{
    template <typename T> struct hash;

    template <> struct hash<Leaper::UUID>
    {
        std::size_t operator()(const Leaper::UUID& uuid) const
        {
            return (uint64_t)uuid;
        }
    };

}  // namespace std