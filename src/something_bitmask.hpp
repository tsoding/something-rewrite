#ifndef SOMETHING_BITMASK_HPP_
#define SOMETHING_BITMASK_HPP_

template <typename Mask, typename Bits = uint32_t>
struct Bitmask {
    Bits bits;

    Mask &operator|=(const Mask &that)
    {
        this->bits |= that.bits;
        return *static_cast<Mask*>(this);
    }

    Mask &operator&=(const Mask &that)
    {
        this->bits &= that.bits;
        return *static_cast<Mask*>(this);
    }

    Mask operator|(const Mask &that) const
    {
        return Mask { this->bits | that.bits };
    }

    Mask operator&(const Mask &that) const
    {
        return Mask { this->bits & that.bits };
    }

    operator bool() const
    {
        return bits;
    }
};

#endif // SOMETHING_BITMASK_HPP_
