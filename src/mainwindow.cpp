#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringList>
#include <QLabel>
#include <QScroller>
#include <QMessageBox>

#include "CppAndroid/utils.h"
#include "CppAndroid/galleryResultReceiver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
#ifdef Q_OS_ANDROID
     , m_galleryReceiver{std::make_unique<android::GalleryResultReceiver>()}
#endif
{
    ui->setupUi(this);

#ifdef Q_OS_ANDROID
    connect(m_galleryReceiver.get(),&android::GalleryResultReceiver::filesSelected,this,&MainWindow::onFilesSelected);

    QStringList appPermissions{"android.permission.READ_EXTERNAL_STORAGE","android.permission.WRITE_EXTERNAL_STORAGE"};

    QStringList refusedPerm{android::requirePermissions(appPermissions)};
    while(refusedPerm.size() != 0)
    {
        QString errorMsg{tr("You need to grant following permissions :\n")};
        for(const auto& e : refusedPerm)
            errorMsg += e+"\n";
        QMessageBox::warning(this,"Error",errorMsg);
        refusedPerm = android::requirePermissions(appPermissions);
    }

    QScroller::grabGesture(ui->scrollArea,QScroller::LeftMouseButtonGesture);
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pb_openGallery_clicked()
{
#ifdef Q_OS_ANDROID

    if(!android::askImages(m_galleryReceiver.get()))
    {
        throw std::runtime_error{"Cannot open gallery"};
    }

#endif
}

void MainWindow::onFilesSelected(QStringList fileList)
{
#ifdef Q_OS_ANDROID
    auto vlay_im{ui->vlay_im};
    clear(vlay_im);

    for(const auto& e : fileList)
    {
        auto lbl{new QLabel{this}};
        QImage im{e};
        im = im.scaled(ui->centralwidget->width()-100,im.height(),Qt::KeepAspectRatio);

        lbl->setPixmap(QPixmap::fromImage(im));
        ui->vlay_im->addWidget(lbl);
    }
#endif
}
