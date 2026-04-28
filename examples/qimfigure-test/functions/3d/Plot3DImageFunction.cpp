#include "Plot3DImageFunction.h"
#include "QImFigureWidget.h"
#include "plot3d/QImPlot3DNode.h"
#include "plot3d/QImPlot3DAxisInfo.h"
#include "plot3d/QImPlot3DImageItemNode.h"
#include "imgui.h"

/**
 * \if ENGLISH
 * @brief Constructor for Plot3DImageFunction
 * @param parent Parent object
 * @details Initializes the test function and registers properties for interactive editing.
 * \endif
 * 
 * \if CHINESE
 * @brief Plot3DImageFunction 构造函数
 * @param parent 父对象
 * @details 初始化测试函数并注册属性用于交互式编辑。
 * \endif
 */
Plot3DImageFunction::Plot3DImageFunction(QObject* parent)
    : TestFunction(parent)
{
    // Register title property
    PropertyRegistration titleReg;
    titleReg.category = tr("Plot");
    titleReg.subcategory = tr("Basic Info");
    titleReg.displayName = tr("Title");
    titleReg.briefDesc = tr("Plot title");
    titleReg.detailDesc = tr("Sets the title text displayed at the top of the 3D plot");
    titleReg.editorType = EditorType::LineEdit;
    titleReg.defaultValue = m_title;
    titleReg.propertyName = "title";
    titleReg.target = this;
    registerProperty(titleReg);
    
    // Register axis labels
    PropertyRegistration xLabelReg;
    xLabelReg.category = tr("Plot"); xLabelReg.subcategory = tr("Axis"); xLabelReg.displayName = tr("X Label");
    xLabelReg.briefDesc = tr("X axis label"); xLabelReg.detailDesc = tr("Sets the label text for the X axis");
    xLabelReg.editorType = EditorType::LineEdit; xLabelReg.defaultValue = m_xLabel;
    xLabelReg.propertyName = "xLabel"; xLabelReg.target = this;
    registerProperty(xLabelReg);
    
    PropertyRegistration yLabelReg;
    yLabelReg.category = tr("Plot"); yLabelReg.subcategory = tr("Axis"); yLabelReg.displayName = tr("Y Label");
    yLabelReg.briefDesc = tr("Y axis label"); yLabelReg.detailDesc = tr("Sets the label text for the Y axis");
    yLabelReg.editorType = EditorType::LineEdit; yLabelReg.defaultValue = m_yLabel;
    yLabelReg.propertyName = "yLabel"; yLabelReg.target = this;
    registerProperty(yLabelReg);
    
    PropertyRegistration zLabelReg;
    zLabelReg.category = tr("Plot"); zLabelReg.subcategory = tr("Axis"); zLabelReg.displayName = tr("Z Label");
    zLabelReg.briefDesc = tr("Z axis label"); zLabelReg.detailDesc = tr("Sets the label text for the Z axis");
    zLabelReg.editorType = EditorType::LineEdit; zLabelReg.defaultValue = m_zLabel;
    zLabelReg.propertyName = "zLabel"; zLabelReg.target = this;
    registerProperty(zLabelReg);
    
    // Register texture ID property
    PropertyRegistration textureIdReg;
    textureIdReg.category = tr("Image"); textureIdReg.subcategory = tr("Texture"); textureIdReg.displayName = tr("Texture ID");
    textureIdReg.briefDesc = tr("Texture ID"); textureIdReg.detailDesc = tr("Sets the GPU texture ID for the image");
    textureIdReg.editorType = EditorType::SpinBox; textureIdReg.defaultValue = m_textureId;
    textureIdReg.minValue = 0; textureIdReg.maxValue = 9999;
    textureIdReg.propertyName = "textureId"; textureIdReg.target = this;
    registerProperty(textureIdReg);
    
    // Register center properties
    PropertyRegistration centerXReg;
    centerXReg.category = tr("Image"); centerXReg.subcategory = tr("Center"); centerXReg.displayName = tr("X");
    centerXReg.briefDesc = tr("Center X"); centerXReg.detailDesc = tr("Sets the X coordinate of the image center in 3D space");
    centerXReg.editorType = EditorType::DoubleSpinBox; centerXReg.defaultValue = m_centerX;
    centerXReg.minValue = -10.0; centerXReg.maxValue = 10.0; centerXReg.stepValue = 0.1;
    centerXReg.propertyName = "centerX"; centerXReg.target = this;
    registerProperty(centerXReg);
    
    PropertyRegistration centerYReg;
    centerYReg.category = tr("Image"); centerYReg.subcategory = tr("Center"); centerYReg.displayName = tr("Y");
    centerYReg.briefDesc = tr("Center Y"); centerYReg.detailDesc = tr("Sets the Y coordinate of the image center in 3D space");
    centerYReg.editorType = EditorType::DoubleSpinBox; centerYReg.defaultValue = m_centerY;
    centerYReg.minValue = -10.0; centerYReg.maxValue = 10.0; centerYReg.stepValue = 0.1;
    centerYReg.propertyName = "centerY"; centerYReg.target = this;
    registerProperty(centerYReg);
    
    PropertyRegistration centerZReg;
    centerZReg.category = tr("Image"); centerZReg.subcategory = tr("Center"); centerZReg.displayName = tr("Z");
    centerZReg.briefDesc = tr("Center Z"); centerZReg.detailDesc = tr("Sets the Z coordinate of the image center in 3D space");
    centerZReg.editorType = EditorType::DoubleSpinBox; centerZReg.defaultValue = m_centerZ;
    centerZReg.minValue = -10.0; centerZReg.maxValue = 10.0; centerZReg.stepValue = 0.1;
    centerZReg.propertyName = "centerZ"; centerZReg.target = this;
    registerProperty(centerZReg);
    
    // Register axis U properties
    PropertyRegistration axisUXReg;
    axisUXReg.category = tr("Image"); axisUXReg.subcategory = tr("Axis U"); axisUXReg.displayName = tr("X");
    axisUXReg.briefDesc = tr("Axis U X"); axisUXReg.detailDesc = tr("Sets the X component of the U axis direction vector");
    axisUXReg.editorType = EditorType::DoubleSpinBox; axisUXReg.defaultValue = m_axisUX;
    axisUXReg.minValue = -5.0; axisUXReg.maxValue = 5.0; axisUXReg.stepValue = 0.1;
    axisUXReg.propertyName = "axisUX"; axisUXReg.target = this;
    registerProperty(axisUXReg);
    
    PropertyRegistration axisUYReg;
    axisUYReg.category = tr("Image"); axisUYReg.subcategory = tr("Axis U"); axisUYReg.displayName = tr("Y");
    axisUYReg.briefDesc = tr("Axis U Y"); axisUYReg.detailDesc = tr("Sets the Y component of the U axis direction vector");
    axisUYReg.editorType = EditorType::DoubleSpinBox; axisUYReg.defaultValue = m_axisUY;
    axisUYReg.minValue = -5.0; axisUYReg.maxValue = 5.0; axisUYReg.stepValue = 0.1;
    axisUYReg.propertyName = "axisUY"; axisUYReg.target = this;
    registerProperty(axisUYReg);
    
    PropertyRegistration axisUZReg;
    axisUZReg.category = tr("Image"); axisUZReg.subcategory = tr("Axis U"); axisUZReg.displayName = tr("Z");
    axisUZReg.briefDesc = tr("Axis U Z"); axisUZReg.detailDesc = tr("Sets the Z component of the U axis direction vector");
    axisUZReg.editorType = EditorType::DoubleSpinBox; axisUZReg.defaultValue = m_axisUZ;
    axisUZReg.minValue = -5.0; axisUZReg.maxValue = 5.0; axisUZReg.stepValue = 0.1;
    axisUZReg.propertyName = "axisUZ"; axisUZReg.target = this;
    registerProperty(axisUZReg);
    
    // Register axis V properties
    PropertyRegistration axisVXReg;
    axisVXReg.category = tr("Image"); axisVXReg.subcategory = tr("Axis V"); axisVXReg.displayName = tr("X");
    axisVXReg.briefDesc = tr("Axis V X"); axisVXReg.detailDesc = tr("Sets the X component of the V axis direction vector");
    axisVXReg.editorType = EditorType::DoubleSpinBox; axisVXReg.defaultValue = m_axisVX;
    axisVXReg.minValue = -5.0; axisVXReg.maxValue = 5.0; axisVXReg.stepValue = 0.1;
    axisVXReg.propertyName = "axisVX"; axisVXReg.target = this;
    registerProperty(axisVXReg);
    
    PropertyRegistration axisVYReg;
    axisVYReg.category = tr("Image"); axisVYReg.subcategory = tr("Axis V"); axisVYReg.displayName = tr("Y");
    axisVYReg.briefDesc = tr("Axis V Y"); axisVYReg.detailDesc = tr("Sets the Y component of the V axis direction vector");
    axisVYReg.editorType = EditorType::DoubleSpinBox; axisVYReg.defaultValue = m_axisVY;
    axisVYReg.minValue = -5.0; axisVYReg.maxValue = 5.0; axisVYReg.stepValue = 0.1;
    axisVYReg.propertyName = "axisVY"; axisVYReg.target = this;
    registerProperty(axisVYReg);
    
    PropertyRegistration axisVZReg;
    axisVZReg.category = tr("Image"); axisVZReg.subcategory = tr("Axis V"); axisVZReg.displayName = tr("Z");
    axisVZReg.briefDesc = tr("Axis V Z"); axisVZReg.detailDesc = tr("Sets the Z component of the V axis direction vector");
    axisVZReg.editorType = EditorType::DoubleSpinBox; axisVZReg.defaultValue = m_axisVZ;
    axisVZReg.minValue = -5.0; axisVZReg.maxValue = 5.0; axisVZReg.stepValue = 0.1;
    axisVZReg.propertyName = "axisVZ"; axisVZReg.target = this;
    registerProperty(axisVZReg);
    
    // Register UV0 properties
    PropertyRegistration uv0XReg;
    uv0XReg.category = tr("Image"); uv0XReg.subcategory = tr("UV0"); uv0XReg.displayName = tr("X");
    uv0XReg.briefDesc = tr("UV0 X"); uv0XReg.detailDesc = tr("Sets the X component of the lower-left texture coordinate");
    uv0XReg.editorType = EditorType::DoubleSpinBox; uv0XReg.defaultValue = m_uv0X;
    uv0XReg.minValue = 0.0; uv0XReg.maxValue = 1.0; uv0XReg.stepValue = 0.01;
    uv0XReg.propertyName = "uv0X"; uv0XReg.target = this;
    registerProperty(uv0XReg);
    
    PropertyRegistration uv0YReg;
    uv0YReg.category = tr("Image"); uv0YReg.subcategory = tr("UV0"); uv0YReg.displayName = tr("Y");
    uv0YReg.briefDesc = tr("UV0 Y"); uv0YReg.detailDesc = tr("Sets the Y component of the lower-left texture coordinate");
    uv0YReg.editorType = EditorType::DoubleSpinBox; uv0YReg.defaultValue = m_uv0Y;
    uv0YReg.minValue = 0.0; uv0YReg.maxValue = 1.0; uv0YReg.stepValue = 0.01;
    uv0YReg.propertyName = "uv0Y"; uv0YReg.target = this;
    registerProperty(uv0YReg);
    
    // Register UV1 properties
    PropertyRegistration uv1XReg;
    uv1XReg.category = tr("Image"); uv1XReg.subcategory = tr("UV1"); uv1XReg.displayName = tr("X");
    uv1XReg.briefDesc = tr("UV1 X"); uv1XReg.detailDesc = tr("Sets the X component of the upper-right texture coordinate");
    uv1XReg.editorType = EditorType::DoubleSpinBox; uv1XReg.defaultValue = m_uv1X;
    uv1XReg.minValue = 0.0; uv1XReg.maxValue = 1.0; uv1XReg.stepValue = 0.01;
    uv1XReg.propertyName = "uv1X"; uv1XReg.target = this;
    registerProperty(uv1XReg);
    
    PropertyRegistration uv1YReg;
    uv1YReg.category = tr("Image"); uv1YReg.subcategory = tr("UV1"); uv1YReg.displayName = tr("Y");
    uv1YReg.briefDesc = tr("UV1 Y"); uv1YReg.detailDesc = tr("Sets the Y component of the upper-right texture coordinate");
    uv1YReg.editorType = EditorType::DoubleSpinBox; uv1YReg.defaultValue = m_uv1Y;
    uv1YReg.minValue = 0.0; uv1YReg.maxValue = 1.0; uv1YReg.stepValue = 0.01;
    uv1YReg.propertyName = "uv1Y"; uv1YReg.target = this;
    registerProperty(uv1YReg);
    
    // Register tint color property
    PropertyRegistration tintColorReg;
    tintColorReg.category = tr("Image"); tintColorReg.subcategory = tr("Style"); tintColorReg.displayName = tr("Tint Color");
    tintColorReg.briefDesc = tr("Tint color"); tintColorReg.detailDesc = tr("Sets the tint color applied to the image texture");
    tintColorReg.editorType = EditorType::ColorPicker; tintColorReg.defaultValue = m_tintColor;
    tintColorReg.propertyName = "tintColor"; tintColorReg.target = this;
    registerProperty(tintColorReg);
    
    // Register quad mode enabled property
    PropertyRegistration quadModeReg;
    quadModeReg.category = tr("Image"); quadModeReg.subcategory = tr("Flags"); quadModeReg.displayName = tr("Quad Mode");
    quadModeReg.briefDesc = tr("Quad mode enabled"); quadModeReg.detailDesc = tr("Enables quad mode for the 3D image (uses corner points instead of center/axis)");
    quadModeReg.editorType = EditorType::CheckBox; quadModeReg.defaultValue = m_quadModeEnabled;
    quadModeReg.propertyName = "quadModeEnabled"; quadModeReg.target = this;
    registerProperty(quadModeReg);
    
    // Register P0 properties
    PropertyRegistration p0xReg;
    p0xReg.category = tr("Image"); p0xReg.subcategory = tr("Point 0"); p0xReg.displayName = tr("X");
    p0xReg.briefDesc = tr("P0 X"); p0xReg.detailDesc = tr("Sets the X coordinate of quad corner point 0");
    p0xReg.editorType = EditorType::DoubleSpinBox; p0xReg.defaultValue = m_p0x;
    p0xReg.minValue = -10.0; p0xReg.maxValue = 10.0; p0xReg.stepValue = 0.1;
    p0xReg.propertyName = "p0x"; p0xReg.target = this;
    registerProperty(p0xReg);
    
    PropertyRegistration p0yReg;
    p0yReg.category = tr("Image"); p0yReg.subcategory = tr("Point 0"); p0yReg.displayName = tr("Y");
    p0yReg.briefDesc = tr("P0 Y"); p0yReg.detailDesc = tr("Sets the Y coordinate of quad corner point 0");
    p0yReg.editorType = EditorType::DoubleSpinBox; p0yReg.defaultValue = m_p0y;
    p0yReg.minValue = -10.0; p0yReg.maxValue = 10.0; p0yReg.stepValue = 0.1;
    p0yReg.propertyName = "p0y"; p0yReg.target = this;
    registerProperty(p0yReg);
    
    PropertyRegistration p0zReg;
    p0zReg.category = tr("Image"); p0zReg.subcategory = tr("Point 0"); p0zReg.displayName = tr("Z");
    p0zReg.briefDesc = tr("P0 Z"); p0zReg.detailDesc = tr("Sets the Z coordinate of quad corner point 0");
    p0zReg.editorType = EditorType::DoubleSpinBox; p0zReg.defaultValue = m_p0z;
    p0zReg.minValue = -10.0; p0zReg.maxValue = 10.0; p0zReg.stepValue = 0.1;
    p0zReg.propertyName = "p0z"; p0zReg.target = this;
    registerProperty(p0zReg);
    
    // Register P1 properties
    PropertyRegistration p1xReg;
    p1xReg.category = tr("Image"); p1xReg.subcategory = tr("Point 1"); p1xReg.displayName = tr("X");
    p1xReg.briefDesc = tr("P1 X"); p1xReg.detailDesc = tr("Sets the X coordinate of quad corner point 1");
    p1xReg.editorType = EditorType::DoubleSpinBox; p1xReg.defaultValue = m_p1x;
    p1xReg.minValue = -10.0; p1xReg.maxValue = 10.0; p1xReg.stepValue = 0.1;
    p1xReg.propertyName = "p1x"; p1xReg.target = this;
    registerProperty(p1xReg);
    
    PropertyRegistration p1yReg;
    p1yReg.category = tr("Image"); p1yReg.subcategory = tr("Point 1"); p1yReg.displayName = tr("Y");
    p1yReg.briefDesc = tr("P1 Y"); p1yReg.detailDesc = tr("Sets the Y coordinate of quad corner point 1");
    p1yReg.editorType = EditorType::DoubleSpinBox; p1yReg.defaultValue = m_p1y;
    p1yReg.minValue = -10.0; p1yReg.maxValue = 10.0; p1yReg.stepValue = 0.1;
    p1yReg.propertyName = "p1y"; p1yReg.target = this;
    registerProperty(p1yReg);
    
    PropertyRegistration p1zReg;
    p1zReg.category = tr("Image"); p1zReg.subcategory = tr("Point 1"); p1zReg.displayName = tr("Z");
    p1zReg.briefDesc = tr("P1 Z"); p1zReg.detailDesc = tr("Sets the Z coordinate of quad corner point 1");
    p1zReg.editorType = EditorType::DoubleSpinBox; p1zReg.defaultValue = m_p1z;
    p1zReg.minValue = -10.0; p1zReg.maxValue = 10.0; p1zReg.stepValue = 0.1;
    p1zReg.propertyName = "p1z"; p1zReg.target = this;
    registerProperty(p1zReg);
    
    // Register P2 properties
    PropertyRegistration p2xReg;
    p2xReg.category = tr("Image"); p2xReg.subcategory = tr("Point 2"); p2xReg.displayName = tr("X");
    p2xReg.briefDesc = tr("P2 X"); p2xReg.detailDesc = tr("Sets the X coordinate of quad corner point 2");
    p2xReg.editorType = EditorType::DoubleSpinBox; p2xReg.defaultValue = m_p2x;
    p2xReg.minValue = -10.0; p2xReg.maxValue = 10.0; p2xReg.stepValue = 0.1;
    p2xReg.propertyName = "p2x"; p2xReg.target = this;
    registerProperty(p2xReg);
    
    PropertyRegistration p2yReg;
    p2yReg.category = tr("Image"); p2yReg.subcategory = tr("Point 2"); p2yReg.displayName = tr("Y");
    p2yReg.briefDesc = tr("P2 Y"); p2yReg.detailDesc = tr("Sets the Y coordinate of quad corner point 2");
    p2yReg.editorType = EditorType::DoubleSpinBox; p2yReg.defaultValue = m_p2y;
    p2yReg.minValue = -10.0; p2yReg.maxValue = 10.0; p2yReg.stepValue = 0.1;
    p2yReg.propertyName = "p2y"; p2yReg.target = this;
    registerProperty(p2yReg);
    
    PropertyRegistration p2zReg;
    p2zReg.category = tr("Image"); p2zReg.subcategory = tr("Point 2"); p2zReg.displayName = tr("Z");
    p2zReg.briefDesc = tr("P2 Z"); p2zReg.detailDesc = tr("Sets the Z coordinate of quad corner point 2");
    p2zReg.editorType = EditorType::DoubleSpinBox; p2zReg.defaultValue = m_p2z;
    p2zReg.minValue = -10.0; p2zReg.maxValue = 10.0; p2zReg.stepValue = 0.1;
    p2zReg.propertyName = "p2z"; p2zReg.target = this;
    registerProperty(p2zReg);
    
    // Register P3 properties
    PropertyRegistration p3xReg;
    p3xReg.category = tr("Image"); p3xReg.subcategory = tr("Point 3"); p3xReg.displayName = tr("X");
    p3xReg.briefDesc = tr("P3 X"); p3xReg.detailDesc = tr("Sets the X coordinate of quad corner point 3");
    p3xReg.editorType = EditorType::DoubleSpinBox; p3xReg.defaultValue = m_p3x;
    p3xReg.minValue = -10.0; p3xReg.maxValue = 10.0; p3xReg.stepValue = 0.1;
    p3xReg.propertyName = "p3x"; p3xReg.target = this;
    registerProperty(p3xReg);
    
    PropertyRegistration p3yReg;
    p3yReg.category = tr("Image"); p3yReg.subcategory = tr("Point 3"); p3yReg.displayName = tr("Y");
    p3yReg.briefDesc = tr("P3 Y"); p3yReg.detailDesc = tr("Sets the Y coordinate of quad corner point 3");
    p3yReg.editorType = EditorType::DoubleSpinBox; p3yReg.defaultValue = m_p3y;
    p3yReg.minValue = -10.0; p3yReg.maxValue = 10.0; p3yReg.stepValue = 0.1;
    p3yReg.propertyName = "p3y"; p3yReg.target = this;
    registerProperty(p3yReg);
    
    PropertyRegistration p3zReg;
    p3zReg.category = tr("Image"); p3zReg.subcategory = tr("Point 3"); p3zReg.displayName = tr("Z");
    p3zReg.briefDesc = tr("P3 Z"); p3zReg.detailDesc = tr("Sets the Z coordinate of quad corner point 3");
    p3zReg.editorType = EditorType::DoubleSpinBox; p3zReg.defaultValue = m_p3z;
    p3zReg.minValue = -10.0; p3zReg.maxValue = 10.0; p3zReg.stepValue = 0.1;
    p3zReg.propertyName = "p3z"; p3zReg.target = this;
    registerProperty(p3zReg);
    
    // Register UV Point 0 properties
    PropertyRegistration uvP0xReg;
    uvP0xReg.category = tr("Image"); uvP0xReg.subcategory = tr("UV Point 0"); uvP0xReg.displayName = tr("X");
    uvP0xReg.briefDesc = tr("UV P0 X"); uvP0xReg.detailDesc = tr("Sets the X component of UV coordinate for corner point 0");
    uvP0xReg.editorType = EditorType::DoubleSpinBox; uvP0xReg.defaultValue = m_uvP0x;
    uvP0xReg.minValue = 0.0; uvP0xReg.maxValue = 1.0; uvP0xReg.stepValue = 0.01;
    uvP0xReg.propertyName = "uvP0x"; uvP0xReg.target = this;
    registerProperty(uvP0xReg);
    
    PropertyRegistration uvP0yReg;
    uvP0yReg.category = tr("Image"); uvP0yReg.subcategory = tr("UV Point 0"); uvP0yReg.displayName = tr("Y");
    uvP0yReg.briefDesc = tr("UV P0 Y"); uvP0yReg.detailDesc = tr("Sets the Y component of UV coordinate for corner point 0");
    uvP0yReg.editorType = EditorType::DoubleSpinBox; uvP0yReg.defaultValue = m_uvP0y;
    uvP0yReg.minValue = 0.0; uvP0yReg.maxValue = 1.0; uvP0yReg.stepValue = 0.01;
    uvP0yReg.propertyName = "uvP0y"; uvP0yReg.target = this;
    registerProperty(uvP0yReg);
    
    // Register UV Point 1 properties
    PropertyRegistration uvP1xReg;
    uvP1xReg.category = tr("Image"); uvP1xReg.subcategory = tr("UV Point 1"); uvP1xReg.displayName = tr("X");
    uvP1xReg.briefDesc = tr("UV P1 X"); uvP1xReg.detailDesc = tr("Sets the X component of UV coordinate for corner point 1");
    uvP1xReg.editorType = EditorType::DoubleSpinBox; uvP1xReg.defaultValue = m_uvP1x;
    uvP1xReg.minValue = 0.0; uvP1xReg.maxValue = 1.0; uvP1xReg.stepValue = 0.01;
    uvP1xReg.propertyName = "uvP1x"; uvP1xReg.target = this;
    registerProperty(uvP1xReg);
    
    PropertyRegistration uvP1yReg;
    uvP1yReg.category = tr("Image"); uvP1yReg.subcategory = tr("UV Point 1"); uvP1yReg.displayName = tr("Y");
    uvP1yReg.briefDesc = tr("UV P1 Y"); uvP1yReg.detailDesc = tr("Sets the Y component of UV coordinate for corner point 1");
    uvP1yReg.editorType = EditorType::DoubleSpinBox; uvP1yReg.defaultValue = m_uvP1y;
    uvP1yReg.minValue = 0.0; uvP1yReg.maxValue = 1.0; uvP1yReg.stepValue = 0.01;
    uvP1yReg.propertyName = "uvP1y"; uvP1yReg.target = this;
    registerProperty(uvP1yReg);
    
    // Register UV Point 2 properties
    PropertyRegistration uvP2xReg;
    uvP2xReg.category = tr("Image"); uvP2xReg.subcategory = tr("UV Point 2"); uvP2xReg.displayName = tr("X");
    uvP2xReg.briefDesc = tr("UV P2 X"); uvP2xReg.detailDesc = tr("Sets the X component of UV coordinate for corner point 2");
    uvP2xReg.editorType = EditorType::DoubleSpinBox; uvP2xReg.defaultValue = m_uvP2x;
    uvP2xReg.minValue = 0.0; uvP2xReg.maxValue = 1.0; uvP2xReg.stepValue = 0.01;
    uvP2xReg.propertyName = "uvP2x"; uvP2xReg.target = this;
    registerProperty(uvP2xReg);
    
    PropertyRegistration uvP2yReg;
    uvP2yReg.category = tr("Image"); uvP2yReg.subcategory = tr("UV Point 2"); uvP2yReg.displayName = tr("Y");
    uvP2yReg.briefDesc = tr("UV P2 Y"); uvP2yReg.detailDesc = tr("Sets the Y component of UV coordinate for corner point 2");
    uvP2yReg.editorType = EditorType::DoubleSpinBox; uvP2yReg.defaultValue = m_uvP2y;
    uvP2yReg.minValue = 0.0; uvP2yReg.maxValue = 1.0; uvP2yReg.stepValue = 0.01;
    uvP2yReg.propertyName = "uvP2y"; uvP2yReg.target = this;
    registerProperty(uvP2yReg);
    
    // Register UV Point 3 properties
    PropertyRegistration uvP3xReg;
    uvP3xReg.category = tr("Image"); uvP3xReg.subcategory = tr("UV Point 3"); uvP3xReg.displayName = tr("X");
    uvP3xReg.briefDesc = tr("UV P3 X"); uvP3xReg.detailDesc = tr("Sets the X component of UV coordinate for corner point 3");
    uvP3xReg.editorType = EditorType::DoubleSpinBox; uvP3xReg.defaultValue = m_uvP3x;
    uvP3xReg.minValue = 0.0; uvP3xReg.maxValue = 1.0; uvP3xReg.stepValue = 0.01;
    uvP3xReg.propertyName = "uvP3x"; uvP3xReg.target = this;
    registerProperty(uvP3xReg);
    
    PropertyRegistration uvP3yReg;
    uvP3yReg.category = tr("Image"); uvP3yReg.subcategory = tr("UV Point 3"); uvP3yReg.displayName = tr("Y");
    uvP3yReg.briefDesc = tr("UV P3 Y"); uvP3yReg.detailDesc = tr("Sets the Y component of UV coordinate for corner point 3");
    uvP3yReg.editorType = EditorType::DoubleSpinBox; uvP3yReg.defaultValue = m_uvP3y;
    uvP3yReg.minValue = 0.0; uvP3yReg.maxValue = 1.0; uvP3yReg.stepValue = 0.01;
    uvP3yReg.propertyName = "uvP3y"; uvP3yReg.target = this;
    registerProperty(uvP3yReg);
}

Plot3DImageFunction::~Plot3DImageFunction() {}

void Plot3DImageFunction::createPlot(QIM::QImFigureWidget* figure)
{
    if (!figure) { return; }
    figure->setSubplot3DGrid(1, 1);
    m_plot3DNode = figure->createPlot3DNode();
    m_plot3DNode->xAxis()->setLabel(m_xLabel);
    m_plot3DNode->yAxis()->setLabel(m_yLabel);
    m_plot3DNode->zAxis()->setLabel(m_zLabel);
    m_plot3DNode->setTitle(m_title);
    m_plot3DNode->setBoxRotation(35.264, 45.0);
    
    m_image3DNode = new QIM::QImPlot3DImageItemNode(m_plot3DNode);
    ImTextureID fontTexId = ImGui::GetIO().Fonts->TexRef.GetTexID();
    m_image3DNode->setTextureId(static_cast<quintptr>(fontTexId));
    m_image3DNode->setCenterX(m_centerX);
    m_image3DNode->setCenterY(m_centerY);
    m_image3DNode->setCenterZ(m_centerZ);
    m_image3DNode->setAxisUX(m_axisUX);
    m_image3DNode->setAxisUY(m_axisUY);
    m_image3DNode->setAxisUZ(m_axisUZ);
    m_image3DNode->setAxisVX(m_axisVX);
    m_image3DNode->setAxisVY(m_axisVY);
    m_image3DNode->setAxisVZ(m_axisVZ);
    m_image3DNode->setTintColor(m_tintColor);
    m_image3DNode->setUv0X(m_uv0X);
    m_image3DNode->setUv0Y(m_uv0Y);
    m_image3DNode->setUv1X(m_uv1X);
    m_image3DNode->setUv1Y(m_uv1Y);
    m_image3DNode->setQuadModeEnabled(m_quadModeEnabled);
    m_image3DNode->setP0x(m_p0x);
    m_image3DNode->setP0y(m_p0y);
    m_image3DNode->setP0z(m_p0z);
    m_image3DNode->setP1x(m_p1x);
    m_image3DNode->setP1y(m_p1y);
    m_image3DNode->setP1z(m_p1z);
    m_image3DNode->setP2x(m_p2x);
    m_image3DNode->setP2y(m_p2y);
    m_image3DNode->setP2z(m_p2z);
    m_image3DNode->setP3x(m_p3x);
    m_image3DNode->setP3y(m_p3y);
    m_image3DNode->setP3z(m_p3z);
    m_image3DNode->setUvP0x(m_uvP0x);
    m_image3DNode->setUvP0y(m_uvP0y);
    m_image3DNode->setUvP1x(m_uvP1x);
    m_image3DNode->setUvP1y(m_uvP1y);
    m_image3DNode->setUvP2x(m_uvP2x);
    m_image3DNode->setUvP2y(m_uvP2y);
    m_image3DNode->setUvP3x(m_uvP3x);
    m_image3DNode->setUvP3y(m_uvP3y);
}

// Setters - standard pattern
void Plot3DImageFunction::setTitle(const QString& title) {
    if (m_title != title) { m_title = title; Q_EMIT titleChanged(title); if (m_plot3DNode) m_plot3DNode->setTitle(title); }
}
void Plot3DImageFunction::setXLabel(const QString& label) {
    if (m_xLabel != label) { m_xLabel = label; Q_EMIT xLabelChanged(label); if (m_plot3DNode) m_plot3DNode->xAxis()->setLabel(label); }
}
void Plot3DImageFunction::setYLabel(const QString& label) {
    if (m_yLabel != label) { m_yLabel = label; Q_EMIT yLabelChanged(label); if (m_plot3DNode) m_plot3DNode->yAxis()->setLabel(label); }
}
void Plot3DImageFunction::setZLabel(const QString& label) {
    if (m_zLabel != label) { m_zLabel = label; Q_EMIT zLabelChanged(label); if (m_plot3DNode) m_plot3DNode->zAxis()->setLabel(label); }
}
void Plot3DImageFunction::setTextureId(int id) {
    if (m_textureId != id) { m_textureId = id; Q_EMIT textureIdChanged(id); if (m_image3DNode) m_image3DNode->setTextureId(static_cast<quintptr>(id)); }
}
void Plot3DImageFunction::setCenterX(double x) {
    if (m_centerX != x) { m_centerX = x; Q_EMIT centerXChanged(x); if (m_image3DNode) m_image3DNode->setCenterX(x); }
}
void Plot3DImageFunction::setCenterY(double y) {
    if (m_centerY != y) { m_centerY = y; Q_EMIT centerYChanged(y); if (m_image3DNode) m_image3DNode->setCenterY(y); }
}
void Plot3DImageFunction::setCenterZ(double z) {
    if (m_centerZ != z) { m_centerZ = z; Q_EMIT centerZChanged(z); if (m_image3DNode) m_image3DNode->setCenterZ(z); }
}
void Plot3DImageFunction::setAxisUX(double x) {
    if (m_axisUX != x) { m_axisUX = x; Q_EMIT axisUXChanged(x); if (m_image3DNode) m_image3DNode->setAxisUX(x); }
}
void Plot3DImageFunction::setAxisUY(double y) {
    if (m_axisUY != y) { m_axisUY = y; Q_EMIT axisUYChanged(y); if (m_image3DNode) m_image3DNode->setAxisUY(y); }
}
void Plot3DImageFunction::setAxisUZ(double z) {
    if (m_axisUZ != z) { m_axisUZ = z; Q_EMIT axisUZChanged(z); if (m_image3DNode) m_image3DNode->setAxisUZ(z); }
}
void Plot3DImageFunction::setAxisVX(double x) {
    if (m_axisVX != x) { m_axisVX = x; Q_EMIT axisVXChanged(x); if (m_image3DNode) m_image3DNode->setAxisVX(x); }
}
void Plot3DImageFunction::setAxisVY(double y) {
    if (m_axisVY != y) { m_axisVY = y; Q_EMIT axisVYChanged(y); if (m_image3DNode) m_image3DNode->setAxisVY(y); }
}
void Plot3DImageFunction::setAxisVZ(double z) {
    if (m_axisVZ != z) { m_axisVZ = z; Q_EMIT axisVZChanged(z); if (m_image3DNode) m_image3DNode->setAxisVZ(z); }
}
void Plot3DImageFunction::setUv0X(double x) {
    if (m_uv0X != x) { m_uv0X = x; Q_EMIT uv0XChanged(x); if (m_image3DNode) m_image3DNode->setUv0X(x); }
}
void Plot3DImageFunction::setUv0Y(double y) {
    if (m_uv0Y != y) { m_uv0Y = y; Q_EMIT uv0YChanged(y); if (m_image3DNode) m_image3DNode->setUv0Y(y); }
}
void Plot3DImageFunction::setUv1X(double x) {
    if (m_uv1X != x) { m_uv1X = x; Q_EMIT uv1XChanged(x); if (m_image3DNode) m_image3DNode->setUv1X(x); }
}
void Plot3DImageFunction::setUv1Y(double y) {
    if (m_uv1Y != y) { m_uv1Y = y; Q_EMIT uv1YChanged(y); if (m_image3DNode) m_image3DNode->setUv1Y(y); }
}
void Plot3DImageFunction::setTintColor(const QColor& color) {
    if (m_tintColor != color) { m_tintColor = color; Q_EMIT tintColorChanged(color); if (m_image3DNode) m_image3DNode->setTintColor(color); }
}
void Plot3DImageFunction::setQuadModeEnabled(bool enabled) {
    if (m_quadModeEnabled != enabled) { m_quadModeEnabled = enabled; Q_EMIT quadModeEnabledChanged(enabled); if (m_image3DNode) m_image3DNode->setQuadModeEnabled(enabled); }
}
void Plot3DImageFunction::setP0x(double x) {
    if (m_p0x != x) { m_p0x = x; Q_EMIT p0xChanged(x); if (m_image3DNode) m_image3DNode->setP0x(x); }
}
void Plot3DImageFunction::setP0y(double y) {
    if (m_p0y != y) { m_p0y = y; Q_EMIT p0yChanged(y); if (m_image3DNode) m_image3DNode->setP0y(y); }
}
void Plot3DImageFunction::setP0z(double z) {
    if (m_p0z != z) { m_p0z = z; Q_EMIT p0zChanged(z); if (m_image3DNode) m_image3DNode->setP0z(z); }
}
void Plot3DImageFunction::setP1x(double x) {
    if (m_p1x != x) { m_p1x = x; Q_EMIT p1xChanged(x); if (m_image3DNode) m_image3DNode->setP1x(x); }
}
void Plot3DImageFunction::setP1y(double y) {
    if (m_p1y != y) { m_p1y = y; Q_EMIT p1yChanged(y); if (m_image3DNode) m_image3DNode->setP1y(y); }
}
void Plot3DImageFunction::setP1z(double z) {
    if (m_p1z != z) { m_p1z = z; Q_EMIT p1zChanged(z); if (m_image3DNode) m_image3DNode->setP1z(z); }
}
void Plot3DImageFunction::setP2x(double x) {
    if (m_p2x != x) { m_p2x = x; Q_EMIT p2xChanged(x); if (m_image3DNode) m_image3DNode->setP2x(x); }
}
void Plot3DImageFunction::setP2y(double y) {
    if (m_p2y != y) { m_p2y = y; Q_EMIT p2yChanged(y); if (m_image3DNode) m_image3DNode->setP2y(y); }
}
void Plot3DImageFunction::setP2z(double z) {
    if (m_p2z != z) { m_p2z = z; Q_EMIT p2zChanged(z); if (m_image3DNode) m_image3DNode->setP2z(z); }
}
void Plot3DImageFunction::setP3x(double x) {
    if (m_p3x != x) { m_p3x = x; Q_EMIT p3xChanged(x); if (m_image3DNode) m_image3DNode->setP3x(x); }
}
void Plot3DImageFunction::setP3y(double y) {
    if (m_p3y != y) { m_p3y = y; Q_EMIT p3yChanged(y); if (m_image3DNode) m_image3DNode->setP3y(y); }
}
void Plot3DImageFunction::setP3z(double z) {
    if (m_p3z != z) { m_p3z = z; Q_EMIT p3zChanged(z); if (m_image3DNode) m_image3DNode->setP3z(z); }
}
void Plot3DImageFunction::setUvP0x(double x) {
    if (m_uvP0x != x) { m_uvP0x = x; Q_EMIT uvP0xChanged(x); if (m_image3DNode) m_image3DNode->setUvP0x(x); }
}
void Plot3DImageFunction::setUvP0y(double y) {
    if (m_uvP0y != y) { m_uvP0y = y; Q_EMIT uvP0yChanged(y); if (m_image3DNode) m_image3DNode->setUvP0y(y); }
}
void Plot3DImageFunction::setUvP1x(double x) {
    if (m_uvP1x != x) { m_uvP1x = x; Q_EMIT uvP1xChanged(x); if (m_image3DNode) m_image3DNode->setUvP1x(x); }
}
void Plot3DImageFunction::setUvP1y(double y) {
    if (m_uvP1y != y) { m_uvP1y = y; Q_EMIT uvP1yChanged(y); if (m_image3DNode) m_image3DNode->setUvP1y(y); }
}
void Plot3DImageFunction::setUvP2x(double x) {
    if (m_uvP2x != x) { m_uvP2x = x; Q_EMIT uvP2xChanged(x); if (m_image3DNode) m_image3DNode->setUvP2x(x); }
}
void Plot3DImageFunction::setUvP2y(double y) {
    if (m_uvP2y != y) { m_uvP2y = y; Q_EMIT uvP2yChanged(y); if (m_image3DNode) m_image3DNode->setUvP2y(y); }
}
void Plot3DImageFunction::setUvP3x(double x) {
    if (m_uvP3x != x) { m_uvP3x = x; Q_EMIT uvP3xChanged(x); if (m_image3DNode) m_image3DNode->setUvP3x(x); }
}
void Plot3DImageFunction::setUvP3y(double y) {
    if (m_uvP3y != y) { m_uvP3y = y; Q_EMIT uvP3yChanged(y); if (m_image3DNode) m_image3DNode->setUvP3y(y); }
}

void Plot3DImageFunction::cleanupPlot()
{
    TestFunction::cleanupPlot();
    m_plot3DNode = nullptr;
    m_image3DNode = nullptr;
}