#include<iostream>

using std::cin;
using std::cout;

template<typename T>
class List {
private:
    class Node {
    public:
        T value;
        Node *right;

        explicit Node(const T &v = T()) {
            value = v;
            right = nullptr;
        }

        bool operator==(const Node &other) const {
            return value == other.value && right == other.right;
        }
    };

    // [first; last)
    Node *first;
    Node *last;
    int size;

public:
    class Iterator {
    private:
        Node *cur;

    public:
        typedef T value_type;

        explicit Iterator(Node *curr)
                : cur(curr) {}

        T &operator*() {
            return cur->value;
        }

        Iterator &operator++() {
            cur = cur->right;
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
            cur = cur->right;
            delete tmp;
        }
        delete last;
    }

    void push_front(const T &value) {
        auto *new_first = new Node(value);
        new_first->right = first;
        first = new_first;
        ++size;
    }

    Iterator begin() {
        return Iterator(first);
    }

    Iterator end() {
        return Iterator(last);
    }

    int get_size() const {
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
void advance(Iter &it, int delta) {
    while (delta != 0) {
        ++it;
        --delta;
    }
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
           Iter ans_first) {
    for (; a_first != a_last; ++ans_first) {
        if (b_first == b_last) {
            return copy(a_first, a_last, ans_first);
        }
        if (*b_first < *a_first) {
            *ans_first = *b_first;
            ++b_first;
        } else {
            *ans_first = *a_first;
            ++a_first;
        }
    }
    return copy(b_first, b_last, ans_first);
}

template<typename Iter, typename T = typename Iter::value_type>
void mergesort(Iter first, Iter last) {
    if (distance(first, last) > 1) {
        int m = distance(first, last) / 2;
        List<T> buf(distance(first, last));
        auto middle = first;
        advance(middle, m);
        mergesort(first, middle);
        mergesort(middle, last);
        merge(first, middle, middle, last, buf.begin());
        copy(buf.begin(), buf.end(), first);
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
    mergesort(l.begin(), l.end());
    for (auto elem : l) {
        cout << elem << " ";
    }
}

