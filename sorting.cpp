#include<iostream>

using namespace std;

class List {
private:
    class Node {
    public:
        int value;
        Node *right;

        Node(const int &v = int()) {
            value = v;
            right = nullptr;
        }

        bool operator==(const Node &other) const {
            return value == other.value && right == other.right;
        }
    };

public:
    class Iterator {
    private:
        Node *cur;

    public:
        Iterator(Node *curr) : cur(curr) {}

        int &operator*() {
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

    // [first; last)
    Node *first;
    Node *last;
    int Size;

    List(int size = 0) {
        first = new Node;
        last = first;
        Size = 0;
        while (size) {
            push_front(0);
            --size;
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

    void push_front(const int &value) {
        Node *new_first = new Node(value);
        new_first->right = first;
        first = new_first;
        ++Size;
    }

    Iterator begin() {
        return Iterator(first);
    }

    Iterator end() {
        return Iterator(last);
    }

    int size() const {
        return Size;
    }
};

int _distance(List::Iterator f, List::Iterator l) {
    int ans = 0;
    while (f != l) {
        ++f;
        ++ans;
    }
    return ans;
}

void _advance(List::Iterator &l, int d) {
    while (d) {
        ++l;
        --d;
    }
}

List::Iterator _copy(List::Iterator f, List::Iterator l, List::Iterator out) {
    while (f != l) {
        *out = *f;
        ++out;
        ++f;
    }
    return out;
}

List::Iterator _merge(List::Iterator first1, List::Iterator last1,
                      List::Iterator first2, List::Iterator last2,
                      List::Iterator d_first) {
    for (; first1 != last1; ++d_first) {
        if (first2 == last2) {
            return _copy(first1, last1, d_first);
        }
        if (*first2 < *first1) {
            *d_first = *first2;
            ++first2;
        } else {
            *d_first = *first1;
            ++first1;
        }
    }
    return _copy(first2, last2, d_first);
}

void mergesort(List::Iterator first, List::Iterator last) {
    if (_distance(first, last) > 1) {
        int m = _distance(first, last) / 2;
        List buf(_distance(first, last));
        auto it = first;
        _advance(it, m);
        mergesort(first, it);
        mergesort(it, last);
        _merge(first, it, it, last, buf.begin());
        _copy(buf.begin(), buf.end(), first);
    }
}

int main() {
    int n;
    cin >> n;
    List L;
    for (int i = 0; i < n; ++i) {
        int elem;
        cin >> elem;
        L.push_front(elem);
    }
    mergesort(L.begin(), L.end());
    for (auto elem : L) {
        cout << elem << " ";
    }
}
