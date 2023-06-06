#ifndef MAINWINDOW_2_H
#define MAINWINDOW_2_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>


namespace Ui {
class MainWindow_2;
}

class MainWindow_2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_2(QString patientId, QWidget *parent = nullptr);
    //(QWidget *parent = nullptr);
    ~MainWindow_2();

private slots:
    void on_pushButton_searchallpatient_clicked();

    void on_pushButton_search_medical_records_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_create_medical_records_clicked();

    void on_pushButton_confirm_prescription_clicked();

    void on_pushButton_searchall_medical_records_clicked();

private:
    Ui::MainWindow_2 *ui;
    QSqlDatabase db;
    QSqlTableModel *model;

};

#endif // MAINWINDOW_2_H
