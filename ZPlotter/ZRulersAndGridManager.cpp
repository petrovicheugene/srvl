//========================================================
#include "ZRulersAndGridManager.h"
#include "ZPlotGraphicsView.h"
#include "ZRulerWidget.h"
#include "ZPlotter.h"
#include "ZGeneral.h"

#include <QPoint>
#include <QPointF>
#include <QDebug>

//========================================================
ZRulersAndGridManager::ZRulersAndGridManager(ZPlotter *parent) : QObject(parent)
{
    zv_minVpMarkInterval = 0;
    zv_markIntervalList << 1 << 2 << 5 << 10;
    zv_plotView = 0;
    zv_rulerWidget = 0;
    zv_minimalVerticalScaleInterval = 1.0;
    zv_minimalHorizontalScaleInterval = 1.0;
    zv_minimalScratchVpInterval = 10;
    zv_plotter = parent;
}
//========================================================
//QList<qreal> ZRulersAndGridManager::zp_XMarkGridList() const
//{
//    return zv_XMarkGridMap.values();
//}
////========================================================
//QList<qreal> ZRulersAndGridManager::zp_YMarkGridList() const
//{
//    return zv_YMarkGridMap.values();
//}
////========================================================
//QList<qreal> ZRulersAndGridManager::zp_XScratchGridList() const
//{
//    return zv_XScratchGridMap.values();
//}
////========================================================
//QList<qreal> ZRulersAndGridManager::zp_YScratchGridList() const
//{
//    return zv_YScratchGridMap.values();
//}
//========================================================
void ZRulersAndGridManager::zp_setPlotGraphicsView(ZPlotGraphicsView* plotView)
{
    zv_plotView = plotView;

    zv_plotView->zp_setHorizontalRulePointList(&zv_XRulePointList);
    zv_plotView->zp_setVerticalRulePointList(&zv_YRulePointList);

}
//========================================================
void ZRulersAndGridManager::zp_setRulerWidget(ZRulerWidget* rulerWidget)
{
    zv_rulerWidget = rulerWidget;
    //    zv_rulerWidget->zp_setHorizontalGridMaps(&zv_XScratchGridMap);
    //    zv_rulerWidget->zp_setHorizontalMarkMaps(&zv_XMarkGridMap);
    //    zv_rulerWidget->zp_setVerticalGridMaps(&zv_YScratchGridMap);
    //    zv_rulerWidget->zp_setVerticalMarkMaps(&zv_YMarkGridMap);

    zv_rulerWidget->zp_setHorizontalRulePointList(&zv_XRulePointList);
    zv_rulerWidget->zp_setVerticalRulePointList(&zv_YRulePointList);
}
//========================================================
bool ZRulersAndGridManager::zp_setMinimalVerticalScaleInterval(qreal interval)
{
    if(interval <= 0)
    {
        return false;
    }

    zv_minimalVerticalScaleInterval = interval;
    return true;
}
//========================================================
qreal ZRulersAndGridManager::zp_minimalVerticalScaleInterval()
{
    return zv_minimalVerticalScaleInterval;
}
//========================================================
bool ZRulersAndGridManager::zp_setMinimalHorizontalScaleInterval(qreal interval)
{
    if(interval <= 0)
    {
        return false;
    }

    zv_minimalHorizontalScaleInterval = interval;
    return true;
}
//========================================================
qreal ZRulersAndGridManager::zp_minimalHorizontalScaleInterval()
{
    return zv_minimalHorizontalScaleInterval;
}
//========================================================
void ZRulersAndGridManager::zp_recalcRulesAndGrid()
{
    if(zv_plotView == 0)
    {
        zv_XRulePointList.clear();
        zv_YRulePointList.clear();
        return;
    }

    QRect viewportGlobalRect;
    QRectF viewportSceneRect;
    QPoint globalZeroPoint;
    if(!zv_plotView->zp_viewPortCoordinates(viewportGlobalRect,viewportSceneRect, globalZeroPoint))
    {
        zv_XRulePointList.clear();
        zv_YRulePointList.clear();
        return;
    }

    qreal distortionFactor;
    qreal distortionCorrectionFactor;
    zv_plotter->zp_verticalDistortionFactors(distortionFactor, distortionCorrectionFactor);

    if(zv_viewportGlobalRect == viewportGlobalRect
            && zv_viewportSceneRect == viewportSceneRect
            && zv_globalZeroPoint == globalZeroPoint
            && zv_distortionFactor - distortionFactor == 0.0
            && zv_distortionCorrectionFactor - distortionCorrectionFactor == 0.0)
    {
        // coordinates are not changed
        return;
    }

    zv_distortionFactor = distortionFactor;
    zv_distortionCorrectionFactor = distortionCorrectionFactor;
    zv_viewportGlobalRect = viewportGlobalRect;
    zv_viewportSceneRect = viewportSceneRect;
    zv_globalZeroPoint = globalZeroPoint;
    zh_recalcRectsForPaint();

    // bottom rule haven't be distorted
    zh_recalcBottomRule();

    if(distortionFactor == 0.0 || distortionFactor == 1.0 || distortionCorrectionFactor == 0.0)
    {
        zh_recalcLeftRule();
    }
    else
    {
        zh_recalcDistortedLeftRule(distortionFactor, distortionCorrectionFactor);
    }

    zv_rulerWidget->update();
}
//========================================================
void ZRulersAndGridManager::zh_recalcBottomRule()
{
    zv_XRulePointList.clear();

    if(zv_viewportGlobalRect.left() != zv_viewportGlobalRect.right()
            && zv_viewportSceneRect.left() - zv_viewportSceneRect.right() != 0.0
            && zv_rulerWidget != 0)
    {
        int maxVpMarkInterval = zv_rulerWidget->zp_maxMarkWidth() * 2 ;
        qreal maxScMarkInterval = zv_plotView->mapToScene(QPoint(maxVpMarkInterval,0)).x()
                - zv_plotView->mapToScene(QPoint(0,0)).x();

        // new mark and scratch interval calculation
        qreal newScMarkInterval = 1;
        qreal newScScratchInterval = 1;

        if(maxScMarkInterval < zv_minimalHorizontalScaleInterval)
        {
            newScMarkInterval = zv_minimalHorizontalScaleInterval;
        }
        else
        {
            QString maxScMarkIntervalString = QString::number(maxScMarkInterval, 'E', 1);
            QStringList intervalPartList = maxScMarkIntervalString.split('E');
            QString integralPart = intervalPartList.value(0, QString());

            bool ok;
            double nIntegralPart = qAbs(integralPart.toDouble(&ok));
            if(!ok)
            {
                newScMarkInterval = maxScMarkInterval;
            }

            for(int i = 0; i < zv_markIntervalList.count(); i++)
            {
                if(zv_markIntervalList.value(i) < nIntegralPart)
                {
                    continue;
                }

                integralPart = QString::number(zv_markIntervalList.value(i), 'f', 1);
                maxScMarkIntervalString = integralPart + QString("E") + intervalPartList.value(1);

                newScMarkInterval = maxScMarkIntervalString.toDouble(&ok);
                if(!ok || newScMarkInterval < zv_minimalHorizontalScaleInterval)
                {
                    newScMarkInterval = maxScMarkInterval;
                }

                break;
            }

            // scratch interval calculation
            if(integralPart.at(0) == '1')
            {
                QList<qreal> devisionList;
                devisionList << 10.0 << 5.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(newScScratchInterval, zv_viewportSceneRect.top()));
                    int firstSratchXPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                    int newVpScratchInterval = qAbs(firstSratchXPos - zv_globalZeroPoint.x());
                    if(newVpScratchInterval < zv_minimalScratchVpInterval || newScScratchInterval < zv_minimalHorizontalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '2')
            {
                QList<qreal> devisionList;
                devisionList << 20.0 << 4.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(newScScratchInterval, zv_viewportSceneRect.top()));
                    int firstSratchXPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                    int newVpScratchInterval = qAbs(firstSratchXPos - zv_globalZeroPoint.x());
                    if(newVpScratchInterval < zv_minimalScratchVpInterval || newScScratchInterval < zv_minimalHorizontalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '5')
            {
                newScScratchInterval = newScMarkInterval / 5.0;
                QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(newScScratchInterval, zv_viewportSceneRect.top()));
                int firstSratchXPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).x();
                int newVpScratchInterval = qAbs(firstSratchXPos - zv_globalZeroPoint.x());
                if(newVpScratchInterval < zv_minimalScratchVpInterval || newScScratchInterval < zv_minimalHorizontalScaleInterval)
                {
                    newScScratchInterval = newScMarkInterval;
                }
            }
        }  // end new mark interval calculation


        // vp-coord = sc-coord*k + a
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        zv_K = ((qreal)zv_viewportGlobalRect.right() - (qreal)zv_viewportGlobalRect.left())
                / (zv_viewportSceneRect.right() - zv_viewportSceneRect.left());
        zv_A = (qreal)zv_viewportGlobalRect.left() - zv_viewportSceneRect.left()*zv_K;


        int markScratchesInterval;
        if(newScScratchInterval > 0)
        {
            markScratchesInterval = qRound(newScMarkInterval / newScScratchInterval);
        }
        else
        {
            markScratchesInterval = 1;
        }

        int emptyMarkScratchesInterval;
        switch(markScratchesInterval)
        {
        case 1:
        case 2:
            emptyMarkScratchesInterval = 1;
            break;
        case 4:
            emptyMarkScratchesInterval = 2;
            break;
        case 5:
            emptyMarkScratchesInterval = markScratchesInterval;
            break;
        case 10:
            emptyMarkScratchesInterval = 5;
            break;
        case 20:
            emptyMarkScratchesInterval = 5;
            break;
        default:
            emptyMarkScratchesInterval = markScratchesInterval;
        }

        int scratchInMarkIntervalCount = 0;
        int scratchInEmptyMarkInterval = 0;
        RulePoint rulePoint;
        RulePoint::MarkType markType;

        // Marks
        for(qreal sc = newScScratchInterval; sc <= zv_sceneRectForPaint.right(); sc+= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc < zv_sceneRectForPaint.left())
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*zv_K + zv_A;
            rulePoint = RulePoint(vp, sc, markType);
            zv_XRulePointList.append(rulePoint);
        }

        scratchInMarkIntervalCount = -1;
        scratchInEmptyMarkInterval = -1;
        for(qreal sc = 0; sc >= zv_sceneRectForPaint.left(); sc-= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc > zv_sceneRectForPaint.right())
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*zv_K + zv_A;
            rulePoint = RulePoint(vp, sc, markType);
            zv_XRulePointList.append(rulePoint);
        }
    }
}
//========================================================
void ZRulersAndGridManager::zh_recalcLeftRule()
{
    // cleaning
    zv_YRulePointList.clear();


    if(zv_viewportGlobalRect.top() != zv_viewportGlobalRect.bottom()
            && zv_viewportSceneRect.top() != zv_viewportSceneRect.bottom()
            && zv_rulerWidget != 0)
    {

        zv_minVpMarkInterval = zv_rulerWidget->zp_maxMarkHeight() * 3 ;
        qreal minScMarkInterval = zv_plotView->mapToScene(QPoint(0, zv_minVpMarkInterval)).y()
                - zv_plotView->mapToScene(QPoint(0,0)).y();

        // new mark and scratch interval calculation
        qreal newScMarkInterval = zv_minimalVerticalScaleInterval;
        qreal newScScratchInterval = zv_minimalVerticalScaleInterval;

        if(minScMarkInterval < zv_minimalVerticalScaleInterval)
        {
            newScMarkInterval = zv_minimalVerticalScaleInterval;
        }
        else
        {
            QString maxScMarkIntervalString = QString::number(minScMarkInterval, 'E', 1);
            QStringList intervalPartList = maxScMarkIntervalString.split('E');
            QString integralPart = intervalPartList.value(0, QString());

            bool ok;
            double nIntegralPart = qAbs(integralPart.toDouble(&ok));
            if(!ok)
            {
                newScMarkInterval = minScMarkInterval;
            }

            for(int i = 0; i < zv_markIntervalList.count(); i++)
            {
                if(zv_markIntervalList.value(i) < nIntegralPart)
                {
                    continue;
                }

                integralPart = QString::number(zv_markIntervalList.value(i), 'f', 1);
                maxScMarkIntervalString = integralPart + QString("E") + intervalPartList.value(1);

                newScMarkInterval = maxScMarkIntervalString.toDouble(&ok);
                if(!ok || newScMarkInterval < zv_minimalHorizontalScaleInterval)
                {
                    newScMarkInterval = minScMarkInterval;
                }

                break;
            }

            // scratch interval calculation
            if(integralPart.at(0) == '1')
            {
                QList<qreal> devisionList;
                devisionList << 10.0 << 5.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(zv_viewportSceneRect.left(), newScScratchInterval));
                    int firstSratchYPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                    int newVpScratchInterval = qAbs(firstSratchYPos - zv_globalZeroPoint.y());
                    if(newVpScratchInterval < zv_minimalScratchVpInterval || newScScratchInterval < zv_minimalVerticalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '2')
            {
                QList<qreal> devisionList;
                devisionList << 20.0 << 4.0 << 2.0;
                for(int i = 0; i < devisionList.count(); i++)
                {
                    newScScratchInterval = newScMarkInterval / devisionList.value(i, 1.0);
                    QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(zv_viewportSceneRect.left(), newScScratchInterval));
                    int firstSratchYPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                    int newVpScratchInterval = qAbs(firstSratchYPos - zv_globalZeroPoint.y());
                    if(newVpScratchInterval < zv_minimalScratchVpInterval || newScScratchInterval < zv_minimalVerticalScaleInterval)
                    {
                        newScScratchInterval = newScMarkInterval;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(integralPart.at(0) == '5')
            {
                newScScratchInterval = newScMarkInterval / 5.0;
                QPoint vp_scratchIntervalPos = zv_plotView->mapFromScene(QPointF(zv_viewportSceneRect.left(), newScScratchInterval));
                int firstSratchYPos = zv_plotView->viewport()->mapToGlobal(vp_scratchIntervalPos).y();
                int newVpScratchInterval = qAbs(firstSratchYPos - zv_globalZeroPoint.y());
                if(newVpScratchInterval < zv_minimalScratchVpInterval || newScScratchInterval < zv_minimalVerticalScaleInterval)
                {
                    newScScratchInterval = newScMarkInterval;
                }
            }
        }  // end new mark interval calculation

        // vp-coord = sc-coord*k + a
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        zv_K = ((qreal)zv_viewportGlobalRect.bottom() - (qreal)zv_viewportGlobalRect.top())
                / (zv_viewportSceneRect.bottom() - zv_viewportSceneRect.top());
        zv_A = (qreal)zv_viewportGlobalRect.top() - zv_viewportSceneRect.top()*zv_K;

        // defining empty mark interval
        int markScratchesInterval;
        if(newScScratchInterval > 0)
        {
            markScratchesInterval = qRound(newScMarkInterval / newScScratchInterval);
        }
        else
        {
            markScratchesInterval = 1;
        }

        int emptyMarkScratchesInterval;
        switch(markScratchesInterval)
        {
        case 1:
        case 2:
            emptyMarkScratchesInterval = 1;
            break;
        case 4:
            emptyMarkScratchesInterval = 2;
            break;
        case 5:
            emptyMarkScratchesInterval = markScratchesInterval;
            break;
        case 10:
            emptyMarkScratchesInterval = 5;
            break;
        case 20:
            emptyMarkScratchesInterval = 5;
            break;
        default:
            emptyMarkScratchesInterval = markScratchesInterval;
        }


        int scratchInMarkIntervalCount = 0;
        int scratchInEmptyMarkInterval = 0;
        RulePoint rulePoint;
        RulePoint::MarkType markType;
        // Marks
        for(qreal sc = newScScratchInterval; sc <= zv_sceneRectForPaint.bottom(); sc+= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc < zv_sceneRectForPaint.top() )
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*zv_K + zv_A;
            rulePoint = RulePoint(vp, sc, markType);
            zv_YRulePointList.append(rulePoint);
        }

        scratchInMarkIntervalCount = -1;
        scratchInEmptyMarkInterval = -1;
        for(qreal sc = 0; sc >= zv_sceneRectForPaint.top(); sc-= newScScratchInterval)
        {
            // count in mark interval
            if(++scratchInMarkIntervalCount >= markScratchesInterval)
            {
                scratchInMarkIntervalCount = 0;
            }

            if(++scratchInEmptyMarkInterval >= emptyMarkScratchesInterval)
            {
                scratchInEmptyMarkInterval = 0;
            }

            if(sc > zv_sceneRectForPaint.bottom())
            {
                continue;
            }

            if(scratchInMarkIntervalCount == 0)
            {
                markType = RulePoint::MT_MARK;
            }
            else if(scratchInEmptyMarkInterval == 0)
            {
                markType = RulePoint::MT_EMPTY_MARK;
            }
            else
            {
                markType = RulePoint::MT_SCRATCH;
            }

            qreal vp = sc*zv_K + zv_A;
            rulePoint = RulePoint(vp, sc, markType);
            zv_YRulePointList.append(rulePoint);
        }
    }
}
//========================================================
void ZRulersAndGridManager::zh_recalcDistortedLeftRule(qreal distortionFactor, qreal distortionCorrectionFactor)
{
    zv_YRulePointList.clear();

    zv_distortionFactor = distortionFactor;
    zv_distortionCorrectionFactor = distortionCorrectionFactor;

    if(zv_viewportGlobalRect.top() != zv_viewportGlobalRect.bottom()
            && zv_viewportSceneRect.top() != zv_viewportSceneRect.bottom()
            && zv_rulerWidget != 0)
    {
        zv_minVpMarkInterval = zv_rulerWidget->zp_maxMarkHeight() * 3 ;

        // vp-coord = sc-coord*k + a
        // sc-coord = (vp-coord - a) / k
        // solution:
        // k = (vp-max - vp-min) / (sc-max - sc-min)
        // a = vp-min - sc-min*k

        // sc-coord = value^distortionFactor * distortionCorrectionFactor
        //

        zv_K = ((qreal)zv_viewportGlobalRect.bottom() - (qreal)zv_viewportGlobalRect.top())
                / (zv_viewportSceneRect.bottom() - zv_viewportSceneRect.top());
        zv_A = (qreal)zv_viewportGlobalRect.top() - zv_viewportSceneRect.top()*zv_K;

        RulePoint rulePoint;
        RulePoint::MarkType markType;


        qreal currentMarkValue = 0;
        qreal currentMarkScPos = 0;
        qreal currentMarkVpPos = zv_A;

        //
        zv_displayedTopYValue = pow((qAbs(zv_viewportSceneRect.top()) / zv_distortionCorrectionFactor), 1/zv_distortionFactor);
        if(zv_viewportSceneRect.top() < 0)
        {
            zv_displayedTopYValue *= -1;
        }
        zv_displayedBottomYValue = pow(qAbs((zv_viewportSceneRect.bottom()) / zv_distortionCorrectionFactor), 1/zv_distortionFactor);
        if(zv_viewportSceneRect.bottom() < 0)
        {
            zv_displayedBottomYValue *= -1;
        }

        qreal prevMarkValue;
        while(currentMarkValue >= zv_displayedTopYValue)
        {
            if(currentMarkValue <= zv_displayedBottomYValue)
            {
                markType = RulePoint::MT_MARK;
                //qreal vp = *k + a;
                rulePoint = RulePoint(currentMarkVpPos, currentMarkScPos, currentMarkValue, markType);
                zv_YRulePointList.append(rulePoint);
            }

            prevMarkValue = currentMarkValue;

            // next min passible current value
            currentMarkVpPos = currentMarkVpPos - zv_minVpMarkInterval;
            currentMarkScPos = (currentMarkVpPos - zv_A) / zv_K;
            currentMarkValue = pow((qAbs(currentMarkScPos) / zv_distortionCorrectionFactor), 1/zv_distortionFactor);
            // next to the min passible value tenfold value
            qreal p = 0.0;
            for(;;p+=1.0 )
            {
                if(currentMarkValue <= pow(10.0, p))
                {
                    currentMarkValue = pow(10.0, p);
                    break;
                }
            }

            if(currentMarkScPos < 0)
            {
                currentMarkValue *= -1;
            }

            // back recalculation
            currentMarkScPos = pow(qAbs(currentMarkValue), zv_distortionFactor) * zv_distortionCorrectionFactor;
            if(currentMarkValue < 0)
            {
                currentMarkScPos *= -1;
            }
            currentMarkVpPos = currentMarkScPos * zv_K + zv_A;

            // sratches defining (they may be marks if interval will allow)

            zh_divideDistortedVerticalInterval(p, currentMarkValue, currentMarkVpPos, prevMarkValue);
        }


        // from zero to bottom
        currentMarkValue = zv_minVpMarkInterval;
        currentMarkScPos = pow(qAbs(currentMarkValue), zv_distortionFactor) * zv_distortionCorrectionFactor;
        currentMarkVpPos = currentMarkScPos * zv_K + zv_A;


        while(currentMarkValue <= zv_displayedBottomYValue)
        {
            if(currentMarkValue >= zv_displayedTopYValue)
            {
                markType = RulePoint::MT_MARK;
                //qreal vp = *k + a;
                rulePoint = RulePoint(currentMarkVpPos, currentMarkScPos, currentMarkValue, markType);
                zv_YRulePointList.append(rulePoint);
            }

            prevMarkValue = currentMarkValue;

            // next min passible current value
            currentMarkVpPos = currentMarkVpPos + zv_minVpMarkInterval;
            currentMarkScPos = (currentMarkVpPos - zv_A) / zv_K;
            currentMarkValue = pow((qAbs(currentMarkScPos) / zv_distortionCorrectionFactor), 1/zv_distortionFactor);
            // next to the min passible value tenfold value
            qreal p = 0.0;
            for(;;p+=1.0 )
            {
                if(currentMarkValue <= pow(10.0, p))
                {
                    currentMarkValue = pow(10.0, p);
                    break;
                }
            }

//            if(currentMarkScPos < 0)
//            {
//                currentMarkValue *= -1;
//            }

            // back recalculation
            currentMarkScPos = pow(qAbs(currentMarkValue), zv_distortionFactor) * zv_distortionCorrectionFactor;
//            if(currentMarkValue < 0)
//            {
//                currentMarkScPos *= -1;
//            }
            currentMarkVpPos = currentMarkScPos * zv_K + zv_A;

            // sratches defining (they may be marks if interval will allow)

            zh_divideDistortedVerticalInterval(p, currentMarkValue, currentMarkVpPos, prevMarkValue);
        }
    }
}
//========================================================
bool ZRulersAndGridManager::zh_divideDistortedVerticalInterval(qreal p, qreal currentMarkValue,
                                                               qreal currentMarkVpPos, qreal prevMarkValue)
{
    qreal scratchValue;
    qreal scratchScPos;
    qreal scratchVpPos;
    RulePoint rulePoint;
    RulePoint::MarkType markType;

    //    if(qAbs(currentMarkValue) <= pow(10, p-1.0))
    //    {
    //        continue;
    //    }
    // top scratch value
    scratchValue = qAbs(currentMarkValue) - pow(10, p-1.0);
    if(currentMarkValue < 0)
    {
        scratchValue *= -1;
    }
    // top scratch coords
    scratchScPos = pow(qAbs(scratchValue), zv_distortionFactor) * zv_distortionCorrectionFactor;
    if(scratchValue < 0)
    {
        scratchScPos *= -1;
    }
    scratchVpPos = scratchScPos * zv_K + zv_A;

    // if vp interval is big scratches become marks
    bool scratchIsMark = false;
    bool divideCurrentInterval = false;
    qreal prevScratchValue;
    qreal prevScratchVpPos;

    if(qAbs(scratchVpPos - currentMarkVpPos) >= zv_minVpMarkInterval)
    {
        scratchIsMark = true;
    }
    else
    {
        scratchIsMark = false;
    }

    if(qAbs(scratchVpPos - currentMarkVpPos) >= zv_minimalScratchVpInterval && scratchIsMark)
    {
        divideCurrentInterval = zh_divideDistortedVerticalInterval(p - 1, currentMarkValue,
                                                                   currentMarkVpPos, scratchValue);
    }

    if(qAbs(scratchVpPos - currentMarkVpPos) >= zv_minimalScratchVpInterval
            && qAbs(scratchValue - currentMarkValue) >= zv_minimalVerticalScaleInterval)
    {
        int scratchCount = 0;
        while(qAbs(scratchValue) > qAbs(prevMarkValue))
        {
            if(scratchValue >=  zv_displayedTopYValue && scratchValue <= zv_displayedBottomYValue)
            {
                if(scratchIsMark)
                {
                    markType = RulePoint::MT_MARK;
                }
                else if(scratchCount == 4)
                {
                    markType = RulePoint::MT_MARK;
                }
                else
                {
                    markType = RulePoint::MT_SCRATCH;
                }

                //qreal vp = *k + a;
                rulePoint = RulePoint(scratchVpPos, scratchScPos, scratchValue, markType);
                zv_YRulePointList.append(rulePoint);
            }

            // next scratch to below
            prevScratchValue = scratchValue;
            prevScratchVpPos = scratchVpPos;

            scratchValue = qAbs(scratchValue) - pow(10, p-1.0);
            if(prevScratchValue < 0)
            {
                scratchValue *= -1;
            }
            //scratchValue = scratchValue + pow(10, p-1.0);
            scratchScPos = pow(qAbs(scratchValue), zv_distortionFactor) * zv_distortionCorrectionFactor;
            if(scratchValue < 0)
            {
                scratchScPos *= -1;
            }
            scratchVpPos = scratchScPos * zv_K + zv_A;

            if(divideCurrentInterval && scratchIsMark)
            {
                zh_divideDistortedVerticalInterval(p - 1, prevScratchValue,
                                                   prevScratchVpPos, scratchValue);
            }
            scratchCount++;
        }
        return true;
    }
    else
    {
        // trying to divide to half
        scratchValue = qAbs(currentMarkValue) - (pow(10, p-1.0) * 5);
        if(currentMarkValue < 0)
        {
            scratchValue *= -1;
        }
        // top scratch coords
        scratchScPos = pow(qAbs(scratchValue), zv_distortionFactor) * zv_distortionCorrectionFactor;
        if(scratchValue < 0)
        {
            scratchScPos *= -1;
        }
        scratchVpPos = scratchScPos * zv_K + zv_A;

        bool scratchIsMark;
        if(qAbs(scratchVpPos - currentMarkVpPos) >= zv_minVpMarkInterval)
        {
            scratchIsMark = true;
        }
        else
        {
            scratchIsMark = false;
        }

        if(qAbs(scratchVpPos - currentMarkVpPos) >= zv_minimalScratchVpInterval
                && qAbs(scratchValue - currentMarkValue) >= zv_minimalVerticalScaleInterval)
        {
            if(scratchValue >=  zv_displayedTopYValue && scratchValue <= zv_displayedBottomYValue)
            {
                if(scratchIsMark)
                {
                    markType = RulePoint::MT_MARK;
                }
                else
                {
                    markType = RulePoint::MT_EMPTY_MARK;
                }

                //qreal vp = *k + a;
                rulePoint = RulePoint(scratchVpPos, scratchScPos, scratchValue, markType);
                zv_YRulePointList.append(rulePoint);
            }
            return true;
        }
    }
    return false;
}
//========================================================
void ZRulersAndGridManager::zh_recalcRectsForPaint()
{
    qreal K_width = zv_viewportSceneRect.width() /
            (qreal)zv_viewportGlobalRect.width();
    qreal K_height = zv_viewportSceneRect.height() /
            (qreal)zv_viewportGlobalRect.height();

    int dPix = 1;
    zv_globalRectForPaint = zv_viewportGlobalRect.adjusted(-1*dPix, -1*dPix, dPix, dPix);
    zv_sceneRectForPaint = zv_viewportSceneRect.adjusted(-1*K_width*dPix, -1*K_height*dPix, K_width*dPix, K_height*dPix);
}
//========================================================

