#ifndef QIMWAVEFORMGENERATOR_HPP
#define QIMWAVEFORMGENERATOR_HPP
#include <vector>
#include <cmath>
#include <utility>  // for std::pair
#include <tuple>
#ifndef M_PI
constexpr double M_PI = 3.14159265358979323846;
#endif
namespace QIM
{
// ============ 内置波形策略 ============

struct SineWave
{
    static double eval(double x, double amplitude, double phase = 0.0)
    {
        return amplitude * std::sin(x + phase);
    }
};

struct CosineWave
{
    static double eval(double x, double amplitude, double phase = 0.0)
    {
        return amplitude * std::cos(x + phase);
    }
};

struct DampedSineWave
{
    static double eval(double x, double amplitude, double damping = 0.1, double phase = 0.0)
    {
        return amplitude * std::sin(x + phase) * std::exp(-damping * x);
    }
};

struct ExponentialDecay
{
    static double eval(double x, double amplitude, double decay_rate = 0.1)
    {
        return amplitude * std::exp(-decay_rate * x);
    }
};

struct LinearWave
{
    static double eval(double x, double slope, double offset = 0.0)
    {
        return slope * x + offset;
    }
};

// ============ 工具：递归调用 tuple 元素（C++14 友好） ============

namespace detail
{

// 通用调用器：通过索引序列展开 tuple（手动实现 index_sequence）
template< std::size_t... Is >
struct index_sequence
{
};

template< std::size_t N, std::size_t... Is >
struct make_index_sequence_impl : make_index_sequence_impl< N - 1, N - 1, Is... >
{
};

template< std::size_t... Is >
struct make_index_sequence_impl< 0, Is... >
{
    using type = index_sequence< Is... >;
};

template< std::size_t N >
using make_index_sequence = typename make_index_sequence_impl< N >::type;

// 调用 WavePolicy::eval(x, args...)
template< typename WavePolicy, typename Tuple, std::size_t... Is >
double call_eval_impl(double x, const Tuple& params, index_sequence< Is... >)
{
    return WavePolicy::eval(x, std::get< Is >(params)...);
}

}  // namespace detail


/**
 * \if ENGLISH
 * @brief A template-based waveform generator for scientific plotting and signal simulation.
 *
 * This class uses policy-based design to generate various types of waveforms (e.g., sine, damped sine,
 * exponential decay, etc.) over a specified x-interval. The waveform type is determined by the
 * WavePolicy template parameter, and its parameters (amplitude, damping, phase, etc.) are passed at
 * construction time. The generate() method returns paired x and y vectors suitable for plotting.
 *
 * The class is header-only, requires only C++14, and has zero runtime overhead due to compile-time
 * policy resolution.
 *
 * @tparam WavePolicy A struct with a static `eval(double x, ...)` function that defines the waveform shape.
 *                    Built-in policies include: SineWave, CosineWave, DampedSineWave, ExponentialDecay, etc.
 *
 * @note All waveform parameters must be passed via the factory function QIM::make_waveform().
 * @warning The number of arguments passed to make_waveform() must exactly match the number of
 *          parameters (after 'x') expected by WavePolicy::eval().
 *
 * @par Example: Generate a damped sine wave
 * @code
 * #include "waveform_generator.hpp"
 *
 * auto wave = QIM::make_waveform<QIM::DampedSineWave>(3.0, 0.15); // amplitude=3.0, damping=0.15
 * auto [x, y] = wave.generate(200, 0.0, 4.0 * M_PI);
 * // Now x and y can be passed to a plotting library like ImPlot or matplotlib-cpp
 * @endcode
 *
 * @par Example: Generate a cosine wave with phase shift
 * @code
 * auto cos_wave = QIM::make_waveform<QIM::CosineWave>(2.0, M_PI / 4.0); // amp=2.0, phase=π/4
 * auto data = cos_wave.generate(150, -M_PI, M_PI);
 * const auto& x_vals = data.first;
 * const auto& y_vals = data.second;
 * @endcode
 *
 * @see QIM::make_waveform(), QIM::SineWave, QIM::DampedSineWave, QIM::ExponentialDecay
 * \endif
 *
 * \if CHINESE
 * @brief 基于模板的波形发生器，用于科学绘图与信号仿真。
 *
 * 该类采用基于策略（policy-based）的设计，可在指定 x 区间内生成多种波形（如正弦波、阻尼正弦波、
 * 指数衰减等）。波形类型由模板参数 WavePolicy 决定，其参数（振幅、阻尼系数、相位等）在构造时传入。
 * generate() 方法返回一对 x 和 y 向量，可直接用于绘图。
 *
 * 此类为头文件-only 实现，仅需 C++14 标准支持，且由于在编译期解析策略，运行时无额外开销。
 *
 * @tparam WavePolicy 一个包含静态函数 `eval(double x, ...)` 的结构体，用于定义波形形状。
 *                    内置策略包括：SineWave、CosineWave、DampedSineWave、ExponentialDecay 等。
 *
 * @note 所有波形参数必须通过工厂函数 QIM::make_waveform() 传入。
 * @warning 传递给 make_waveform() 的参数数量必须严格匹配 WavePolicy::eval() 函数中除 'x' 外的参数个数。
 *
 * @par 示例：生成阻尼正弦波
 * @code
 * #include "waveform_generator.hpp"
 *
 * auto wave = QIM::make_waveform<QIM::DampedSineWave>(3.0, 0.15); // 振幅=3.0，阻尼=0.15
 * auto [x, y] = wave.generate(200, 0.0, 4.0 * M_PI);
 * // 此时 x 和 y 可传入 ImPlot 或 matplotlib-cpp 等绘图库
 * @endcode
 *
 * @par 示例：生成带相位偏移的余弦波
 * @code
 * auto cos_wave = QIM::make_waveform<QIM::CosineWave>(2.0, M_PI / 4.0); // 振幅=2.0，相位=π/4
 * auto data = cos_wave.generate(150, -M_PI, M_PI);
 * const auto& x_vals = data.first;
 * const auto& y_vals = data.second;
 * @endcode
 *
 * @see QIM::make_waveform(), QIM::SineWave, QIM::DampedSineWave, QIM::ExponentialDecay
 * \endif
 */
template<typename WavePolicy, typename... Params>
class QImWaveformGenerator {
public:
    using value_type = double;

    /**
     * \if ENGLISH
     * @brief Constructs the waveform generator with waveform-specific parameters.
     *
     * This constructor is typically not called directly. Use QIM::make_waveform() instead.
     *
     * @tparam Args Types of waveform parameters (e.g., amplitude, damping, phase).
     * @param args Parameters forwarded to the internal tuple storage.
     * \endif
     *
     * \if CHINESE
     * @brief 使用波形特定参数构造波形发生器。
     *
     * 此构造函数通常不直接调用，请使用 QIM::make_waveform()。
     *
     * @tparam Args 波形参数的类型（如振幅、阻尼、相位等）。
     * @param args 转发到内部 tuple 存储的参数。
     * \endif
     */
    explicit QImWaveformGenerator(Params... args)
        : params_(std::make_tuple(std::forward<Params>(args)...)) {}

    /**
     * \if ENGLISH
     * @brief Generates waveform data points over a specified x-interval.
     *
     * Produces `numPoints` evenly spaced samples from `x_start` to `x_end` (inclusive),
     * evaluates the waveform at each x, and returns paired vectors {x, y}.
     *
     * @param numPoints Number of data points to generate (must be >= 2).
     * @param x_start Start of x-interval (default: 0.0).
     * @param x_end End of x-interval (default: 4π). Must be > x_start.
     * @return std::pair<std::vector<double>, std::vector<double>> containing x and y coordinates.
     *
     * @note If invalid input is provided (e.g., numPoints < 2), returns empty vectors.
     * @warning For trigonometric waves, ensure x range covers desired cycles (e.g., 2π per cycle).
     * \endif
     *
     * \if CHINESE
     * @brief 在指定 x 区间内生成波形数据点。
     *
     * 生成从 `x_start` 到 `x_end`（含端点）之间均匀分布的 `numPoints` 个采样点，
     * 在每个 x 处计算波形值，并返回一对向量 {x, y}。
     *
     * @param numPoints 要生成的数据点数量（必须 ≥ 2）。
     * @param x_start x 区间的起始值（默认：0.0）。
     * @param x_end x 区间的结束值（默认：4π），必须大于 x_start。
     * @return 包含 x 和 y 坐标的 std::pair<std::vector<double>, std::vector<double>>。
     *
     * @note 若输入无效（如 numPoints < 2），返回空向量。
     * @warning 对于三角函数波形，请确保 x 范围覆盖所需周期数（例如每周期 2π）。
     * \endif
     */
    std::pair<std::vector<double>, std::vector<double>>
    generate(int numPoints, double x_start = 0.0, double x_end = 4.0 * M_PI) const {
        if (numPoints < 2 || x_end <= x_start) {
            return {{}, {}};
        }

        std::vector<double> x(numPoints);
        std::vector<double> y(numPoints);
        const double dx = (x_end - x_start) / (numPoints - 1);

        constexpr std::size_t N = sizeof...(Params);
        auto idx_seq = detail::make_index_sequence<N>{};

        for (int i = 0; i < numPoints; ++i) {
            x[i] = x_start + i * dx;
            y[i] = detail::call_eval_impl<WavePolicy>(x[i], params_, idx_seq);
        }

        return {std::move(x), std::move(y)};
    }

private:
    std::tuple<Params...> params_;
};

// ============ 工厂函数（推荐使用方式） ============
template<typename WavePolicy, typename... Args>
auto make_waveform(Args&&... args) -> QImWaveformGenerator<WavePolicy, typename std::decay<Args>::type...> {
    return QImWaveformGenerator<WavePolicy, typename std::decay<Args>::type...>(
        std::forward<Args>(args)...
        );
}

}
#endif  // QIMWAVEFORMGENERATOR_HPP
