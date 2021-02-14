#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVBoxLayout>

#include "CppAndroid/galleryResultReceiver.h"
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

inline
void clear(QBoxLayout* lay)
{
    while(lay->itemAt(0) != nullptr)
    {
        auto it{lay->itemAt(0)};
        it->widget()->hide();
        it->widget()->deleteLater();
        lay->removeItem(it);
    }
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFilesSelected(QStringList fileList);

    void on_pb_openGallery_clicked();


private:
    Ui::MainWindow *ui;

#ifdef Q_OS_ANDROID

    std::unique_ptr<android::GalleryResultReceiver> m_galleryReceiver;

#endif
};
#endif // MAINWINDOW_H
