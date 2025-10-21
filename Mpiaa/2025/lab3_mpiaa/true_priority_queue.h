#include "header.h"

template<typename T,
    typename Container = std::vector<T>,
    typename Compare = std::greater<T>>
    class TruePriorityQueue {
    private:
        struct TimestampedElement {
            T value;
            size_t timestamp;

            bool operator>(const TimestampedElement& other) const {
                Compare comp;
                if (comp(value, other.value)) return true;
                if (comp(other.value, value)) return false;
                return timestamp > other.timestamp;
            }
        };

        std::priority_queue<TimestampedElement,
            std::vector<TimestampedElement>,
            std::greater<TimestampedElement>> pq;

        size_t counter;

    public:
        TruePriorityQueue() : counter(0) {}

        void push(const T& value) {
            pq.push({ value, counter++ });
        }

        void pop() {
            pq.pop();
        }

        const T& top() const {
            return pq.top().value;
        }

        bool empty() const {
            return pq.empty();
        }

        size_t size() const {
            return pq.size();
        }
};
