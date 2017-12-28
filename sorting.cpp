#include<iostream>

using std::cin;
using std::cout;

// single linked list

template<typename T>
class List {
private:
    class Node {
    public:
        T value;
        Node *next;

        explicit Node(const T &v = T()) {
            value = v;
            next = nullptr;
        }

        bool operator==(const Node &other) const {
            return value == other.value && next == other.next;
        }
    };

    // [first; last)
    Node *first;
    Node *last;
    size_t size;

public:
    class Iterator {
    private:
        Node *cur;

    public:
        typedef List<T> list_type;

        explicit Iterator(Node *curr)
                : cur(curr) {}

        T &operator*() {
            return cur->value;
        }

        Iterator &operator++() {
            cur = cur->next;
            return *this;
        }

        bool operator==(const Iterator &other) const {
            return (*cur) == *(other.cur);
        }

        bool operator!=(const Iterator &other) const {
            return !(*this == other);
        }
    };

    explicit List(size_t list_size = 0) {
        first = new Node;
        last = first;
        size = list_size;
        while (list_size) {
            push_front(0);
            --list_size;
        }
    }

    ~List() {
        Node *cur = first;
        while (cur != last) {
            auto tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        delete last;
    }

    void push_front(const T &value) {
        auto *new_first = new Node(value);
        new_first->next = first;
        first = new_first;
        ++size;
    }

    void pop_front() {
        Node *old_first = first;
        first = first->next;
        delete old_first;
        --size;
    }

    Iterator begin() {
        return Iterator(first);
    }

    Iterator end() {
        return Iterator(last);
    }

    size_t get_size() const {
        return size;
    }
};

template<typename Iter>
size_t distance(Iter first, Iter last) {
    size_t ans = 0;
    while (first != last) {
        ++first;
        ++ans;
    }
    return ans;
}

template<typename Iter>
Iter advance_forward(Iter it, size_t delta) {
    while (delta != 0) {
        ++it;
        --delta;
    }
    return it;
}

template<typename Iter>
Iter copy(Iter first, Iter last, Iter out) {
    while (first != last) {
        *out = *first;
        ++out;
        ++first;
    }
    return out;
}

template<typename Iter>
Iter merge(Iter a_first, Iter a_last,
           Iter b_first, Iter b_last,
           Iter out) {
    while(a_first != a_last && b_first != b_last) {
        if(*a_first < *b_first) {
            *out = *a_first;
            ++a_first;
        }
        else {
            *out = *b_first;
            ++b_first;
        }
        ++out;
    }
    out = copy(a_first, a_last, out);
    out = copy(b_first, b_last, out);
    return out;
}

template<typename Iter, typename LList = typename Iter::list_type>
void mergesort(Iter first, Iter last, LList &buf) {
    size_t dist = distance(first, last);
    if (dist > 1) {
        auto middle = advance_forward(first, dist / 2);
        mergesort(first, middle, buf);
        mergesort(middle, last, buf);
        Iter buf_end = merge(first, middle, middle, last, buf.begin());
        copy(buf.begin(), buf_end, first);
    }
}

int main() {
    size_t n = 0;
    cin >> n;
    List<int> l;
    for (size_t i = 0; i < n; ++i) {
        int elem = 0;
        cin >> elem;
        l.push_front(elem);
    }
    List<int> buf(n);
    mergesort(l.begin(), l.end(), buf);
    for (auto elem : l) {
        cout << elem << " ";
    }
}
