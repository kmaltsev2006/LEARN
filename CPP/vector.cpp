#include <iostream>

template <typename T>
class vector // C++03
{
private:
	
	template<bool is_const>
	class BaseIterator
	{
	friend vector;
	
	friend inline bool operator==(
		const BaseIterator& lhs, 
		const BaseIterator& rhs) { return lhs.ptr == rhs.ptr; }

	friend inline bool operator!=(
		const BaseIterator& lhs, 
		const BaseIterator& rhs) { return lhs.ptr != rhs.ptr; }
	
	public:
		using ptr_T = std::conditional_t<is_const, const T*, T*>;
		using ref_T = std::conditional_t<is_const, const T&, T&>;

	public:
		BaseIterator(const BaseIterator& other) = default;
		BaseIterator& operator=(const BaseIterator& other) = default;
		
		ptr_T operator->() const { return ptr; }
		ref_T operator*() const { return *ptr; }
		
		BaseIterator& operator++()
		{ ++ptr; return *this; }

		BaseIterator operator++(int)
		{ BaseIterator copy = *this; ++this; return copy; }
		
		BaseIterator& operator--()
		{ --ptr; return *this; }

		BaseIterator operator--(int)
		{ BaseIterator copy = *this; --this; return copy; }

	private:
		BaseIterator(T* ptr) : ptr(ptr) {}
		ptr_T ptr;
	};

public:
	using Iterator = BaseIterator<false /*is_const*/>;
	using ConstIterator = BaseIterator<true /*is_const*/>;
	// using ReverseIterator = std::reverse_iterator<Iterator>; // methods are not implemented for this
	// using ReverseConstIterator = std::reverse_iterator<ConstIterator>; // methods are not implemented for this

public:

	vector() = default;

	~vector()
	{
		for (size_t i = 0; i < _sz; ++i)
		{
			_v[i].~T();
		}
		delete[] reinterpret_cast<char*>(_v);
	}

	// non-copyable by now
	vector(const vector<T>& other) = delete;
	vector& operator=(const vector<T>& other) = delete;
	
	Iterator begin() { return _v; }
	Iterator end() { return  _v + _sz; }
	ConstIterator begin() const { return _v; }
	ConstIterator end() const { return _v + _sz; }
	ConstIterator cbegin() const { return _v; }
	ConstIterator cend() const { return _v + _sz; }
	
	void push_back(const T& val)
	{
		// No safety guarantee!
		// Reserve method must be copy-pasted inside,
		// with the addition of a new placement for val under a try-catch

		if (_sz == _cap)
		{
			reserve(_sz == 0 ? 1 : _cap * 2);
		}
		new(_v + _sz++) T(val); // no safety guarantee here
	}
	
	void reserve(size_t new_cap)
	{
		if (new_cap < _cap)
			return;

		T *new_v = reinterpret_cast<T*>(new char[sizeof(T) * new_cap]);
		
		size_t i = 0;
		
		try // strong safety (T ctor can throw)
		{
			for (; i < _sz; ++i)
				new(new_v + i) T(_v[i]); // placement new
		}
		catch(...)
		{
			for (size_t j = 0; j < i; ++j)
				new_v[j].~T();
			delete[] reinterpret_cast<char*>(new_v);
			throw;
		}

		for (size_t i = 0; i < _sz; ++i)
			_v[i].~T();
		delete[] reinterpret_cast<char*>(_v);

		_v = new_v;
		_cap = new_cap;
	}

private:
	T *_v 		 = nullptr;
	size_t _sz   = 0;
	size_t _cap  = 0;
};


template<typename T>
void foo(T) = delete; // get type

int main()
{
	vector<int> v;

	for (int i = 0; i < 10; ++i)
	{
		v.push_back(i);		
	}
	
	for (auto& x : v)
	{
		std::cout << x << std::endl;
	}
}