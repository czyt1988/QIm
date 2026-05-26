#ifndef QIMPLOTAXISINFO_H
#define QIMPLOTAXISINFO_H
#include <QObject>
#include <QList>
#include "QImAPI.h"
#include "QImPlot.h"
namespace QIM
{
class QImPlotNode;
/**
 * \if ENGLISH
 * @brief Qt wrapper class for managing ImPlot axis configuration with type-safe properties
 * @details Provides a QObject-based interface to configure ImPlot axes without directly
 *          manipulating low-level ImPlotAxisFlags bitmasks. Encapsulates:
 *          - Axis label text and visibility
 *          - Range limits (min/max) with auto-fit behavior
 *          - All ImPlotAxisFlags_ options exposed as intuitive boolean properties
 *          - Scale type configuration (linear/log/time/symlog)
 *          - Bidirectional conversion between Qt enums (QImPlotAxisId) and ImPlot enums (ImAxis)
 * @note This class does NOT render plots itself. It stores configuration state that should be
 *       applied to ImPlot via SetupAxis(), SetupAxisLimits(), and SetupAxisScale() during
 *       plot setup (inside BeginPlot/EndPlot block).
 * @note All property changes are stored locally; actual plot appearance updates only after
 *          re-rendering with updated configuration applied to ImPlot context.
 * @see ImPlot::SetupAxis(), ImPlot::SetupAxisLimits(), ImPlot::SetupAxisScale()
 * \endif
 *
 * \if CHINESE
 * @brief ���ڹ��� ImPlot ���������õ� Qt ��װ�࣬�ṩ���Ͱ�ȫ�����Խӿ�
 * @details �ṩ���� QObject �Ľӿ������� ImPlot �����ᣬ����ֱ�Ӳ����ײ� ImPlotAxisFlags λ���롣
 *          ��װ���ܰ�����
 *          - �������ǩ�ı����ɼ���
 *          - ��Χ���ƣ���С/���ֵ�����Զ�������Ϊ
 *          - ���� ImPlotAxisFlags_ ѡ����ֱ�۵Ĳ���������ʽ��¶
 *          - �̶��������ã�����/����/ʱ��/�Գƶ�����
 *          - Qt ö�٣�QImPlotAxisId���� ImPlot ö�٣�ImAxis����˫��ת��
 * @note ���౾������Ⱦ��ͼ�������洢����״̬�����ڻ�ͼ���ý׶Σ�BeginPlot/EndPlot ���ڣ�
 *       ͨ�� SetupAxis()��SetupAxisLimits() �� SetupAxisScale() Ӧ���� ImPlot��
 * @note �������Ա�����ڱ��ش洢��ʵ�ʻ�ͼ��۸�������������Ⱦʱ������Ӧ�õ� ImPlot �����ĺ���Ч��
 * @see ImPlot::SetupAxis(), ImPlot::SetupAxisLimits(), ImPlot::SetupAxisScale()
 * \endif
 */
class QIM_CORE_API QImPlotAxisInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(double minLimits READ minLimits WRITE setMinLimits NOTIFY limitsChanged)
    Q_PROPERTY(double maxLimits READ maxLimits WRITE setMaxLimits NOTIFY limitsChanged)
    // About ImPlotAxisFlags
    Q_PROPERTY(bool autoFit READ isAutoFit WRITE setAutoFit NOTIFY axisFlagChanged)
    Q_PROPERTY(bool inverted READ isInverted WRITE setInverted NOTIFY axisFlagChanged)
    Q_PROPERTY(bool labelEnabled READ isLabelEnabled WRITE setLabelEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool gridLinesEnabled READ isGridLinesEnabled WRITE setGridLinesEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool tickMarksEnabled READ isTickMarksEnabled WRITE setTickMarksEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool tickLabelsEnabled READ isTickLabelsEnabled WRITE setTickLabelsEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool initialFitEnabled READ isInitialFitEnabled WRITE setInitialFitEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool menusEnabled READ isMenusEnabled WRITE setMenusEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool sideSwitchEnabled READ isSideSwitchEnabled WRITE setSideSwitchEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool highlightEnabled READ isHighlightEnabled WRITE setHighlightEnabled NOTIFY axisFlagChanged)
    Q_PROPERTY(bool opposite READ isOpposite WRITE setOpposite NOTIFY axisFlagChanged)
    Q_PROPERTY(bool foreground READ isForeground WRITE setForeground NOTIFY axisFlagChanged)
    Q_PROPERTY(bool rangeFit READ isRangeFit WRITE setRangeFit NOTIFY axisFlagChanged)
    Q_PROPERTY(bool panStretch READ isPanStretch WRITE setPanStretch NOTIFY axisFlagChanged)
    Q_PROPERTY(bool lockMin READ isLockMin WRITE setLockMin NOTIFY axisFlagChanged)
    Q_PROPERTY(bool lockMax READ isLockMax WRITE setLockMax NOTIFY axisFlagChanged)
    Q_PROPERTY(bool lock READ isLock WRITE setLock NOTIFY axisFlagChanged)
    Q_PROPERTY(bool noDecorations READ isNoDecorations WRITE setNoDecorations NOTIFY axisFlagChanged)
    //  scaleType
    Q_PROPERTY(QImPlotScaleType scaleType READ scaleType WRITE setScaleType NOTIFY scaleTypeChanged)
    // Tick configuration
    Q_PROPERTY(QList<double> tickValues READ tickValues WRITE setTickValues NOTIFY tickConfigChanged)
    Q_PROPERTY(QList<QByteArray> tickLabels READ tickLabels WRITE setTickLabels NOTIFY tickConfigChanged)
    Q_PROPERTY(bool keepDefaultTicks READ isKeepDefaultTicks WRITE setKeepDefaultTicks NOTIFY tickConfigChanged)

    QIM_DECLARE_PRIVATE(QImPlotAxisInfo)
    Q_DISABLE_COPY(QImPlotAxisInfo)
public:
    explicit QImPlotAxisInfo(QImPlotAxisId axis, QImPlotNode* plot = nullptr);
    ~QImPlotAxisInfo();
    QImPlotAxisId axisId() const;
    int imAxis() const;
    // ��ȡplot
    QImPlotNode* plotNode() const;
    // ��ǩ
    QString label() const;
    void setLabel(const QString& label);

    // ��Χ����
    double minLimits() const;
    void setMinLimits(double min);
    double maxLimits() const;
    void setMaxLimits(double max);

    QImPlotCondition limitsCondition() const;
    void setLimitsCondition(QImPlotCondition v);

    void setLimits(double min, double max, QImPlotCondition cond = QImPlotCondition::Once);
    // About ImPlotAxisFlags / ����Ϊ����ӦQt������ϰ�ߣ�����Ϊ�϶�����
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_AutoFit / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_AutoFit
    bool isAutoFit() const;
    void setAutoFit(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Invert / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_Invert
    bool isInverted() const;
    void setInverted(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoLabel / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoLabel
    bool isLabelEnabled() const;
    void setLabelEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoGridLines / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoGridLines
    bool isGridLinesEnabled() const;
    void setGridLinesEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoTickMarks / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoTickMarks
    bool isTickMarksEnabled() const;
    void setTickMarksEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoTickLabels / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoTickLabels
    bool isTickLabelsEnabled() const;
    void setTickLabelsEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoInitialFit / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoInitialFit
    bool isInitialFitEnabled() const;
    void setInitialFitEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoMenus / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoMenus
    bool isMenusEnabled() const;
    void setMenusEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoSideSwitch / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoSideSwitch
    bool isSideSwitchEnabled() const;
    void setSideSwitchEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoHighlight / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoHighlight
    bool isHighlightEnabled() const;
    void setHighlightEnabled(bool enabled);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Opposite / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_Opposite
    bool isOpposite() const;
    void setOpposite(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Foreground / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_Foreground
    bool isForeground() const;
    void setForeground(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_RangeFit / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_RangeFit
    bool isRangeFit() const;
    void setRangeFit(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_PanStretch / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_PanStretch
    bool isPanStretch() const;
    void setPanStretch(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_LockMin / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_LockMin
    bool isLockMin() const;
    void setLockMin(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_LockMax / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_LockMax
    bool isLockMax() const;
    void setLockMax(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_Lock / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_Lock
    bool isLock() const;  // LockMin && LockMax
    void setLock(bool on);
    // Corresponds to ImPlotAxisFlags::ImPlotAxisFlags_NoDecorations / cn:��ӦImPlotAxisFlags::ImPlotAxisFlags_NoDecorations
    bool isNoDecorations() const;
    void setNoDecorations(bool enabled);
    // ԭʼ��־���ʣ��߼��÷���
    int axisFlags() const;
    void setAxisFlags(int flags);
    // Gets the current axis scale type (linear, logarithmic, time, etc.)
    QImPlotScaleType scaleType() const;
    void setScaleType(QImPlotScaleType t);
    int imPlotScale() const;
    // Tick configuration properties
    QList<double> tickValues() const;
    void setTickValues(const QList<double>& values);
    QList<QByteArray> tickLabels() const;
    void setTickLabels(const QList<QByteArray>& labels);
    bool isKeepDefaultTicks() const;
    void setKeepDefaultTicks(bool keep);

    // Convenience methods for tick configuration
    void setAxisTicks(const QList<double>& values, const QList<QByteArray>& labels = {}, bool keepDefault = false);
    void setAxisTicksRange(double v_min, double v_max, int n_ticks, const QList<QByteArray>& labels = {}, bool keepDefault = false);
    // ����������Ƿ���Ч�����ػ���ʾ������X1��Y1���ػ���ʾ��ͨ��setNoDecorations��ʵ�֣�
    bool isEnabled() const;
    void setEnabled(bool on);
Q_SIGNALS:
    /**
     * \if ENGLISH
     * @brief Emitted when the axis label text changes
     * @param label The new label text string (empty string indicates default axis name)
     * @details Triggered by setLabel() when the new label differs from current value.
     *          Typically used to update UI elements displaying axis names or to persist label changes.
     * @note This signal is NOT emitted when label visibility changes (controlled by labelEnabled property).
     *       Only the text content change triggers this signal.
     * @see setLabel(), isLabelEnabled()
     * \endif
     *
     * \if CHINESE
     * @brief ���������ǩ�ı��ı�ʱ����
     * @param label �µı�ǩ�ı��ַ��������ַ�����ʾʹ��Ĭ�������ƣ�
     * @details �� setLabel() ���õ��±�ǩ�뵱ǰֵ��ͬʱ������
     *          ͨ�����ڸ�����ʾ�����Ƶ� UI Ԫ�ػ�־û���ǩ�����
     * @note ���ź��ڱ�ǩ�ɼ��Ա��ʱ**����**�������� labelEnabled ���Կ��ƣ���
     *       ����ǩ�ı����ݱ���ᴥ�����źš�
     * @see setLabel(), isLabelEnabled()
     * \endif
     */
    void labelChanged(const QString& label);

    /**
     * \if ENGLISH
     * @brief Emitted when axis range limits change (min, max, or both)
     * @param min New minimum value of the visible axis range
     * @param max New maximum value of the visible axis range
     * @details Triggered by setMinLimits(), setMaxLimits(), or setLimits() when values differ from current limits.
     *          Note: Changing limits does NOT immediately affect the plot; must call SetupAxisLimits() during
     *          next plot render to apply changes. This signal allows UI to reflect pending limit changes.
     * @note Values are NOT validated for min < max; invalid ranges (min >= max) may cause ImPlot rendering issues.
     * @see setLimits(), ImPlot::SetupAxisLimits()
     * \endif
     *
     * \if CHINESE
     * @brief �������᷶Χ���Ƹı�ʱ���䣨��Сֵ�����ֵ������ͬʱ��
     * @param min ������ɼ���Χ������Сֵ
     * @param max ������ɼ���Χ�������ֵ
     * @details �� setMinLimits()��setMaxLimits() �� setLimits() ���õ�ֵ�뵱ǰ���Ʋ�ͬʱ������
     *          ע�⣺�޸�����ֵ��������Ӱ���ͼ���������´λ�ͼ��Ⱦʱ���� SetupAxisLimits() ����Ӧ�ñ����
     *          ���ź����� UI ��ӳ�����������Ʊ����
     * @note ֵ**��**��֤ min < max����Ч��Χ��min >= max�����ܵ��� ImPlot ��Ⱦ���⡣
     * @see setLimits(), ImPlot::SetupAxisLimits()
     * \endif
     */
    void limitsChanged(double min, double max);

    /**
     * \if ENGLISH
     * @brief Emitted when any axis flag property changes (autoFit, inverted, gridLinesEnabled, etc.)
     * @details Triggered by ANY flag-modifying setter (e.g., setAutoFit(), setLock(), setDecorationsEnabled())
     *          when the underlying ImPlotAxisFlags bitmask changes. Single signal aggregates all flag changes
     *          for efficiency - connected slots should query relevant properties to determine exact change.
     * @note This signal is NOT emitted when setting flags to identical value (change detection via != comparison).
     *       Also NOT emitted by direct axisFlags() getter access (only setters trigger emission).
     * @note Does NOT indicate which specific flag changed; slots must inspect individual properties or
     *          call axisFlags() to determine changed flags. For fine-grained change tracking, connect to
     *          individual property signals if available (e.g., onAutoFitChanged in QML).
     * @see setAxisFlags(), axisFlags()
     * \endif
     *
     * \if CHINESE
     * @brief �������������־���Ա��ʱ���䣨autoFit��inverted��gridLinesEnabled �ȣ�
     * @details ��**�κ�**�޸ı�־������������ setAutoFit()��setLock()��setDecorationsEnabled()��
     *          ���µײ� ImPlotAxisFlags λ������ʱ�����������źžۺ����б�־����������Ч�� ��
     *          �����ӵĲۺ������ѯ���������ȷ����������
     * @note ���ñ�־Ϊ��ֵͬʱ**����**�������źţ�ͨ�� != �ȽϽ��б����⣩��
     *       ֱ��ͨ�� axisFlags() getter ����**����**�������źţ�����������������
     * @note ��ָʾ�����ĸ���־����������ۺ��������鵥�����Ի���� axisFlags() ��ȷ������ı�־��
     *          ����ϸ���ȱ�����٣������ӵ������������źţ��� QML �е� onAutoFitChanged����
     * @see setAxisFlags(), axisFlags()
     * \endif
     */
    void axisFlagChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when the axis scale type changes (linear �� log, time �� symlog, etc.)
     * @param scaleType The new scale type as QImPlotScaleType enum value
     * @details Triggered by setScaleType() when the new scale type differs from current type.
     *          Scale types control value transformation and formatting:
     *          - Linear: Standard linear mapping (default)
     *          - Time:   Unix timestamp interpretation with date/time formatting
     *          - Log10:  Base-10 logarithmic mapping (requires positive values)
     *          - SymLog: Symmetric logarithmic mapping (handles negative values near zero)
     * @note Actual scale change takes effect only after calling ImPlot::SetupAxisScale() during plot setup.
     *       This signal allows UI to update scale-type dependent controls (e.g., disable negative values for Log10).
     * @note Changing to Log10 scale while data contains non-positive values may cause rendering artifacts.
     *         Applications should validate data compatibility before scale type changes.
     * @see setScaleType(), ImPlot::SetupAxisScale()
     * \endif
     *
     * \if CHINESE
     * @brief ��������̶����ͱ��ʱ���䣨���� �� ������ʱ�� �� �Գƶ����ȣ�
     * @param scaleType �� QImPlotScaleType ö��ֵ��ʾ���¿̶�����
     * @details �� setScaleType() ���õ��¿̶������뵱ǰ���Ͳ�ͬʱ������
     *          �̶����Ϳ���ֵ�ı任�͸�ʽ����
     *          - Linear: ��׼����ӳ�䣨Ĭ�ϣ�
     *          - Time:   Unix ʱ������ͣ�������/ʱ���ʽ��
     *          - Log10:  �� 10 Ϊ�׵Ķ���ӳ�䣨Ҫ����ֵ��
     *          - SymLog: �Գƶ���ӳ�䣨����㸽���ɴ�����ֵ��
     * @note ʵ�ʿ̶ȱ�����ڻ�ͼ���ý׶ε��� ImPlot::SetupAxisScale() ����Ч��
     *       ���ź����� UI ���������̶����͵Ŀؼ�������� Log10 ���ø�ֵ���룩��
     * @note �����ݰ�������ֵʱ�л��� Log10 �̶ȿ��ܵ�����Ⱦ�쳣��
     *         Ӧ��Ӧ�ڱ���̶�����ǰ��֤���ݼ����ԡ�
     * @see setScaleType(), ImPlot::SetupAxisScale()
     * \endif
     */
    void scaleTypeChanged();

    /**
     * \if ENGLISH
     * @brief Emitted when tick configuration changes (tick values, labels, or keepDefaultTicks)
     * @details Triggered by setTickValues(), setTickLabels(), setKeepDefaultTicks(), setAxisTicks(),
     *          or setAxisTicksRange() when tick configuration changes.
     * \endif
     *
     * \if CHINESE
     * @brief ���̶����ñ��ʱ���䣨�̶�ֵ����ǩ���Ƿ���Ĭ�Ͽ̶ȣ�
     * @details �� setTickValues()��setTickLabels()��setKeepDefaultTicks()��setAxisTicks()
     *          �� setAxisTicksRange() �޸Ŀ̶�����ʱ������
     * \endif
     */
    void tickConfigChanged();

public:
    // render
    void render();
};
}
#endif
