---
title: QIm — Qt × ImGui Visualization
description: A high-performance 2D/3D data visualization library that brings Dear ImGui, ImPlot, and ImPlot3D to Qt through retained-mode encapsulation.
hide:
  - navigation
  - toc
---

<div class="qim-hero">
<div class="qim-hero-bg"></div>
<div class="qim-hero-inner">

<p class="qim-hero-badge">Open Source &middot; MIT Licensed</p>

<h1 class="qim-hero-title">Qt &times; ImGui Visualization</h1>

<p class="qim-hero-desc">
A high-performance 2D/3D data visualization library that brings
Dear ImGui, ImPlot, and ImPlot3D to Qt through retained-mode encapsulation &mdash;
signals, slots, properties, and object trees.
</p>

<div class="qim-hero-pills">
<span>C++17</span>
<span>CMake</span>
<span>Qt 5.14+ / Qt 6</span>
<span>OpenGL</span>
</div>

<div class="qim-hero-actions">
<a href="plot2d/" class="qim-btn qim-btn-primary">Get Started</a>
<a href="https://github.com/czyt1988/QIm" class="qim-btn qim-btn-ghost">GitHub</a>
</div>

</div>
</div>

<div class="qim-stats-bar">
<div class="qim-stat">
<strong>2D &amp; 3D</strong>
<span>Visualization</span>
</div>
<div class="qim-stat-divider"></div>
<div class="qim-stat">
<strong>25+</strong>
<span>Chart Types</span>
</div>
<div class="qim-stat-divider"></div>
<div class="qim-stat">
<strong>Qt 5 &amp; 6</strong>
<span>Fully Compatible</span>
</div>
<div class="qim-stat-divider"></div>
<div class="qim-stat">
<strong>MIT</strong>
<span>Commercial Friendly</span>
</div>
</div>

<div class="qim-container">

<div class="qim-section-head">
<h2>Core Features</h2>
<p>Everything you need for high-performance real-time data visualization in Qt applications.</p>
</div>

<div class="qim-card-grid">

<div class="qim-card">
<div class="qim-card-icon">
<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"></polygon></svg>
</div>
<h3>High Performance</h3>
<p>OpenGL-accelerated rendering pipeline handles million-point datasets with smooth real-time plotting. Up to 2&times; faster than QPainter-based libraries on raw rendering.</p>
</div>

<div class="qim-card">
<div class="qim-card-icon">
<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M9 12l2 2 4-4"></path><path d="M21 12c-1 0-3-1-3-3s2-3 3-3 3 1 3 3-2 3-3 3"></path><path d="M3 12c1 0 3-1 3-3s-2-3-3-3-3 1-3 3 2 3 3 3"></path><path d="M12 3c0 1 1 3 3 3s3-2 3-3-1-3-3-3-3 2-3 3"></path><path d="M12 21c0-1 1-3 3-3s3 2 3 3-1 3-3 3-3-2-3-3"></path></svg>
</div>
<h3>Qt Paradigm</h3>
<p>Native Qt signals/slots, property system (<code>Q_PROPERTY</code>), and QObject object tree. No ImGui knowledge required &mdash; program the way you already know.</p>
</div>

<div class="qim-card">
<div class="qim-card-icon">
<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="2" y="3" width="20" height="14" rx="2" ry="2"></rect><line x1="8" y1="21" x2="16" y2="21"></line><line x1="12" y1="17" x2="12" y2="21"></line></svg>
</div>
<h3>2D &amp; 3D Engines</h3>
<p>Unified API across ImPlot (2D) and ImPlot3D (3D). Line, scatter, bars, surface, mesh, heatmap, pie, K-line &mdash; 25+ chart types in a single <code>QImFigureWidget</code>.</p>
</div>

<div class="qim-card">
<div class="qim-card-icon">
<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="22 12 18 12 15 21 9 3 6 12 2 12"></polyline></svg>
</div>
<h3>LTTB Downsampling</h3>
<p>Built-in <code>QImLTTBDownsampler</code> with SIMD acceleration. Adaptive sampling keeps visualization fidelity while taming multi-million-point datasets.</p>
</div>

<div class="qim-card">
<div class="qim-card-icon">
<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M16 3h5v5"></path><path d="M8 3H3v5"></path><path d="M21 16v5h-5"></path><path d="M3 16v5h5"></path><rect x="8" y="8" width="8" height="8" rx="1"></rect></svg>
</div>
<h3>Retained Mode</h3>
<p>Converts ImGui's immediate mode into Qt-friendly retained mode. Build UI through an object tree &mdash; set properties once, let the framework render every frame.</p>
</div>

<div class="qim-card">
<div class="qim-card-icon">
<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"></circle><line x1="22" y1="12" x2="18" y2="12"></line><line x1="6" y1="12" x2="2" y2="12"></line><line x1="12" y1="6" x2="12" y2="2"></line><line x1="12" y1="22" x2="12" y2="18"></line></svg>
</div>
<h3>Rich Interactivity</h3>
<p>Built-in drag points/lines/rects, annotations, axis tags, and a <code>ValueTracker</code> that auto-picks the nearest data point under the cursor &mdash; with multi-subplot linked tracking, all wired through Qt signals.</p>
</div>

</div>
</div>

<div class="qim-showcase">

<div class="qim-section-head">
<h2>Visualization Showcase</h2>
<p>From real-time monitoring to 3D scientific visualization.</p>
</div>

<div class="qim-gallery">

<a class="qim-gallery-item" href="plot2d/plot-line/">
<img src="assets/plot2D/rt.gif" alt="Real-Time Plot" loading="lazy">
<span>Real-Time Plot</span>
</a>

<a class="qim-gallery-item" href="plot2d/plot-bar-charts/">
<img src="assets/plot2D/bars.gif" alt="Bar Chart" loading="lazy">
<span>Bar Chart</span>
</a>

<a class="qim-gallery-item" href="plot2d/plot-basic-charts/">
<img src="assets/plot2D/candle.gif" alt="Candle Chart" loading="lazy">
<span>Candle / K-Line</span>
</a>

<a class="qim-gallery-item" href="plot2d/plot-special-charts/">
<img src="assets/plot2D/pie.gif" alt="Pie Chart" loading="lazy">
<span>Pie Chart</span>
</a>

<a class="qim-gallery-item" href="plot2d/plot-special-charts/">
<img src="assets/plot2D/heat.gif" alt="Heatmap" loading="lazy">
<span>Heatmap</span>
</a>

<a class="qim-gallery-item" href="plot2d/plot-area-charts/">
<img src="assets/plot2D/shaded.gif" alt="Shaded Area" loading="lazy">
<span>Shaded Area</span>
</a>

<a class="qim-gallery-item" href="plot2d/plot-basic-charts/">
<img src="assets/plot2D/stem.gif" alt="Stem Plot" loading="lazy">
<span>Stem Plot</span>
</a>

<a class="qim-gallery-item" href="plot2d/plot-special-charts/">
<img src="assets/plot2D/tables.gif" alt="Tables" loading="lazy">
<span>Tables</span>
</a>

<a class="qim-gallery-item" href="plot3d/">
<img src="assets/plot3D/plot3d-demo1.gif" alt="3D Surface" loading="lazy">
<span>3D Surface</span>
</a>

<a class="qim-gallery-item" href="plot3d/basic-charts/">
<img src="assets/plot3D/plot3d-demo2.gif" alt="3D Scatter" loading="lazy">
<span>3D Scatter</span>
</a>

<a class="qim-gallery-item" href="plot3d/mesh/">
<img src="assets/plot3D/plot3d-demo3.gif" alt="3D Mesh" loading="lazy">
<span>3D Mesh</span>
</a>

<a class="qim-gallery-item" href="plot3d/surface-charts/">
<img src="assets/plot3D/plot3d-demo4.gif" alt="3D Visualization" loading="lazy">
<span>3D Visualization</span>
</a>

</div>

</div>

<div class="qim-quickstart" markdown>

<div class="qim-quickstart-inner" markdown>

<div class="qim-section-head">
<h2>Quick Start</h2>
<p>Integrate QIm into your Qt project in minutes.</p>
</div>

=== "CMake Integration"

    ```cmake
    # Find and link QIm
    find_package(QIm REQUIRED)

    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::OpenGL
        QIm::Core
        QIm::Widgets
    )

    # Qt6 requires additional OpenGLWidgets
    if(${QT_VERSION_MAJOR} EQUAL 6)
        target_link_libraries(${PROJECT_NAME} PRIVATE
            Qt${QT_VERSION_MAJOR}::OpenGLWidgets
        )
    endif()
    ```

=== "First 2D Plot"

    ```cpp
    #include <QImFigureWidget.h>

    // Create figure with 2x2 subplot grid
    auto* figure = new QIM::QImFigureWidget(this);
    figure->setSubplotGrid(2, 2);

    // Subplot 1 - line chart
    if (auto* plot = figure->createPlotNode()) {
        plot->setTitle("Sine Wave");
        std::vector<double> x, y;
        for (int i = 0; i < 400; ++i) {
            double v = i * 2.0 * M_PI / 399.0;
            x.push_back(v);
            y.push_back(std::sin(v));
        }
        plot->addLine(x, y, "sin(x)");
    }

    // Subplot 2 - scatter
    if (auto* plot = figure->createPlotNode()) {
        plot->setTitle("Scatter");
        std::vector<double> x{0.2, 0.5, 0.9, 1.3, 1.8, 2.1, 2.6, 3.0};
        std::vector<double> y{1.4, 1.0, 1.8, 1.3, 2.0, 1.7, 2.3, 2.1};
        auto* scatter = new QIM::QImPlotScatterItemNode(plot);
        scatter->setData(x, y);
        scatter->setColor(QColor(0, 114, 189));
    }
    ```

=== "Custom Node"

    ```cpp
    // Inherit QImAbstractNode to wrap any ImGui component
    class CustomPlotNode : public QImAbstractNode {
        Q_OBJECT
    public:
        CustomPlotNode(QObject* parent = nullptr) : QImAbstractNode(parent) {}

    protected:
        bool beginDraw() override {
            // Maps to ImGui::Begin("MyCustomWindow")
            return ImGui::Begin("MyCustomWindow", nullptr, m_flags);
        }

        void endDraw() override {
            ImGui::End();
        }

    private:
        ImGuiWindowFlags m_flags = 0;
    };
    ```

</div>
</div>

<div class="qim-footer">
<div class="qim-footer-inner">

<h2>Start Building Today</h2>
<p>Explore the documentation or browse the source code.</p>

<div class="qim-footer-links">
<a href="plot2d/" class="qim-btn qim-btn-primary">Read the Docs</a>
<a href="zh/" class="qim-btn qim-btn-ghost">中文文档</a>
<a href="https://github.com/czyt1988/QIm" class="qim-btn qim-btn-ghost">GitHub</a>
</div>

</div>
</div>
