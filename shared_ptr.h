#pragma once

template<typename T>
class SharedCount
{
private:
    T* m_ptr;
    int m_count;

    SharedCount(const SharedCount&) = delete;
    SharedCount& operator=(const SharedCount&) = delete;
public:
    SharedCount(T* p) : m_ptr(p), m_count(1) {}
    ~SharedCount() { delete m_ptr; }
    void increment() { m_count++; };
    void decrement()
    {
        if (0 == --m_count)
        {
            delete this;
        }
    };
    T* get() { return m_ptr; }
};

template<typename T>
class shared_ptr
{
private:
    T* m_ptr;
    SharedCount<T>* m_count;
public:
    shared_ptr(T* p = nullptr) : m_ptr(p)
    {
        if (p)
        {
            m_count = new SharedCount<T>(p);
        }
        else
        {
            m_ptr = nullptr;
        }
    }
    shared_ptr(const shared_ptr& rhs) : m_ptr(rhs.m_ptr), m_count(rhs.m_count)
    {
        if (m_count)
        {
            m_count->increment();
        }
    }
    shared_ptr(const shared_ptr&& rhs) : m_ptr(rhs.m_ptr), m_count(rhs.m_count)
    {
        rhs.m_ptr = nullptr;
        rhs.m_count = nullptr;
    }
    ~shared_ptr()
    {
        if (m_count)
        {
            m_count->decrement();
        }
    };
    T* get() { return m_ptr; }
    T* operator->() const { return m_ptr; }
    T& operator*() const { return *m_ptr; }
    void reset(T* p)
    {
        if (p == m_ptr) return;

        if (m_count)
        {
            m_count->decrement();
        }

        m_ptr = p;
        if (p)
        {
            m_count = new SharedCount<T>(p);
        }
        else
        {
            m_count = nullptr;
        }
    }
};
