#include "mainwindow.h"
#include "DlgBackground.h"
#include "DlgGrid.h"
#include "dlgtxtalign.h"
#include "dlgfreealign.h"
#include "dlgbuttonx2.h"
#include "dlgnuminput.h"
#include "keypad_help_function.h"


int  DISPLAY_CX;
int  DISPLAY_CY;



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    keybar(this), iconbar(this)
{
    DISPLAY_CX = qApp->screens()[0]->availableSize().width();
    DISPLAY_CY = qApp->screens()[0]->availableSize().height();

    ///////////////////////////////////////////
    QToolBar *toolbar = addToolBar(tr("File"));

    //toolbar->addAction(style()->standardIcon(QStyle::SP_TitleBarCloseButton), "Exit", this, &MainWindow::close);
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogNewFolder), "New", this, &MainWindow::setDrawType);
    toolbar->addAction(style()->standardIcon(QStyle::SP_DialogOpenButton), "Open...", this, &MainWindow::Open);
    toolbar->addAction(style()->standardIcon(QStyle::SP_DialogSaveButton), "Save...", this, &MainWindow::Save);

    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogInfoView), "Header Data", this, &MainWindow::onHdrList);
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogInfoView), "Key Data", this, &MainWindow::onKeyData);
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogInfoView), "List of Keys Data", this, &MainWindow::onKeyList);
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogListView), "open buttons gallery", this, &MainWindow::OpenButtonBar);
    toolbar->addAction(style()->standardIcon(QStyle::SP_FileDialogListView), "open icons gallery", this, &MainWindow::OpenIconBar);
    toolbar->addAction(style()->standardIcon(QStyle::SP_BrowserReload), "Redraw Keypad", this, [=]{pad.Redraw2(); kpdscrl.show(); kpdscrl.activateWindow(); if(listkeys) {listkeys->deleteLater(); listkeys=0;}}); ///if(listkeys) listkeys->reload(); Mission Imposible!

        /*
        QStyle::SP_MessageBoxInformation
        QStyle::SP_DesktopIcon
        QStyle::SP_DriveFDIcon
        QStyle::SP_DriveHDIcon
        QStyle::SP_ComputerIcon
        QStyle::SP_DirHomeIcon
        QStyle::SP_DirOpenIcon
        QStyle::SP_DirIcon
        QStyle::SP_FileDialogNewFolder
        QStyle::SP_FileDialogInfoView
        QStyle::SP_FileDialogListView
        QStyle::SP_DialogOpenButton
        QStyle::SP_DialogSaveButton
        QStyle::SP_DialogApplyButton
        QStyle::SP_BrowserReload
        QStyle::SP_LineEditClearButton
        */

    ///////////////////////////////////////////
    ///Icons in The Resource File
    ///http://doc.qt.io/qt-5/qtwidgets-mainwindows-application-example.html

    setStyleSheet("font-size:13pt");
    /*
                "* {font-size:14px; background-color:rgb(20,50,50); color:rgb(0,255,255); border:1px solid #008080;} "
                "QPushButton {color: #033;border: 2px solid #088;border-radius: 11px;padding: 5px;background: qradialgradient(cx: 0.3, cy: -0.4,fx: 0.3, fy: -0.4,radius: 1.35, stop: 0 #fff, stop: 1 #888);} QPushButton:pressed {background: qradialgradient(cx: 0.4, cy: -0.1, fx: 0.4, fy: -0.1, radius: 1.35, stop: 0 #fff, stop: 1 #ddd);} "
                "QMenu {font-size:16pt; background-color:#003333; color:#00ffff; border:5px solid #007777;} "
                "QMenu::item:selected{background-color:#00ffff; color:#003333}"
                );
    */

    ///////////////////////////////////////////
    kpdscrl.setWidget(&pad);
    pad.setFocusPolicy(Qt::StrongFocus); // the QScrollArea breaks the keyPressEvent()

    connect(&pad, &BuildKeypad::kpd_update, this, &MainWindow::setUpdate);
    connect(&pad, &BuildKeypad::KeyPressed, this, &MainWindow::SetKeyData);

    ///////////////////////////////////////////
    LbKey = new QLabel(this);
    LbKey->setGeometry(80, 300, 220, 30);
    LbKey->setStyleSheet("* { font-size:28px; color:#ffffff; border:0px solid }");
    LbKey->setText("K E Y   D A T A");

    /////////////////////////////////////////// KEYBAR
    keybar.setKeypad("KeyBar.kpd", "");
    keybar.setKpdGeometry(0, 0, keybar.kpd.cx, keybar.kpd.cy, KPDKPD_CX | KPDKPD_CY);
    keybar.setWindowTitle("KEY BAR");
    keybar.hide();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave())
    {
        //writeSettings();
        event->accept();
    }
    else /////// selected cancel
    {
        event->ignore();
        return;
    }

    /////// the other windows remain open, just close them
    if(listkeys) listkeys->deleteLater();
    kpdscrl.takeWidget();
    kpdscrl.close();
    pad.close();
}


////////////////////////////////////////////////////////
// return directory name:  str = QDir::home().dirName()


void MainWindow::Open()
{
    if(maybeSave())
    {
        QString filepath = QFileDialog::getOpenFileName(this,
                                                        "Open",
                                                        OpenFilePath,
                                                        "KeyPad (*.kpd)",
                                                        Q_NULLPTR,	//&selectedFilter,
                                                        QFileDialog::ReadOnly);

        OpenFile(filepath);
    }
}


void MainWindow::OpenFile(const QString &filepath)
{
    //if(QFile::exists(filepath))
    if(!filepath.isEmpty())
    {
        QFileInfo finf(filepath);
        OpenFilePath = filepath;

        pad.setKeypad(finf.fileName().toUtf8(), finf.path()+'/');
        pad.setKpdGeometry(0, 0, pad.kpd.cx, pad.kpd.cy, KPDKPD_CX | KPDKPD_CY);
        kpdscrl.resize(pad.m_wnd_cx+2, pad.m_wnd_cy+2);
        kpdscrl.setWindowTitle(OpenFilePath);
        kpdscrl.show();

        //iconbar.SetIconBar("IconBar.kpd", pad.m_PathName.toLatin1().constData(), DISPLAY_CX-320, 0, 320, DISPLAY_CY);
        //iconbar.setWindowTitle("ICON BAR");
        //iconbar.show();

        pad.m_update_file = false;
        statusBar()->showMessage("ok");
        setWindowTitle(pDrwM[pad.kpd.type]);
    }
}


bool MainWindow::Save()
{
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                "Save",
                                OpenFilePath,
                                "KeyPad (*.kpd);;Image (*.jpg);;Image (*.png)",
                                &selectedFilter,
                                QFileDialog::DontConfirmOverwrite);

    if (!fileName.isEmpty())
    {
        pad.m_update_file = false;
        statusBar()->showMessage("ok");
        OpenFilePath = fileName;
        pad.setWindowTitle(OpenFilePath);
        //LbM[1]->setText(selectedFilter);
        //LbM[1]->setText(OpenFilePath);

        if(selectedFilter=="KeyPad (*.kpd)")
        {
            QFile f_cnf(fileName);
            if (f_cnf.open(QIODevice::WriteOnly))
            {
                f_cnf.write((char*)&pad.kpd, sizeof(KEYHDR));
                f_cnf.write((char*)pad.pkey, sizeof(KEY)*pad.kpd.nKeys);
            }
        }
        else if(selectedFilter=="Image (*.jpg)")
        {
            pad.m_xmp->save(fileName,"JPG",100);
        }
        else if(selectedFilter=="Image (*.png)")
        {
            pad.m_xmp->save(fileName,"PNG",100);
        }

        return true;
    }
    return false;
}

/*
bool MainWindow::saveAs()
{
    QFileDialog dialog(this, "Lets save Save the Keypad now !", OpenFilePath,"KeyPad (*.kpd);;Image (*.jpg);;Image (*.png);;Image (*.bmp)",);
    //dialog.setDirectory();
    //dialog.setNameFilter("*.cpp *.cc *.C *.cxx *.c++");
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}
*/

bool MainWindow::maybeSave()
{
    if (pad.m_update_file==false)
        return true;

    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret)
    {
    case QMessageBox::Save:
        return Save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}


////////////////////////////////////////////////////////

/*
void MainWindow::readSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 3, availableGeometry.height() / 2);
        move((availableGeometry.width() - width()) / 2,
             (availableGeometry.height() - height()) / 2);
    } else {
        restoreGeometry(geometry);
    }
}


void MainWindow::writeSettings()
{
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("geometry", saveGeometry());
}
*/

////////////////////////////////////////////////////////


void MainWindow::SetKeyData(qint32 idx)
{
    LbKey->setText(QString("K E Y  -  %1").arg(idx+1));
}


void MainWindow::onHdrList()
{
    if(pad.kpd.nKeys==0) return;

    if(keydata==centralWidget())
    {
        takeCentralWidget()->deleteLater();
        keydata = 0;
    }

    if(listhdr==0)
    {
        listhdr = new HdrTableView(this, &pad, &pad.kpd);
        connect(listhdr->model, &HdrListModel::kpd_update, this, &MainWindow::setUpdate);
        setCentralWidget(listhdr);
    }
    else
    {
        //centralWidget()->setVisible(!centralWidget()->isVisible());
        //takeCentralWidget()->deleteLater();
        listhdr = 0;
    }
}


void MainWindow::onKeyData()
{
    if(pad.kpd.nKeys==0) return;

    if(listhdr==centralWidget())
    {
        takeCentralWidget()->deleteLater();
        listhdr = 0;
    }

    if(keydata==0)
    {
        keydata = new KeyDataView(this, &pad.kpd, pad.pkey);
        connect(&pad, &BuildKeypad::KeyPressed, keydata, &KeyDataView::selectIdx);
        connect(keydata->model, &KeyDataModel::kpd_update, this, &MainWindow::setUpdate);
        keydata->selectIdx(pad.m_idx);
        setCentralWidget(keydata);
    }
    else
    {
        //centralWidget()->setVisible(!centralWidget()->isVisible());
        takeCentralWidget()->deleteLater();
        keydata = 0;
    }
}


void MainWindow::onKeyList()
{
    if(pad.kpd.nKeys==0) return;

    if(listkeys==0)
    {
        listkeys = new KeyListDlg(0, 0, 0, DISPLAY_CX, DISPLAY_CY, &pad.kpd, pad.pkey, pad.m_idx);
        connect(listkeys->model, &KeyListModel::kpd_update, this, &MainWindow::setUpdate);
        connect(&pad, &BuildKeypad::KeyPressed, listkeys, &KeyListDlg::selectIdx);
        connect(listkeys, &KeyListDlg::focusKey, &pad, &BuildKeypad::FocusOnKey);
        connect(listkeys, &KeyListDlg::destroyed, this, [=]{listkeys=0;}); /// setAttribute(Qt::WA_DeleteOnClose)
    }
    else
    {
        listkeys->deleteLater();
        listkeys=0;
    }

    /*
    KeyListDlg dlg(this, 0, 0, DISPLAY_CX, DISPLAY_CY, &pad.kpd, pad.pkey, pad.m_idx);
    connect(dlg.model, &KeyListModel::kpd_update, this, &MainWindow::setUpdate);
    connect(&pad, &BuildKeypad::KeyPressed, &dlg, &KeyListDlg::selectRow);
    connect(&dlg, &KeyListDlg::focusKey, &pad, &BuildKeypad::FocusOnKey);
    dlg.exec();
    */
}


void MainWindow::OpenIconBar()
{
    if(pad.kpd.nKeys==0) return;

    if(iconbar.isVisible())
    {
        iconbar.hide();
    }
    else
    {
        if(iconbar.SetIconBar("IconBar.kpd", pad.m_FilePath))
        {
            iconbar.setWindowTitle("ICON BAR");
            iconbar.show();
        }
    }
}


void MainWindow::OpenButtonBar()
{
    /*
    takeCentralWidget()->deleteLater();

    ButtonsBar *kbr = new ButtonsBar(this);
    kbr->SetKeyPad("KeyBar.kpd", "", 0, 0, 220, DISPLAY_CY);
    kbr->setWindowTitle("KEY BAR");
    kbr->showMaximized();
    setCentralWidget(kbr);
    */
    keybar.setVisible(!keybar.isVisible());
}


void MainWindow::setDrawType()
{
    pad.setKeypadType();
    setWindowTitle(pDrwM[pad.kpd.type]);

    kpdscrl.resize(pad.m_wnd_cx+2, pad.m_wnd_cy+2);
    kpdscrl.show();
}


void MainWindow::setUpdate(int level)
{
    pad.m_update_file = true;

    statusBar()->showMessage("Modified");

    if(level==1)
    {
        pad.Redraw();
    }
    else if(level==2)
    {
        pad.Redraw2();
    }
    else if(level==3)
    {
        pad.ResizeKeyGrid(pad.m_wnd_cx, pad.m_wnd_cy);
        pad.Redraw2();
    }
}


/*
    #include <time.h>
    //--------------------test speed
    long bgn = clock();
    //------------------------------
    for(int i=0; i<5000; i++)
        //pad.DrawButtonAx(0,0,200,65,0);		// 16-31 !!!  winer
        //pad.DrawButtonGr(0,0,200,65,0);		// 250-350	with QLinearGradient
        //pad.DrawButtonAr(0,0,200,65,0);		// 32-46 with fillRect till the midle (from color to color and back from color) (split the button)

    //--------------------test speed
    char str[40];
    sprintf(str, "speed = %d", clock()-bgn);
    LbM[2]->setText(str);
    //------------------------------
*/
