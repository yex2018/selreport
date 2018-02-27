#include <QImage>
#include <QPainter>
#include <QPaintEngine>

#include "reportpdfwriter.h"
#include "scalenorms.h"

ReportPdfWriter::ReportPdfWriter(const QString &strFileName) :
    QPdfWriter(strFileName), m_iCurrentPageIndex(0), m_imageLogo(":/selreport/images/logo.png")
{
}

bool ReportPdfWriter::newPage()
{
    if(QPainter *pPainter = paintEngine()->painter())
    {
        if(m_iCurrentPageIndex == 0)
        {
            const QRect rcFull = pageLayout().fullRectPoints();
            const QRect rcPaint = pageLayout().paintRectPoints();

            QImage imageCover(":/selreport/images/cover.png");
            pPainter->drawImage(QRect((rcPaint.width() - rcFull.width()) / 2, (rcPaint.height() - rcFull.height()) / 2, rcFull.width(), rcFull.width() * imageCover.height() / imageCover.width()), imageCover);
        }
    }

    bool bret = QPdfWriter::newPage();

    if(QPainter *pPainter = paintEngine()->painter())
    {
        if(m_iCurrentPageIndex > 0)
        {
            QRect rc(pageLayout().paintRectPoints().right() - 83 - 78, 10, 78, 42);
            pPainter->drawImage(rc, m_imageLogo);
        }
    }

    m_iCurrentPageIndex++;

    return bret;
}
