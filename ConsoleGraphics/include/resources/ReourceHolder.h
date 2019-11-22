#ifndef RESOURCE_HOLDER_H
#define RESOURCE_HOLDER_H

#include <unordered_map>
#include <stdexcept>
#include <utility>

namespace cg
{
	template <typename K, typename V>
	class ResourceHolder
	{
	public:
		[[nodiscard]] auto acquire(const K& key, const V& value) -> void;
		auto release(const K& key) -> void;
		[[nodiscard]] auto operator[](const K&) -> V&;
		[[nodiscard]] auto operator[](const K&) const -> const V&;
	private:
		std::unordered_map<K, V> m_storage;
	};

	template<typename K, typename V>
	inline auto cg::ResourceHolder<K, V>::acquire(const K& key, const V& value) -> void
	{
		m_storage.insert(std::make_pair(key, value));
	}

	template<typename K, typename V>
	inline auto cg::ResourceHolder<K, V>::release(const K& key) -> void
	{
		m_storage.erase(key);
	}

	template <typename K, typename V>
	inline auto ResourceHolder<K, V>::operator[](const K& key) -> V&
	{
		const auto elem = m_storage.find(key);
		if (elem == m_storage.end())
			throw std::logic_error{ "K not found" };
		return elem->second;
	}

	template <typename K, typename V>
	inline auto ResourceHolder<K, V>::operator[](const K& key) const -> const V&
	{
		return this->operator[](key);
	}

} // namespace cg

#endif
