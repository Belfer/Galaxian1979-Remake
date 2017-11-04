#pragma once

#include <assert.h>
#include <memory>
#include <stdlib.h>
#include <unordered_map>
#include <utility>
#include <vector>

namespace NHTV {
struct IPool {
  virtual ~IPool() {}
  virtual void clear() = 0;
};

template <typename T> class Pool : public IPool {
public:
  Pool(int size = 100) { resize(size); }
  virtual ~Pool() {}

  bool empty() const;

  uint size() const;

  void resize(int n);

  void clear();

  size_t add(const T &object);

  void remove(const T &object);

  void remove(uint index);

  bool set(uint index, const T &object);

  T &get(uint index);

  T &operator[](uint index);

  const T &operator[](uint index) const;

  std::vector<std::pair<bool, T>> &data();

private:
  T &recycle(size_t &idx);

  std::vector<std::pair<bool, T>> m_data;
};

template <typename T> inline bool Pool<T>::empty() const {
  return m_data.empty();
}

template <typename T> inline uint Pool<T>::size() const {
  return m_data.size();
}

template <typename T> inline void Pool<T>::resize(int n) {
  const size_t dataSize = size();
  m_data.resize(n);
  if (size() > dataSize) {
    for (auto it = m_data.begin() + dataSize; it != m_data.end(); ++it) {
      it->first = false;
    }
  }
}

template <typename T> inline void Pool<T>::clear() { m_data.clear(); }

template <typename T> inline size_t Pool<T>::add(const T &object) {
  size_t idx;
  auto &obj = recycle(idx);
  obj = object;
  return idx;
}

template <typename T> inline void Pool<T>::remove(const T &object) {
  m_data.erase(std::remove(m_data.begin(), m_data.end(), object), m_data.end());
}

template <typename T> inline void Pool<T>::remove(uint index) {
  m_data.erase(m_data.begin() + index);
}

template <typename T> inline bool Pool<T>::set(uint index, const T &object) {
  assert(index < size());
  m_data[index].first = true;
  m_data[index].second = object;
  return true;
}

template <typename T> inline T &Pool<T>::get(uint index) {
  assert(index < size());
  return static_cast<T &>(m_data[index].second);
}

template <typename T> inline T &Pool<T>::recycle(size_t &idx) {
  idx = 0;
  for (auto &e : m_data) {
    if (!e.first) {
      e.first = true;
      return static_cast<T &>(e.second);
    }
    idx++;
  }

  resize(size() * 2);
  return recycle(idx);
}

template <typename T> inline T &Pool<T>::operator[](uint index) {
  return m_data[index].second;
}

template <typename T> inline const T &Pool<T>::operator[](uint index) const {
  return m_data[index].second;
}

template <typename T> inline std::vector<std::pair<bool, T>> &Pool<T>::data() {
  return m_data;
}
}
