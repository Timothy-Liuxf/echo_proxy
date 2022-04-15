#ifndef CONCURRENCY_UNORDERED_MAP_HPP_
#define CONCURRENCY_UNORDERED_MAP_HPP_

#include <mutex>
#include <unordered_map>
#include <utility>

namespace echo_proxy
{
namespace concurrency
{

template <typename Key, typename T, typename Hash = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = std::allocator<std::pair<const Key, T> > >
class unordered_map
{
private:
  using internal_type = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

public:
  using key_type = typename internal_type::key_type;
  using mapped_type = typename internal_type::mapped_type;
  using value_type = typename internal_type::value_type;
  using size_type = typename internal_type::size_type;
  using difference_type = typename internal_type::difference_type;
  using hasher = typename internal_type::hasher;
  using key_equal = typename internal_type::key_equal;
  using allocator_type = typename internal_type::allocator_type;

  unordered_map () = default;
  unordered_map (const unordered_map &) = delete;
  unordered_map &operator= (const unordered_map) = delete;

  bool
  try_get (const key_type &key, mapped_type &value) const
  {
    std::unique_lock<std::mutex> lock{ m_mtx };
    auto itr = m_map.find (key);
    if (itr != m_map.end ())
      {
        value = itr->second;
        return true;
      }
    return false;
  }

  bool
  try_remove (const key_type &key)
  {
    std::unique_lock<std::mutex> lock{ m_mtx };
    auto itr = m_map.find (key);
    if (itr != m_map.end ())
      {
        m_map.erase (itr);
        return true;
      }
    return false;
  }

  bool
  try_pop (const key_type &key, mapped_type &value)
  {
    std::unique_lock<std::mutex> lock{ m_mtx };
    auto itr = m_map.find (key);
    if (itr != m_map.end ())
      {
        value = std::move (itr->second);
        m_map.erase (itr);
        return true;
      }
    return false;
  }

  bool
  contains (const key_type &key) const
  {
    std::unique_lock<std::mutex> lock{ m_mtx };
    auto itr = m_map.find (key);
    return itr != m_map.end ();
  }

  template <typename... Ts>
  void
  emplace (Ts &&...args)
  {
    std::unique_lock<std::mutex> lock{ m_mtx };
    m_map.emplace (std::forward<Ts> (args)...);
  }

private:
  internal_type m_map;
  mutable std::mutex m_mtx;
};
}

}

#endif // !CONCURRENCY_UNORDERED_MAP_HPP_
