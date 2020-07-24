//--------------------------------------------------------------------------------------------------
// 
//	Chart: A more useful extension of QtChart
//
//--------------------------------------------------------------------------------------------------
//
// The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
// and associated documentation files (the "Software"), to deal in the Software without 
// restriction, including without limitation the rights to use, copy, modify, merge, publish, 
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or 
// substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//--------------------------------------------------------------------------------------------------
// 
// Copyright (c) 2017 Nic Holthaus
// 
//--------------------------------------------------------------------------------------------------

#ifndef lineSeries_h__
#define lineSeries_h__

//------------------------
//	INCLUDES
//------------------------
#include <QLineSeries>

//	----------------------------------------------------------------------------
//	CLASS		LineSeries
//  ----------------------------------------------------------------------------
///	@brief		Thing wrapper around QLineSeries which implements some conveniences
///	@details	
//  ----------------------------------------------------------------------------
class LineSeries : public QtCharts::QLineSeries
{
	Q_OBJECT

public:

	explicit LineSeries();

	void append(qreal x, qreal y);

	QRectF bounds() const;
	
signals:

	void leftClicked(LineSeries*);
	void rightClicked(LineSeries*);

protected:

	
	
private:

	qreal m_xMin;
	qreal m_xMax;
	qreal m_yMin;
	qreal m_yMax;

};

#endif // lineSeries_h__