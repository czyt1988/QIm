#ifndef QIMTRACKEDVALUE_HPP
#define QIMTRACKEDVALUE_HPP
#include <type_traits>
#include <cmath>
#include <utility>
#include <algorithm>

namespace QIM
{

namespace detail
{
// C++17 兼容的 SFINAE 检测
template< typename T, typename = void >
struct has_not_equal : std::false_type
{
};

template< typename T >
struct has_not_equal< T, std::void_t< decltype(std::declval< T >() != std::declval< T >()) > > : std::true_type
{
};

// C++17 兼容的检测 == 操作符
template< typename T, typename = void >
struct has_equal : std::false_type
{
};

template< typename T >
struct has_equal< T, std::void_t< decltype(std::declval< T >() == std::declval< T >()) > > : std::true_type
{
};

// 检测是否有比较函数
template< typename T, typename Comp, typename = void >
struct is_comparable_with : std::false_type
{
};

template< typename T, typename Comp >
struct is_comparable_with< T, Comp, std::void_t< decltype(std::declval< Comp >()(std::declval< T >(), std::declval< T >())) > >
    : std::true_type
{
};
}

// 默认比较器
template< typename T >
class DefaultComparator
{
public:
    bool operator()(const T& a, const T& b) const
    {
        if constexpr (detail::has_not_equal< T >::value) {
            return a != b;
        } else if constexpr (detail::has_equal< T >::value) {
            return !(a == b);
        } else {
            // C++17 可以使用 std::is_pointer 等特性
            if constexpr (std::is_pointer< T >::value) {
                return a != b;
            } else if constexpr (std::is_arithmetic< T >::value) {
                return a != b;
            } else {
                // 最终回退：使用地址比较
                return &a != &b;
            }
        }
    }
};

// 浮点数模糊比较器
template< typename T >
class FloatComparator
{
    static_assert(std::is_floating_point< T >::value, "FloatComparator only works with floating point types");

public:
    explicit FloatComparator(T epsilon = static_cast< T >(1e-6)) : epsilon_(epsilon)
    {
    }

    bool operator()(const T& a, const T& b) const
    {
        // 使用快速路径
        if (a == b)
            return false;

        // 处理 NaN
        if (std::isnan(a) || std::isnan(b)) {
            // NaN 永远不等于任何值，包括自身
            return true;
        }

        // 处理无穷大
        if (std::isinf(a) || std::isinf(b)) {
            return a != b;
        }

        // 组合误差方法
        T diff = std::abs(a - b);
        if (diff <= epsilon_)
            return false;

        // 相对误差
        T max_val = std::max(std::abs(a), std::abs(b));
        return diff > (epsilon_ * max_val);
    }

    T epsilon() const
    {
        return epsilon_;
    }

private:
    T epsilon_;
};

// 特化浮点类型
template<>
class DefaultComparator< float > : public FloatComparator< float >
{
public:
    DefaultComparator() : FloatComparator< float >(1e-6f)
    {
    }
};

template<>
class DefaultComparator< double > : public FloatComparator< double >
{
public:
    DefaultComparator() : FloatComparator< double >(1e-6)
    {
    }
};

template<>
class DefaultComparator< long double > : public FloatComparator< long double >
{
public:
    DefaultComparator() : FloatComparator< long double >(1e-6L)
    {
    }
};

/**
 * @class QImTrackedValue
 * @brief 跟踪值变化的状态包装器
 * @tparam T 要包装的值类型
 *
 * 这个类包装一个值并跟踪其是否被修改过（脏标记）。当给值赋新值时，
 * 如果新值与旧值不同，会自动设置脏标记。提供了一系列方法来检查、
 * 清除和操作这个状态。
 *
 * @note 类型T必须支持不等于操作符（operator!=）进行比较
 *
 * @example 基本使用
 * @code
 * QImTrackedValue<int> count(0);
 *
 * count = 10;  // 值改变，标记为脏
 * if (count.is_dirty()) {
 *     std::cout << "Value changed!" << std::endl;
 *     count.clear();  // 清除脏标记
 * }
 *
 * count = 10;  // 值相同，不会标记为脏
 * std::cout << "Is dirty? " << count.is_dirty() << std::endl;  // 输出: 0 (false)
 * @endcode
 *
 * @example 配置管理
 * @code
 * class Settings {
 *     QImTrackedValue<std::string> username;
 *     QImTrackedValue<int> timeout;
 *     QImTrackedValue<bool> auto_save;
 *
 * public:
 *     bool has_unsaved_changes() const {
 *         return username.is_dirty() || timeout.is_dirty() || auto_save.is_dirty();
 *     }
 *
 *     void save() {
 *         // 保存到数据库或文件...
 *         username.clear();
 *         timeout.clear();
 *         auto_save.clear();
 *     }
 * };
 * @endcode
 *
 * @example 游戏状态跟踪
 * @code
 * class Player {
 *     QImTrackedValue<int> health{100};
 *     QImTrackedValue<int> score{0};
 *
 * public:
 *     void take_damage(int damage) {
 *         health = health.value() - damage;
 *         if (health.is_dirty()) {
 *             // 更新UI显示血条
 *             update_health_bar();
 *             health.clear();
 *         }
 *     }
 * };
 * @endcode
 */
template< typename T, typename Comparator = DefaultComparator< T > >
class QImTrackedValue
{
public:
    /**
     * @brief 默认构造函数
     * @details 使用T的默认构造函数初始化值，脏标记为false
     */
    QImTrackedValue() : value_(), is_dirty_(false), comparator_()
    {
    }
    /**
     * @brief 带比较器的构造函数
     */
    explicit QImTrackedValue(const Comparator& comp) : value_(), is_dirty_(false), comparator_(comp)
    {
    }

    /**
     * @brief 从值构造
     */
    explicit QImTrackedValue(const T& value, const Comparator& comp = Comparator())
        : value_(value), is_dirty_(false), comparator_(comp)
    {
    }

    /**
     * @brief 从右值构造
     */
    explicit QImTrackedValue(T&& value, const Comparator& comp = Comparator())
        : value_(std::move(value)), is_dirty_(false), comparator_(comp)
    {
    }

    /**
     * @brief 通过参数包构造
     */
    template< typename... Args >
    explicit QImTrackedValue(Args&&... args) : value_(std::forward< Args >(args)...), is_dirty_(false), comparator_()
    {
    }

    /**
     * @brief 通过参数包构造，带比较器
     */
    template< typename... Args >
    QImTrackedValue(const Comparator& comp, Args&&... args)
        : value_(std::forward< Args >(args)...), is_dirty_(false), comparator_(comp)
    {
    }

    // 拷贝构造函数
    QImTrackedValue(const QImTrackedValue& other)
        : value_(other.value_), is_dirty_(other.is_dirty_), comparator_(other.comparator_)
    {
    }

    // 移动构造函数
    QImTrackedValue(QImTrackedValue&& other) noexcept
        : value_(std::move(other.value_)), is_dirty_(other.is_dirty_), comparator_(std::move(other.comparator_))
    {
        other.is_dirty_ = false;
    }
    /**
     * @brief 拷贝赋值操作符
     * @param other 要拷贝的另一个QImTrackedValue对象
     * @return 当前对象的引用
     */
    QImTrackedValue& operator=(const QImTrackedValue& other)
    {
        if (this != &other) {
            set_value(other.value_);
            is_dirty_   = other.is_dirty_;
            comparator_ = other.comparator_;
        }
        return *this;
    }

    /**
     * @brief 移动赋值操作符
     * @param other 要移动的另一个QImTrackedValue对象
     * @return 当前对象的引用
     */
    QImTrackedValue& operator=(QImTrackedValue&& other) noexcept
    {
        if (this != &other) {
            set_value(std::move(other.value_));
            is_dirty_       = other.is_dirty_;
            comparator_     = std::move(other.comparator_);
            other.is_dirty_ = false;
        }
        return *this;
    }

    /**
     * @brief 从值赋值
     * @param new_value 新值
     * @return 当前对象的引用
     * @details 如果新值与当前值不同，会设置脏标记
     */
    QImTrackedValue& operator=(const T& new_value)
    {
        set_value(new_value);
        return *this;
    }

    /**
     * @brief 从右值赋值
     * @param new_value 新值的右值引用
     * @return 当前对象的引用
     * @details 如果新值与当前值不同，会设置脏标记
     */
    QImTrackedValue& operator=(T&& new_value)
    {
        set_value(std::move(new_value));
        return *this;
    }

    /**
     * @brief 设置自定义比较器
     */
    void set_comparator(const Comparator& comp)
    {
        comparator_ = comp;
    }

    /**
     * @brief 获取比较器
     */
    Comparator get_comparator() const
    {
        return comparator_;
    }

    /**
     * @brief 获取值的const引用
     * @return 值的const引用
     */
    const T& value() const& noexcept
    {
        return value_;
    }

    /**
     * @brief 获取值的非常量引用
     * @return 值的非常量引用
     * @warning 直接通过引用修改值不会自动设置脏标记
     * @see mark_dirty()
     */
    T& value() & noexcept
    {
        return value_;
    }

    /**
     * @brief 获取右值引用（用于移动）
     * @return 值的右值引用
     */
    T&& value() && noexcept
    {
        return std::move(value_);
    }

    /**
     * @brief 检查值是否脏（是否被修改过）
     * @return true如果值被修改过，false否则
     */
    bool is_dirty() const noexcept
    {
        return is_dirty_;
    }

    /**
     * @brief 检查值是否有变化（is_dirty的别名）
     * @return true如果值有变化，false否则
     */
    bool has_changed() const noexcept
    {
        return is_dirty_;
    }

    /**
     * @brief 清除脏标记
     * @details 将脏标记设置为false
     */
    void clear() noexcept
    {
        is_dirty_ = false;
    }

    /**
     * @brief 清除脏标记（clear的别名）
     */
    void reset_dirty_flag() noexcept
    {
        is_dirty_ = false;
    }

    /**
     * @brief 强制设置脏标记
     * @details 将脏标记设置为true，即使值没有改变
     * @example 用于直接修改值内部的情况
     * @code
     * QImTrackedValue<std::vector<int>> vec({1, 2, 3});
     * vec.value().push_back(4);  // 直接修改内部
     * vec.mark_dirty();          // 手动标记为脏
     * @endcode
     */
    void mark_dirty() noexcept
    {
        is_dirty_ = true;
    }

    /**
     * @brief 标记为已更新（mark_clean的别名）
     */
    void mark_clean() noexcept
    {
        is_dirty_ = false;
    }

    /**
     * @brief 获取值并清除脏标记
     * @return 当前值的副本
     * @details 这个函数在一次调用中完成获取值和清除脏标记的操作，
     *          适用于需要处理脏数据后立即标记为已处理的场景
     * @example
     * @code
     * QImTrackedValue<int> data(0);
     * data = 100;  // 标记为脏
     *
     * if (data.is_dirty()) {
     *     // 获取数据并立即清除脏标记
     *     int processed_value = data.get_and_clear();
     *     // 现在脏标记已清除
     *     process(processed_value);
     * }
     * @endcode
     */
    T get_and_clear() noexcept
    {
        is_dirty_ = false;
        return value_;
    }

    /**
     * @brief 获取值（const解引用操作符）
     * @return 值的const引用
     */
    const T& operator*() const noexcept
    {
        return value_;
    }

    /**
     * @brief 获取值（解引用操作符）
     * @return 值的引用
     */
    T& operator*() noexcept
    {
        return value_;
    }

    /**
     * @brief 获取值的指针（const箭头操作符）
     * @return 指向值的const指针
     */
    const T* operator->() const noexcept
    {
        return &value_;
    }

    /**
     * @brief 获取值的指针（箭头操作符）
     * @return 指向值的指针
     */
    T* operator->() noexcept
    {
        return &value_;
    }

    /**
     * @brief 转换为原始类型
     * @return 值的副本
     */
    explicit operator T() const
    {
        return value_;
    }

    /**
     * @brief 重置为默认值
     * @details 将值重置为T的默认值，并清除脏标记
     */
    void reset()
    {
        value_    = T {};
        is_dirty_ = false;
    }

    /**
     * @brief 重置为新值
     * @param new_value 新值
     * @details 设置新值，如果新值与当前值不同，会设置脏标记
     */
    void reset(const T& new_value)
    {
        set_value(new_value);
    }

    /**
     * @brief 交换两个QImTrackedValue对象的内容
     * @param other 要交换的另一个QImTrackedValue对象
     */
    void swap(QImTrackedValue& other) noexcept
    {
        using std::swap;
        swap(value_, other.value_);
        swap(is_dirty_, other.is_dirty_);
    }

    /**
     * @brief 比较两个QImTrackedValue对象的值是否相等
     * @param other 要比较的另一个QImTrackedValue对象
     * @return true如果值相等，false否则
     */
    bool operator==(const QImTrackedValue& other) const
    {
        return value_ == other.value_;
    }

    /**
     * @brief 比较两个QImTrackedValue对象的值是否不相等
     * @param other 要比较的另一个QImTrackedValue对象
     * @return true如果值不相等，false否则
     */
    bool operator!=(const QImTrackedValue& other) const
    {
        return !(*this == other);
    }

    /**
     * @brief 比较值与原始类型是否相等
     * @param other 要比较的原始值
     * @return true如果值相等，false否则
     */
    bool operator==(const T& other) const
    {
        return value_ == other;
    }

    /**
     * @brief 比较值与原始类型是否不相等
     * @param other 要比较的原始值
     * @return true如果值不相等，false否则
     */
    bool operator!=(const T& other) const
    {
        return value_ != other;
    }

    /**
     * @brief 显式bool转换（检查是否脏）
     * @return true如果值脏，false否则
     * @example
     * @code
     * QImTrackedValue<int> data(0);
     * data = 10;  // 标记为脏
     *
     * if (data) {  // 等价于 if (data.is_dirty())
     *     // 处理脏数据...
     * }
     * @endcode
     */
    explicit operator bool() const noexcept
    {
        return is_dirty_;
    }

private:
    T value_;        ///< 存储的实际值
    bool is_dirty_;  ///< 脏标记，表示值是否被修改过
    Comparator comparator_;
    void set_value(const T& new_value)
    {
        if (comparator_(value_, new_value)) {
            value_    = new_value;
            is_dirty_ = true;
        }
    }

    void set_value(T&& new_value)
    {
        if (comparator_(value_, new_value)) {
            value_    = std::move(new_value);
            is_dirty_ = true;
        }
    }
};

/**
 * @brief 交换两个QImTrackedValue对象
 * @tparam T 值类型
 * @param lhs 第一个对象
 * @param rhs 第二个对象
 */
template< typename T >
void swap(QImTrackedValue< T >& lhs, QImTrackedValue< T >& rhs) noexcept
{
    lhs.swap(rhs);
}

/**
 * @brief 创建QImTrackedValue的辅助函数
 * @tparam T 值类型
 * @param value 初始值
 * @return 新的QImTrackedValue对象
 * @example
 * @code
 * auto count = make_qim_tracked_value(0);
 * auto name = make_qim_tracked_value(std::string("Alice"));
 * @endcode
 */
template< typename T >
QImTrackedValue< T > make_qim_tracked_value(const T& value)
{
    return QImTrackedValue< T >(value);
}

/**
 * @brief 原始类型与QImTrackedValue的比较（左侧为原始类型）
 * @tparam T 值类型
 * @param lhs 原始值
 * @param rhs QImTrackedValue对象
 * @return true如果值相等，false否则
 */
template< typename T >
bool operator==(const T& lhs, const QImTrackedValue< T >& rhs)
{
    return rhs == lhs;
}

/**
 * @brief 原始类型与QImTrackedValue的比较（左侧为原始类型）
 * @tparam T 值类型
 * @param lhs 原始值
 * @param rhs QImTrackedValue对象
 * @return true如果值不相等，false否则
 */
template< typename T >
bool operator!=(const T& lhs, const QImTrackedValue< T >& rhs)
{
    return rhs != lhs;
}

}  // end namespace QIM
#endif  // QIMTRACKEDVALUE_HPP
