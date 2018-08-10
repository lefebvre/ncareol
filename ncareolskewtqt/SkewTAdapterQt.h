// SkewTAdapterQt.h: interface for the SkewTAdapterQt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(SkewTAdapterQt_INCLUDED_)
#define SkewTAdapterQt_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#ifdef SKEWTQT5
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QPen>
#include <QtPrintSupport/QPrinter>
#include <QtWidgets/QWidget>
#else
#include <QPainter>
#include <QPen>
#include <QPrinter>
#include <QTimer>
#include <QWidget>
#endif

#include "ncareolskewt/SkewT.h"
#include "ncareolskewt/SkewTAdapter.h"

class QRubberBand;
class QPaintEvent;
class QMouseEvent;
class QResizeEvent;

namespace skewt
{
/**
 * This implementation of SkewTAdapter draws the SkewT using a Qt Qwidget.
 * SkewTAdapterQt is derived from both SkewTAdapter and QWidget.
 *
 * The QWidget provides a blank area that the skew-t will be drawn. A QPainter
 *performs the actual drawing. Fortunately, QPainter provides an adequate
 *collection of primitives for drawing polylines, text, etc.
 *
 * At the heart of this adapter, all drawing activities are broken down into a
 *small number of graphic elements. These are strictly graphic constructs;
 *SkewTAdapterQt really has very little (but some) specialized knowledge of what
 *a skew-t graph is.
 *
 * All coordinates are given as a value between 0 and 1. Whenever drawing takse
 *place, these are converted to screen coordinates through scaling by the
 *QWidget height or width.
 *
 * One of the nicest features of the Qt model is the builtin support for
 *resizing, which is handled by resize and paint event handlers. Naturally, to
 *exploit this dynamic resizing capability, a certain amount of imformation
 *about each graphic element must be stored, so that the complete plot can be
 *regenerated when the geometry changes. The elements that are stored within
 *SkewTAdapterQt are: <ul> <li> Texts: strings for labelling anything on the
 *graph <li> Polylines: arbitrary lines with specified color and
 *characteristics. These are used for isopleths and any other fixed lines. <li>
 *Tdry points: A collection of temperature values to be plotted as the
 *temperature trace. <li> DP points: A collection of dew point values to be
 *plotted as the dew point trace.
 *</ul>
 *
 * SkewTAdapterQt defines some helper classes (SkewTQtText, SkewTQtPolyline, and
 * SkewTQtDatum) which facilitate the storage and rendering of the graphic
 *elements. Lists of these elemnts are managed by standard STL containers.
 *
 * <h3>The Resizing Issue</h3>
 * I ran across a common resizing issue. The skew-t plot, which renders data
 * in a fairly complex fashion, must expend a significant amount of computation
 * to regenerate the diagram, when the window is resized. When a window is being
 * resized, Qt sends pairs of resize/paint events to the SkewTAdapterQt.
 *
 * Now this would be fine, if Qt only sent the events after the final size of
 * the widget had been established.  However, during a resize, many events get
 * sent as the user drags the window border. This is seen to happen
 * when the SkewtAdapterQt is managed as by a layout, within a workspace widget.
 *
 * A workaround is employed here, at least until we find a way to eliminate the
 *sending of multiple paint events. (Trolltech said that they might add this
 *capability to QWorkspace for us, in a post Qt 4.0 version!).
 *
 * The technique uses a timer and a "dontPaint" flag. If the dontPaint flag is
 *true, a paint event will not redraw the display. Every time a resize event
 *occurs, dontPaint is set true, and the timer is started. The dontPaint flag
 *will continue to be set true as long as resize events are received before the
 *timer run out. When the timer finally is able to time out, dontPaint is
 *returned to its false state, and a redraw is initiated.
 *
 * SkewTAdapterQt provides zooming support. When it detects a mouse press it
 *captures the starting position. Then it draws a box as the mouse is dragged.
 *When the mouse is released, the stop position is recorded, and then calls the
 *SkewT::zoomin() function. Thus, this adapter does know about the SkewT. We
 *could have built an opaque interface to hide SkewT from the adapter, but it
 *doesn't seem necessary.
 *
 *
 * @todo Investigate whether QCanvas would be a better graphics component for
 *this application.
 * @todo Find out if a QPainter can be interrogated for the device limits (width
 *and height). It seems foolish to have to send a QPainter, and width and
 *height, into the draw functions.
 * @todo The paint event handler gets called no fewer than 4 times during the
 *initial construction, and this is after all of the graphic elements have been
 *defined. Since draw_finished() is called from paintEvent(). the complete plot
 *is drawn four times in a row. The drawing procedure should be modified, to
 *draw into a QPixmap, and then just have paintEvent() bitBlt this pixmap.
 */
class SkewTAdapterQt : public QWidget, public SkewTAdapter
{
  Q_OBJECT

 public:
  /**
   * Construct the adapter.
   * @param parent The parent for the QWidget.
   * @param h The height of drawing area. if -1, use the height of _skewtFrame
   * on the Aspen SkewtView
   * @param w The width of drawing area, if -1, use the width of _skewtFrame on
   * the Aspen SkewtView
   * @param symbolSize The size, in pixels, that symbols will be drawn at.
   * @param resizeHoldOffMs The delay time, after a resize event, before a paint
   * event can be performed.
   */
  SkewTAdapterQt(QWidget* parent, int h = -1, int w = -1, int symbolSize = 5,
                 int resizeHoldOffMs = 500);

  /**
   * Destructor
   */
  virtual ~SkewTAdapterQt();

  void addDp(double x, double y);

  void addTdry(double x, double y);

  void print(){};

  /**
   * If the current _title is empty, and title is non-empty, tempporarily set
   * _title=title for the printing.
   **/
  void print(QPrinter* printer, std::string title);

  double aspectRatio();

  void subTitle(std::string s);

  void title(std::string s);

  void symbol(double x, double y, unsigned int color, SymType st = SmallPlus);

  void Text(const std::string& s, const double x, const double y,
            unsigned int color);

  void extents(double& xmin, double& xmax, double& ymin, double& ymax);

  void unzoom();

  void maximize();

  void draw_finished();

  void init();

  uint getSymbolSize() { return _symbolSize; }
  void setSymbolSize(int s) { _symbolSize = s; }

  /**
   * This function is clever. It tracks the incoming requests,
   * and consolidates them into a single polyline when  possible.
   */
  void line(double x1, double y1, double x2, double y2, unsigned int colorCode,
            SkewTAdapter::LineType lineType = SkewTAdapter::SolidLine);

  void savePlot(std::string path, int xPixels, int yPixels,
                PlotFileType fileType);

  void setSkewT(SkewT* pSkewT);

 public slots:
  /**
   * Draw symbols at the data points for tdry and dp
   * @param flag True if yes, false if no.
   */
  void markPoints(bool flag);
  /**
   * Draw lines between the data points for tdry and dp
   * @param flag True if yes, false if no.
   */
  void drawLines(bool flag);

 protected slots:
  /**
   * This slot is called when the resize event timer has completed.
   */
  void resizeTimeout();

 protected:
  /**
   * A little helper class which carries along extra information
   * that contain some rendering state. This is useful for managing
   * incremental plotting of the various elements of the plot.
   */
  template <class T>
  class VectorPlus : public std::vector<T>
  {
   public:
    /// Consructor
    VectorPlus() { _next = 0; };
    /// Destructor
    virtual ~VectorPlus(){};
    /// remove all items and reset the next draw index.
    virtual void clear()
    {
      _next = 0;
      std::vector<T>::clear();
    };
    /// The next item in the vector that needs to be drawn.
    /// The rendering routine will start at this index,
    /// render all objects up to the end of the vector,
    /// and set this to one plus the last index.
    /// Routines that want to force rendering from the beginning
    /// such as after a zoom or resize, will reset this to zero.
    int _next;
  };

  /**
   * A text graphic element.
   */
  class SkewTQtText
  {
   public:
    /**
     * Constructor.
     * @param text The text.
     * @param x The x location.
     * @param y The y location
     * @param pen The pen for drawing the text.
     * @param alignFlag An alignment flag.
     * @todo Implement the alignment flag.
     */
    SkewTQtText(std::string text, double x, double y, QPen pen, int alignFlag);
    /**
     * Default constructor
     */
    SkewTQtText();
    /**
     * Destructor
     */
    virtual ~SkewTQtText();
    /**
     * @return The size of the text
     **/
    int size();
    /**
     * Draw the text.
     * @param painter The device to draw on.
     * @param width The width of the drawing area.
     * @param height The height of the drawing area.
     */
    void draw(QPainter& painter, int width, int height);

   protected:
    std::string _text;
    double _x;
    double _y;
    QPen _pen;
    int _alignFlag;
  };

  /**
   * A polyline graphic element. It contains a series of points which will
   * be connected by a line,
   */
  class SkewTQtPolyline
  {
   public:
    /**
     * Constructor.
     * @param x The x locations.
     * @param y The y locations
     * @param pen The pen for drawing the text.
     */
    SkewTQtPolyline(std::vector<double> x, std::vector<double> y, QPen pen);
    /**
     * Destructor
     */
    virtual ~SkewTQtPolyline();
    /**
     * Draw the polyline.
     * @param painter The device to draw on.
     * @param width The width of the drawing area.
     * @param height The height of the drawing area.
     * @todo Implement dashed line drawing.
     */
    void draw(QPainter& painter, int width, int height);

   protected:
    std::vector<double> _x;  ///< The x values.
    std::vector<double> _y;  ///< The y values.
    QPen _pen;               ///< The pen used for drawing the line.
  };

  /**
   * A graphic element, used to represent a data point. A collection
   * of SkewTQtDatum will represent the tdry trace; another collection
   * will represent the dew point trace.
   */
  class SkewTQtDatum
  {
   public:
    /**
     * Constructor.
     * @param x The x location.
     * @param y The y location
     * @param size The size of the symbol, in pixels.
     * @param pen The pen for drawing the datum.
     * @param brush The brush for drawing the datum. This establishes the fill
     * pattern..
     */
    SkewTQtDatum(double x, double y, int size, QPen pen, QBrush brush);
    /**
     * Destructor.
     */
    virtual ~SkewTQtDatum();
    /**
     * Draw the datum.
     * @param painter The device to draw on.
     * @param width The width of the drawing area.
     * @param height The height of the drawing area.
     */
    void draw(QPainter& painter, int width, int height);
    /**
     * @return The x coordinate
     */
    double x();
    /**
     * @return The y coordinate
     */
    double y();

   protected:
    double _x;      ///< The x location
    double _y;      ///< The y location
    int _size;      ///< The size of the symbol, in pixels
    QPen _pen;      ///< The pen for drawing.
    QBrush _brush;  ///< the brush for drawing.
  };

  /**
   * Called by Qt when there is a paint event. The pixmap
   * will be copied to the widget.
   * @param e The paint event
   */
  void paintEvent(QPaintEvent* e);
  /**
   * Called by Qt when there is a resize event. The dontPaint flag is set true,
   *and a timer is initiated. When the timer completes, the dontPaint flag is
   *cleared, the skewt elements are rendered into the pixmap, and an update() is
   *called to generate a paint event.
   *@param e Resize event.
   */
  void resizeEvent(QResizeEvent* e);
  /**
   * Called when the user presses the mouse.
   */
  void mousePressEvent(QMouseEvent* e);
  /**
   * Called when the user releases the mouse.
   */
  void mouseReleaseEvent(QMouseEvent* e);
  /**
   * Called as then mouse is dragged.
   */
  void mouseMoveEvent(QMouseEvent* e);
  /**
   * Draw the rubberband rectangle as the mouse is dragged.
   */
  void drawBoundingRect(QRubberBand* p, const QPoint& p1, const QPoint& p2);
  /**
   * Return the QColor equivalent of the SkewT color code. See SkewTdefs.h
   * @return The coresponding QColor.
   */
  QColor getQColor(unsigned int colorCode);
  /**
   * draw all of the graphic elements.
   * @param selective True if only the most recently received objects should be
   * rendered. This give performance gains when incrementally adding points for
   * large datasets.
   */
  void drawElements(bool selective = false);
  /**
   * Draw all of the graphic elements to a printer.
   * @param printer The printer to draw to.
   */
  void drawElements(QPrinter* printer);
  /**
   * Draw a line segment.
   * @param painter The device to draw on.
   * @param pen The drawing pen
   * @param x1 The virtual x location of the begin point
   * @param y1 The virtual y location of the begin point
   * @param x2 The virtual x location of the end point
   * @param y2 The virtual y location of the end point
   * @param width The width of the drawing area.
   * @param height The height of the drawing area.
   */
  void drawLine(QPainter& painter, QPen pen, double x1, double y1, double x2,
                double y2, int width, int height);
  /**
   * remove all of the graphic elements.
   */
  void removeElements();

  QPixmap* _pixmap;  ///< All drawing will be rendered here, and this will be
                     ///< copied to the widget in paintEvent()
  QRubberBand* _rb;  ///< Rubberband used for zoming
  VectorPlus<SkewTQtText> _texts;  ///< Text graphic elements
  VectorPlus<SkewTQtPolyline*>
      _pLines;  ///< Polyline graphic elements. (Note that these are pointers,
                ///< and so must be deleted during destruction).
  VectorPlus<SkewTQtDatum> _tdryPoints;  ///< Tdry datum graphic elements
  VectorPlus<SkewTQtDatum> _dpPoints;    ///< Dew point datum graphic elements
  VectorPlus<SkewTQtDatum> _symbols;     ///< Symbol datum graphic elements
  SkewTQtText _title;  ///< The title text; appears top center, first line.
  SkewTQtText
      _subTitle;  ///< The sub title text, appears top center, second line

  unsigned int
      _lastColorCode;  ///< Used by the line() function, to detect a change in a
                       ///< line color. and thus complete a polyline.
  bool _firstLineCall;  ///< Set true before line() has been called; Use to
                        ///< initialize the consolidation algorithm within
                        ///< line().
  double _lastX2;  ///< Used by the line() function, to detect a change in a
                   ///< break in the line. and thus complete a polyline.
  double _lastY2;  ///< Used by the line() function, to detect a change in a
                   ///< break in the line. and thus complete a polyline.
  std::vector<double> _xvals;  ///< USed by line() to accumlate x values.
  std::vector<double> _yvals;  ///< Used by line() to accumlate y values.

  int _symbolSize;  ///< the edge length of a symbol, for tdry and dp points, in
                    ///< pixels
  bool _dontPaint;  ///< Set true to prohibit painting, during a paint event.
  QTimer _resizeTimer;  ///< This timer is used to clear the _dontPaint flag, at
                        ///< some delta after a resize event.
  int _resizeHoldOffMs;  ///< The number of ms to hold off on repainting after
                         ///< repaint events.

  QPen _redPen;    ///< A red pen.
  QPen _bluePen;   ///< A blue pen.
  QPen _blackPen;  ///< A black pen.

  QBrush _redBrush;    ///< A red brush.
  QBrush _blueBrush;   ///< A blue brush.
  QBrush _blackBrush;  ///< A black brush.

  QPoint _zoomStart;  ///< The position of the mouse when a mouse press event
                      ///< occurs.
  QPoint _zoomStop;   ///< The position of the mouse when a mouse release event
                      ///< occurs.
  SkewT* _pSkewT;  ///< Set to the SkewT that we are controlling, if zooming is
                   ///< desired. Null otherwise.

  bool _ready;  ///< Set true after SkewT calls draw_finished().

  bool _doPoints;  ///< True if symbols are to be drawn for tdry and dp
  bool _doLines;   ///< True if lines are to be drawn between tdry and dp

  /// The dimension of the adapter (i.e. drawing area).  For drawing on computer
  /// screen, it will be the dimension of the drawing frame on the Aspen
  /// SkewtView. For drawing on a pixmap, it will be the given dimension of the
  /// pixmap.
  int _height;  ///< Adapter (drawing area) height
  int _width;   ///< Adapter (drawing area) width
  /// When saving the skewt plot, the skewt is drawn on a given drawing area
  /// (the _height and _width are given), so its aspect ratio is given. But when
  /// drawing on computer (on Aspen SkewtView), the aspect ratio of the drawing
  /// area has to be determined at the time of drawing (therefore it is not
  /// given)
  bool _givenRatio;  ///< Ture if aspect ratio of drawing area is given.
};
}  // namespace skewt

#endif
