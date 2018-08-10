/* ------------------------------------------------------------------------- */

#include "SkewtQtTest.h"
#include <QLayout>
#include <QPushbutton>
#include <QDialog>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QApplication>

SkewtQtTest::SkewtQtTest(QDialog* parent, const char* name): 
_n(0), _N(0)
{
    name;

    _skewtAdapter = new skewt::SkewTAdapterQt(0);

    QVBoxLayout* topLayout = new QVBoxLayout;
    parent->setLayout(topLayout);

    topLayout->addWidget(_skewtAdapter);
    QHBoxLayout* hLayout = new QHBoxLayout;
    topLayout->addLayout(hLayout);

    QLabel* l = new QLabel("Points:");
    hLayout->addWidget(l);
    _pointsLabel = new QLabel(" 0");
    hLayout->addWidget(_pointsLabel);

    hLayout->addStretch();

    QPushButton* unzoomButton = new QPushButton;
    unzoomButton->setText("unzoom");
    hLayout->addWidget(unzoomButton);

    connect( &_timer, SIGNAL(timeout()), this, SLOT(timeOut()) );
    connect( unzoomButton, SIGNAL(released()), this, SLOT(unzoomSlot()));

    _timer.start(500);

}

/* -------------------------------------------------------------------- */

SkewtQtTest::~SkewtQtTest()
{

    delete [] _pres;
    delete [] _tdry;
    delete [] _rh;
    delete [] _wspd;
    delete [] _wdir;

    delete _skewt;

}

/* -------------------------------------------------------------------- */

void
SkewtQtTest::unzoomSlot()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    _skewt->unzoom();
    QApplication::restoreOverrideCursor();
}

/* -------------------------------------------------------------------- */

void
SkewtQtTest::makeData()
{
    // create a semi-plausible profile of _n points.

    _pres = new double[_n];
    _tdry = new double[_n];
    _rh   = new double[_n];
    _wspd = new double[_n];
    _wdir = new double[_n];

    double pInc = 800.0/_n;
    double tInc = -90.0/_n;

    for (int i = 0; i < _n; i++) {
        
        double pres = 1000.0 - i*pInc;

        double tdry = 30.0 + pow((double)i,1.01)*tInc + 5.0*sin(9*3.14158*i/_n) + 1.0*sin(45*3.14158*i/_n) + 1.0*rand()/32767.0*sin(5*3.14158*i/_n);

        double rh = 50.0 + 5.0*sin(50*3.14158*i/_n) + 20*rand()/32767.0*sin(3*3.14158*i/_n);

        double wspd  = 20.0 + 5.0*sin(50*3.14158*i/_n) + 20*rand()/32767.0*sin(3*3.14158*i/_n); 
        if (wspd < 0.0)
            wspd = 0.0;

        double wdir = 40.0*sin(3.14158*i/_n) + 10.0*rand()/32767.0*sin(5*3.14158*i/_n) + 90*sin(2*3.14159*i/_n);

        _pres[i] = pres;
        _tdry[i] = tdry;
        _rh[i]   = rh;
        _wspd[i] = wspd;
        _wdir[i] = wdir;    

    }

    _lastPres = _pres[_n-1];


}

/* -------------------------------------------------------------------- */

void
SkewtQtTest::timeOut() 
{


    QApplication::setOverrideCursor(Qt::WaitCursor);

    // on first call, create SkewT and make data
    if (_n == 0) {

        _n = 5000;
        makeData();
        _skewt        = new skewt::SkewT(*_skewtAdapter, 
            _n,  _tdry, _rh, _wspd, _wdir, _pres, 20, 
            "SkewTAdapterQt", "Test", "NCAR/EOL");
        _skewtAdapter->setSkewT(_skewt);

        _skewt->draw();

        _N = _n;
    }

    _N++;
    _lastPres -= 1.0;
    _skewt->drawTdry(_lastPres, _tdry[_n-1]);
    _skewt->drawDp(_lastPres, 50.0);

    _skewt->drawWind(_lastPres, 24.0, 73.0);
    _pointsLabel->setNum(_N);

    QApplication::restoreOverrideCursor();
}

