#ifndef ENGINE_ENGINE_UTIL_POINTER_CACHE_HPP_
#define ENGINE_ENGINE_UTIL_POINTER_CACHE_HPP_

#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include "base.h"

namespace soil::util {
    template <class Key, class Tp>
    class PointerCache final {
    public:
        struct CacheEntry {
            std::weak_ptr<Tp> weakPtr;
        };

        explicit PointerCache(const std::function<void(Tp *)> removeHook = nullptr) : PointerCache(false, removeHook) {}

        explicit PointerCache(const bool holdPointer, const std::function<void(Tp *)> removeHook = nullptr) :
            holdPointer_(holdPointer), removeHook_(removeHook) {}

        ~PointerCache() {
            holdCache_.clear();
            clear();
        }

        std::shared_ptr<Tp> get(Key key) {
            std::unique_lock lock(mutex_);
            _prune();
            auto itr = cache_.find(key);
            if (itr != cache_.end()) {
                if (itr->second.expired()) {
                    cache_.erase(itr);
                } else {
                    return itr->second.lock();
                }
            }
            return nullptr;
        }

        std::shared_ptr<Tp> cache(Key key, Tp *value) {
            std::unique_lock lock(mutex_);
            _prune();
            // std::shared_ptr<_Tp> sharedPtr(value, std::bind(&PointerCache::removeValue, this,
            // std::placeholders::_1));
            std::shared_ptr<Tp> sharedPtr;
            if (removeHook_) {
                sharedPtr = std::shared_ptr<Tp>(value, removeHook_);
            } else {
                sharedPtr = std::shared_ptr<Tp>(value);
            }
            /*Cache Model*/
            std::weak_ptr<Tp> weakPtr(sharedPtr);
            auto itr = cache_.find(key);
            if (itr != cache_.end()) {
                /*Replace value*/
                cache_[key] = weakPtr;
            }
            std::pair<Key, std::weak_ptr<Tp> > pair(key, weakPtr);

            if (auto insertResult = cache_.insert(pair); !insertResult.second) {
                throw std::runtime_error("Insert failed");
            }
            if (holdPointer_) {
                holdCache_.push_back(sharedPtr);
            }
            return sharedPtr;
        }

        void remove(Key key) {
            std::unique_lock lock(mutex_);
            _prune();
            if (holdPointer_) {
                auto itrCache = cache_.find(key);
                std::weak_ptr<Tp> weakPtr;
                if (itrCache != cache_.end()) {
                    weakPtr = itrCache->second;
                }
                if (!weakPtr) {
                    throw std::runtime_error("Remove failed");
                }
                for (auto itrHoldCache = holdCache_.begin(); itrHoldCache != holdCache_.end(); ++itrHoldCache) {
                    if (std::shared_ptr<Tp> holdPointer = *itrHoldCache; holdPointer.get() == weakPtr.lock().get()) {
                        holdCache_.erase(itrHoldCache);
                        break;
                    }
                }
                cache_.erase(itrCache);
                return;
            }
            if (const size_t numErased = cache_.erase(key); numErased != 1) {
                throw std::runtime_error("Remove failed");
            }
        }

        void clear() {
            std::unique_lock lock(mutex_);
            cache_.clear();
        }

        void prune() {
            std::unique_lock lock(mutex_);
            _prune();
        }

        typename HashMap<Key, std::weak_ptr<Tp> >::size_type size() const {
            return cache_.size();
        }

    protected:
        typename HashMap<Key, std::weak_ptr<Tp> >::iterator begin() {
            return cache_.begin();
        }

        typename HashMap<Key, std::weak_ptr<Tp> >::iterator end() {
            return cache_.end();
        }

    private:
        void _prune() {
            uint numErased = 0;
            auto itr = cache_.begin();
            while (itr != cache_.end()) {
                if (itr->second.expired()) {
                    itr = cache_.erase(itr);
                    ++numErased;
                } else {
                    ++itr;
                }
            }
        }

        void removeValue(Tp *value) {
            std::unique_lock lock(mutex_);
            /*Call Hook*/
            if (removeHook_) {
                removeHook_(value);
            }
            /*Remove from cache*/
            size_t numErased = 0;
            auto itr = cache_.begin();
            while (itr != cache_.end()) {
                if (itr->second.expired()) {
                    itr = cache_.erase(itr);
                    ++numErased;
#ifndef DEBUG
                    break;
#endif
                }
                if (itr->second.lock().get() == value) {
                    itr = cache_.erase(itr);
                    ++numErased;
                } else {
                    ++itr;
                }
            }
            if (numErased != 1) {
                throw std::runtime_error("Remove failed");
            }
            delete value;
        }

        HashMap<Key, std::weak_ptr<Tp> > cache_;
        std::list<std::shared_ptr<Tp> > holdCache_;
        bool holdPointer_;
        std::mutex mutex_;
        std::function<void(Tp *)> removeHook_;
    };

    template <typename T>
    using StringCache = PointerCache<std::string, T>;
    template <typename T>
    using IntegerCache = PointerCache<int, T>;
    template <typename T>
    using UIntegerCache = PointerCache<uint, T>;
} // namespace soil::util

#endif // ENGINE_ENGINE_UTIL_POINTER_CACHE_HPP_
