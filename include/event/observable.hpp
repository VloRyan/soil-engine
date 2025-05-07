#ifndef ENGINE_EVENT_OBSERVABLE_H
#define ENGINE_EVENT_OBSERVABLE_H
#include <event/handler.hpp>
#include <exception.h>
#include <vector>

namespace soil::event {
    template <class T, std::enable_if_t<std::is_base_of_v<Event, T> > * = nullptr>
    class Observable {
    public:
        Observable() = default;

        virtual ~Observable() = default;

        virtual void AddListener(Handler<T> *listener) { listeners_.push_back(listener); }

        virtual void RemoveListener(Handler<T> *listener) {
            for (auto itr = listeners_.begin(); itr < listeners_.end(); ++itr) {
                if (listener == *itr) {
                    listeners_.erase(itr);
                    return;
                }
            }
            throw Exception("RemoveListener failed");
        }

    protected:
        virtual void fire(const T &event) {
            for (Handler<T> *listener : listeners_) {
                listener->Handle(event);
            }
        }

    private:
        std::vector<Handler<T> *> listeners_;
    };
} // namespace soil::event
#endif /*  ENGINE_EVENT_OBSERVABLE_H */
