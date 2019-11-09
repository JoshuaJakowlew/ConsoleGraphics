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
		[[nodiscard]] auto acquire(K&& key, V&& value) -> void;
		auto release(K&& key) -> void;
		[[nodiscard]] auto operator[](K&&) -> V&;
		[[nodiscard]] auto operator[](K&&) const -> const V&;
	private:
		std::unordered_map<K, V> m_storage;
	};

	template<typename K, typename V>
	inline auto cg::ResourceHolder<K, V>::acquire(K&& key, V&& value) -> void
	{
		m_storage.insert(std::make_pair(std::forward<K>(key), std::forward<V>(value)));
	}

	template<typename K, typename V>
	inline auto cg::ResourceHolder<K, V>::release(K&& key) -> void
	{
		m_storage.erase(std::forward<K>(key));
	}

	template <typename K, typename V>
	inline auto ResourceHolder<K, V>::operator[](K&& key) -> V&
	{
		const auto elem = m_storage.find(std::forward<K>(key));
		if (elem == m_storage.end())
			throw std::logic_error{ "K not found" };
		return elem->second;
	}

	template <typename K, typename V>
	inline auto ResourceHolder<K, V>::operator[](K&& key) const -> const V&
	{
		return this->operator[](std::forward<K>(key));
	}

} // namespace cg

#endif
