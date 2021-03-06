//===========================================================
#include <ZGeneral.h>

//===========================================================
extern const QString glDefaultDBSuffixString = "sqlite";
extern const QString glDefaultChemElementString = "Not defined";
extern const QString glAppSettingsKeyName = "AppSettings";
extern const QString glAppGeometryKeyName = "AppGeometry";
extern const QString glAppStateKeyName = "AppState";

// Icons
namespace NS_Icons
{
extern const QString glIconSettings = ":/images/ZImages/settings-8.png";
extern const QString glIconExitApp = ":/images/ZImages/exit-1.png";
extern const QString glIconAbout = ":/images/ZImages/info-1.png";
extern const QString glIconHelp = ":/images/ZImages/help-1.png";
extern const QString glIconQt = ":/images/ZImages/Qt-logo.png";

extern const QString glIconSidebarLeftOpen = ":/images/ZWidgets/sidebarLeftOpen-1.png";
extern const QString glIconSidebarLeftClose = ":/images/ZWidgets/sidebarLeftClose-1.png";
extern const QString glIconSidebarRightOpen = ":/images/ZWidgets/sidebarRightOpen-1.png";
extern const QString glIconSidebarRightClose = ":/images/ZWidgets/sidebarRightClose-1.png";

extern const QString glIconAxisToEnergy = ":/images/ZImages/energy.png";
extern const QString glIconShowGrid = ":/images/ZImages/gridVisible-2.png";
extern const QString glIconVerticalIncrease = ":/images/ZImages/verticalIncrease-2.png";
extern const QString glIconVerticalDecrease = ":/images/ZImages/verticalDecrease-2.png";
extern const QString glIconHorizontalIncrease = ":/images/ZImages/horizontalIncrease-2.png";
extern const QString glIconHorizontalDecrease = ":/images/ZImages/horizontalDecrease-2.png";
extern const QString glIconFitInRect = ":/images/ZImages/fitInSceneRect.png";

}

// Common non translatable strings

// object Names
namespace NS_ObjectNames
{
extern const QString glObjectNameMenuFile = "File";
extern const QString glObjectNameMenuEdit = "Edit";
extern const QString glObjectNameMenuView = "View";
extern const QString glObjectNameMenuActions = "Actions";
extern const QString glObjectNameMenuHelp = "Help";
}

// Common strings
namespace NS_CommonStrings
{
extern const QString glSettings = QObject::tr("Settings");
extern const QString glSettingsToolTip = QObject::tr("Run settings of the application");
extern const QString glExit = QObject::tr("Exit");
extern const QString glExitToolTip = QObject::tr("Exit the application");
extern const QString glAbout = QObject::tr("About");
extern const QString glAboutToolTip = QObject::tr("About the application");
extern const QString glHelp = QObject::tr("Help");
extern const QString glHelpToolTip = QObject::tr("Show user guide");

extern const QString glError = QObject::tr("Error");
extern const QString glWarning = QObject::tr("Warning");
extern const QString glCommon = QObject::tr("Common");
extern const QString glColor = QObject::tr("Color");
extern const QString glSelectColor = QObject::tr("Color...");

}

// Menu strings
namespace NS_Menus
{
extern const QString glMenuFile = QObject::tr("File");
extern const QString glMenuEdit = QObject::tr("Edit");
extern const QString glMenuView = QObject::tr("View");
extern const QString glMenuActions = QObject::tr("Actions");
extern const QString glMenuHelp = QObject::tr("Help");
}
// button strings
namespace NS_Buttons
{
extern const QString glButtonOk = QObject::tr("OK");
extern const QString glButtonApply = QObject::tr("Apply");
extern const QString glButtonCancel = QObject::tr("Cancel");
extern const QString glButtonBrowse = QObject::tr("Browse");
extern const QString glButtonNew = QObject::tr("New");

}
// window captions
namespace NS_WindowCaptions
{
extern const QString glOpenFile = QObject::tr("Open file");
extern const QString glSelectColor = QObject::tr("Select color");
}

extern const int glMaxGainFactor = 255;
extern const int glSpectrumDataType = 1;

namespace NS_DataRole
{

extern const int PathRole = Qt::UserRole + 1;
extern const int CommentRole = Qt::UserRole + 2;
extern const int DataTypeRole = Qt::UserRole + 3;
extern const int VisibleRole = Qt::UserRole + 4;
extern const int AbstractSpectrumRole = Qt::UserRole + 5;

}

// FUNCS
QString glCreateCaption(const QString& simpleText)
{
    if(simpleText.isEmpty())
    {
        return QString();
    }

    return QString("<font color=darkGreen><b>%1</b></font>").arg(simpleText);
}
