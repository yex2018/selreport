#ifndef REPORTPDFWRITER_H
#define REPORTPDFWRITER_H

#include <QPdfWriter>

class ReportPdfWriter : public QPdfWriter
{
// Construction and Destruction
public:
    ReportPdfWriter(const QString &strFileName);

// Operations
public:
    virtual bool    newPage();

// Properties
private:
    QImage          m_imageLogo;
    int             m_iCurrentPageIndex;
};

#endif // REPORTPDFWRITER_H
