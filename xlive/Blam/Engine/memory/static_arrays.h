#pragma once

template<typename t_type, typename t_storage_type, size_t k_count>
class c_flags_no_init 
{
	static_assert(std::is_unsigned_v<t_storage_type>, "error: t_storage_type is not an unsigned integral type");

public:
	const t_type get_unsafe(void) const
	{
		return m_storage;
	}

	bool valid_bit(t_type bit) const
	{
		return VALID_INDEX(bit, k_count);
	}

	// Checks if current value set for the bitflag is valud
	bool valid(void) const
	{
		return !TEST_FLAG(m_storage, ~(FLAG(k_count) - 1));
	}

	void clear(void)
	{
		m_storage = 0;
		return;
	}

	void set(t_type bit, bool enable)
	{
		ASSERT(valid_bit(bit));

		if (bit < k_count)
		{
			if (enable)
			{
				m_storage |= FLAG(bit);
			}
			else
			{
				m_storage &= ~FLAG(bit);
			}
		}
		return;
	}

	// Set the bitflag value manually
	void set_unsafe(t_storage_type raw_bits)
	{
		m_storage = raw_bits;
		ASSERT(valid());
		return;
	}

	bool test(t_type bit) const
	{
		return TEST_BIT(m_storage, (t_storage_type)(bit));
	}

	// Untested
	bool test_range(t_type start_bit, t_type end_bit) const
	{
		ASSERT(valid_bit(start_bit) && valid_bit(end_bit) && (start_bit <= end_bit));

		t_type bit = (FLAG(end_bit - start_bit + 1) - 1) << start_bit;
		return TEST_BIT(m_storage, bit);
	}

	c_flags_no_init<t_type, t_storage_type, k_count> operator~(void) const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> result;
		result.set_unsafe(~m_storage);
		return result;
	}

	c_flags_no_init<t_type, t_storage_type, k_count> operator&(c_flags_no_init<t_type, t_storage_type, k_count>& value) const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> result;
		result.set_unsafe(m_storage & value.m_storage);
		return result;
	}

	void operator|=(c_flags_no_init<t_type, t_storage_type, k_count>& value)
	{
		m_storage |= value.m_storage;
		ASSERT(valid());
		return;
	}

	bool operator!=(c_flags_no_init<t_type, t_storage_type, k_count>& value) const
	{
		return m_storage != value.m_storage;
	}

	bool operator==(c_flags_no_init<t_type, t_storage_type, k_count>& value) const
	{
		return m_storage == value.m_storage;
	}

protected:
	t_storage_type m_storage : k_count;
};

template<typename t_type, typename t_storage_type, size_t k_count>
class c_flags : public c_flags_no_init<t_type, t_storage_type, k_count>
{
public:
	c_flags(void) 
	{ 
		clear();
		return; 
	}

	c_flags(t_storage_type raw_bits)  
	{
		set_unsafe(raw_bits);
		return; 
	}
};

template<size_t k_maximum_bit_count>
class c_static_flags_no_init
{
public:
	void clear(void)
	{
		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_bit_count));
		return;
	}

	void clear_range(int32 count)
	{
		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(count));
		return;
	}

	void fill(int32 count, uint8 value)
	{
		csmemset(m_flags, value, BIT_VECTOR_SIZE_IN_BYTES(count));
		return;
	}

	const int32* get_bits_direct(void) const
	{
		return m_flags;
	}

	int32* get_writeable_bits_direct(void)
	{
		return m_flags;
	}

	void set(int32 index, bool enable)
	{
		BIT_VECTOR_SET_FLAG(m_flags, index, enable);
		return;
	}

	void set_bits_direct_destructive(int32 count, uint8 value)
	{
		csmemcpy(m_flags, value, BIT_VECTOR_SIZE_IN_BYTES(count));
		return;
	}

	bool test(int32 index)
	{
		return BIT_VECTOR_TEST_FLAG(m_flags, index);
	}

protected:
	int32 m_flags[k_maximum_bit_count / LONG_BITS];
};

template<size_t k_maximum_bit_count>
class c_static_flags : public c_static_flags_no_init<k_maximum_bit_count>
{
public:
	c_static_flags(void) = default;
	c_static_flags(int32 count, uint8 value)
	{
		fill(count, value);
		return;
	}
	~c_static_flags(void) = default;
};
